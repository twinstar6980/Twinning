namespace TwinStar.Support.PvZ2.LawnStringText {

	// ------------------------------------------------

	export const VersionE = [
		'text',      // 文本形式，早期版本（6.?及之前）
		'json_map',  // JSON键值对形式，中期版本
		'json_list', // JSON数组形式，当前版本
	] as const;

	export type Version = typeof VersionE[number];

	// ------------------------------------------------

	export function convert(
		source_data: ArrayBuffer,
		source_version: Version | 'auto',
		destination_version: Version,
	): ArrayBuffer {
		let string_map: Record<string, string> = {};
		let actual_source_version: Version;
		let source_map: Record<string, string> | null = null;
		let source_list: Array<string> | null = null;
		if (source_version === 'auto') {
			try {
				let source = CoreX.JSON.read(source_data).value as any;
				let source_variant = source?.objects[0]?.objdata?.LocStringValues;
				assert(typeof source_variant === 'object', `invalid source`);
				if (source_variant instanceof Array) {
					source_list = source_variant;
					actual_source_version = 'json_list';
				} else {
					source_map = source_variant;
					actual_source_version = 'json_map';
				}
			} catch (e: any) {
				actual_source_version = 'text';
			}
		} else {
			actual_source_version = source_version;
		}
		switch (actual_source_version) {
			case 'text': {
				let source_text = Core.Miscellaneous.cast_CharacterListView_to_JS_String(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(Core.ByteListView.value(source_data)));
				let key_regexp = /^\[.+\]$/gm;
				let value_regexp = /(.|[\n\r])*?(?=[\n\r]*?(\[|$))/gy;
				let key_match: RegExpExecArray | null;
				let value_match: RegExpExecArray | null;
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
					let source = CoreX.JSON.read(source_data).value as any;
					source_map = source?.objects[0]?.objdata?.LocStringValues as Record<string, string>;
					assert(typeof source_map === 'object' && (source_map as Object).constructor.name === 'Object', `invalid source`);
				}
				for (let key in source_map) {
					assert(typeof source_map[key] === 'string', `invalid map element`);
					string_map[key] = source_map[key];
				}
				break;
			}
			case 'json_list': {
				if (source_list === null) {
					let source = CoreX.JSON.read(source_data).value as any;
					source_list = source?.objects[0]?.objdata?.LocStringValues as Array<string>;
					assert(typeof source_list === 'object' && (source_map as Object).constructor.name === 'Array', `invalid source`);
				}
				assert(source_list.length % 2 === 0, `invalid list size`);
				for (let i = 0; i < source_list.length; i += 2) {
					let key = source_list[i + 0];
					let value = source_list[i + 1];
					assert(typeof key === 'string' && typeof value === 'string', `invalid list element`);
					string_map[source_list[i]] = source_list[i + 1];
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
				destination_data = Core.Miscellaneous.cast_moveable_String_to_ByteArray(Core.String.value(destination_text.join('\n'))).release();
				break;
			}
			case 'json_map': {
				let destination = {
					"version": 1n,
					"objects": [
						{
							"aliases": [
								"LawnStringsData"
							],
							"objclass": "LawnStringsData",
							"objdata": {
								"LocStringValues": string_map
							}
						}
					]
				};
				destination_data = Core.ByteArray.value(CoreX.JSON.write_js(destination)).release();
				break;
			}
			case 'json_list': {
				let destination = {
					"version": 1n,
					"objects": [
						{
							"aliases": [
								"LawnStringsData"
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
								})()
							}
						}
					]
				};
				destination_data = Core.ByteArray.value(CoreX.JSON.write_js(destination)).release();
				break;
			}
		}
		return destination_data;
	}

	// ------------------------------------------------

	export function convert_fs(
		source_file: string,
		destination_file: string,
		source_version: Version | 'auto',
		destination_version: Version,
	): void {
		let source_data = CoreX.FileSystem.read_file(source_file);
		let destination_data = convert(source_data.value, source_version, destination_version);
		CoreX.FileSystem.write_file(destination_file, destination_data);
		return;
	}

	// ------------------------------------------------

}
