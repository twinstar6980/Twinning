namespace Twinning.Script.Support.Nitrome.TwinShotDeluxe.Record.Encode {

	// #region utility

	function process_whole(
		data: ByteStreamView,
		definition: Record<string, any>,
	): void {
		let property_list = Object.keys(definition);
		let property_count = property_list.length;
		data.u32(BigInt(property_count));
		for (let property_index = 0; property_index < property_count; property_index++) {
			let property_name = property_list[property_index];
			let property_name_size = property_name.length;
			data.u8(BigInt(property_name_size));
			// TODO: use utf-8
			ConvertHelper.write_eascii_string(data, property_name);
			let property_value = definition[property_name];
			switch (typeof property_value) {
				case 'bigint': {
					data.u8(0x00n);
					data.u32(property_value);
					break;
				}
				case 'boolean': {
					data.u8(0x01n);
					data.u8(!property_value ? 0x00n : 0x01n);
					break;
				}
				default: throw new Error(`unknown value type ${typeof property_value}`);
			}
		}
		return;
	}

	// ----------------

	export function process(
		data: ByteStreamView,
		definition: Record<string, any>,
	): void {
		return process_whole(data, definition);
	}

	// ----------------

	export function process_fs(
		data_file: StoragePath,
		definition_file: StoragePath,
		data_buffer: Kernel.ByteArray,
	): void {
		let definition = JsonHelper.decode_file(definition_file) as Record<string, any>;
		let data_stream = new ByteStreamView(data_buffer.view().value);
		process(data_stream, definition);
		StorageHelper.write_file(data_file, data_stream.sub(0, data_stream.p()));
		return;
	}

	// #endregion

}
