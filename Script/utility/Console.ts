namespace TwinStar.Script.Console {

	// ------------------------------------------------

	/**
	 * 消息分类
	 * + Verbosity   常规
	 * + Information 信息
	 * + Warning     警告
	 * + Error       错误
	 * + Success     成功
	 * + inpuT/Type  输入
	 */
	export type MessageType = 'v' | 'i' | 'w' | 'e' | 's' | 't';

	// ------------------------------------------------

	export let g_disable_cli_virtual_terminal_sequence = false;

	const k_cli_message_text_attribute: Record<MessageType, VirtualTerminalSequence.TextAttribute> = {
		v: {
			background: null,
			foreground: 'default',
			underline: false,
			bold: false,
		},
		i: {
			background: null,
			foreground: ['blue', false],
			underline: null,
			bold: false,
		},
		w: {
			background: null,
			foreground: ['yellow', false],
			underline: null,
			bold: false,
		},
		e: {
			background: null,
			foreground: ['red', false],
			underline: null,
			bold: false,
		},
		s: {
			background: null,
			foreground: ['green', false],
			underline: null,
			bold: false,
		},
		t: {
			background: null,
			foreground: ['magenta', false],
			underline: false,
			bold: true,
		},
	};

	function cli_set_message_text_attribute(
		type: MessageType,
	): void {
		if (!g_disable_cli_virtual_terminal_sequence) {
			Shell.cli_output(VirtualTerminalSequence.text_attribute(k_cli_message_text_attribute[type]));
		}
		return;
	}

	// ------------------------------------------------

	function basic_input_with_checker<Value>(
		inputer: () => string,
		echoer: (value: string) => void,
		converter: (value: string) => string | [null | Value],
		nullable: boolean,
		checker: Check.CheckerX<Value>,
		initial: undefined | null | Value,
	): null | Value {
		let result: null | Value;
		while (true) {
			let state: null | string = null;
			let input: null | string = null;
			result = null;
			if (initial !== undefined) {
				result = initial;
				initial = undefined;
			} else {
				input = inputer();
				echoer(input);
				let convert_result = converter(input);
				if (typeof convert_result === 'string') {
					state = convert_result;
				} else {
					result = convert_result[0];
				}
			}
			if (state === null) {
				if (result === null) {
					state = nullable ? null : los('console:not_nullable');
				} else {
					state = checker(result);
				}
			}
			if (state === null) {
				break;
			}
			warning(los('console:input_invalid_should_retry'), [state]);
		}
		return result;
	}

	// ------------------------------------------------

	function cli_basic_output(
		text: string,
		leading: boolean,
		indent: number,
		line_feed: boolean,
	): void {
		Shell.cli_output(`${leading ? '●' : ' '} ${'  '.repeat(indent)}${text}${line_feed ? '\n' : ''}`);
		return;
	}

	function cli_basic_input<Value>(
		leading: string,
		messager: () => void,
		converter: (value: string) => string | [null | Value],
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial: undefined | null | Value,
	): null | Value {
		let first_input = true;
		return basic_input_with_checker(
			() => {
				if (first_input) {
					first_input = false;
					messager();
				}
				cli_set_message_text_attribute('t');
				cli_basic_output(leading, true, 0, true);
				cli_set_message_text_attribute('v');
				cli_basic_output('', false, 1, false);
				return Shell.cli_input();
			},
			(value) => {
				return;
			},
			converter,
			nonnull_or(nullable, false),
			nonnull_or(checker, () => (null)),
			initial,
		);
	}

	// ------------------------------------------------

	function gui_basic_output(
		type: MessageType,
		title: string,
		description: Array<string>,
	): void {
		Shell.gui_output_message(type, title, description);
		return;
	}

	function gui_basic_input<Value>(
		inputer: () => string,
		leading: string,
		echoer: (value: string) => string,
		converter: (value: string) => string | [null | Value],
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial: undefined | null | Value,
	): null | Value {
		return basic_input_with_checker(
			inputer,
			(value) => {
				gui_basic_output('t', leading, [echoer(value)]);
				return;
			},
			converter,
			nonnull_or(nullable, false),
			nonnull_or(checker, () => (null)),
			initial,
		);
	}

	// ------------------------------------------------

	export function message(
		type: MessageType,
		title: string,
		description: Array<string>,
	): void {
		if (Shell.is_cli) {
			cli_set_message_text_attribute(type);
			cli_basic_output(title, true, 0, true);
			cli_set_message_text_attribute('v');
			for (let description_element of description) {
				cli_basic_output(description_element, false, 1, true);
			}
		}
		if (Shell.is_gui) {
			gui_basic_output(type, title, description);
		}
		return;
	}

	export function verbosity(
		title: string,
		description: Array<string>,
	): void {
		return message('v', title, description);
	}

	export function information(
		title: string,
		description: Array<string>,
	): void {
		return message('i', title, description);
	}

	export function warning(
		title: string,
		description: Array<string>,
	): void {
		return message('w', title, description);
	}

	export function error(
		title: string,
		description: Array<string>,
	): void {
		return message('e', title, description);
	}

	export function success(
		title: string,
		description: Array<string>,
	): void {
		return message('s', title, description);
	}

	export function error_of(
		error: any,
	): void {
		let [title, description] = parse_error_message(error);
		message('e', title, description);
		return;
	}

	// ------------------------------------------------

	export function pause(
	): void {
		let leading = 'Pause';
		if (Shell.is_cli) {
			cli_set_message_text_attribute('t');
			cli_basic_output(leading + ' ', true, 0, false);
			cli_set_message_text_attribute('v');
			if (Shell.is_windows) {
				KernelX.Process.system_command(`pause > NUL`);
				Shell.cli_output('\n');
			}
			if (Shell.is_linux || Shell.is_macintosh || Shell.is_android || Shell.is_iphone) {
				Shell.cli_input();
			}
		}
		if (Shell.is_gui) {
			Shell.gui_input_pause();
			gui_basic_output('t', leading, []);
		}
		return;
	}

	// ------------------------------------------------

	export function boolean(
		nullable: null,
		checker: null | Check.CheckerX<boolean>,
		initial?: boolean,
	): boolean;

	export function boolean(
		nullable: boolean,
		checker: null | Check.CheckerX<boolean>,
		initial?: null | boolean,
	): null | boolean;

	export function boolean(
		nullable: null | boolean,
		checker: null | Check.CheckerX<boolean>,
		initial?: null | boolean,
	): null | boolean {
		let result: null | boolean = undefined!;
		let leading = 'Boolean';
		let converter = (value: string): string | [null | boolean] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.enumeration_checker_x(['n', 'y'])(value);
			if (regexp_check_result !== null) {
				return los('console:boolean_format_error');
			}
			return [parse_confirmation_boolean_string(value)];
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_boolean();
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function integer(
		nullable: null,
		checker: null | Check.CheckerX<bigint>,
		initial?: bigint,
	): bigint;

	export function integer(
		nullable: boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint;

	export function integer(
		nullable: null | boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint {
		let result: null | bigint = undefined!;
		let leading = 'Integer';
		let converter = (value: string): string | [null | bigint] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.regexp_checker_x(/^([+-])?([\d]+)$/)(value);
			if (regexp_check_result !== null) {
				return los('console:integer_format_error', regexp_check_result);
			}
			return [BigInt(value)];
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_integer();
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function floater(
		nullable: null,
		checker: null | Check.CheckerX<number>,
		initial?: number,
	): number;

	export function floater(
		nullable: boolean,
		checker: null | Check.CheckerX<number>,
		initial?: null | number,
	): null | number;

	export function floater(
		nullable: null | boolean,
		checker: null | Check.CheckerX<number>,
		initial?: null | number,
	): null | number {
		let result: null | number = undefined!;
		let leading = 'Floater';
		let converter = (value: string): string | [null | number] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.regexp_checker_x(/^([+-])?([\d]+)([.][\d]+)?$/)(value);
			if (regexp_check_result !== null) {
				return los('console:floater_format_error', regexp_check_result);
			}
			return [Number(value)];
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_floater();
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function size(
		nullable: null,
		checker: null | Check.CheckerX<bigint>,
		initial?: bigint,
	): bigint;

	export function size(
		nullable: boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint;

	export function size(
		nullable: null | boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint {
		let result: null | bigint = undefined!;
		let leading = 'Size';
		let converter = (value: string): string | [null | bigint] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.regexp_checker_x(/^([\d]+)([.][\d]+)?([bkmg])$/)(value);
			if (regexp_check_result !== null) {
				return los('console:size_format_error', regexp_check_result);
			}
			return [parse_size_string(value)];
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_size();
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function string(
		nullable: null,
		checker: null | Check.CheckerX<string>,
		initial?: string,
	): string;

	export function string(
		nullable: boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string;

	export function string(
		nullable: null | boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string {
		let result: null | string = undefined!;
		let leading = 'String';
		let converter = (value: string): string | [null | string] => {
			if (value === '') {
				return [null];
			}
			let result: string;
			if (value[0] !== ':') {
				result = value;
			} else {
				switch (value[1]) {
					case ':': {
						result = value.substring(2);
						break;
					}
					default: {
						return los('console:input_command_invalid');
					}
				}
			}
			return [result];
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_string();
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function path(
		type: 'any' | 'file' | 'directory',
		rule: 'any' | 'input' | 'output',
		nullable: null,
		checker: null | Check.CheckerX<string>,
		initial?: string,
	): string;

	export function path(
		type: 'any' | 'file' | 'directory',
		rule: 'any' | 'input' | 'output',
		nullable: boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string;

	export function path(
		type: 'any' | 'file' | 'directory',
		rule: 'any' | 'input' | 'output',
		nullable: null | boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string {
		let result: null | string = undefined!;
		let leading = 'Path';
		let state_data = {
			last_value: null as null | string,
			allow_overwrite: false as boolean,
		};
		if (initial !== undefined && initial !== null) {
			initial = Home.of(PathUtility.regularize(initial));
		}
		let converter = (value: string): string | [null | string] => {
			if (value === '') {
				return [null];
			}
			let result: string;
			if (value[0] !== ':') {
				result = Home.of(PathUtility.regularize(unquote_string(value)));
			} else {
				switch (value[1]) {
					case ':': {
						result = Home.of(PathUtility.regularize(value.substring(2)));
						break;
					}
					case 'p': {
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						let pick_result = pick_path(type);
						if (pick_result === null) {
							return los('console:path_command_pick_cancel');
						}
						result = pick_result;
						message(`t`, leading, [result]);
						break;
					}
					case 'g': {
						if (rule !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.FileSystem.exist(result)) {
							result = PathUtility.generate_suffix_path(result);
							warning(los('console:path_is_exist_but_generate'), [result]);
						}
						break;
					}
					case 'm': {
						if (rule !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.FileSystem.exist(result)) {
							let move_path = PathUtility.generate_suffix_path(result);
							KernelX.FileSystem.rename(result, move_path);
							warning(los('console:path_is_exist_but_move'), [move_path]);
						}
						break;
					}
					case 'd': {
						if (rule !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.FileSystem.exist(result)) {
							KernelX.FileSystem.remove(result);
							warning(los('console:path_is_exist_but_delete'), []);
						}
						break;
					}
					case 'o': {
						if (rule !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.FileSystem.exist(result)) {
							state_data.allow_overwrite = true;
							warning(los('console:path_is_exist_but_overwrite'), []);
						}
						break;
					}
					default: {
						return los('console:input_command_invalid');
					}
				}
			}
			return [result];
		};
		let checker_proxy = (value: string) => {
			let result = null as null | string;
			if (rule === 'any') {
				result = null;
			}
			if (rule === 'input') {
				if (!KernelX.FileSystem.exist(value)) {
					result = los('console:path_not_exist');
				} else {
					switch (type) {
						case 'any': {
							result = null;
							break;
						}
						case 'file': {
							result = KernelX.FileSystem.exist_file(value) ? null : los('console:path_is_exist_not_file');
							break;
						}
						case 'directory': {
							result = KernelX.FileSystem.exist_directory(value) ? null : los('console:path_is_exist_not_directory');
							break;
						}
					}
				}
			}
			if (rule === 'output') {
				if (!KernelX.FileSystem.exist(value)) {
					result = null;
				} else {
					result = state_data.allow_overwrite ? null : los('console:path_is_exist');
				}
			}
			state_data.last_value = value;
			state_data.allow_overwrite = false;
			if (result !== null) {
				return result;
			}
			return checker === null ? null : checker(value);
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker_proxy,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_path();
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker_proxy,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function enumeration<Value>(
		option: Array<[Value, string, string]>,
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial?: Value,
	): Value;

	export function enumeration<Value>(
		option: Array<[Value, string, string]>,
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial?: null | Value,
	): null | Value;

	export function enumeration<Value>(
		option: Array<[Value, string, string]>,
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial?: null | Value,
	): null | Value {
		let result: null | Value = undefined!;
		let leading = 'Enumeration';
		let maximum_key_length = Math.max(...option.map((e) => (e[1].length)));
		let message = option.map((value) => (`${make_prefix_padded_string(value[1], ' ', maximum_key_length)}. ${value[2]}`));
		let converter = (value: string): string | [null | Value] => {
			if (value === '') {
				return [null];
			}
			let index = option.findIndex((e) => (e[1] === value));
			if (index === -1) {
				return los('console:option_invalid');
			}
			return [option[index][0]];
		};
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					for (let item of message) {
						cli_basic_output(item, false, 1, true);
					}
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					let index = Shell.gui_input_enumeration(message);
					return index === '' ? '' : option[Number(index) - 1][1];
				},
				leading,
				(value) => {
					if (value === '') {
						return '';
					}
					let index = option.findIndex((e) => (e[1] === value));
					if (index === -1) {
						return los('console:option_invalid');
					}
					return message[index];
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function option_boolean<Value extends boolean>(
		value: Array<Value>,
	): Array<[Value, string, string]> {
		let destination: Array<[Value, string, string]> = [];
		for (let index = 0; index < value.length; index++) {
			destination.push([value[index], `${value[index] === false ? 'n' : 'y'}`, ``]);
		}
		return destination;
	}

	export function option_floater<Value extends number>(
		value: Array<Value>,
	): Array<[Value, string, string]> {
		let destination: Array<[Value, string, string]> = [];
		for (let index = 0; index < value.length; index++) {
			destination.push([value[index], `${value[index]}`, ``]);
		}
		return destination;
	}

	export function option_integer<Value extends bigint>(
		value: Array<Value>,
	): Array<[Value, string, string]> {
		let destination: Array<[Value, string, string]> = [];
		for (let index = 0; index < value.length; index++) {
			destination.push([value[index], `${value[index]}`, ``]);
		}
		return destination;
	}

	export function option_string<Value extends string>(
		value: Array<Value>,
	): Array<[Value, string, string]> {
		let destination: Array<[Value, string, string]> = [];
		for (let index = 0; index < value.length; index++) {
			destination.push([value[index], `${index + 1}`, `${value[index]}`]);
		}
		return destination;
	}

	// ------------------------------------------------

	export function pick_path(
		type: 'any' | 'file' | 'directory',
	): null | string {
		let result: null | string = undefined!;
		if (type === 'any') {
			information(los('console:pick_path_type'), []);
			type = enumeration(option_string(['file', 'directory']), null, null);
		}
		if (Shell.is_cli) {
			result = Shell.cli_pick_path(type);
		}
		if (Shell.is_gui) {
			result = Shell.gui_pick_path(type);
		}
		if (result === '') {
			result = null;
		}
		return result;
	}

	// ------------------------------------------------

	export let g_disable_notification = false;

	export function push_notification(
		title: string,
		description: string,
	): void {
		if (g_disable_notification) {
			return;
		}
		if (Shell.is_cli) {
			Shell.cli_push_notification(title, description);
		}
		if (Shell.is_gui) {
			Shell.gui_push_notification(title, description);
		}
		return;
	}

	// ------------------------------------------------

}