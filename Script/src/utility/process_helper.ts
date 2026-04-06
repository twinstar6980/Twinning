namespace Twinning.Script.ProcessHelper {

	// #region environment

	export function query_environment(
		name: string,
	): null | string {
		return KernelX.Process.get_environment(name);
	}

	// ----------------

	export function list_environment(
	): Record<string, string> {
		return ConvertHelper.record_from_array(
			KernelX.Process.list_environment(),
			(index, element) => {
				let split_index = element.indexOf('=');
				return [element.slice(0, split_index), element.slice(split_index + 1)];
			},
		);
	}

	// #endregion

	// #region process

	export function run_process(
		program: StoragePath,
		argument: Array<string>,
		environment: null | Record<string, string>,
	): {
		code: bigint;
		output: string;
		error: string;
	} {
		if (environment === null) {
			environment = list_environment();
		}
		let temporary_directory = StorageHelper.temporary('directory');
		let temporary_directory_fallback: null | StoragePath = null;
		let input_file = temporary_directory.join('input');
		let output_file = temporary_directory.join('output');
		let error_file = temporary_directory.join('error');
		StorageHelper.write_file_text(input_file, '');
		if (KernelX.is_android && !Shell.is_basic) {
			temporary_directory_fallback = AndroidHelper.k_temporary_directory.join(temporary_directory.name()!);
			output_file = temporary_directory_fallback.join('output');
			error_file = temporary_directory_fallback.join('error');
			assert_test(KernelX.Process.execute_command(`su -c "mkdir -p -m 777 ${temporary_directory_fallback.emit_native()} ; touch ${output_file.emit_native()} ; chmod 777 ${output_file.emit_native()} ; touch ${error_file.emit_native()} ; chmod 777 ${error_file.emit_native()}"`) === 0n);
		}
		else {
			StorageHelper.create_file(output_file);
			StorageHelper.create_file(error_file);
		}
		let code = KernelX.Process.run_process(program, argument, ConvertHelper.record_to_array(environment, (key, value) => `${key}=${value}`), input_file, output_file, error_file);
		let read_file = (path: StoragePath): string => {
			let data = StorageHelper.read_file_text(path);
			return ConvertHelper.normalize_string_line_feed(data);
		};
		let result: ReturnType<typeof run_process> = {
			code: code,
			output: read_file(output_file),
			error: read_file(error_file),
		};
		StorageHelper.remove(temporary_directory);
		if (KernelX.is_android && !Shell.is_basic) {
			assert_test(temporary_directory_fallback !== null);
			assert_test(KernelX.Process.execute_command(`su -c "rm -rf ${temporary_directory_fallback}"`) === 0n);
		}
		return result;
	}

	// #endregion

	// #region program

	export let g_program_path_map: Record<string, null | StoragePath> = {};

	// ----------------

	export function search_program(
		name: string,
		allow_extension: boolean,
	): null | StoragePath {
		if (g_program_path_map[name] !== undefined && g_program_path_map[name] !== null) {
			return g_program_path_map[name];
		}
		let result: null | StoragePath = null;
		let item_delimiter = KernelX.is_windows ? ';' : ':';
		let path_environment = query_environment('PATH');
		assert_test(path_environment !== null);
		let path_list = path_environment.split(item_delimiter).map((it) => new StoragePath(it));
		let path_extension_list = [''];
		if (KernelX.is_windows && allow_extension) {
			let path_extension_environment = query_environment('PATHEXT');
			assert_test(path_extension_environment !== null);
			path_extension_list.push(...path_extension_environment.split(item_delimiter).map((value) => (value.toLowerCase())));
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
		return result;
	}

	export function search_program_ensure(
		name: string,
		allow_extension: boolean,
	): StoragePath {
		let result = search_program(name, allow_extension);
		if (result === null) {
			throw new Error(`could not find '${name}' program from 'PATH' environment`);
		}
		return result;
	}

	// #endregion

}
