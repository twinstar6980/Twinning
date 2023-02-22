namespace TwinStar.Script.EnvironmentVariable {

	// ------------------------------------------------

	export function parse(
		list: Array<string>,
	): Record<string, string> {
		return record_from_array(
			list,
			(index, element) => {
				let split_index = element.indexOf('=');
				return [element.substring(0, split_index), element.substring(split_index + 1, undefined)];
			},
		);
	}

	export function query(
		map: Record<string, string>,
		key: string,
	): null | string {
		let result: null | string = null;
		for (let element_key in map) {
			if (Shell.is_windows ? (element_key.toLowerCase() === key.toLowerCase()) : (element_key === key)) {
				result = map[element_key];
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

	export function search_from_path(
		name: string,
	): null | string {
		let result: null | string = null;
		let item_delimiter = Shell.is_windows ? ';' : ':';
		let environment = parse(CoreX.Process.environment());
		let path_string = query(environment, 'PATH');
		assert_test(path_string !== null, `environment PATH not found`);
		let path_list = path_string.split(item_delimiter);
		let path_extension_list = [''];
		if (Shell.is_windows) {
			let path_extension_string = query(environment, 'PATHEXT');
			assert_test(path_extension_string !== null, `environment PATHEXT not found`);
			path_extension_list.push(...path_extension_string.split(item_delimiter).map((e) => (e.toLowerCase())));
		}
		for (let path of path_list) {
			let path_prefix = `${path}/${name}`;
			let path_extension = path_extension_list.find((e) => (CoreX.FileSystem.exist_file(`${path_prefix}${e}`)));
			if (path_extension !== undefined) {
				result = `${path_prefix}${path_extension}`;
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

}