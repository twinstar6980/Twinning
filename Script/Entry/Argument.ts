/** 参数解析 */
namespace TwinKleS.Entry.Argument {

	// ------------------------------------------------

	export type Require<T> = T;

	export type Request<T, ProvideDefaultValue> = T | '?input' | (ProvideDefaultValue extends true ? '?default' : never);

	// ------------------------------------------------

	export function require<Given, Raw, Result extends Raw>(
		name: string,
		message: string,
		given: Given,
		given_converter: (value: Given) => Raw,
		checker: Check.Checker<Raw>,
	): Result {
		let result: Raw;
		result = given_converter(given);
		Console.notify('i', `已取得参数：${name}`, [`${result}`]);
		if (!checker(result)) {
			Console.notify('w', `参数值无效`, []);
			throw new Error(`require argument is invalid`);
		}
		return result as Result;
	}

	export function request<Given, Raw, Result extends Raw>(
		name: string,
		message: string,
		given: Given | '?default' | '?input',
		given_converter: (value: Given) => Raw,
		default_generator: (() => Raw) | null,
		input_generator: () => Raw,
		checker: Check.CheckerX<Raw>,
	): Result {
		let result: Raw;
		if (given === '?input') {
			Console.notify('i', `请输入参数：${name}`, []);
			result = input_generator();
		} else if (given === '?default') {
			if (default_generator === null) {
				throw new Error(`?default is disabled`);
			}
			let default_value = default_generator();
			result = default_value;
			Console.notify('i', `已默认参数：${name}`, [`${result}`]);
		} else {
			result = given_converter(given);
			Console.notify('i', `已取得参数：${name}`, [`${result}`]);
		}
		while (true) {
			let check_result = checker(result);
			if (check_result === null) {
				break;
			}
			Console.notify('w', `参数值无效，请重新输入`, [check_result]);
			result = input_generator();
		}
		return result as Result;
	}

	// ------------------------------------------------

	export function requester_for_path(
		type: 'any' | 'file' | 'directory',
		require_type: [null] | [false, 'trash' | 'delete' | 'override' | null] | [true],
	): [() => string, (value: string) => null | string] {
		let state_data = {
			last_value: null as string | null,
			tactic_if_exist: null as 'trash' | 'delete' | 'override' | null,
		};
		return [
			(): string => {
				let input = Console.string(null);
				if (input.length > 0 && input[0] === ':') {
					if (input.length !== 2) {
						throw new Error(`command name is too long`);
					}
					switch (input[1]) {
						default: {
							throw new Error(`command name is invalid`);
							break;
						}
						case 't': {
							if (require_type[0] !== false) {
								throw new Error(`trash command precondition failed`);
							}
							if (state_data.last_value === null) {
								throw new Error(`last_value is undefined`);
							}
							state_data.tactic_if_exist = 'trash';
							input = state_data.last_value;
							break;
						}
						case 'd': {
							if (require_type[0] !== false) {
								throw new Error(`delete command precondition failed`);
							}
							if (state_data.last_value === null) {
								throw new Error(`last_value is undefined`);
							}
							state_data.tactic_if_exist = 'delete';
							input = state_data.last_value;
							break;
						}
						case 'o': {
							if (require_type[0] !== false) {
								throw new Error(`override command precondition failed`);
							}
							if (state_data.last_value === null) {
								throw new Error(`last_value is undefined`);
							}
							state_data.tactic_if_exist = 'override';
							input = state_data.last_value;
							break;
						}
						case 's': {
							if (Shell.is_windows) {
								throw new Error(`show(_open_fild_dialog) command only enabled on windows shell`);
							}
							let pick_folder: boolean;
							switch (type) {
								case 'any': {
									Console.notify('i', `选择窗口是否应选择目录（否则为文件）？`, []);
									pick_folder = Console.confirm(null);
									break;
								}
								case 'file': {
									pick_folder = false;
									break;
								}
								case 'directory': {
									pick_folder = true;
									break;
								}
							}
							let selected = Shell.windows_cli_open_file_dialog(pick_folder, false);
							if (selected.length === 0) {
								input = '';
							} else {
								input = selected[0];
							}
							break;
						}
					}
				}
				return input;
			},
			(value): null | string => {
				let result: null | string;
				state_data.last_value = value;
				if (require_type[0] === false && state_data.tactic_if_exist === null) {
					state_data.tactic_if_exist = require_type[1];
				}
				if (require_type[0] === null) {
					result = null;
				} else {
					if (!require_type[0]) {
						if (CoreX.FileSystem.exist(value)) {
							switch (state_data.tactic_if_exist) {
								case 'trash': {
									Console.notify('w', `指定路径已存在，现已回收`, []);
									trash(value);
									result = null;
									break;
								}
								case 'delete': {
									Console.notify('w', `指定路径已存在，现已删除`, []);
									CoreX.FileSystem.remove(value);
									result = null;
									break;
								}
								case 'override': {
									Console.notify('w', `指定路径已存在，现将覆盖`, []);
									result = null;
									break;
								}
								case null: {
									result = `指定路径已存在`;
									break;
								}
							}
						} else {
							result = null;
						}
					} else {
						let checker: (path: string) => boolean;
						switch (type) {
							case 'any': {
								checker = CoreX.FileSystem.exist;
								break;
							}
							case 'file': {
								checker = CoreX.FileSystem.exist_file;
								break;
							}
							case 'directory': {
								checker = CoreX.FileSystem.exist_directory;
								break;
							}
						}
						result = checker(value) ? null : `指定路径不存在`;
					}
				}
				return result;
			},
		];
	}

	// ------------------------------------------------

}