/** 资源清单转换 */
namespace TwinStar.Support.PvZ2.RSB.ResourceManifest.Convert {

	// ------------------------------------------------

	/**
	 * 将原生资源清单转换至自定义资源清单
	 * @param source 原生资源清单
	 * @returns 自定义资源清单
	 */
	export function from_official(
		source: OfficialResourceManifest.Package,
	): ResourceManifest.Package {
		let destination: ResourceManifest.Package = {
			group: {},
		};
		for (let source_group of source.groups) {
			if (!OfficialResourceManifest.GroupTypeE.includes(source_group.type)) {
				throw new Error(`group type invalid`);
			}
			switch (source_group.type) {
				case 'composite': {
					destination.group[source_group.id] = {
						composite: true,
						subgroup: {},
					};
					break;
				}
				case 'simple': {
					let source_subgroup = source_group as OfficialResourceManifest.SimpleGroupInformation;
					let destination_group: ResourceManifest.Group;
					if (source_subgroup.parent === undefined) {
						destination.group[source_group.id] = {
							composite: false,
							subgroup: {},
						};
						destination_group = destination.group[source_group.id];
					} else {
						let destination_group_if = destination.group[source_subgroup.parent];
						if (destination_group_if === undefined) {
							throw new Error(`subgroup's parent is not found : ${source_group.id}`);
						}
						destination_group = destination_group_if;
					}
					destination_group.subgroup[source_group.id] = {
						category: [
							JSONGenericGetter.integer(source_subgroup.res, null),
							JSONGenericGetter.string(source_subgroup.loc, null),
						],
						resource: {},
					};
					let destination_subgroup = destination_group.subgroup[source_group.id];
					for (let source_resource of source_subgroup.resources) {
						if ('atlas' in source_resource) {
							destination_subgroup.resource[source_resource.id] = {
								path: source_resource.path.join('/'),
								type: source_resource.type,
								expand: ['atlas', {
									size: [
										JSONGenericGetter.integer(source_resource.width),
										JSONGenericGetter.integer(source_resource.height),
									],
									sprite: {},
								}],
							};
						} else if ('parent' in source_resource) {
							let atlas = destination_subgroup.resource[(source_resource as OfficialResourceManifest.SpriteImageResourceInformation).parent];
							if (atlas === undefined) {
								throw new Error(`sprite's parent is not found : ${source_resource.parent}`);
							}
							if (atlas.expand[0] !== 'atlas') {
								throw new Error(`sprite's expand type is not 'atlas' : ${source_resource.parent}`);
							}
							atlas.expand[1].sprite[source_resource.id] = {
								path: source_resource.path.join('/'),
								position: [
									JSONGenericGetter.integer(source_resource.ax),
									JSONGenericGetter.integer(source_resource.ay),
								],
								size: [
									JSONGenericGetter.integer(source_resource.aw),
									JSONGenericGetter.integer(source_resource.ah),
								],
								offset: [
									JSONGenericGetter.integer(source_resource.x, 0n),
									JSONGenericGetter.integer(source_resource.y, 0n),
								],
							};
						} else {
							destination_subgroup.resource[source_resource.id] = {
								path: source_resource.path.join('/'),
								type: source_resource.type,
								expand: ['generic', {}],
							};
						}
					}
					break;
				}
			}
		}
		return destination;
	}

	/**
	 * 将自定义资源清单转换至原生资源清单
	 * @param source 自定义资源清单
	 * @returns 原生资源清单
	 */
	export function to_official(
		source: ResourceManifest.Package,
	): OfficialResourceManifest.Package {
		let slot_map: Map<string, bigint> = new Map();
		let slot_of = (
			key: string,
		): bigint => {
			let value = slot_map.get(key);
			if (value === undefined) {
				value = BigInt(slot_map.size);
				slot_map.set(key, value);
			}
			return value;
		};
		let destination: OfficialResourceManifest.Package = {
			slot_count: 0n,
			groups: [],
		};
		for (let group_id in source.group) {
			let source_group = source.group[group_id];
			let destination_group: (OfficialResourceManifest.GroupBase & { type: 'composite'; } & OfficialResourceManifest.CompositeGroupInformation) | null = null;
			if (source_group.composite) {
				destination_group = {
					type: 'composite',
					id: group_id,
					subgroups: [],
				};
				destination.groups.push(destination_group);
			}
			for (let subgroup_id in source_group.subgroup) {
				let source_subgroup = source_group.subgroup[subgroup_id];
				let destination_subgroup: OfficialResourceManifest.GroupBase & { type: 'simple'; } & OfficialResourceManifest.SimpleGroupInformation = {
					type: 'simple',
					id: subgroup_id,
				} as any;
				let category_property = {} as any;
				if (source_subgroup.category[0] !== null) {
					category_property['res'] = source_subgroup.category[0];
				}
				if (source_subgroup.category[1] !== null) {
					category_property['loc'] = source_subgroup.category[1];
				}
				Object.assign(destination_subgroup, category_property);
				if (source_group.composite) {
					destination_subgroup['parent'] = group_id;
					destination_group!.subgroups.push({
						id: subgroup_id,
						...category_property,
					});
				}
				destination_subgroup['resources'] = [];
				for (let resource_id in source_subgroup.resource) {
					let source_resource = source_subgroup.resource[resource_id];
					let destination_resource: OfficialResourceManifest.Resource = {
						slot: slot_of(resource_id),
						id: resource_id,
						path: PathUtility.split(source_resource.path),
						type: source_resource.type,
					} as OfficialResourceManifest.Resource;
					destination_subgroup.resources.push(destination_resource);
					if (source_resource.expand[0] === 'atlas') {
						(destination_resource as OfficialResourceManifest.AtlasImageResourceInformation).atlas = true;
						(destination_resource as OfficialResourceManifest.AtlasImageResourceInformation).width = source_resource.expand[1].size[0];
						(destination_resource as OfficialResourceManifest.AtlasImageResourceInformation).height = source_resource.expand[1].size[1];
						for (let sprite_resource_id in source_resource.expand[1].sprite) {
							let source_sprite_resource = source_resource.expand[1].sprite[sprite_resource_id];
							let destination_sprite_resource: OfficialResourceManifest.ResourceBase & OfficialResourceManifest.SpriteImageResourceInformation = {
								slot: slot_of(sprite_resource_id),
								id: sprite_resource_id,
								path: PathUtility.split(source_sprite_resource.path),
								type: 'Image',
								parent: resource_id,
								ax: source_sprite_resource.position[0],
								ay: source_sprite_resource.position[1],
								aw: source_sprite_resource.size[0],
								ah: source_sprite_resource.size[1],
							};
							destination_subgroup.resources.push(destination_sprite_resource);
						}
					}
				}
				destination.groups.push(destination_subgroup);
			}
		}
		destination.slot_count = BigInt(slot_map.size);
		return destination;
	}

	// ------------------------------------------------

}