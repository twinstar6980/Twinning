namespace Twinning.Script.Support.PvZ2.ResourceManifest.NewTypeObjectNotation.Encode {

	// ------------------------------------------------

	function write_enumeration(
		data: ByteStreamView,
		value: bigint,
	): void {
		data.u8(value);
		return;
	}

	function write_boolean(
		data: ByteStreamView,
		value: boolean,
	): void {
		data.u8(!value ? 0x00n : 0x01n);
		return;
	}

	function write_integer(
		data: ByteStreamView,
		value: bigint,
	): void {
		data.i32(value);
		return;
	}

	function write_string(
		data: ByteStreamView,
		value: string,
	): void {
		data.i32(BigInt(value.length));
		let string_c = Kernel.String.value(value);
		let string_view = new Uint8Array(Kernel.Miscellaneous.cast_CharacterListView_to_ByteListView(Kernel.Miscellaneous.cast_String_to_CharacterListView(string_c)).value);
		for (let index = 0; index < string_view.length; index++) {
			data.u8(BigInt(string_view[index]));
		}
		return;
	}

	// ------------------------------------------------

	export function process(
		data: ByteStreamView,
		definition: ResourceManifest.Package,
	): void {
		write_integer(data, definition.slot_count);
		write_integer(data, BigInt(definition.groups.length));
		for (let groups_index = 0; groups_index < definition.groups.length; groups_index++) {
			let group: ResourceManifest.GroupBase & (ResourceManifest.CompositeGroupAdditional & ResourceManifest.SimpleGroupAdditional) = { ...definition.groups[groups_index] } as any;
			group.subgroups = not_undefined_or(group.subgroups, []);
			group.resources = not_undefined_or(group.resources, []);
			let group_type_index = GroupTypeEnumeration.find((item) => (item.value === group.type))?.index!;
			assert_test(group.type !== undefined, `unknown group type value ${group.type}`);
			write_enumeration(data, group_type_index);
			write_integer(data, not_undefined_or(group.res, 0n));
			write_integer(data, BigInt(group.subgroups.length));
			write_integer(data, BigInt(group.resources.length));
			write_boolean(data, true);
			write_boolean(data, group.parent !== undefined);
			write_string(data, group.id);
			if (group.parent !== undefined) {
				write_string(data, group.parent);
			}
			for (let subgroups_index = 0; subgroups_index < group.subgroups.length; subgroups_index++) {
				let subgroup: ResourceManifest.CompositeGroupAdditional['subgroups'][number] = group.subgroups[subgroups_index];
				write_integer(data, not_undefined_or(subgroup.res, 0n));
				write_string(data, subgroup.id);
			}
			for (let resources_index = 0; resources_index < group.resources.length; resources_index++) {
				let resource: ResourceManifest.ResourceBase & (ResourceManifest.GeneralResourceAdditional & ResourceManifest.AtlasImageResourceAdditional & ResourceManifest.SpriteImageResourceAdditional) = { ...group.resources[resources_index] } as any;
				if (resource.type !== 'Image' || resource.parent === undefined) {
					resource.y = 0x7FFFFFFFn;
				}
				let resource_type_index = ResourceTypeEnumeration.find((item) => (item.value === resource.type))?.index!;
				assert_test(resource.type !== undefined, `unknown resource type value ${resource_type_index}`);
				write_enumeration(data, resource_type_index);
				write_integer(data, resource.slot);
				write_integer(data, not_undefined_or(resource.width, 0n));
				write_integer(data, not_undefined_or(resource.height, 0n));
				write_integer(data, not_undefined_or(resource.x, 0n));
				write_integer(data, not_undefined_or(resource.y, 0n));
				write_integer(data, not_undefined_or(resource.ax, 0n));
				write_integer(data, not_undefined_or(resource.ay, 0n));
				write_integer(data, not_undefined_or(resource.aw, 0n));
				write_integer(data, not_undefined_or(resource.ah, 0n));
				write_integer(data, not_undefined_or(resource.cols, 1n));
				write_integer(data, not_undefined_or(resource.rows, 1n));
				write_boolean(data, not_undefined_or(resource.atlas, false));
				write_boolean(data, true);
				write_boolean(data, true);
				write_boolean(data, resource.parent !== undefined);
				write_string(data, resource.id);
				write_string(data, is_string(resource.path) ? resource.path : resource.path.join('\\'));
				if (resource.parent !== undefined) {
					write_string(data, resource.parent);
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	export function process_fs(
		data_file: string,
		definition_file: string,
		data_buffer: Kernel.ByteListView | bigint,
	): void {
		let data_buffer_if = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
		let data_buffer_view = data_buffer instanceof Kernel.ByteListView ? data_buffer : data_buffer_if!.view();
		let definition = KernelX.JSON.read_fs_js<ResourceManifest.Package>(definition_file);
		let stream = new ByteStreamView(data_buffer_view.value);
		process(stream, definition);
		KernelX.Storage.write_file(data_file, data_buffer_view.sub(Kernel.Size.value(0n), Kernel.Size.value(BigInt(stream.p()))));
		return;
	}

	// ------------------------------------------------

}