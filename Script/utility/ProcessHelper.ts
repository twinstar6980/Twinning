namespace Twinning.Script.ProcessHelper {

	// #region environment

	export function parse_environment(
		list: Array<string>,
	): Record<string, string> {
		return record_from_array(
			list,
			(index, element) => {
				let split_index = element.indexOf('=');
				return [element.slice(0, split_index), element.slice(split_index + 1)];
			},
		);
	}

	export function query_environment(
		map: Record<string, string>,
		key: string,
	): null | string {
		let result: null | string = null;
		for (let element_key in map) {
			if (KernelX.is_windows ? element_key.toLowerCase() === key.toLowerCase() : element_key === key) {
				result = map[element_key];
				break;
			}
		}
		return result;
	}

	// #endregion

	// #region child

	export type ProgramResult = {
		code: bigint;
		output: string;
		error: string;
	};

	// ----------------

	export function run_process(
		program: string | [string],
		argument: Array<string>,
		environment: null | Array<string>,
		input_data: string = '',
	): ProgramResult {
		if (is_object_of_array(program)) {
			program = search_program_ensure(program[0]);
		}
		if (environment === null) {
			environment = KernelX.Process.list_environment();
		}
		let temporary_directory = HomePath.new_temporary(null, 'directory');
		let temporary_directory_fallback: null | string = null;
		let input = `${temporary_directory}/input`;
		let output = `${temporary_directory}/output`;
		let error = `${temporary_directory}/error`;
		KernelX.Storage.write_file_s(input, input_data);
		if (KernelX.is_android && !Shell.is_basic) {
			temporary_directory_fallback = `${AndroidHelper.k_remote_temporary_directory}/${PathUtility.name(temporary_directory)}`;
			output = `${temporary_directory_fallback}/output`;
			error = `${temporary_directory_fallback}/error`;
			assert_test(KernelX.Process.execute_command(`su -c "mkdir -p -m 777 ${temporary_directory_fallback} ; touch ${output} ; chmod 777 ${output} ; touch ${error} ; chmod 777 ${error}"`) === 0n);
		}
		else {
			KernelX.Storage.create_file(output);
			KernelX.Storage.create_file(error);
		}
		let code = KernelX.Process.run_process(program, argument, environment, input, output, error);
		let read_file = (path: string) => {
			let data = KernelX.Storage.read_file_s(path);
			return normalize_string_line_feed(data);
		};
		let result: ProgramResult = {
			code: code,
			output: read_file(output),
			error: read_file(error),
		};
		KernelX.Storage.remove(temporary_directory);
		if (KernelX.is_android && !Shell.is_basic) {
			assert_test(temporary_directory_fallback !== null);
			assert_test(KernelX.Process.execute_command(`su -c "rm -rf ${temporary_directory_fallback}"`) === 0n);
		}
		return result;
	}

	// #endregion

	// #region program path

	export let g_program_path_map: Record<string, string> = {};

	// ----------------

	export function search_program(
		name: string,
		allow_extension: boolean,
	): null | string {
		if (g_program_path_map[name] !== undefined) {
			return g_program_path_map[name];
		}
		let result: null | string = null;
		let item_delimiter = KernelX.is_windows ? ';' : ':';
		let path_environment = KernelX.Process.get_environment('PATH');
		if (path_environment === null) {
			throw new Error(`could not find 'PATH' environment`);
		}
		let path_list = path_environment.split(item_delimiter);
		let path_extension_list = [''];
		if (allow_extension && KernelX.is_windows) {
			let path_extension_environment = KernelX.Process.get_environment('PATHEXT');
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
		allow_extension: boolean = true,
	): string {
		let path = search_program(name, allow_extension);
		if (path === null) {
			throw new Error(`could not find '${name}' program from 'PATH' environment`);
		}
		return path;
	}

	// #endregion

}