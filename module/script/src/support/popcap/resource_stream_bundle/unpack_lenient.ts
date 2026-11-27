namespace Twinning.Script.Support.Popcap.ResourceStreamBundle.UnpackLenient {

	// #region utility

	function decode_compiled_map<TValue>(
		data: BinaryStreamView,
		value_parser: (stream: BinaryStreamView) => TValue,
	): Record<string, TValue> {
		let stream = data.split_sub(0, data.size());
		let parent_string: Record<number, string> = {};
		let map: Record<string, TValue> = {};
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
		data.pr(stream.p());
		return map;
	}

	function unpack_resource(
		ripe: BinaryStreamView,
		size: null | number,
		type: number,
		list: Record<string, {type: number, offset: number, size: number}>,
		directory: StoragePath,
	): void {
		let raw: BinaryStreamView;
		if (size === null) {
			raw = ripe;
		}
		else {
			raw = new BinaryStreamView(new DataView(new ArrayBuffer(size)));
			if (size !== 0) {
				let ripe_stream = Kernel.BinaryStreamView.watch(Kernel.BinaryView.value(ripe.sub_buffer(0, ripe.size())));
				let raw_stream = Kernel.BinaryStreamView.watch(Kernel.BinaryView.value(raw.sub_buffer(0, raw.size())));
				Kernel.Tool.Data.Compression.Deflate.Uncompress.process(raw_stream, ripe_stream, Kernel.Tool.Data.Compression.Deflate.Container.value('zlib'), Kernel.Integer.value(15n));
			}
		}
		for (let item in list) {
			let item_information = list[item];
			if (item_information.type === type) {
				StorageHelper.write_file_data(directory.push(new StoragePath(item)), Kernel.BinaryView.value(raw.sub_buffer(item_information.offset, item_information.size)));
			}
		}
		return;
	}

	function process_package(
		package_data: BinaryStreamView,
		package_definition: Kernel.Tool.Popcap.ResourceStreamBundle.Definition.JS_N.Package,
		resource_directory: StoragePath,
	): void {
		let seek_data_to = (stream: BinaryStreamView, position: number): BinaryStreamView => {
			package_data.p(position);
			return package_data;
		};
		package_definition.group = [];
		let package_header = {
			information_size: Number(seek_data_to(package_data, 0x0C).u32()),
			resource_path_size: Number(seek_data_to(package_data, 0x10).u32()),
			resource_path_offset: Number(seek_data_to(package_data, 0x14).u32()),
			subgroup_identifier_size: Number(seek_data_to(package_data, 0x20).u32()),
			subgroup_identifier_offset: Number(seek_data_to(package_data, 0x24).u32()),
			subgroup_information_block_count: Number(seek_data_to(package_data, 0x28).u32()),
			subgroup_information_offset: Number(seek_data_to(package_data, 0x2C).u32()),
			subgroup_information_block_size: Number(seek_data_to(package_data, 0x30).u32()),
			group_information_block_count: Number(seek_data_to(package_data, 0x34).u32()),
			group_information_offset: Number(seek_data_to(package_data, 0x38).u32()),
			group_information_block_size: Number(seek_data_to(package_data, 0x3C).u32()),
			group_identifier_size: Number(seek_data_to(package_data, 0x40).u32()),
			group_identifier_offset: Number(seek_data_to(package_data, 0x44).u32()),
			pool_information_block_count: Number(seek_data_to(package_data, 0x48).u32()),
			pool_information_offset: Number(seek_data_to(package_data, 0x4C).u32()),
			pool_information_block_size: Number(seek_data_to(package_data, 0x50).u32()),
			texture_resource_information_block_count: Number(seek_data_to(package_data, 0x54).u32()),
			texture_resource_information_offset: Number(seek_data_to(package_data, 0x58).u32()),
			texture_resource_information_block_size: Number(seek_data_to(package_data, 0x5C).u32()),
			manifest_group_information_offset: Number(seek_data_to(package_data, 0x60).u32()),
			manifest_resource_information_offset: Number(seek_data_to(package_data, 0x64).u32()),
			manifest_string_information_offset: Number(seek_data_to(package_data, 0x68).u32()),
			information_without_manifest_size: Number(seek_data_to(package_data, 0x6C).u32()),
		};
		let group_identifier_list = decode_compiled_map(
			package_data.split_sub(package_header.group_identifier_offset, package_header.group_identifier_size),
			(stream) => {
				return Number(stream.u32());
			},
		);
		let group_identifier_map = ConvertHelper.record_transform(group_identifier_list, (key, value) => ([String(value), key]));
		let subgroup_identifier_list = decode_compiled_map(
			package_data.split_sub(package_header.subgroup_identifier_offset, package_header.subgroup_identifier_size),
			(stream) => {
				return Number(stream.u32());
			},
		);
		let subgroup_identifier_map = ConvertHelper.record_transform(subgroup_identifier_list, (key, value) => ([String(value), key]));
		let resource_path_list = decode_compiled_map(
			package_data.split_sub(package_header.resource_path_offset, package_header.resource_path_size),
			(stream) => {
				return Number(stream.u32());
			},
		);
		for (let group_index = 0; group_index < package_header.group_information_block_count; group_index++) {
			let group_information_data = package_data.split_sub(package_header.group_information_offset + package_header.group_information_block_size * group_index, package_header.group_information_block_size);
			let subgroup_count = Number(seek_data_to(group_information_data, 0x480).u32());
			let group_identifier = group_identifier_map[group_index];
			if (group_identifier === undefined) {
				Console.warning(los('support.popcap.resource_stream_bundle.unpack_lenient:unknown_group_identifier'), []);
				group_identifier = `<unknown>:${group_index}`;
			}
			let group_definition: Kernel.Tool.Popcap.ResourceStreamBundle.Definition.JS_N.Group = {
				identifier: '',
				composite: true,
				subgroup: [],
			};
			if (group_identifier.endsWith('_COMPOSITESHELL')) {
				group_identifier = group_identifier.substring(0, group_identifier.length - '_COMPOSITESHELL'.length);
				group_definition.composite = false;
			}
			group_definition.identifier = group_identifier;
			package_definition.group.push(group_definition);
			for (let subgroup_index = 0; subgroup_index < subgroup_count; subgroup_index++) {
				try {
					let simple_subgroup_information_data = group_information_data.split_sub(0x80 + 0x10 * subgroup_index, 0x10);
					let simple_subgroup_information = {
						index: Number(seek_data_to(simple_subgroup_information_data, 0x0).u32()),
						resolution: Number(seek_data_to(simple_subgroup_information_data, 0x4).u32()),
						locale: Number(seek_data_to(simple_subgroup_information_data, 0x8).u32()),
					};
					let subgroup_information_data = package_data.split_sub(package_header.subgroup_information_offset + package_header.subgroup_information_block_size * simple_subgroup_information.index, package_header.subgroup_information_block_size);
					let subgroup_information = {
						offset: Number(seek_data_to(subgroup_information_data, 0x80).u32()),
						size: Number(seek_data_to(subgroup_information_data, 0x84).u32()),
						pool: Number(seek_data_to(subgroup_information_data, 0x88).u32()),
						resource_data_store_mode: Number(seek_data_to(subgroup_information_data, 0x8C).u32()),
						information_size: Number(seek_data_to(subgroup_information_data, 0x90).u32()),
						general_resource_data_offset: Number(seek_data_to(subgroup_information_data, 0x94).u32()),
						general_resource_data_size: Number(seek_data_to(subgroup_information_data, 0x98).u32()),
						general_resource_data_size_original: Number(seek_data_to(subgroup_information_data, 0x9C).u32()),
						general_resource_data_size_pool: Number(seek_data_to(subgroup_information_data, 0xA0).u32()),
						texture_resource_data_offset: Number(seek_data_to(subgroup_information_data, 0xA4).u32()),
						texture_resource_data_size: Number(seek_data_to(subgroup_information_data, 0xA8).u32()),
						texture_resource_data_size_original: Number(seek_data_to(subgroup_information_data, 0xAC).u32()),
						texture_resource_data_size_pool: Number(seek_data_to(subgroup_information_data, 0xB0).u32()),
						texture_resource_count: Number(seek_data_to(subgroup_information_data, 0xC4).u32()),
						texture_resource_begin: Number(seek_data_to(subgroup_information_data, 0xC8).u32()),
					};
					let pool_information_data = package_data.split_sub(package_header.pool_information_offset + package_header.pool_information_block_size * subgroup_information.pool, package_header.pool_information_block_size);
					let pool_information = {
						texture_resource_data_offset: Number(seek_data_to(pool_information_data, 0x80).u32()),
						texture_resource_data_size: Number(seek_data_to(pool_information_data, 0x84).u32()),
					};
					subgroup_information.texture_resource_data_size_original = pool_information.texture_resource_data_size;
					let subgroup_identifier = subgroup_identifier_map[simple_subgroup_information.index];
					if (subgroup_identifier === undefined) {
						Console.warning(los('support.popcap.resource_stream_bundle.unpack_lenient:unknown_subgroup_identifier'), []);
						subgroup_identifier = `<unknown>:${simple_subgroup_information.index}`;
					}
					let subgroup_definition: Kernel.Tool.Popcap.ResourceStreamBundle.Definition.JS_N.Subgroup = {
						identifier: subgroup_identifier,
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
					Console.verbosity(`${group_index}.${subgroup_index} ${simple_subgroup_information.index} ${group_identifier}.${subgroup_identifier} ${subgroup_information.offset.toString(16)}`, []);
					let packet_data = package_data.split_sub(subgroup_information.offset, package_data.size() - subgroup_information.offset);
					let packet_header = {
						resource_data_store_mode: Number(seek_data_to(packet_data, 0x10).u32()),
						information_size: Number(seek_data_to(packet_data, 0x14).u32()),
						general_resource_data_offset: Number(seek_data_to(packet_data, 0x18).u32()),
						general_resource_data_size: Number(seek_data_to(packet_data, 0x1C).u32()),
						general_resource_data_size_original: Number(seek_data_to(packet_data, 0x20).u32()),
						texture_resource_data_offset: Number(seek_data_to(packet_data, 0x28).u32()),
						texture_resource_data_size: Number(seek_data_to(packet_data, 0x2C).u32()),
						texture_resource_data_size_original: Number(seek_data_to(packet_data, 0x30).u32()),
						resource_information_size: Number(seek_data_to(packet_data, 0x48).u32()),
						resource_information_offset: Number(seek_data_to(packet_data, 0x4C).u32()),
					};
					let packet_resource_information_list = decode_compiled_map(
						packet_data.split_sub(packet_header.resource_information_offset, packet_header.resource_information_size),
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
						let resource_definition: Kernel.Tool.Popcap.ResourceStreamBundle.Definition.JS_N.Resource = {
							path: new StoragePath(packet_resource_path).emit(),
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
							let texture_information_data = package_data.split_sub(package_header.texture_resource_information_offset + package_header.texture_resource_information_block_size * texture_index, package_header.texture_resource_information_block_size);
							resource_definition.additional = {
								type: 'texture',
								value: {
									size: [
										seek_data_to(texture_information_data, 0x00).u32(),
										seek_data_to(texture_information_data, 0x04).u32(),
									],
									format: seek_data_to(texture_information_data, 0x0C).u32(),
									pitch: seek_data_to(texture_information_data, 0x08).u32(),
								},
							};
						}
					}
					unpack_resource(
						packet_data.split_sub(subgroup_information.general_resource_data_offset, subgroup_information.general_resource_data_size),
						(subgroup_information.resource_data_store_mode & 0b10) === 0 ? null : subgroup_information.general_resource_data_size_original,
						0,
						packet_resource_information_list,
						resource_directory,
					);
					unpack_resource(
						packet_data.split_sub(subgroup_information.texture_resource_data_offset, subgroup_information.texture_resource_data_size),
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

	// ----------------

	export function process(
		data: BinaryStreamView,
		definition: Kernel.Tool.Popcap.ResourceStreamBundle.Definition.JS_N.Package,
		resource_directory: StoragePath,
	): void {
		return process_package(data, definition, resource_directory);
	}

	// ----------------

	export function process_fs(
		data_file: StoragePath,
		definition_file: StoragePath,
		manifest_file: StoragePath,
		resource_directory: StoragePath,
	): void {
		let data = StorageHelper.read_file_data(data_file);
		let definition = {} as Kernel.Tool.Popcap.ResourceStreamBundle.Definition.JS_N.Package;
		process(new BinaryStreamView(new DataView(data.view().value)), definition, resource_directory);
		JsonHelper.encode_file(definition_file, definition);
		JsonHelper.encode_file(manifest_file, null);
		return;
	}

	// #endregion

}
