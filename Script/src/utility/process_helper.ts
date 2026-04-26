namespace Twinning.Script.ProcessHelper {

	// #region workspace

	export function get_workspace(
	): StoragePath {
		return KernelX.Process.get_workspace();
	}

	// #endregion

	// #region environment

	export function get_environment(
	): Record<string, string> {
		return ConvertHelper.record_from_array(
			KernelX.Process.get_environment(),
			(index, element) => {
				let split_index = element.indexOf('=');
				return [element.slice(0, split_index), element.slice(split_index + 1)];
			},
		);
	}

	export function find_environment(
		name: string,
	): null | string {
		return KernelX.Process.find_environment(name);
	}

	// #endregion

	// #region process

	export function run_process(
		program: StoragePath,
		argument: Array<string>,
		workspace: null | StoragePath,
		environment: null | Record<string, string>,
	): {
		code: bigint;
		output: string;
		error: string;
	} {
		if (workspace === null) {
			workspace = get_workspace();
		}
		if (environment === null) {
			environment = get_environment();
		}
		let temporary_directory = StorageHelper.temporary('directory');
		let temporary_directory_fallback: null | StoragePath = null;
		let input_file = temporary_directory.join('input');
		let output_file = temporary_directory.join('output');
		let error_file = temporary_directory.join('error');
		StorageHelper.write_file_text(input_file, '');
		StorageHelper.create_file(output_file);
		StorageHelper.create_file(error_file);
		let code = KernelX.Process.run_process(program, argument, workspace, ConvertHelper.record_to_array(environment, (key, value) => `${key}=${value}`), input_file, output_file, error_file);
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
		let path_environment = find_environment('PATH');
		assert_test(path_environment !== null);
		let path_list = path_environment.split(item_delimiter).map((it) => new StoragePath(it));
		let path_extension_list = [''];
		if (KernelX.is_windows && allow_extension) {
			let path_extension_environment = find_environment('PATHEXT');
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
