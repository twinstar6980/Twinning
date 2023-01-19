/** 环境变量 */
namespace TwinStar.EnvironmentVariable {

	// ------------------------------------------------

	export function parse(
		source: Array<string>,
	): Record<string, string> {
		let result: Record<string, string> = {};
		for (let element of source) {
			let split_index = element.indexOf('=');
			let key = element.substring(0, split_index);
			let value = element.substring(split_index + 1, undefined);
			result[key] = value;
		}
		return result;
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

	export function search_from_path(
		name: string,
	): null | string {
		let result: null | string = null;
		let path_delimiter = Shell.is_windows ? ';' : ':';
		let environment = parse(CoreX.Process.environment());
		let path_string = query(environment, 'PATH');
		if (path_string === null) {
			throw new Error(`environment PATH not found`);
		}
		let path_list = path_string.split(path_delimiter);
		let path_extension_list = [''];
		if (Shell.is_windows) {
			let path_extension_string = query(environment, 'PATHEXT');
			if (path_extension_string === null) {
				throw new Error(`environment PATHEXT not found`);
			}
			path_extension_list.push(...path_extension_string.split(path_delimiter).map((e) => (e.toLowerCase())));
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