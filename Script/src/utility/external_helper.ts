namespace Twinning.Script.ExternalHelper {

	// #region common

	export let g_enable_android_termux: boolean = false;

	export let g_program_path_map: Record<string, null | StoragePath> = {};

	// ----------------

	function throw_execution_error(
		result: ReturnType<typeof ProcessHelper.run_process>,
	): never {
		let error = new Error(`path: ${result.path.emit()}\ncode: ${result.code}\noutput:\n${result.output}\nerror:\n${result.error}`);
		error.name = 'ExecutionError';
		throw error;
	}

	function search_program(
		name: string,
	): StoragePath {
		let result: null | StoragePath = null;
		let item_delimiter = KernelX.is_windows ? ';' : ':';
		let path_environment = ProcessHelper.find_environment('PATH');
		assert_test(path_environment !== null);
		let path_list = path_environment.split(item_delimiter).map((it) => new StoragePath(it));
		let path_extension_list = [''];
		if (KernelX.is_windows) {
			let path_extension_environment = ProcessHelper.find_environment('PATHEXT');
			assert_test(path_extension_environment !== null);
			path_extension_list.push(...path_extension_environment.split(item_delimiter).map((value) => (value.toLowerCase())));
		}
		if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
			path_extension_list.push('.sh');
		}
		for (let path of path_list) {
			for (let path_extension of path_extension_list) {
				let current_path = path.join(`${name}${path_extension}`);
				if (StorageHelper.exist_file(current_path)) {
					result = current_path;
					break;
				}
			}
		}
		if (result === null) {
			throw new Error(`could not find '${name}' program from 'PATH' environment`);
		}
		return result;
	}

	function run_process(
		program: string,
		argument: Array<string>,
		workspace: null | StoragePath,
		environment: null | Record<string, string>,
		input_data: null | string,
		allow_android_termux: boolean,
	): ReturnType<typeof ProcessHelper.run_process> {
		let program_path: null | StoragePath = null;
		if (g_program_path_map[program] !== undefined) {
			program_path = g_program_path_map[program];
		}
		if (!(KernelX.is_android && allow_android_termux && g_enable_android_termux)) {
			if (program_path === null) {
				program_path = search_program(program)
			}
			return ProcessHelper.run_process(program_path, argument, workspace, environment, input_data);
		}
		else {
			if (program_path === null) {
				program_path = new StoragePath(program);
			}
			return AndroidHelper.termux_run_process(program_path, argument, input_data);
		}
	}

	// #endregion

	// #region shell

	export function run_sh(
		argument: Array<string>,
		always_in_native: boolean = false,
	): string {
		let process_result = run_process(
			'sh',
			argument,
			null,
			null,
			null,
			!always_in_native,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		return process_result.output;
	}

	// #endregion

	// #region android tool

	export function run_adb(
		argument: Array<string>,
	): string {
		let process_result = run_process(
			'adb',
			argument,
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		return process_result.output;
	}

	// ----------------

	export function run_zipalign_align(
		zip_file: StoragePath,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let aligned_file = temporary_directory.join('aligned');
		let process_result = run_process(
			'zipalign',
			[
				'-P', '16',
				'-f',
				'4',
				`${zip_file.emit_native()}`,
				`${aligned_file.emit_native()}`,
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		StorageHelper.remove(zip_file);
		StorageHelper.copy(aligned_file, zip_file, false);
		return;
	}

	export function run_apksigner_sign(
		apk_file: StoragePath,
		keystore_file: StoragePath,
		keystore_password: String,
	): void {
		let process_result = run_process(
			'apksigner',
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
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		return;
	}

	// #endregion

	// #region 7z

	export function run_7z_list_content(
		zip_file: StoragePath,
	): Array<StoragePath> {
		let process_result = run_process(
			'7z',
			[
				'l',
				'-sccUTF-8',
				'-tzip',
				`${zip_file.emit_native()}`,
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		let output_line_list = ConvertHelper.split_string_by_line_feed(ConvertHelper.normalize_string_line_feed(process_result.output), true);
		let content_list = [] as Array<StoragePath>;
		let match_rule = /^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) (.{5}) ( +\d+) ( +\d+)  (.+)$/;
		for (let output_line of output_line_list) {
			let match = match_rule.exec(output_line);
			if (match === null) {
				continue;
			}
			content_list.push(new StoragePath(match[5]));
		}
		return content_list;
	}

	export function run_7z_extract_content(
		zip_file: StoragePath,
		content: Array<{location: StoragePath; placement: StoragePath}>,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let process_result = run_process(
			'7z',
			[
				'x',
				'-sccUTF-8',
				'-tzip',
				`${zip_file.emit_native()}`,
				...content.map((it) => `${it.location.emit_posix()}`),
				`-o${temporary_directory.emit_native()}`,
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		for (let content_item of content) {
			StorageHelper.copy(temporary_directory.push(content_item.location), content_item.placement, false);
		}
		return;
	}

	export function run_7z_append_content(
		zip_file: StoragePath,
		content: Array<{location: StoragePath; placement: StoragePath}>,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		for (let content_item of content) {
			StorageHelper.copy(content_item.placement, temporary_directory.push(content_item.location), false);
		}
		let process_result = run_process(
			'7z',
			[
				'a',
				'-sccUTF-8',
				'-tzip',
				'-aoa',
				`${zip_file.emit_native()}`,
				`${temporary_directory.join('*').emit_native()}`,
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		return;
	}

	// #endregion

	// #region Wwise

	export function run_wwise_create_new_project(
		project_file: StoragePath,
		platform_list: Array<string>,
	): void {
		let process_result = run_process(
			'WwiseConsole',
			[
				'create-new-project',
				project_file.emit_native(),
				'--platform',
				...platform_list,
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		return;
	}

	export function run_wwise_convert_external_source(
		project_file: StoragePath,
		sources_file: StoragePath,
		platform: string,
	): void {
		let process_result = run_process(
			'WwiseConsole',
			[
				'convert-external-source',
				project_file.emit_native(),
				'--platform',
				platform,
				'--source-file',
				sources_file.emit_native(),
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		return;
	}

	// #endregion

	// #region vgmstream-cli

	export function run_vgmstream_decode(
		raw_file: StoragePath,
		ripe_file: StoragePath,
		extension: string,
	): string {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let ripe_file_fallback = ripe_file;
		if (ripe_file.extension()?.toLowerCase() !== extension) {
			ripe_file_fallback = temporary_directory.join(`ripe.${extension}`);
			StorageHelper.copy(ripe_file, ripe_file_fallback, false);
		}
		let raw_file_directory = raw_file.parent();
		if (raw_file_directory !== null && !StorageHelper.exist_directory(raw_file_directory)) {
			StorageHelper.create_directory(raw_file_directory);
		}
		let process_result = run_process(
			'vgmstream-cli',
			[
				'-o',
				raw_file.emit_native(),
				ripe_file_fallback.emit_native(),
			],
			null,
			null,
			null,
			true,
		);
		if (process_result.code !== 0n) {
			throw_execution_error(process_result);
		}
		let regex_result = /^encoding: (.+)$/m.exec(process_result.output);
		assert_test(regex_result !== null);
		let encoding = regex_result[1];
		return encoding;
	}

	// #endregion

	// #region Il2CppDumper

	export function run_il2cppdumper_dump(
		program_file: StoragePath,
		metadata_file: StoragePath,
	): Array<string> {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let dump_directory = temporary_directory.join('dump');
		StorageHelper.create_directory(dump_directory);
		let process_result = run_process(
			'Il2CppDumper',
			[
				program_file.emit_native(),
				metadata_file.emit_native(),
				dump_directory.emit_native(),
			],
			null,
			null,
			null,
			true,
		);
		if (!/\nDone!\n(Press any key to exit\.\.\.\n)?$/.test(ConvertHelper.normalize_string_line_feed(process_result.output))) {
			throw_execution_error(process_result);
		}
		let dump_data = ConvertHelper.split_string_by_line_feed(StorageHelper.read_file_text(dump_directory.join(`dump.cs`)), true);
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

}
