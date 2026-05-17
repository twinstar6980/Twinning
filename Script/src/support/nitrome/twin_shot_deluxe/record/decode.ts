namespace Twinning.Script.Support.Nitrome.TwinShotDeluxe.Record.Decode {

	// #region utility

	function process_whole(
		data: ByteStreamView,
	): Record<string, any> {
		let definition = {} as Record<string, any>;
		let property_count = Number(data.u32());
		for (let property_index = 0; property_index < property_count; property_index++) {
			let property_name_size = data.u8();
			let property_name = ConvertHelper.read_utf8_string_by_size(data, property_name_size);
			let property_value_type = data.u8();
			let property_value = null as any;
			switch (property_value_type) {
				case 0n: {
					property_value = data.u32();
					break;
				}
				case 1n: {
					property_value = data.u8() === 0x00n ? false : true;
					break;
				}
				default: throw new Error(`unknown value type ${property_value_type}`);
			}
			definition[property_name] = property_value;
		}
		return definition;
	}

	// ----------------

	export function process(
		data: ByteStreamView,
	): Record<string, any> {
		return process_whole(data);
	}

	// ----------------

	export function process_fs(
		data_file: StoragePath,
		definition_file: StoragePath,
	): void {
		let data = StorageHelper.read_file(data_file);
		let definition = process(new ByteStreamView(data.view().value));
		JsonHelper.encode_file(definition_file, definition);
		return;
	}

	// #endregion

}
