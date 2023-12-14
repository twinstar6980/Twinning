namespace TwinStar.Script.ProcessHelper {

	// ------------------------------------------------

	export function parse_environment_variable(
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

	export function query_environment_variable(
		map: Record<string, string>,
		key: string,
	): null | string {
		let result: null | string = null;
		for (let element_key in map) {
			if (Shell.is_windows ? element_key.toLowerCase() === key.toLowerCase() : element_key === key) {
				result = map[element_key];
				break;
			}
		}
		return result;
	}

	export function search_path(
		name: string,
	): null | string {
		let result: null | string = null;
		let item_delimiter = Shell.is_windows ? ';' : ':';
		let path_environment = KernelX.Process.get_environment_variable('PATH');
		assert_test(path_environment !== null, `environment PATH not found`);
		let path_list = path_environment.split(item_delimiter);
		let path_extension_list = [''];
		if (Shell.is_windows) {
			let path_extension_environment = KernelX.Process.get_environment_variable('PATHEXT');
			assert_test(path_extension_environment !== null, `environment PATHEXT not found`);
			path_extension_list.push(...path_extension_environment.split(item_delimiter).map((value) => (value.toLowerCase())));
		}
		for (let path of path_list) {
			let path_base = `${path}/${name}`;
			let path_extension = path_extension_list.find((value) => (KernelX.FileSystem.exist_file(`${path_base}${value}`)));
			if (path_extension !== undefined) {
				result = `${path_base}${path_extension}`;
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

	export type ExecuteResult = {
		code: bigint;
		output: string;
		error: string;
	};

	export function execute(
		program: string,
		argument: Array<string>,
		environment: Array<string>,
		input_data: string = '',
	): ExecuteResult {
		let temporary_directory = Home.new_temporary(null, 'directory');
		let temporary_directory_fallback: null | string = null;
		let input = `${temporary_directory}/input`;
		let output = `${temporary_directory}/output`;
		let error = `${temporary_directory}/error`;
		KernelX.FileSystem.write_file(input, Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(input_data)));
		if (Shell.is_android && Shell.is_gui) {
			temporary_directory_fallback = `${AndroidHelper.k_remote_temporary_directory}/${PathUtility.name(temporary_directory)}`;
			output = `${temporary_directory_fallback}/output`;
			error = `${temporary_directory_fallback}/error`;
			assert_test(KernelX.Process.system_command(`su -c "mkdir -p -m 777 ${temporary_directory_fallback} ; touch ${output} ; chmod 777 ${output} ; touch ${error} ; chmod 777 ${error}"`) === 0n);
		}
		else {
			KernelX.FileSystem.create_file(output);
			KernelX.FileSystem.create_file(error);
		}
		let code = KernelX.Process.spawn_process(program, argument, environment, input, output, error);
		let read_file = (path: string) => {
			let data = KernelX.FileSystem.read_file(path);
			let data_string = Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
			return normalize_string_line_feed(data_string);
		};
		let result = {
			code: code,
			output: read_file(output),
			error: read_file(error),
		};
		KernelX.FileSystem.remove(temporary_directory);
		if (Shell.is_android && Shell.is_gui) {
			assert_test(temporary_directory_fallback !== null);
			assert_test(KernelX.Process.system_command(`su -c "rm -rf ${temporary_directory_fallback}"`) === 0n);
		}
		return result;
	}

	// ------------------------------------------------

}