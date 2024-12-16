namespace Twinning.Script.Support.PopCap.ResourceStreamBundle.UnpackLenient {

	// ------------------------------------------------

	export function decode_compiled_map<Value>(
		data: ByteListView,
		value_parser: (stream: ByteStreamView) => Value,
	): Record<string, Value> {
		let stream = new ByteStreamView(data.sub(0, data.size()));
		let parent_string: Record<number, string> = {};
		let map: Record<string, Value> = {};
		while (stream.p() < stream.size()) {
			let key = '';
			let position = stream.p() / 4;
			if (parent_string[position] !== undefined) {
				key += parent_string[position];
				delete parent_string[position];
			}
			while (true) {
				let composite_value = stream.u32();
				let child_string_offset = Number((composite_value & 0xFFFFFF00n) >> 8n);
				let current_character = Number((composite_value & 0x000000FFn) >> 0n);
				if (child_string_offset !== 0) {
					parent_string[child_string_offset] = key;
				}
				if (current_character === 0) {
					break;
				}
				key += String.fromCharCode(current_character);
			}
			if (key.length === 0) {
				break;
			}
			let value = value_parser(stream);
			map[key] = value;
		}
		return map;
	}

	export function unpack_resource(
		ripe: ByteListView,
		size: null | number,
		type: number,
		list: Record<string, { type: number, offset: number, size: number; }>,
		directory: string,
	): void {
		let raw: ByteListView;
		if (size === null) {
			raw = ripe;
		}
		else {
			raw = new ByteListView(new ArrayBuffer(size));
			if (size !== 0) {
				let ripe_stream = Kernel.ByteStreamView.watch(Kernel.ByteListView.value(ripe.sub(0, ripe.size())));
				let raw_stream = Kernel.ByteStreamView.watch(Kernel.ByteListView.value(raw.sub(0, raw.size())));
				Kernel.Tool.Data.Compression.Deflate.Uncompress.process(ripe_stream, raw_stream, Kernel.Size.value(15n), Kernel.Tool.Data.Compression.Deflate.Wrapper.value('zlib'));
			}
		}
		for (let item in list) {
			let item_information = list[item];
			if (item_information.type === type) {
				KernelX.Storage.write_file(`${directory}/${item}`, raw.sub(item_information.offset, item_information.size));
			}
		}
		return;
	}

	export function process_package(
		package_data: ByteListView,
		package_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package,
		resource_directory: string,
	): void {
		package_definition.group = [];
		let package_header = {
			information_size: Number(package_data.u32(0x0C)),
			resource_path_size: Number(package_data.u32(0x10)),
			resource_path_offset: Number(package_data.u32(0x14)),
			subgroup_id_size: Number(package_data.u32(0x20)),
			subgroup_id_offset: Number(package_data.u32(0x24)),
			subgroup_information_block_count: Number(package_data.u32(0x28)),
			subgroup_information_offset: Number(package_data.u32(0x2C)),
			subgroup_information_block_size: Number(package_data.u32(0x30)),
			group_information_block_count: Number(package_data.u32(0x34)),
			group_information_offset: Number(package_data.u32(0x38)),
			group_information_block_size: Number(package_data.u32(0x3C)),
			group_id_size: Number(package_data.u32(0x40)),
			group_id_offset: Number(package_data.u32(0x44)),
			pool_information_block_count: Number(package_data.u32(0x48)),
			pool_information_offset: Number(package_data.u32(0x4C)),
			pool_information_block_size: Number(package_data.u32(0x50)),
			texture_resource_information_block_count: Number(package_data.u32(0x54)),
			texture_resource_information_offset: Number(package_data.u32(0x58)),
			texture_resource_information_block_size: Number(package_data.u32(0x5C)),
			manifest_group_information_offset: Number(package_data.u32(0x60)),
			manifest_resource_information_offset: Number(package_data.u32(0x64)),
			manifest_string_information_offset: Number(package_data.u32(0x68)),
			information_without_manifest_size: Number(package_data.u32(0x6C)),
		};
		let group_id_list = decode_compiled_map(
			new ByteListView(package_data.sub(package_header.group_id_offset, package_header.group_id_size)),
			(stream) => {
				return Number(stream.u32());
			},
		);
		let group_id_map = record_transform(group_id_list, (key, value) => ([String(value), key]));
		let subgroup_id_list = decode_compiled_map(
			new ByteListView(package_data.sub(package_header.subgroup_id_offset, package_header.subgroup_id_size)),
			(stream) => {
				return Number(stream.u32());
			},
		);
		let subgroup_id_map = record_transform(subgroup_id_list, (key, value) => ([String(value), key]));
		let resource_path_list = decode_compiled_map(
			new ByteListView(package_data.sub(package_header.resource_path_offset, package_header.resource_path_size)),
			(stream) => {
				return Number(stream.u32());
			},
		);
		for (let group_index = 0; group_index < package_header.group_information_block_count; group_index++) {
			let group_information_data = new ByteListView(package_data.sub(package_header.group_information_offset + package_header.group_information_block_size * group_index, package_header.group_information_block_size), 0);
			let subgroup_count = Number(group_information_data.u32(0x480));
			let group_id = group_id_map[group_index];
			if (group_id === undefined) {
				Console.warning(los('support.popcap.resource_stream_bundle.unpack_lenient:unknown_group_id'), []);
				group_id = `<unknown>:${group_index}`;
			}
			let group_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Group = {
				id: '',
				composite: true,
				subgroup: [],
			};
			if (group_id.endsWith('_COMPOSITESHELL')) {
				group_id = group_id.substring(0, group_id.length - '_COMPOSITESHELL'.length);
				group_definition.composite = false;
			}
			group_definition.id = group_id;
			package_definition.group.push(group_definition);
			for (let subgroup_index = 0; subgroup_index < subgroup_count; subgroup_index++) {
				try {
					let simple_subgroup_information_data = new ByteListView(group_information_data.sub(0x80 + 0x10 * subgroup_index, 0x10));
					let simple_subgroup_information = {
						index: Number(simple_subgroup_information_data.u32(0x0)),
						resolution: Number(simple_subgroup_information_data.u32(0x4)),
						locale: Number(simple_subgroup_information_data.u32(0x8)),
					};
					let subgroup_information_data = new ByteListView(package_data.sub(package_header.subgroup_information_offset + package_header.subgroup_information_block_size * simple_subgroup_information.index, package_header.subgroup_information_block_size), 0);
					let subgroup_information = {
						offset: Number(subgroup_information_data.u32(0x80)),
						size: Number(subgroup_information_data.u32(0x84)),
						pool: Number(subgroup_information_data.u32(0x88)),
						resource_data_store_mode: Number(subgroup_information_data.u32(0x8C)),
						information_size: Number(subgroup_information_data.u32(0x90)),
						general_resource_data_offset: Number(subgroup_information_data.u32(0x94)),
						general_resource_data_size: Number(subgroup_information_data.u32(0x98)),
						general_resource_data_size_original: Number(subgroup_information_data.u32(0x9C)),
						general_resource_data_size_pool: Number(subgroup_information_data.u32(0xA0)),
						texture_resource_data_offset: Number(subgroup_information_data.u32(0xA4)),
						texture_resource_data_size: Number(subgroup_information_data.u32(0xA8)),
						texture_resource_data_size_original: Number(subgroup_information_data.u32(0xAC)),
						texture_resource_data_size_pool: Number(subgroup_information_data.u32(0xB0)),
						texture_resource_count: Number(subgroup_information_data.u32(0xC4)),
						texture_resource_begin: Number(subgroup_information_data.u32(0xC8)),
					};
					let pool_information_data = new ByteListView(package_data.sub(package_header.pool_information_offset + package_header.pool_information_block_size * subgroup_information.pool, package_header.pool_information_block_size), 0);
					let pool_information = {
						texture_resource_data_offset: Number(pool_information_data.u32(0x80)),
						texture_resource_data_size: Number(pool_information_data.u32(0x84)),
					};
					subgroup_information.texture_resource_data_size_original = pool_information.texture_resource_data_size;
					let subgroup_id = subgroup_id_map[simple_subgroup_information.index];
					if (subgroup_id === undefined) {
						Console.warning(los('support.popcap.resource_stream_bundle.unpack_lenient:unknown_subgroup_id'), []);
						subgroup_id = `<unknown>:${simple_subgroup_information.index}`;
					}
					let subgroup_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Subgroup = {
						id: subgroup_id,
						category: {
							resolution: null,
							locale: null,
						},
						compression: {
							general: false,
							texture: false,
						},
						resource: [],
					};
					group_definition.subgroup.push(subgroup_definition);
					subgroup_definition.category.resolution = simple_subgroup_information.resolution === 0 ? null : BigInt(simple_subgroup_information.resolution);
					subgroup_definition.category.locale = simple_subgroup_information.locale === 0 ? null :
						String.fromCharCode((simple_subgroup_information.locale & 0xFF000000) >> 24) +
						String.fromCharCode((simple_subgroup_information.locale & 0x00FF0000) >> 16) +
						String.fromCharCode((simple_subgroup_information.locale & 0x0000FF00) >> 8) +
						String.fromCharCode((simple_subgroup_information.locale & 0x000000FF) >> 0);
					subgroup_definition.compression.general = (subgroup_information.resource_data_store_mode & 0b10) !== 0;
					subgroup_definition.compression.texture = (subgroup_information.resource_data_store_mode & 0b01) !== 0;
					Console.verbosity(`${group_index}.${subgroup_index} ${simple_subgroup_information.index} ${group_id}.${subgroup_id} ${subgroup_information.offset.toString(16)}`, []);
					let packet_data = new ByteListView(package_data.sub(subgroup_information.offset, package_data.size() - subgroup_information.offset));
					let packet_header = {
						resource_data_store_mode: Number(packet_data.u32(0x10)),
						information_size: Number(packet_data.u32(0x14)),
						general_resource_data_offset: Number(packet_data.u32(0x18)),
						general_resource_data_size: Number(packet_data.u32(0x1C)),
						general_resource_data_size_original: Number(packet_data.u32(0x20)),
						texture_resource_data_offset: Number(packet_data.u32(0x28)),
						texture_resource_data_size: Number(packet_data.u32(0x2C)),
						texture_resource_data_size_original: Number(packet_data.u32(0x30)),
						resource_information_size: Number(packet_data.u32(0x48)),
						resource_information_offset: Number(packet_data.u32(0x4C)),
					};
					let packet_resource_information_list = decode_compiled_map(
						new ByteListView(packet_data.sub(packet_header.resource_information_offset, packet_header.resource_information_size)),
						(stream) => {
							let result: {
								type: number;
								offset: number;
								size: number;
								additional: null | {
									index: number;
									width: number;
									height: number;
								};
							} = {} as any;
							result.type = Number(stream.u32());
							result.offset = Number(stream.u32());
							result.size = Number(stream.u32());
							if (result.type === 1) {
								result.additional = {} as any;
								result.additional!.index = Number(stream.u32());
								stream.u32();
								stream.u32();
								result.additional!.width = Number(stream.u32());
								result.additional!.height = Number(stream.u32());
							}
							return result;
						},
					);
					for (let packet_resource_path in packet_resource_information_list) {
						let packet_resource_information = packet_resource_information_list[packet_resource_path];
						let resource_definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Resource = {
							path: PathUtility.regularize(packet_resource_path),
							additional: undefined!,
						};
						subgroup_definition.resource.push(resource_definition);
						if (packet_resource_information.type === 0) {
							resource_definition.additional = {
								type: 'general',
								value: {
								},
							};
						}
						if (packet_resource_information.type === 1) {
							let texture_index = subgroup_information.texture_resource_begin + packet_resource_information.additional!.index;
							let texture_information_data = new ByteListView(package_data.sub(package_header.texture_resource_information_offset + package_header.texture_resource_information_block_size * texture_index, package_header.texture_resource_information_block_size));
							resource_definition.additional = {
								type: 'texture',
								value: {
									size: [
										texture_information_data.u32(0x00),
										texture_information_data.u32(0x04),
									],
									format: texture_information_data.u32(0x0C),
									pitch: texture_information_data.u32(0x08),
								},
							};
						}
					}
					unpack_resource(
						new ByteListView(packet_data.sub(subgroup_information.general_resource_data_offset, subgroup_information.general_resource_data_size)),
						(subgroup_information.resource_data_store_mode & 0b10) === 0 ? null : subgroup_information.general_resource_data_size_original,
						0,
						packet_resource_information_list,
						resource_directory,
					);
					unpack_resource(
						new ByteListView(packet_data.sub(subgroup_information.texture_resource_data_offset, subgroup_information.texture_resource_data_size)),
						(subgroup_information.resource_data_store_mode & 0b01) === 0 ? null : subgroup_information.texture_resource_data_size_original,
						1,
						packet_resource_information_list,
						resource_directory,
					);
				}
				catch (e) {
					Console.error_of(e);
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	export function process(
		data: ByteListView,
		definition: Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package,
		resource_directory: string,
	): void {
		return process_package(data, definition, resource_directory);
	}

	// ------------------------------------------------

	export function process_fs(
		data_file: string,
		definition_file: string,
		manifest_file: string,
		resource_directory: string,
	): void {
		let data = KernelX.Storage.read_file(data_file);
		let definition = {} as Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Package;
		process(new ByteListView(data.view().value), definition, resource_directory);
		KernelX.JSON.write_fs_js(definition_file, definition);
		KernelX.JSON.write_fs_js(manifest_file, null);
		return;
	}

	// ------------------------------------------------

}