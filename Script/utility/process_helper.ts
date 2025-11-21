namespace Twinning.Script.ProcessHelper {

	// #region environment

	export function get_environment(
		name: string,
	): null | string {
		return KernelX.Process.get_environment(name);
	}

	export function set_environment(
		name: string,
		value: null | string,
	): void {
		return KernelX.Process.set_environment(name, value);
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
		program: string | [string],
		argument: Array<string>,
		environment: null | Record<string, string>,
		input: null | string,
	): {
		code: bigint;
		output: string;
		error: string;
	} {
		if (CheckHelper.is_object_of_array(program)) {
			program = search_program_ensure(program[0], true);
		}
		if (environment === null) {
			environment = list_environment();
		}
		if (input === null) {
			input = '';
		}
		let temporary_directory = HomePath.new_temporary(null, 'directory');
		let temporary_directory_fallback: null | string = null;
		let input_file = `${temporary_directory}/input`;
		let output_file = `${temporary_directory}/output`;
		let error_file = `${temporary_directory}/error`;
		KernelX.Storage.write_file_s(input_file, input);
		if (KernelX.is_android && !Shell.is_basic) {
			temporary_directory_fallback = `${AndroidHelper.k_temporary_directory}/${StorageHelper.name(temporary_directory)}`;
			output_file = `${temporary_directory_fallback}/output`;
			error_file = `${temporary_directory_fallback}/error`;
			assert_test(KernelX.Process.execute_command(`su -c "mkdir -p -m 777 ${temporary_directory_fallback} ; touch ${output_file} ; chmod 777 ${output_file} ; touch ${error_file} ; chmod 777 ${error_file}"`) === 0n);
		}
		else {
			KernelX.Storage.create_file(output_file);
			KernelX.Storage.create_file(error_file);
		}
		let code = KernelX.Process.run_process(program, argument, ConvertHelper.record_to_array(environment, (key, value) => `${key}=${value}`), input_file, output_file, error_file);
		let read_file = (path: string): string => {
			let data = KernelX.Storage.read_file_s(path);
			return ConvertHelper.normalize_string_line_feed(data);
		};
		let result: ReturnType<typeof run_process> = {
			code: code,
			output: read_file(output_file),
			error: read_file(error_file),
		};
		KernelX.Storage.remove(temporary_directory);
		if (KernelX.is_android && !Shell.is_basic) {
			assert_test(temporary_directory_fallback !== null);
			assert_test(KernelX.Process.execute_command(`su -c "rm -rf ${temporary_directory_fallback}"`) === 0n);
		}
		return result;
	}

	// #endregion

	// #region program

	export let g_program_path_map: Record<string, null | string> = {};

	// ----------------

	export function search_program(
		name: string,
		allow_extension: boolean,
	): null | string {
		if (g_program_path_map[name] !== undefined && g_program_path_map[name] !== null) {
			return g_program_path_map[name];
		}
		let result: null | string = null;
		let item_delimiter = KernelX.is_windows ? ';' : ':';
		let path_environment = get_environment('PATH');
		if (path_environment === null) {
			throw new Error(`could not find 'PATH' environment`);
		}
		let path_list = path_environment.split(item_delimiter);
		if (KernelX.is_windows) {
			path_list = path_list.map((value) => (StorageHelper.regularize(value)));
		}
		let path_extension_list = [''];
		if (KernelX.is_windows && allow_extension) {
			let path_extension_environment = get_environment('PATHEXT');
			if (path_extension_environment === null) {
				throw new Error(`could not find 'PATHEXT' environment`);
			}
			path_extension_list.push(...path_extension_environment.split(item_delimiter).map((value) => (value.toLowerCase())));
		}
		for (let path of path_list) {
			let path_base = `${path}/${name}`;
			let path_extension = path_extension_list.find((value) => (KernelX.Storage.exist_file(`${path_base}${value}`)));
			if (path_extension !== undefined) {
				result = `${path_base}${path_extension}`;
				break;
			}
		}
		return result;
	}

	export function search_program_ensure(
		name: string,
		allow_extension: boolean,
	): string {
		let result = search_program(name, allow_extension);
		if (result === null) {
			throw new Error(`could not find '${name}' program from 'PATH' environment`);
		}
		return result;
	}

	// #endregion

}
