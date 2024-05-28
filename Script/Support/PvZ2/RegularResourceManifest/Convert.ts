namespace Twinning.Script.Support.PvZ2.RegularResourceManifest.Convert {

	// ------------------------------------------------

	export function from_official(
		source: ResourceManifest.Package,
	): RegularResourceManifest.Package {
		let destination: RegularResourceManifest.Package = {
			group: [],
		};
		for (let source_group of source.groups) {
			assert_test(ResourceManifest.GroupTypeE.includes(source_group.type), `group type invalid`);
			switch (source_group.type) {
				case 'composite': {
					destination.group.push({
						id: source_group.id,
						composite: true,
						subgroup: [],
					});
					break;
				}
				case 'simple': {
					let source_subgroup = source_group as ResourceManifest.SimpleGroupAdditional;
					let destination_group: RegularResourceManifest.Group;
					if (source_subgroup.parent === undefined) {
						destination_group = {
							id: source_group.id,
							composite: false,
							subgroup: [],
						};
						destination.group.push(destination_group);
					}
					else {
						let destination_group_if = destination.group.find((value) => (value.id === source_subgroup.parent));
						assert_test(destination_group_if !== undefined, `subgroup's parent is not found : ${source_group.id}`);
						destination_group = destination_group_if;
					}
					let destination_subgroup: RegularResourceManifest.Subgroup = {
						id: source_group.id,
						category: {
							resolution: not_or(JSONGenericGetter.integer(source_subgroup.res, 0n), 0n, null),
							locale: not_or(JSONGenericGetter.string(source_subgroup.loc, ''), '', null),
						},
						resource: [],
					};
					destination_group.subgroup.push(destination_subgroup);
					for (let source_resource of source_subgroup.resources) {
						let destination_resource_path = is_string(source_resource.path) ? PathUtility.regularize(source_resource.path) : PathUtility.catenate(source_resource.path);
						if (destination_resource_path === '!program') {
							destination_subgroup.resource.push({
								id: source_resource.id,
								additional: {
									type: 'dummy',
									value: {
									},
								},
							});
						}
						else {
							if ('atlas' in source_resource) {
								destination_subgroup.resource.push({
									id: source_resource.id,
									additional: {
										type: 'texture',
										value: {
											path: destination_resource_path,
											size: [
												JSONGenericGetter.integer(source_resource.width),
												JSONGenericGetter.integer(source_resource.height),
											],
											sprite: [],
										},
									},
								});
							}
							else if ('parent' in source_resource) {
								let texture_atlas = destination_subgroup.resource.find((value) => (value.id === (source_resource as ResourceManifest.SpriteImageResourceAdditional).parent));
								assert_test(texture_atlas !== undefined, `sprite's parent is not found : ${source_resource.parent}`);
								assert_test(texture_atlas.additional.type === 'texture', `sprite's parent is not texture : ${source_resource.parent}`);
								texture_atlas.additional.value.sprite.push({
									id: source_resource.id,
									path: destination_resource_path,
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
									separate: [
										JSONGenericGetter.integer(source_resource.rows, 1n),
										JSONGenericGetter.integer(source_resource.cols, 1n),
									],
								});
							}
							else {
								destination_subgroup.resource.push({
									id: source_resource.id,
									additional: {
										type: 'general',
										value: {
											path: destination_resource_path,
											type: source_resource.type,
										},
									},
								});
							}
						}
					}
					break;
				}
			}
		}
		return destination;
	}

	export function to_official(
		source: RegularResourceManifest.Package,
		use_array_style_path: boolean,
	): ResourceManifest.Package {
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
		let destination: ResourceManifest.Package = {
			slot_count: 0n,
			groups: [],
		};
		for (let source_group of source.group) {
			let destination_group: null | (ResourceManifest.GroupBase & ResourceManifest.CompositeGroupAdditional) = null;
			if (source_group.composite) {
				destination_group = {
					id: source_group.id,
					type: 'composite',
					subgroups: [],
				};
				destination.groups.push(destination_group);
			}
			for (let source_subgroup of source_group.subgroup) {
				let destination_subgroup: ResourceManifest.GroupBase & ResourceManifest.SimpleGroupAdditional = {
					id: source_subgroup.id,
					type: 'simple',
				} as any;
				let category_property = {} as any;
				if (source_subgroup.category.resolution !== null) {
					category_property.res = source_subgroup.category.resolution;
				}
				if (source_subgroup.category.locale !== null) {
					category_property.loc = source_subgroup.category.locale;
				}
				Object.assign(destination_subgroup, category_property);
				if (source_group.composite) {
					destination_subgroup.parent = source_group.id;
					destination_group!.subgroups.push({
						id: source_subgroup.id,
						...category_property,
					});
				}
				destination_subgroup.resources = [];
				for (let source_resource of source_subgroup.resource) {
					if (source_resource.additional.type === 'dummy') {
						destination_subgroup.resources.push({
							slot: slot_of(source_resource.id),
							id: source_resource.id,
							path: !use_array_style_path ? PathUtility.to_windows_style('!program') : PathUtility.split('!program'),
							type: 'Image',
						} as ResourceManifest.ResourceBase & ResourceManifest.DummyResourceAdditional);
					}
					if (source_resource.additional.type === 'general') {
						destination_subgroup.resources.push({
							slot: slot_of(source_resource.id),
							id: source_resource.id,
							path: !use_array_style_path ? PathUtility.to_windows_style(source_resource.additional.value.path) : PathUtility.split(source_resource.additional.value.path),
							type: source_resource.additional.value.type,
						} as ResourceManifest.ResourceBase & ResourceManifest.DummyResourceAdditional);
					}
					if (source_resource.additional.type === 'texture') {
						destination_subgroup.resources.push({
							slot: slot_of(source_resource.id),
							id: source_resource.id,
							path: !use_array_style_path ? PathUtility.to_windows_style(source_resource.additional.value.path) : PathUtility.split(source_resource.additional.value.path),
							type: 'Image',
							atlas: true,
							width: source_resource.additional.value.size[0],
							height: source_resource.additional.value.size[1],
						} as ResourceManifest.ResourceBase & ResourceManifest.DummyResourceAdditional);
						for (let source_sprite_resource of source_resource.additional.value.sprite) {
							destination_subgroup.resources.push({
								slot: slot_of(source_sprite_resource.id),
								id: source_sprite_resource.id,
								path: !use_array_style_path ? PathUtility.to_windows_style(source_sprite_resource.path) : PathUtility.split(source_sprite_resource.path),
								type: 'Image',
								parent: source_resource.id,
								ax: source_sprite_resource.position[0],
								ay: source_sprite_resource.position[1],
								aw: source_sprite_resource.size[0],
								ah: source_sprite_resource.size[1],
								x: source_sprite_resource.offset[0],
								y: source_sprite_resource.offset[1],
								rows: source_sprite_resource.separate[0],
								cols: source_sprite_resource.separate[1],
							} as ResourceManifest.ResourceBase & ResourceManifest.SpriteImageResourceAdditional);
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