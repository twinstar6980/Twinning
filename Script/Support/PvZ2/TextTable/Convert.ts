namespace Twinning.Script.Support.PvZ2.TextTable.Convert {

	// ------------------------------------------------

	const VersionX = [
		'text',      // 文本形式，早期版本（6.?及之前）
		'json_map',  // JSON键值对形式，中期版本
		'json_list', // JSON数组形式，当前版本
	] as const;

	export type Version = typeof VersionX[number];

	export const VersionE = VersionX as unknown as Version[];

	// ------------------------------------------------

	export function convert(
		source_data: ArrayBuffer,
		source_version: Version | 'automatic',
		destination_version: Version,
	): ArrayBuffer {
		let string_map: Record<string, string> = {};
		let source_map: null | Record<string, string> = null;
		let source_list: null | Array<string> = null;
		if (source_version === 'automatic') {
			try {
				let source = KernelX.JSON.read(source_data).value as any;
				let source_variant = source?.objects[0]?.objdata?.LocStringValues;
				if (is_object_of_object(source_variant)) {
					source_map = source_variant;
					source_version = 'json_map';
				}
				if (is_object_of_array(source_variant)) {
					source_list = source_variant;
					source_version = 'json_list';
				}
				assert_fail(`invalid source`);
			}
			catch (e) {
				source_version = 'text';
			}
		}
		switch (source_version) {
			case 'text': {
				assert_test(!string_data_maybe_utf16(source_data), `invalid charset UTF-16`);
				let source_text = Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(Kernel.ByteListView.value(source_data)));
				let key_regexp = /^\[.+\]$/gm;
				let value_regexp = /(.|[\n\r])*?(?=[\n\r]*?(\[|$))/gy;
				let key_match: null | RegExpExecArray;
				let value_match: null | RegExpExecArray;
				while ((key_match = key_regexp.exec(source_text)) !== null) {
					value_regexp.lastIndex = key_regexp.lastIndex + 1;
					value_match = value_regexp.exec(source_text)!;
					string_map[key_match[0].slice(1, -1)] = value_match[0];
					key_regexp.lastIndex = value_regexp.lastIndex;
				}
				break;
			}
			case 'json_map': {
				if (source_map === null) {
					let source = KernelX.JSON.read(source_data).value as any;
					source_map = source?.objects[0]?.objdata?.LocStringValues;
					assert_test(is_object_of_object(source_map), `invalid source`);
				}
				for (let key in source_map) {
					let value = source_map[key];
					assert_test(is_string(value), `invalid map element`);
					string_map[key] = value;
				}
				break;
			}
			case 'json_list': {
				if (source_list === null) {
					let source = KernelX.JSON.read(source_data).value as any;
					source_list = source?.objects[0]?.objdata?.LocStringValues;
					assert_test(is_object_of_array(source_list), `invalid source`);
				}
				assert_test(source_list.length % 2 === 0, `invalid list size`);
				for (let index = 0; index < source_list.length; index += 2) {
					let key = source_list[index + 0];
					let value = source_list[index + 1];
					assert_test(is_string(key) && is_string(value), `invalid list element`);
					string_map[key] = value;
				}
				break;
			}
		}
		let destination_data: ArrayBuffer;
		switch (destination_version) {
			case 'text': {
				let destination_text: Array<string> = [];
				for (let key in string_map) {
					destination_text.push(`[${key}]\n${string_map[key]}\n`);
				}
				destination_data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(destination_text.join('\n'))).release();
				break;
			}
			case 'json_map': {
				let destination = {
					"version": 1n,
					"objects": [
						{
							"aliases": [
								"LawnStringsData",
							],
							"objclass": "LawnStringsData",
							"objdata": {
								"LocStringValues": string_map,
							},
						},
					],
				};
				destination_data = Kernel.ByteArray.value(KernelX.JSON.write_js(destination)).release();
				break;
			}
			case 'json_list': {
				let destination = {
					"version": 1n,
					"objects": [
						{
							"aliases": [
								"LawnStringsData",
							],
							"objclass": "LawnStringsData",
							"objdata": {
								"LocStringValues": (() => {
									let destination_list: Array<string> = [];
									for (let key in string_map) {
										destination_list.push(key);
										destination_list.push(string_map[key]);
									}
									return destination_list;
								})(),
							},
						},
					],
				};
				destination_data = Kernel.ByteArray.value(KernelX.JSON.write_js(destination)).release();
				break;
			}
		}
		return destination_data;
	}

	// ------------------------------------------------

	export function convert_fs(
		source_file: string,
		destination_file: string,
		source_version: Version | 'automatic',
		destination_version: Version,
	): void {
		let source_data = KernelX.Storage.read_file(source_file);
		let destination_data = convert(source_data.value, source_version, destination_version);
		KernelX.Storage.write_file(destination_file, destination_data);
		return;
	}

	// ------------------------------------------------

}
