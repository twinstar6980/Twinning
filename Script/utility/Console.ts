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

	export let cli_disable_virtual_terminal_sequence = false;

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
		if (!cli_disable_virtual_terminal_sequence) {
			Shell.cli_output(VirtualTerminalSequence.text_attribute(k_cli_message_text_attribute[type]));
		}
		return;
	}

	// ------------------------------------------------

	function basic_input_with_checker<Value>(
		inputer: () => string | null,
		echoer: (value: string | null) => void,
		converter: (value: string) => string | [Value],
		nullable: boolean | null,
		checker: Check.CheckerX<Value> | null,
		initial: Value | null | undefined,
	): Value | null {
		let result: Value | null;
		if (initial !== undefined) {
			let state: string | null;
			result = initial;
			if (result === null) {
				state = nullable ? null : los('console:not_nullable');
			} else {
				state = checker === null ? null : checker(result);
			}
			if (state === null) {
				return result;
			}
			warning(los('console:input_invalid_should_retry'), [`${state}`]);
		}
		while (true) {
			let state: string | null;
			let input = inputer();
			echoer(input);
			result = null;
			if (input === null) {
				state = nullable ? null : los('console:not_nullable');
			} else {
				let convert_result = converter(input);
				if (typeof convert_result === 'string') {
					state = convert_result;
				} else {
					result = convert_result[0];
					state = checker === null ? null : checker(result);
				}
			}
			if (state === null) {
				break;
			}
			warning(los('console:input_invalid_should_retry'), [`${state}`]);
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
		converter: (value: string) => string | [Value],
		nullable: boolean | null,
		checker: Check.CheckerX<Value> | null,
		initial: Value | null | undefined = undefined,
	): Value | null {
		let first_input = true;
		return basic_input_with_checker(
			() => {
				if (first_input) {
					first_input = false;
					messager();
				}
				cli_set_message_text_attribute('t');
				cli_basic_output(`${leading} `, true, 0, false);
				let input = Shell.cli_input();
				cli_set_message_text_attribute('v');
				return input === '' ? null : input;
			},
			(value) => {
				return;
			},
			converter,
			nullable,
			checker,
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
		inputer: () => string | null,
		leading: string,
		echoer: (value: string) => string,
		converter: (value: string) => string | [Value],
		nullable: boolean | null,
		checker: Check.CheckerX<Value> | null,
		initial: Value | null | undefined = undefined,
	): Value | null {
		return basic_input_with_checker(
			inputer,
			(value) => {
				gui_basic_output('t', `${leading} ${value === null ? '' : echoer(value)}`, []);
				return;
			},
			converter,
			nullable,
			checker,
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
			cli_set_message_text_attribute('v');
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
		let leading = 'U';
		if (Shell.is_cli) {
			cli_set_message_text_attribute('t');
			if (Shell.is_windows) {
				cli_basic_output(`${leading} ${los('console:press_to_continue')} `, true, 0, false);
				CoreX.Process.system_command(`pause > NUL`);
				Shell.cli_output('\n');
			}
			if (Shell.is_linux || Shell.is_macintosh || Shell.is_android || Shell.is_iphone) {
				cli_basic_output(`${leading} ${los('console:press_enter_to_continue')} `, true, 0, false);
				Shell.cli_input();
			}
			cli_set_message_text_attribute('v');
		}
		if (Shell.is_gui) {
			Shell.gui_input_pause();
			gui_basic_output('t', `${leading} ${los('console:respond_to_continue')} `, []);
		}
		return;
	}

	// ------------------------------------------------

	export function confirmation(
		nullable: null,
		checker: Check.CheckerX<boolean> | null,
		initial?: boolean,
	): boolean;

	export function confirmation(
		nullable: boolean,
		checker: Check.CheckerX<boolean> | null,
		initial?: boolean | null,
	): boolean | null;

	export function confirmation(
		nullable: boolean | null,
		checker: Check.CheckerX<boolean> | null,
		initial?: boolean | null,
	): boolean | null {
		let result: boolean | null = undefined as any;
		let leading = 'C';
		let converter = (value: string): string | [boolean] => {
			let regexp_check_result = Check.enumeration_checker_x(['n', 'y'])(value);
			if (regexp_check_result !== null) {
				return los('console:confirmation_format_error');
			}
			return [parse_confirmation_string(value)];
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
					return Shell.gui_input_confirmation();
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

	export function number(
		nullable: null,
		checker: Check.CheckerX<number> | null,
		initial?: number,
	): number;

	export function number(
		nullable: boolean,
		checker: Check.CheckerX<number> | null,
		initial?: number | null,
	): number | null;

	export function number(
		nullable: boolean | null,
		checker: Check.CheckerX<number> | null,
		initial?: number | null,
	): number | null {
		let result: number | null = undefined as any;
		let leading = 'N';
		let converter = (value: string): string | [number] => {
			let regexp_check_result = Check.regexp_checker_x(/^([+-])?([\d]+)([.][\d]+)?$/)(value);
			if (regexp_check_result !== null) {
				return los('console:number_format_error', regexp_check_result);
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
					return Shell.gui_input_number();
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
		checker: Check.CheckerX<bigint> | null,
		initial?: bigint,
	): bigint;

	export function integer(
		nullable: boolean,
		checker: Check.CheckerX<bigint> | null,
		initial?: bigint | null,
	): bigint | null;

	export function integer(
		nullable: boolean | null,
		checker: Check.CheckerX<bigint> | null,
		initial?: bigint | null,
	): bigint | null {
		let result: bigint | null = undefined as any;
		let leading = 'I';
		let converter = (value: string): string | [bigint] => {
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

	export function size(
		nullable: null,
		checker: Check.CheckerX<bigint> | null,
		initial?: bigint,
	): bigint;

	export function size(
		nullable: boolean,
		checker: Check.CheckerX<bigint> | null,
		initial?: bigint | null,
	): bigint | null;

	export function size(
		nullable: boolean | null,
		checker: Check.CheckerX<bigint> | null,
		initial?: bigint | null,
	): bigint | null {
		let result: bigint | null = undefined as any;
		let leading = 'Z';
		let converter = (value: string): string | [bigint] => {
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
		checker: Check.CheckerX<string> | null,
		initial?: string,
	): string;

	export function string(
		nullable: boolean,
		checker: Check.CheckerX<string> | null,
		initial?: string | null,
	): string | null;

	export function string(
		nullable: boolean | null,
		checker: Check.CheckerX<string> | null,
		initial?: string | null,
	): string | null {
		let result: string | null = undefined as any;
		let leading = 'S';
		let converter = (value: string): string | [string] => {
			return [value];
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
		rule: ['any'] | ['in'] | ['out', 'none' | 'trash' | 'delete' | 'override'],
		nullable: null,
		checker: Check.CheckerX<string> | null,
		initial?: string,
	): string;

	export function path(
		type: 'any' | 'file' | 'directory',
		rule: ['any'] | ['in'] | ['out', 'none' | 'trash' | 'delete' | 'override'],
		nullable: boolean,
		checker: Check.CheckerX<string> | null,
		initial?: string | null,
	): string | null;

	export function path(
		type: 'any' | 'file' | 'directory',
		rule: ['any'] | ['in'] | ['out', 'none' | 'trash' | 'delete' | 'override'],
		nullable: boolean | null,
		checker: Check.CheckerX<string> | null,
		initial?: string | null,
	): string | null {
		let result: string | null = undefined as any;
		let leading = 'P';
		let state_data = {
			last_value: null as string | null,
			tactic_if_out_exist: 'none' as 'none' | 'trash' | 'delete' | 'override',
		};
		if (rule[0] === 'out') {
			state_data.tactic_if_out_exist = rule[1];
		}
		let converter = (value: string): string | [string] => {
			let result: string;
			if (value.length >= 1 && value[0] === ':') {
				switch (value.substring(1)) {
					case 'p': {
						if (rule[0] !== 'in') {
							return los('console:path_command_need_in');
						}
						let pick_result = Console.pick_path(type);
						if (pick_result === null) {
							return los('console:path_command_pick_cancel');
						}
						result = pick_result;
						cli_basic_output(`${result}`, false, 1, true);
						break;
					}
					case 'o': {
						if (rule[0] !== 'out') {
							return los('console:path_command_need_out');
						}
						if (state_data.last_value === null) {
							return los('console:path_command_need_last_value');
						}
						state_data.tactic_if_out_exist = 'override';
						result = state_data.last_value;
						break;
					}
					case 'd': {
						if (rule[0] !== 'out') {
							return los('console:path_command_need_out');
						}
						if (state_data.last_value === null) {
							return los('console:path_command_need_last_value');
						}
						state_data.tactic_if_out_exist = 'delete';
						result = state_data.last_value;
						break;
					}
					case 't': {
						if (rule[0] !== 'out') {
							return los('console:path_command_need_out');
						}
						if (state_data.last_value === null) {
							return los('console:path_command_need_last_value');
						}
						state_data.tactic_if_out_exist = 'trash';
						result = state_data.last_value;
						break;
					}
					default: {
						return los('console:path_command_invalid');
					}
				}
			} else {
				result = unquote(value);
			}
			return [result];
		};
		let common_checker = (value: string) => {
			state_data.last_value = value;
			if (rule[0] === 'any') {
				result = null;
			}
			if (rule[0] === 'in') {
				if (!CoreX.FileSystem.exist(value)) {
					result = los('console:path_not_exist');
				} else {
					switch (type) {
						case 'any': {
							result = null;
							break;
						}
						case 'file': {
							result = CoreX.FileSystem.exist_file(value) ? null : los('console:path_is_exist_not_file');
							break;
						}
						case 'directory': {
							result = CoreX.FileSystem.exist_directory(value) ? null : los('console:path_is_exist_not_directory');
							break;
						}
					}
				}
			}
			if (rule[0] === 'out') {
				if (!CoreX.FileSystem.exist(value)) {
					result = null;
				} else {
					switch (state_data.tactic_if_out_exist) {
						case 'none': {
							result = los('console:path_is_exist');
							break;
						}
						case 'trash': {
							HomeDirectory.new_trash(value);
							warning(los('console:path_is_exist_but_trash'), []);
							result = null;
							break;
						}
						case 'delete': {
							CoreX.FileSystem.remove(value);
							warning(los('console:path_is_exist_but_delete'), []);
							result = null;
							break;
						}
						case 'override': {
							warning(los('console:path_is_exist_but_override'), []);
							result = null;
							break;
						}
					}
				}
			}
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
				common_checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_path(type, rule[0] === 'any' ? rule[0] : `${rule[0]}put`);
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				common_checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function option<Value>(
		option: Array<[Value, string, string]>,
		nullable: boolean | null,
		checker: Check.CheckerX<Value> | null,
		initial?: Value,
	): Value;

	export function option<Value>(
		option: Array<[Value, string, string]>,
		nullable: boolean | null,
		checker: Check.CheckerX<Value> | null,
		initial?: Value | null,
	): Value | null;

	export function option<Value>(
		option: Array<[Value, string, string]>,
		nullable: boolean | null,
		checker: Check.CheckerX<Value> | null,
		initial?: Value | null,
	): Value | null {
		let result: Value | null = undefined as any;
		let leading = 'O';
		let maximum_key_length = Math.max(...option.map((e) => (e[1].length)));
		let message = option.map((e, i) => (`${make_prefix_padded_string(e[1], ' ', maximum_key_length)}. ${e[2]}`));
		if (Shell.is_cli) {
			result = cli_basic_input(
				leading,
				() => {
					for (let item of message) {
						cli_basic_output(item, false, 1, true);
					}
					return;
				},
				(value) => {
					let item = option.find((e) => (e[1] === value));
					if (item === undefined) {
						return los('console:option_invalid');
					}
					return [item[0]];
				},
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_gui) {
			result = gui_basic_input(
				() => {
					return Shell.gui_input_option(message);
				},
				leading,
				(value) => {
					return message[Number(value) - 1];
				},
				(value) => {
					let item = option[Number(value) - 1];
					return [item[0]];
				},
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
		for (let index = 0; index < value.length; ++index) {
			destination.push([value[index], `${value[index] === false ? 'n' : 'y'}`, ``]);
		}
		return destination;
	}

	export function option_integer<Value extends bigint>(
		value: Array<Value>,
	): Array<[Value, string, string]> {
		let destination: Array<[Value, string, string]> = [];
		for (let index = 0; index < value.length; ++index) {
			destination.push([value[index], `${value[index]}`, ``]);
		}
		return destination;
	}

	export function option_string<Value extends string>(
		value: Array<Value>,
	): Array<[Value, string, string]> {
		let destination: Array<[Value, string, string]> = [];
		for (let index = 0; index < value.length; ++index) {
			destination.push([value[index], `${index + 1}`, `${value[index]}`]);
		}
		return destination;
	}

	// ------------------------------------------------

	export function pick_path(
		type: 'any' | 'file' | 'directory',
	): string | null {
		let result: string | null = undefined as any;
		let actual_type: 'file' | 'directory';
		if (type !== 'any') {
			actual_type = type;
		} else {
			information(los('console:pick_path_type'), []);
			actual_type = option(option_string(['file', 'directory']), null, null);
		}
		if (Shell.is_cli) {
			result = Shell.cli_pick_path(actual_type);
		}
		if (Shell.is_gui) {
			result = Shell.gui_pick_path(actual_type);
		}
		return result;
	}

	export function push_notification(
		title: string,
		description: string,
	): void {
		if (Shell.is_cli && (Shell.is_windows || Shell.is_linux || Shell.is_macintosh)) {
			Shell.cli_push_notification(title, description);
		}
		if (Shell.is_gui && (Shell.is_windows || Shell.is_linux || Shell.is_macintosh || Shell.is_android)) {
			Shell.gui_push_notification(title, description);
		}
		return;
	}

	// ------------------------------------------------

}