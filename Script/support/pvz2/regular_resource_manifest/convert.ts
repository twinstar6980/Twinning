namespace Twinning.Script.Support.Pvz2.RegularResourceManifest.Convert {

	// #region utility

	export function from_official(
		source: ResourceManifest.Package,
	): RegularResourceManifest.Package {
		let destination: RegularResourceManifest.Package = {
			group: [],
		};
		for (let source_group of source.groups) {
			if (!ResourceManifest.GroupTypeE.includes(source_group.type)) {
				throw new Error(`invalid group type '${source_group.type}'`);
			}
			switch (source_group.type) {
				case 'composite': {
					destination.group.push({
						identifier: source_group.id,
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
							identifier: source_group.id,
							composite: false,
							subgroup: [],
						};
						destination.group.push(destination_group);
					}
					else {
						let destination_group_if = destination.group.find((value) => (value.identifier === source_subgroup.parent));
						if (destination_group_if === undefined) {
							throw new Error(`subgroup's parent is not found '${source_group.id}'`);
						}
						destination_group = destination_group_if;
					}
					let destination_subgroup: RegularResourceManifest.Subgroup = {
						identifier: source_group.id,
						category: {
							resolution: CheckHelper.not_or(JsonGenericGetter.integer(source_subgroup.res, 0n), 0n, null),
							locale: CheckHelper.not_or(JsonGenericGetter.string(source_subgroup.loc, ''), '', null),
						},
						resource: [],
					};
					destination_group.subgroup.push(destination_subgroup);
					for (let source_resource of source_subgroup.resources) {
						let destination_resource_path = CheckHelper.is_string(source_resource.path) ? StorageHelper.regularize(source_resource.path) : StorageHelper.catenate(source_resource.path);
						if (destination_resource_path === '!program') {
							destination_subgroup.resource.push({
								identifier: source_resource.id,
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
									identifier: source_resource.id,
									additional: {
										type: 'texture',
										value: {
											path: destination_resource_path,
											size: [
												JsonGenericGetter.integer(source_resource.width),
												JsonGenericGetter.integer(source_resource.height),
											],
											sprite: [],
										},
									},
								});
							}
							else if ('parent' in source_resource) {
								let texture_atlas = destination_subgroup.resource.find((value) => (value.identifier === (source_resource as ResourceManifest.SpriteImageResourceAdditional).parent));
								if (texture_atlas === undefined) {
									throw new Error(`sprite's parent is not found '${source_resource.parent}'`);
								}
								if (texture_atlas.additional.type !== 'texture') {
									throw new Error(`sprite's parent is not texture '${source_resource.parent}'`);
								}
								texture_atlas.additional.value.sprite.push({
									identifier: source_resource.id,
									path: destination_resource_path,
									position: [
										JsonGenericGetter.integer(source_resource.ax),
										JsonGenericGetter.integer(source_resource.ay),
									],
									size: [
										JsonGenericGetter.integer(source_resource.aw),
										JsonGenericGetter.integer(source_resource.ah),
									],
									offset: [
										JsonGenericGetter.integer(source_resource.x, 0n),
										JsonGenericGetter.integer(source_resource.y, 0n),
									],
									separate: [
										JsonGenericGetter.integer(source_resource.rows, 1n),
										JsonGenericGetter.integer(source_resource.cols, 1n),
									],
								});
							}
							else {
								destination_subgroup.resource.push({
									identifier: source_resource.id,
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
					id: source_group.identifier,
					type: 'composite',
					subgroups: [],
				};
				destination.groups.push(destination_group);
			}
			for (let source_subgroup of source_group.subgroup) {
				let optional_property = {} as Pick<ResourceManifest.SimpleGroupAdditional, 'res' | 'loc' | 'parent'>;
				if (source_subgroup.category.resolution !== null) {
					optional_property.res = source_subgroup.category.resolution;
				}
				if (source_subgroup.category.locale !== null) {
					optional_property.loc = source_subgroup.category.locale;
				}
				if (source_group.composite) {
					destination_group!.subgroups.push({
						id: source_subgroup.identifier,
						...optional_property,
					});
					optional_property.parent = source_group.identifier;
				}
				let destination_subgroup: ResourceManifest.GroupBase & ResourceManifest.SimpleGroupAdditional = {
					id: source_subgroup.identifier,
					type: 'simple',
					...optional_property,
					resources: [],
				};
				for (let source_resource of source_subgroup.resource) {
					if (source_resource.additional.type === 'dummy') {
						destination_subgroup.resources.push({
							slot: slot_of(source_resource.identifier),
							id: source_resource.identifier,
							path: !use_array_style_path ? StorageHelper.to_windows_style('!program') : StorageHelper.split('!program'),
							type: 'Image',
						} as ResourceManifest.ResourceBase & ResourceManifest.DummyResourceAdditional);
					}
					if (source_resource.additional.type === 'general') {
						destination_subgroup.resources.push({
							slot: slot_of(source_resource.identifier),
							id: source_resource.identifier,
							path: !use_array_style_path ? StorageHelper.to_windows_style(source_resource.additional.value.path) : StorageHelper.split(source_resource.additional.value.path),
							type: source_resource.additional.value.type,
						} as ResourceManifest.ResourceBase & ResourceManifest.DummyResourceAdditional);
					}
					if (source_resource.additional.type === 'texture') {
						destination_subgroup.resources.push({
							slot: slot_of(source_resource.identifier),
							id: source_resource.identifier,
							path: !use_array_style_path ? StorageHelper.to_windows_style(source_resource.additional.value.path) : StorageHelper.split(source_resource.additional.value.path),
							type: 'Image',
							atlas: true,
							width: source_resource.additional.value.size[0],
							height: source_resource.additional.value.size[1],
						} as ResourceManifest.ResourceBase & ResourceManifest.DummyResourceAdditional);
						for (let source_sprite_resource of source_resource.additional.value.sprite) {
							destination_subgroup.resources.push({
								slot: slot_of(source_sprite_resource.identifier),
								id: source_sprite_resource.identifier,
								path: !use_array_style_path ? StorageHelper.to_windows_style(source_sprite_resource.path) : StorageHelper.split(source_sprite_resource.path),
								type: 'Image',
								parent: source_resource.identifier,
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

	// #endregion

}
