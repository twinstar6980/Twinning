namespace TwinStar.Script.Support.PopCap.ResourceStreamBundle.Repair {

	// ------------------------------------------------

	export function create_empty_packet(
	): ArrayBuffer {
		let data = new ArrayBuffer(0x1000);
		new Uint32Array(data).fill(0x00000000);
		let view = new ByteListView(data, 0x0);
		// magic
		view.u32(0x00, 0x72736770n);
		// version_number
		view.u32(0x04, 0x00000004n);
		// unused_1
		// resource_data_section_store_mode
		view.u32(0x10, 0x00000000n);
		// information_section_size
		view.u32(0x14, 0x00001000n);
		// generic_resource_data_section_offset
		view.u32(0x18, 0x00001000n);
		// generic_resource_data_section_size
		view.u32(0x1C, 0x00000000n);
		// generic_resource_data_section_size_original
		view.u32(0x20, 0x00000000n);
		// unused_2
		// texture_resource_data_section_offset
		view.u32(0x28, 0x00001000n);
		// texture_resource_data_section_size
		view.u32(0x2C, 0x00000000n);
		// texture_resource_data_section_size_original
		view.u32(0x30, 0x00000000n);
		// unused_3
		// resource_information_section_size
		view.u32(0x48, 0x00000000n);
		// resource_information_section_offset
		view.u32(0x4C, 0x0000005Cn);
		// unused_4
		// done
		return data;
	}

	export function test_zlib_ripe(
		data: ArrayBuffer,
		offset: bigint,
		ripe_size: bigint,
		raw_size: bigint,
	): boolean {
		if (ripe_size === 0n) {
			return true;
		}
		let ripe = Core.ByteStreamView.watch(Core.ByteListView.value(data).sub(Core.Size.value(BigInt(offset)), Core.Size.value(BigInt(ripe_size))));
		let raw_buffer = Core.ByteArray.allocate(Core.Size.value(BigInt(raw_size)));
		let raw = Core.ByteStreamView.watch(raw_buffer.view());
		try {
			Core.Tool.Data.Compression.Deflate.Uncompress.process_whole(ripe, raw, Core.Size.value(15n), Core.Tool.Data.Compression.Deflate.Wrapper.value('zlib'));
			return true;
		} catch (e: any) {
			return false;
		}
	}

	// ------------------------------------------------

	export function repair_packet(
		data: ArrayBuffer,
	): void {
		let view = new ByteListView(data, 0x0);
		// magic
		view.u32(0x00, 0x72736770n);
		// version_number
		view.u32(0x04, 0x00000004n);
		// unused_1
		view.u32(0x08, 0x00000000n);
		view.u32(0x0C, 0x00000000n);
		// resource_data_section_store_mode
		let resource_data_section_store_mode = view.u32(0x10);
		view.u32(0x10, resource_data_section_store_mode & 0b11n);
		// information_section_size
		view.u32(0x14, BigInt(Math.max(Number(view.u32(0x14)), 0x1000)));
		// generic_resource_data_section_offset
		let generic_resource_data_section_offset = view.u32(0x18);
		// generic_resource_data_section_size
		let generic_resource_data_section_size = view.u32(0x1C);
		// generic_resource_data_section_size_original
		let generic_resource_data_section_size_original = view.u32(0x20);
		// unused_2
		view.u32(0x24, 0x00000000n);
		// texture_resource_data_section_offset
		let texture_resource_data_section_offset = view.u32(0x28);
		// texture_resource_data_section_size
		let texture_resource_data_section_size = view.u32(0x2C);
		// texture_resource_data_section_size_original
		let texture_resource_data_section_size_original = view.u32(0x30);
		// unused_3
		view.u32(0x34, 0x00000000n);
		view.u32(0x38, 0x00000000n);
		view.u32(0x3C, 0x00000000n);
		view.u32(0x40, 0x00000000n);
		view.u32(0x44, 0x00000000n);
		// resource_information_section_size
		// resource_information_section_offset
		// unused_4
		view.u32(0x50, 0x00000000n);
		view.u32(0x54, 0x00000000n);
		view.u32(0x58, 0x00000000n);
		// test zlib ripe
		if (((resource_data_section_store_mode & 0b10n) && !test_zlib_ripe(data, generic_resource_data_section_offset, generic_resource_data_section_size, generic_resource_data_section_size_original)) ||
			((resource_data_section_store_mode & 0b01n) && !test_zlib_ripe(data, texture_resource_data_section_offset, texture_resource_data_section_size, texture_resource_data_section_size_original))) {
			Console.warning(los('support.popcap.resource_stream_bundle.repair:invalid_zlib'), [
				`generic offset=${generic_resource_data_section_offset.toString(16)} size=${generic_resource_data_section_size.toString(16)}`,
				`texture offset=${texture_resource_data_section_offset.toString(16)} size=${texture_resource_data_section_size.toString(16)}`,
			]);
			view.u32(0x10, 0x00000000n);
			view.u32(0x1C, 0x00000000n);
			view.u32(0x20, 0x00000000n);
			view.u32(0x2C, 0x00000000n);
			view.u32(0x30, 0x00000000n);
			view.u32(0x48, 0x00000000n);
		}
		// done
		return;
	}

	export function repair_package(
		data: ArrayBuffer,
	): void {
		let view = new ByteListView(data, 0x0);
		// magic
		view.u32(0x00, 0x72736231n);
		// version_number
		view.u32(0x04, 0x00000004n);
		// unused_1
		view.u32(0x08, 0x00000000n);
		// information_section_size
		// resource_path_section_size
		// resource_path_section_offset
		// unused_2
		view.u32(0x18, 0x00000000n);
		view.u32(0x1C, 0x00000000n);
		// subgroup_id_section_size
		// subgroup_id_section_offset
		// subgroup_information_section_block_count
		let subgroup_information_section_block_count = view.u32(0x28);
		// subgroup_information_section_offset
		let subgroup_information_section_offset = view.u32(0x2C);
		// subgroup_information_section_block_size
		let subgroup_information_section_block_size = view.u32(0x30);
		// group_information_section_block_count
		let group_information_section_block_count = view.u32(0x34);
		// group_information_section_offset
		let group_information_section_offset = view.u32(0x38);
		// group_information_section_block_size
		let group_information_section_block_size = view.u32(0x3C);
		// group_id_section_size
		// group_id_section_offset
		// subgroup_pool_information_section_block_count
		let subgroup_pool_information_section_block_count = view.u32(0x48);
		// subgroup_pool_information_section_offset
		let subgroup_pool_information_section_offset = view.u32(0x4C);
		// subgroup_pool_information_section_block_size
		let subgroup_pool_information_section_block_size = view.u32(0x50);
		// texture_resource_information_section_block_count
		// texture_resource_information_section_offset
		// texture_resource_information_section_block_size
		// description_group_section_offset
		// description_resource_section_offset
		// description_string_section_offset
		// information_without_description_section_size
		// TODO : 6C
		// group_information_section
		for (let group_index = 0; group_index < group_information_section_block_count; ++group_index) {
			let view = new ByteListView(data, Number(group_information_section_offset) + Number(group_information_section_block_size) * group_index);
			// id
			// subgroup_count
			let subgroup_count = view.u32(0x480);
			// subgroup_information
			for (let subgroup_index = 0; subgroup_index < 64; ++subgroup_index) {
				let view = new ByteListView(data, Number(group_information_section_offset) + Number(group_information_section_block_size) * group_index + 0x80 + 0x10 * subgroup_index);
				if (subgroup_index < subgroup_count) {
					// index
					// resolution
					// locale
					// unused_1
					view.u32(0x0C, 0x00000000n);
				} else {
					view.u32(0x00, 0x00000000n);
					view.u32(0x04, 0x00000000n);
					view.u32(0x08, 0x00000000n);
					view.u32(0x0C, 0x00000000n);
				}
			}
		}
		// subgroup_information_section
		let progress = new TextGenerator.Progress('fraction', false, 20, Number(subgroup_information_section_block_count));
		for (let subgroup_index = 0; subgroup_index < subgroup_information_section_block_count; ++subgroup_index) {
			progress.increase();
			Console.information(`${progress}`, []);
			let view = new ByteListView(data, Number(subgroup_information_section_offset) + Number(subgroup_information_section_block_size) * subgroup_index);
			let pool_view = new ByteListView(data, Number(subgroup_pool_information_section_offset) + Number(subgroup_pool_information_section_block_size) * subgroup_index);
			// id
			// offset
			let offset = view.u32(0x80);
			// size
			// index
			// resource_data_section_store_mode
			let resource_data_section_store_mode = view.u32(0x8C);
			view.u32(0x8C, resource_data_section_store_mode & 0b11n);
			// information_section_size
			let information_section_size = view.u32(0x90);
			// generic_resource_data_section_offset
			let generic_resource_data_section_offset = view.u32(0x94);
			// generic_resource_data_section_size
			let generic_resource_data_section_size = view.u32(0x98);
			// generic_resource_data_section_size_original
			let generic_resource_data_section_size_original = view.u32(0x9C);
			// generic_resource_data_section_size_original_1
			// texture_resource_data_section_offset
			let texture_resource_data_section_offset = view.u32(0xA4);
			// texture_resource_data_section_size
			let texture_resource_data_section_size = view.u32(0xA8);
			// texture_resource_data_section_size_original
			view.u32(0xAC, pool_view.u32(0x84));
			let texture_resource_data_section_size_original = view.u32(0xAC);
			// unused_1
			view.u32(0xB0, 0x00000000n);
			view.u32(0xB4, 0x00000000n);
			view.u32(0xB8, 0x00000000n);
			view.u32(0xBC, 0x00000000n);
			view.u32(0xC0, 0x00000000n);
			// texture_resource_count
			// texture_resource_begin_index
			// packet
			let actual_packet_size = Math.max(Number(information_section_size), Number(generic_resource_data_section_offset + generic_resource_data_section_size), Number(texture_resource_data_section_offset + texture_resource_data_section_size));
			view.u32(0x84, BigInt(actual_packet_size));
			if (actual_packet_size === 0 || Number(offset) + actual_packet_size > data.byteLength) {
				// replace packet information for invalid packet
				Console.warning(los('support.popcap.resource_stream_bundle.repair:invalid_packet'), [
					`subgroup index=${subgroup_index} offset=${offset.toString(16)} size=${actual_packet_size.toString(16)}`,
				]);
				view.u32(0x80, BigInt(data.byteLength));
				view.u32(0x84, 0x00001000n);
				view.u32(0x8C, 0x00000000n);
				view.u32(0x90, 0x00001000n);
				view.u32(0x94, 0x00001000n);
				view.u32(0x98, 0x00000000n);
				view.u32(0x9C, 0x00000000n);
				view.u32(0xA0, 0x00000000n);
				view.u32(0xA4, 0x00001000n);
				view.u32(0xA8, 0x00000000n);
				view.u32(0xAC, 0x00000000n);
				view.u32(0xC4, 0x00000000n);
				view.u32(0xC8, 0x00000000n);
			} else {
				// repair packet
				let packet_view = new ByteListView(data, Number(offset), actual_packet_size);
				// magic
				packet_view.u32(0x00, 0x72736770n);
				// version_number
				packet_view.u32(0x04, 0x00000004n);
				// unused_1
				packet_view.u32(0x08, 0x00000000n);
				packet_view.u32(0x0C, 0x00000000n);
				// resource_data_section_store_mode
				packet_view.u32(0x10, resource_data_section_store_mode & 0b11n);
				// information_section_size
				packet_view.u32(0x14, information_section_size);
				// generic_resource_data_section_offset
				packet_view.u32(0x18, generic_resource_data_section_offset);
				// generic_resource_data_section_size
				packet_view.u32(0x1C, generic_resource_data_section_size);
				// generic_resource_data_section_size_original
				packet_view.u32(0x20, generic_resource_data_section_size_original);
				// unused_2
				packet_view.u32(0x24, 0x00000000n);
				// texture_resource_data_section_offset
				packet_view.u32(0x28, texture_resource_data_section_offset);
				// texture_resource_data_section_size
				packet_view.u32(0x2C, texture_resource_data_section_size);
				// texture_resource_data_section_size_original
				packet_view.u32(0x30, texture_resource_data_section_size_original);
				// unused_3
				packet_view.u32(0x34, 0x00000000n);
				packet_view.u32(0x38, 0x00000000n);
				packet_view.u32(0x3C, 0x00000000n);
				packet_view.u32(0x40, 0x00000000n);
				packet_view.u32(0x44, 0x00000000n);
				// resource_information_section_size
				let resource_information_section_size = packet_view.u32(0x48);
				// resource_information_section_offset
				let resource_information_section_offset = packet_view.u32(0x4C);
				// unused_4
				packet_view.u32(0x50, 0x00000000n);
				packet_view.u32(0x54, 0x00000000n);
				packet_view.u32(0x58, 0x00000000n);
				// test resource_information_section
				if (resource_information_section_offset + resource_information_section_size > information_section_size) {
					resource_information_section_offset = 0x00000000n;
					resource_information_section_size = 0x00000000n;
				}
				if (resource_information_section_size !== 0n) {
					let first_block = packet_view.u32(Number(resource_information_section_offset));
					if (first_block === 0n) {
						resource_information_section_size = 0x00000000n;
					}
				}
				packet_view.u32(0x48, resource_information_section_size);
				packet_view.u32(0x4C, resource_information_section_offset);
				// test zlib ripe
				if (((resource_data_section_store_mode & 0b10n) && !test_zlib_ripe(data, offset + generic_resource_data_section_offset, generic_resource_data_section_size, generic_resource_data_section_size_original)) ||
					((resource_data_section_store_mode & 0b01n) && !test_zlib_ripe(data, offset + texture_resource_data_section_offset, texture_resource_data_section_size, texture_resource_data_section_size_original))) {
					Console.warning(los('support.popcap.resource_stream_bundle.repair:invalid_zlib'), [
						`generic offset=${generic_resource_data_section_offset.toString(16)} size=${generic_resource_data_section_size.toString(16)}`,
						`texture offset=${texture_resource_data_section_offset.toString(16)} size=${texture_resource_data_section_size.toString(16)}`,
					]);
					packet_view.u32(0x10, 0x00000000n);
					packet_view.u32(0x1C, 0x00000000n);
					packet_view.u32(0x20, 0x00000000n);
					packet_view.u32(0x2C, 0x00000000n);
					packet_view.u32(0x30, 0x00000000n);
					packet_view.u32(0x48, 0x00000000n);
					view.u32(0x8C, packet_view.u32(0x10));
					view.u32(0x90, packet_view.u32(0x14));
					view.u32(0x94, packet_view.u32(0x18));
					view.u32(0x98, packet_view.u32(0x1C));
					view.u32(0x9C, packet_view.u32(0x20));
					view.u32(0xA0, packet_view.u32(0x20));
					view.u32(0xA4, packet_view.u32(0x28));
					view.u32(0xA8, packet_view.u32(0x2C));
					view.u32(0xAC, packet_view.u32(0x30));
					actual_packet_size = Math.max(Number(packet_view.u32(0x14)), Number(packet_view.u32(0x18) + packet_view.u32(0x1C)), Number(packet_view.u32(0x28) + packet_view.u32(0x2C)));
					view.u32(0x84, BigInt(actual_packet_size));
				}
			}
		}
		// done
		return;
	}

	// ------------------------------------------------

	export function repair_package_fs(
		raw_file: string,
		ripe_file: string,
	): void {
		let data = CoreX.FileSystem.read_file(raw_file);
		repair_package(data.view().value);
		let empty_packet = create_empty_packet();
		let data_with_empty_packet = Core.ByteArray.allocate(Core.Size.value(data.size().value + 0x1000n));
		new Uint8Array(data_with_empty_packet.view().sub(Core.Size.value(0x0n), data.size()).value).set(new Uint8Array(data.view().value));
		new Uint32Array(data_with_empty_packet.view().sub(data.size(), Core.Size.value(0x1000n)).value).set(new Uint32Array(empty_packet));
		CoreX.FileSystem.write_file(ripe_file, data_with_empty_packet);
		return;
	}

	// ------------------------------------------------

}