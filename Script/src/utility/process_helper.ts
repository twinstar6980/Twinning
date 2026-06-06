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
		path: StoragePath;
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
			path: program,
			code: code,
			output: read_file(output_file),
			error: read_file(error_file),
		};
		StorageHelper.remove(temporary_directory);
		return result;
	}

	// #endregion

}
