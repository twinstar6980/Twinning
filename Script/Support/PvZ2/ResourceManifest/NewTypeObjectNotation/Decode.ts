namespace Twinning.Script.Support.PvZ2.ResourceManifest.NewTypeObjectNotation.Decode {

	// ------------------------------------------------

	function read_enumeration(
		data: ByteStreamView,
	): bigint {
		return data.u8();
	}

	function read_boolean(
		data: ByteStreamView,
	): boolean {
		let value: boolean;
		switch (data.u8()) {
			case 0x00n: {
				value = false;
				break;
			}
			case 0x01n: {
				value = true;
				break;
			}
			default: {
				assert_fail(`invalid boolean integer`);
			}
		}
		return value;
	}

	function read_integer(
		data: ByteStreamView,
	): bigint {
		return data.i32();
	}

	function read_string(
		data: ByteStreamView,
	): string {
		let length = data.u32();
		return PopCap.ReflectionObjectNotation.DecodeLenient.read_utf8_string_by_size(data, length);
	}

	// ------------------------------------------------

	export function process(
		data: ByteStreamView,
	): ResourceManifest.Package {
		let definition: ResourceManifest.Package = {
			slot_count: undefined!,
			groups: [],
		};
		definition.slot_count = read_integer(data);
		definition.groups.length = Number(read_integer(data));
		for (let groups_index = 0; groups_index < definition.groups.length; groups_index++) {
			let group_base: ResourceManifest.GroupBase = {
				id: undefined!,
				type: undefined!,
			};
			let group_composite: ResourceManifest.CompositeGroupAdditional = {
				subgroups: [],
			};
			let group_simple: ResourceManifest.SimpleGroupAdditional = {
				res: undefined!,
				parent: undefined!,
				resources: [],
			};
			let group_type_index = read_enumeration(data);
			group_base.type = GroupTypeEnumeration.find((item) => (item.index === group_type_index))?.value!;
			assert_test(group_base.type !== undefined, `unknown group type index ${group_type_index}`);
			group_simple.res = not_or(read_integer(data), 0n, undefined);
			group_composite.subgroups.length = Number(read_integer(data));
			group_simple.resources.length = Number(read_integer(data));
			assert_test(read_boolean(data) === true);
			let group_has_parent = read_boolean(data);
			group_base.id = read_string(data);
			if (group_has_parent) {
				group_simple.parent = read_string(data);
			}
			for (let subgroups_index = 0; subgroups_index < group_composite.subgroups.length; subgroups_index++) {
				let subgroup_base: ResourceManifest.CompositeGroupAdditional['subgroups'][number] = {
					id: undefined!,
					res: undefined!,
				};
				subgroup_base.res = not_or(read_integer(data), 0n, undefined);
				subgroup_base.id = read_string(data);
				group_composite.subgroups[subgroups_index] = object_clear_undefined({
					...subgroup_base,
				});
			}
			for (let resources_index = 0; resources_index < group_simple.resources.length; resources_index++) {
				let resource_base: ResourceManifest.ResourceBase = {
					slot: undefined!,
					id: undefined!,
					path: undefined!,
					type: undefined!,
				};
				let resource_atlas: ResourceManifest.AtlasImageResourceAdditional = {
					atlas: undefined!,
					width: undefined!,
					height: undefined!,
				};
				let resource_sprite: ResourceManifest.SpriteImageResourceAdditional = {
					parent: undefined!,
					ax: undefined!,
					ay: undefined!,
					aw: undefined!,
					ah: undefined!,
					x: undefined!,
					y: undefined!,
					rows: undefined!,
					cols: undefined!,
					aflag: undefined!,
				};
				let resource_type_index = read_enumeration(data);
				resource_base.type = ResourceTypeEnumeration.find((item) => (item.index === resource_type_index))?.value!;
				assert_test(resource_base.type !== undefined, `unknown resource type index ${resource_type_index}`);
				resource_base.slot = read_integer(data);
				resource_atlas.width = read_integer(data);
				resource_atlas.height = read_integer(data);
				resource_sprite.x = read_integer(data);
				resource_sprite.y = read_integer(data);
				resource_sprite.ax = read_integer(data);
				resource_sprite.ay = read_integer(data);
				resource_sprite.aw = read_integer(data);
				resource_sprite.ah = read_integer(data);
				resource_sprite.cols = read_integer(data);
				resource_sprite.rows = read_integer(data);
				resource_atlas.atlas = read_boolean(data); // atlas or runtime
				assert_test(read_boolean(data) === true);
				assert_test(read_boolean(data) === true);
				let resource_has_parent = read_boolean(data);
				resource_base.id = read_string(data);
				resource_base.path = read_string(data);
				if (resource_has_parent) {
					resource_sprite.parent = read_string(data);
				}
				if (resource_base.type !== 'Image') {
					group_simple.resources[resources_index] = object_clear_undefined({
						...resource_base,
					});
				}
				if (resource_base.type === 'Image') {
					group_simple.resources[resources_index] = object_clear_undefined({
						...resource_base,
						...(!resource_has_parent ? resource_atlas : resource_sprite),
					});
				}
			}
			if (group_base.type === 'composite') {
				assert_test(group_simple.resources.length === 0);
				definition.groups[groups_index] = object_clear_undefined({
					...group_base,
					...group_composite,
				});
			}
			if (group_base.type === 'simple') {
				assert_test(group_composite.subgroups.length === 0);
				definition.groups[groups_index] = object_clear_undefined({
					...group_base,
					...group_simple,
				});
			}
		}
		return definition;
	}

	// ------------------------------------------------

	export function process_fs(
		data_file: string,
		definition_file: string,
	): void {
		let data = KernelX.Storage.read_file(data_file);
		let definition = process(new ByteStreamView(data.view().value));
		KernelX.JSON.write_fs_js(definition_file, definition);
		return;
	}

	// ------------------------------------------------

}