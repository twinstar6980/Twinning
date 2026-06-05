namespace Twinning.Script.ExternalHelper {

	// #region Il2CppDumper

	export function run_il2cppdumper(
		program_file: StoragePath,
		metadata_file: StoragePath,
	): Array<string> {
		let dump_directory = StorageHelper.temporary('directory');
		let process_result = ProcessHelper.run_process(
			ProcessHelper.search_program_ensure('Il2CppDumper', true),
			[
				program_file.emit_native(),
				metadata_file.emit_native(),
				dump_directory.emit_native(),
			],
			null,
			null,
		);
		if (!/\nDone!\n(Press any key to exit\.\.\.\n)?$/.test(ConvertHelper.normalize_string_line_feed(process_result.output))) {
			ProcessHelper.throw_error(process_result);
		}
		let dump_data = ConvertHelper.split_string_by_line_feed(StorageHelper.read_file_text(dump_directory.join(`dump.cs`)), true);
		StorageHelper.remove(dump_directory);
		return dump_data;
	}

	// ----------------

	export function do_il2cppdumper_search_field_from_dump_data(
		source: Array<string>,
		class_name: string,
		field_name: string,
	): Array<{
		address: number;
		modifier: string;
		statically: boolean;
		type: string;
	}> {
		let result: ReturnType<typeof do_il2cppdumper_search_field_from_dump_data> = [];
		let class_rule = /^(private|protected|public) class ([^ ]+)/;
		let field_rule = /^\t(private|protected|public)( static)? (.+) (.+); \/\/ 0x(.+)$/;
		for (let index = 0; index < source.length; index++) {
			let class_match = class_rule.exec(source[index]);
			if (class_match === null || class_match[2] !== class_name) {
				continue;
			}
			for (; index < source.length; index++) {
				if (source[index] === '}') {
					break;
				}
				let field_match = field_rule.exec(source[index]);
				if (field_match === null || field_match[4] !== field_name) {
					continue;
				}
				result.push({
					address: Number.parseInt(field_match[5], 16),
					modifier: field_match[1],
					statically: field_match[2] !== undefined,
					type: field_match[3],
				});
			}
			break;
		}
		return result;
	}

	export function do_il2cppdumper_search_method_from_dump_data(
		source: Array<string>,
		class_name: string,
		method_name: string,
	): Array<{
		address: number;
		modifier: string;
		statically: boolean;
		result: string;
		parameter: string;
	}> {
		let result: ReturnType<typeof do_il2cppdumper_search_method_from_dump_data> = [];
		let class_rule = /^(private|protected|public) class ([^ ]+)/;
		let method_rule = /^\t(private|protected|public)( static)? (.+) (.+)\((.*)\) \{ \}$/;
		let comment_rule = /^\t\/\/ RVA: 0x(.+) Offset: 0x(.+) VA: 0x(.+)$/;
		for (let index = 0; index < source.length; index++) {
			let class_match = class_rule.exec(source[index]);
			if (class_match === null || class_match[2] !== class_name) {
				continue;
			}
			for (; index < source.length; index++) {
				if (source[index] === '}') {
					break;
				}
				let method_match = method_rule.exec(source[index]);
				if (method_match === null || method_match[4] !== method_name) {
					continue;
				}
				let comment_match = comment_rule.exec(source[index - 1]);
				assert_test(comment_match !== null);
				result.push({
					address: Number.parseInt(comment_match[2], 16),
					modifier: method_match[1],
					statically: method_match[2] !== undefined,
					result: method_match[3],
					parameter: method_match[5],
				});
			}
			break;
		}
		return result;
	}

	// #endregion

	// #region Android Tools

	export function run_zipalign(
		zip_file: StoragePath,
	): void {
		let aligned_file = StorageHelper.temporary('file');
		let process_result = ProcessHelper.run_process(
			ProcessHelper.search_program_ensure('zipalign', true),
			[
				'-P', '16',
				'-f',
				'4',
				`${zip_file.emit_native()}`,
				`${aligned_file.emit_native()}`,
			],
			null,
			null,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_error(process_result);
		}
		StorageHelper.remove(zip_file);
		StorageHelper.copy(aligned_file, zip_file, false);
		StorageHelper.remove(aligned_file);
		return;
	}

	// ----------------

	export function run_apksigner(
		apk_file: StoragePath,
		keystore_file: StoragePath,
		keystore_password: String,
	): void {
		let process_result = ProcessHelper.run_process(
			ProcessHelper.search_program_ensure('apksigner', true),
			[
				'sign',
				'--v1-signing-enabled', 'true',
				'--v2-signing-enabled', 'true',
				'--v3-signing-enabled', 'true',
				'--v4-signing-enabled', 'false',
				'--ks', `${keystore_file.emit_native()}`,
				'--ks-pass', `pass:${keystore_password}`,
				`${apk_file.emit_native()}`,
			],
			null,
			null,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_error(process_result);
		}
		return;
	}

	// #endregion

	// #region archive

	export function list_zip_content(
		zip_file: StoragePath,
	): Array<StoragePath> {
		let process_result = ProcessHelper.run_process(
			ProcessHelper.search_program_ensure('unzip', true),
			[
				'-l',
				`${zip_file.emit_native()}`,
			],
			null,
			null,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_error(process_result);
		}
		let output_line_list = ConvertHelper.split_string_by_line_feed(ConvertHelper.normalize_string_line_feed(process_result.output), true);
		output_line_list = output_line_list.slice(3, -2);
		let content_list = [] as Array<StoragePath>;
		let match_rule = /  \d{4}-\d{2}-\d{2} \d{2}:\d{2}   (.+)$/;
		for (let output_line of output_line_list) {
			let match = match_rule.exec(output_line);
			assert_test(match !== null);
			content_list.push(new StoragePath(match[1]));
		}
		return content_list;
	}

	export function extract_zip_content(
		zip_file: StoragePath,
		a1: StoragePath,
		a2: StoragePath,
	): void {
		return;
	}

	export function replace_zip_content(
		zip_file: StoragePath,
		a1: StoragePath,
		a2: StoragePath,
	): void {
		return;
	}

	// #endregion

}
