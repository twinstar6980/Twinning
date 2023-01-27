/** 尽可能地将RSB修复为标准结构，以免核心处理逻辑因拒绝容错的特性而无法解包被刻意处理过的RSB */
namespace TwinStar.Script.RepairRSB {

	// ------------------------------------------------

	function create_empty_packet(
	): ArrayBuffer {
		let data = new ArrayBuffer(0x1000);
		new Uint32Array(data).fill(0x00000000);
		let view = new DataView(data, 0x0);
		// magic
		view.setUint32(0x00, 0x72736770, true);
		// version_number
		view.setUint32(0x04, 0x00000004, true);
		// unused_1
		// resource_data_section_store_mode
		view.setUint32(0x10, 0x00000000, true);
		// information_section_size
		view.setUint32(0x14, 0x00001000, true);
		// generic_resource_data_section_offset
		view.setUint32(0x18, 0x00001000, true);
		// generic_resource_data_section_size
		view.setUint32(0x1C, 0x00000000, true);
		// generic_resource_data_section_size_original
		view.setUint32(0x20, 0x00000000, true);
		// unused_2
		// texture_resource_data_section_offset
		view.setUint32(0x28, 0x00001000, true);
		// texture_resource_data_section_size
		view.setUint32(0x2C, 0x00000000, true);
		// texture_resource_data_section_size_original
		view.setUint32(0x30, 0x00000000, true);
		// unused_3
		// resource_information_section_size
		view.setUint32(0x48, 0x00000000, true);
		// resource_information_section_offset
		view.setUint32(0x4C, 0x0000005C, true);
		// unused_4
		// done
		return data;
	}

	function test_zlib_ripe(
		data: ArrayBuffer,
		offset: number,
		ripe_size: number,
		raw_size: number,
	): boolean {
		if (ripe_size === 0) {
			return true;
		}
		let ripe = Core.ByteStreamView.look(Core.ByteListView.value(data).sub(Core.Size.value(BigInt(offset)), Core.Size.value(BigInt(ripe_size))));
		let raw_buffer = Core.ByteArray.allocate(Core.Size.value(BigInt(raw_size)));
		let raw = Core.ByteStreamView.look(raw_buffer.view());
		try {
			Core.Tool.Data.Compress.Deflate.Uncompress.process_whole(ripe, raw, Core.Size.value(15n), Core.Tool.Data.Compress.Deflate.Wrapper.value('zlib'));
			return true;
		} catch (e: any) {
			return false;
		}
	}

	function repair_packet(
		data: ArrayBuffer,
		offset: number,
	): void {
		let view = new DataView(data, offset);
		// magic
		view.setUint32(0x00, 0x72736770, true);
		// version_number
		view.setUint32(0x04, 0x00000004, true);
		// unused_1
		view.setUint32(0x08, 0x00000000, true);
		view.setUint32(0x0C, 0x00000000, true);
		// resource_data_section_store_mode
		let resource_data_section_store_mode = view.getUint32(0x10, true);
		view.setUint32(0x10, resource_data_section_store_mode & 0b11, true);
		// information_section_size
		view.setUint32(0x14, Math.max(view.getUint32(0x14, true), 0x1000), true);
		// generic_resource_data_section_offset
		let generic_resource_data_section_offset = view.getUint32(0x18, true);
		// generic_resource_data_section_size
		let generic_resource_data_section_size = view.getUint32(0x1C, true);
		// generic_resource_data_section_size_original
		let generic_resource_data_section_size_original = view.getUint32(0x20, true);
		// unused_2
		view.setUint32(0x24, 0x00000000, true);
		// texture_resource_data_section_offset
		let texture_resource_data_section_offset = view.getUint32(0x28, true);
		// texture_resource_data_section_size
		let texture_resource_data_section_size = view.getUint32(0x2C, true);
		// texture_resource_data_section_size_original
		let texture_resource_data_section_size_original = view.getUint32(0x30, true);
		// unused_3
		view.setUint32(0x34, 0x00000000, true);
		view.setUint32(0x38, 0x00000000, true);
		view.setUint32(0x3C, 0x00000000, true);
		view.setUint32(0x40, 0x00000000, true);
		view.setUint32(0x44, 0x00000000, true);
		// resource_information_section_size
		// resource_information_section_offset
		// unused_4
		view.setUint32(0x50, 0x00000000, true);
		view.setUint32(0x54, 0x00000000, true);
		view.setUint32(0x58, 0x00000000, true);
		// test zlib ripe
		if (((resource_data_section_store_mode & 0b10) && !test_zlib_ripe(data, offset + generic_resource_data_section_offset, generic_resource_data_section_size, generic_resource_data_section_size_original)) ||
			((resource_data_section_store_mode & 0b01) && !test_zlib_ripe(data, offset + texture_resource_data_section_offset, texture_resource_data_section_size, texture_resource_data_section_size_original))) {
			Console.notify('w', los(`检测到ZLib异常，现截断数据`), [
				`generic offset=${generic_resource_data_section_offset.toString(16)} size=${generic_resource_data_section_size.toString(16)}`,
				`texture offset=${texture_resource_data_section_offset.toString(16)} size=${texture_resource_data_section_size.toString(16)}`,
			]);
			view.setUint32(0x10, 0x00000000, true);
			view.setUint32(0x1C, 0x00000000, true);
			view.setUint32(0x20, 0x00000000, true);
			view.setUint32(0x2C, 0x00000000, true);
			view.setUint32(0x30, 0x00000000, true);
			view.setUint32(0x48, 0x00000000, true);
		}
		// done
		return;
	}

	function repair_package(
		data: ArrayBuffer,
	): void {
		let view = new DataView(data, 0x0);
		// magic
		view.setUint32(0x00, 0x72736231, true);
		// version_number
		view.setUint32(0x04, 0x00000004, true);
		// unused_1
		view.setUint32(0x08, 0x00000000, true);
		// information_section_size
		// resource_path_section_size
		// resource_path_section_offset
		// unused_2
		view.setUint32(0x18, 0x00000000, true);
		view.setUint32(0x1C, 0x00000000, true);
		// subgroup_id_section_size
		// subgroup_id_section_offset
		// subgroup_information_section_block_count
		let subgroup_information_section_block_count = view.getUint32(0x28, true);
		// subgroup_information_section_offset
		let subgroup_information_section_offset = view.getUint32(0x2C, true);
		// subgroup_information_section_block_size
		let subgroup_information_section_block_size = view.getUint32(0x30, true);
		// group_information_section_block_count
		let group_information_section_block_count = view.getUint32(0x34, true);
		// group_information_section_offset
		let group_information_section_offset = view.getUint32(0x38, true);
		// group_information_section_block_size
		let group_information_section_block_size = view.getUint32(0x3C, true);
		// group_id_section_size
		// group_id_section_offset
		// subgroup_pool_information_section_block_count
		let subgroup_pool_information_section_block_count = view.getUint32(0x48, true);
		// subgroup_pool_information_section_offset
		let subgroup_pool_information_section_offset = view.getUint32(0x4C, true);
		// subgroup_pool_information_section_block_size
		let subgroup_pool_information_section_block_size = view.getUint32(0x50, true);
		// texture_resource_information_section_block_count
		let texture_resource_information_section_block_count = view.getUint32(0x54, true);
		// texture_resource_information_section_offset
		let texture_resource_information_section_offset = view.getUint32(0x58, true);
		// texture_resource_information_section_block_size
		let texture_resource_information_section_block_size = view.getUint32(0x5C, true);
		// description_group_section_offset
		// description_resource_section_offset
		// description_string_section_offset
		// information_without_description_section_size
		// todo : 6C
		// group_information_section
		for (let group_index = 0; group_index < group_information_section_block_count; ++group_index) {
			let view = new DataView(data, group_information_section_offset + group_information_section_block_size * group_index);
			// id
			// subgroup_count
			let subgroup_count = view.getUint32(0x480, true);
			// subgroup_information
			for (let subgroup_index = 0; subgroup_index < 64; ++subgroup_index) {
				let view = new DataView(data, group_information_section_offset + group_information_section_block_size * group_index + 0x80 + 0x10 * subgroup_index);
				if (subgroup_index < subgroup_count) {
					// index
					// resolution
					// locale
					// unused_1
					view.setUint32(0x0C, 0x00000000, true);
				} else {
					view.setUint32(0x00, 0x00000000, true);
					view.setUint32(0x04, 0x00000000, true);
					view.setUint32(0x08, 0x00000000, true);
					view.setUint32(0x0C, 0x00000000, true);
				}
			}
		}
		// subgroup_information_section
		let progress = new TextGenerator.Progress('fraction', false, 20, subgroup_information_section_block_count);
		for (let subgroup_index = 0; subgroup_index < subgroup_information_section_block_count; ++subgroup_index) {
			progress.increase();
			Console.notify('i', `${progress}`, []);
			let view = new DataView(data, subgroup_information_section_offset + subgroup_information_section_block_size * subgroup_index);
			let pool_view = new DataView(data, subgroup_pool_information_section_offset + subgroup_pool_information_section_block_size * subgroup_index);
			// id
			// offset
			let offset = view.getUint32(0x80, true);
			// size
			// index
			// resource_data_section_store_mode
			let resource_data_section_store_mode = view.getUint32(0x8C, true);
			view.setUint32(0x8C, resource_data_section_store_mode & 0b11, true);
			// information_section_size
			let information_section_size = view.getUint32(0x90, true);
			// generic_resource_data_section_offset
			let generic_resource_data_section_offset = view.getUint32(0x94, true);
			// generic_resource_data_section_size
			let generic_resource_data_section_size = view.getUint32(0x98, true);
			// generic_resource_data_section_size_original
			let generic_resource_data_section_size_original = view.getUint32(0x9C, true);
			// generic_resource_data_section_size_original_1
			// texture_resource_data_section_offset
			let texture_resource_data_section_offset = view.getUint32(0xA4, true);
			// texture_resource_data_section_size
			let texture_resource_data_section_size = view.getUint32(0xA8, true);
			// texture_resource_data_section_size_original
			view.setUint32(0xAC, pool_view.getUint32(0x84, true), true);
			let texture_resource_data_section_size_original = view.getUint32(0xAC, true);
			// unused_1
			view.setUint32(0xB0, 0x00000000, true);
			view.setUint32(0xB4, 0x00000000, true);
			view.setUint32(0xB8, 0x00000000, true);
			view.setUint32(0xBC, 0x00000000, true);
			view.setUint32(0xC0, 0x00000000, true);
			// texture_resource_count
			// texture_resource_begin_index
			// packet
			let actual_packet_size = Math.max(information_section_size, generic_resource_data_section_offset + generic_resource_data_section_size, texture_resource_data_section_offset + texture_resource_data_section_size);
			view.setUint32(0x84, actual_packet_size, true);
			if (offset + actual_packet_size > data.byteLength) {
				// replace packet information for invalid packet
				Console.notify('w', los(`检测到Packet异常，现以空Packet替换`), [
					`subgroup index=${subgroup_index} offset=${offset.toString(16)} size=${actual_packet_size.toString(16)}`,
				]);
				view.setUint32(0x80, data.byteLength, true);
				view.setUint32(0x84, 0x00001000, true);
				view.setUint32(0x8C, 0x00000000, true);
				view.setUint32(0x90, 0x00001000, true);
				view.setUint32(0x94, 0x00001000, true);
				view.setUint32(0x98, 0x00000000, true);
				view.setUint32(0x9C, 0x00000000, true);
				view.setUint32(0xA0, 0x00000000, true);
				view.setUint32(0xA4, 0x00001000, true);
				view.setUint32(0xA8, 0x00000000, true);
				view.setUint32(0xAC, 0x00000000, true);
				view.setUint32(0xC4, 0x00000000, true);
				view.setUint32(0xC8, 0x00000000, true);
			} else {
				// repair packet
				let packet_view = new DataView(data, offset);
				// magic
				packet_view.setUint32(0x00, 0x72736770, true);
				// version_number
				packet_view.setUint32(0x04, 0x00000004, true);
				// unused_1
				packet_view.setUint32(0x08, 0x00000000, true);
				packet_view.setUint32(0x0C, 0x00000000, true);
				// resource_data_section_store_mode
				packet_view.setUint32(0x10, resource_data_section_store_mode & 0b11, true);
				// information_section_size
				packet_view.setUint32(0x14, information_section_size, true);
				// generic_resource_data_section_offset
				packet_view.setUint32(0x18, generic_resource_data_section_offset, true);
				// generic_resource_data_section_size
				packet_view.setUint32(0x1C, generic_resource_data_section_size, true);
				// generic_resource_data_section_size_original
				packet_view.setUint32(0x20, generic_resource_data_section_size_original, true);
				// unused_2
				packet_view.setUint32(0x24, 0x00000000, true);
				// texture_resource_data_section_offset
				packet_view.setUint32(0x28, texture_resource_data_section_offset, true);
				// texture_resource_data_section_size
				packet_view.setUint32(0x2C, texture_resource_data_section_size, true);
				// texture_resource_data_section_size_original
				packet_view.setUint32(0x30, texture_resource_data_section_size_original, true);
				// unused_3
				packet_view.setUint32(0x34, 0x00000000, true);
				packet_view.setUint32(0x38, 0x00000000, true);
				packet_view.setUint32(0x3C, 0x00000000, true);
				packet_view.setUint32(0x40, 0x00000000, true);
				packet_view.setUint32(0x44, 0x00000000, true);
				// resource_information_section_size
				// resource_information_section_offset
				// unused_4
				packet_view.setUint32(0x50, 0x00000000, true);
				packet_view.setUint32(0x54, 0x00000000, true);
				packet_view.setUint32(0x58, 0x00000000, true);
				// test zlib ripe
				// Console.notify('i', `test ${offset.toString(16)}`, [
				// 	`generic offset=${generic_resource_data_section_offset.toString(16)} size=${generic_resource_data_section_size.toString(16)}~${generic_resource_data_section_size_original.toString(16)}`,
				// 	`texture offset=${texture_resource_data_section_offset.toString(16)} size=${texture_resource_data_section_size.toString(16)}~${texture_resource_data_section_size_original.toString(16)}`,
				// ]);
				if (((resource_data_section_store_mode & 0b10) && !test_zlib_ripe(data, offset + generic_resource_data_section_offset, generic_resource_data_section_size, generic_resource_data_section_size_original)) ||
					((resource_data_section_store_mode & 0b01) && !test_zlib_ripe(data, offset + texture_resource_data_section_offset, texture_resource_data_section_size, texture_resource_data_section_size_original))) {
					Console.notify('w', los(`检测到ZLib异常，现截断数据`), [
						`generic offset=${generic_resource_data_section_offset.toString(16)} size=${generic_resource_data_section_size.toString(16)}`,
						`texture offset=${texture_resource_data_section_offset.toString(16)} size=${texture_resource_data_section_size.toString(16)}`,
					]);
					packet_view.setUint32(0x10, 0x00000000, true);
					packet_view.setUint32(0x1C, 0x00000000, true);
					packet_view.setUint32(0x20, 0x00000000, true);
					packet_view.setUint32(0x2C, 0x00000000, true);
					packet_view.setUint32(0x30, 0x00000000, true);
					packet_view.setUint32(0x48, 0x00000000, true);
					view.setUint32(0x8C, packet_view.getUint32(0x10, true), true);
					view.setUint32(0x90, packet_view.getUint32(0x14, true), true);
					view.setUint32(0x94, packet_view.getUint32(0x18, true), true);
					view.setUint32(0x98, packet_view.getUint32(0x1C, true), true);
					view.setUint32(0x9C, packet_view.getUint32(0x20, true), true);
					view.setUint32(0xA0, packet_view.getUint32(0x20, true), true);
					view.setUint32(0xA4, packet_view.getUint32(0x28, true), true);
					view.setUint32(0xA8, packet_view.getUint32(0x2C, true), true);
					view.setUint32(0xAC, packet_view.getUint32(0x30, true), true);
					actual_packet_size = Math.max(packet_view.getUint32(0x14, true), packet_view.getUint32(0x18, true) + packet_view.getUint32(0x1C, true), packet_view.getUint32(0x28, true) + packet_view.getUint32(0x2C, true));
					view.setUint32(0x84, actual_packet_size, true);
	
				}
			}
		}
		// texture_resource_information_section
		for (let resource_index = 0; resource_index < texture_resource_information_section_block_count; ++resource_index) {
		}
		// done
		return;
	}

	export function execute(
	): void {
		Console.notify('i', los(`请输入需要修复的RSB文件路径`), []);
		let input_file = Console.string(null);
		let output_file = `${input_file}.repair.rsb`;
		let data = CoreX.FileSystem.read_file(input_file);
		let data_js = data.view().value;
		repair_package(data_js);
		let empty_packet = create_empty_packet();
		let data_with_empty_packet = Core.ByteArray.allocate(Core.Size.value(data.size().value + 0x1000n));
		new Uint8Array(data_with_empty_packet.view().sub(Core.Size.value(0x0n), data.size()).value).set(new Uint8Array(data_js));
		new Uint32Array(data_with_empty_packet.view().sub(data.size(), Core.Size.value(0x1000n)).value).set(new Uint32Array(empty_packet));
		CoreX.FileSystem.write_file(output_file, data_with_empty_packet);
		Console.notify('i', los(`修复完成`), [output_file]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.RepairRSB.execute();