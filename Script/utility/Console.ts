namespace TwinStar.Script.Console {

	// ------------------------------------------------

	/**
	 * 消息分类
	 * + Verbose     常规
	 * + Information 信息
	 * + Warning     警告
	 * + Error       错误
	 * + Success     成功
	 * + inpuT/Type  输入
	 */
	export type MessageType = 'v' | 'i' | 'w' | 'e' | 's' | 't';

	// ------------------------------------------------

	export let cli_disable_virtual_terminal_sequences = false;

	const k_cli_message_text_attribute: Record<MessageType, VirtualTerminalSequences.TextAttribute> = {
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
		if (!cli_disable_virtual_terminal_sequences) {
			Shell.cli_output(VirtualTerminalSequences.text_attribute(k_cli_message_text_attribute[type]));
		}
		return;
	}

	// ------------------------------------------------

	function basic_input_with_checker<Value>(
		inputer: () => Value | null,
		printer: (value: Value | null) => void,
		checker: Check.CheckerX<Value> | null,
		nullable: boolean,
	): Value | null {
		let result: Value | null;
		while (true) {
			let input = inputer();
			printer(input);
			let check_result: string | null;
			if (input === null) {
				check_result = nullable ? null : los(`不可为空`);
			} else {
				check_result = checker === null ? null : checker(input);
			}
			if (check_result === null) {
				result = input;
				break;
			}
			notify('w', los(`输入无效，请重新输入`), [`${check_result}`]);
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

	function cli_basic_input(
		leading: string,
		checker: Check.CheckerX<string> | null,
		nullable: boolean,
	): string | null {
		return basic_input_with_checker(
			() => {
				cli_set_message_text_attribute('t');
				cli_basic_output(`${leading} `, true, 0, false);
				let input = Shell.cli_input();
				cli_set_message_text_attribute('v');
				return input === '' ? null : input;
			},
			(value) => {
				return;
			},
			checker,
			nullable,
		);
	}

	// ------------------------------------------------

	function gui_basic_output(
		type: MessageType,
		title: string,
		description: Array<string>,
	): void {
		Shell.gui_output_notify(type, title, description);
		return;
	}

	function gui_basic_input<Value>(
		inputer: () => Value | null,
		printer_leading: string,
		printer_content: (value: Value) => string,
		checker: Check.CheckerX<Value> | null,
		nullable: boolean,
	): Value | null {
		return basic_input_with_checker(
			inputer,
			(value) => {
				gui_basic_output('t', `${printer_leading} ${value === null ? '' : printer_content(value)}`, []);
				return;
			},
			checker,
			nullable,
		);
	}

	// ------------------------------------------------

	export function notify(
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

	export function notify_error(
		error: any,
	): void {
		if (error instanceof Error) {
			if (error.name === 'NativeError') {
				Console.notify('e', `${error.name}`, [...error.message.split('\n'), ...parse_stack_string(error.stack)]);
			} else {
				Console.notify('e', `${error.name} : ${error.message}`, [...parse_stack_string(error.stack)]);
			}
		} else {
			Console.notify('e', `${error}`, []);
		}
		return;
	}

	// ------------------------------------------------

	export function pause(
	): void {
		if (Shell.is_cli) {
			cli_set_message_text_attribute('t');
			if (Shell.is_windows) {
				cli_basic_output(`P ${los(`键入以继续 ...`)} `, true, 0, false);
				CoreX.Process.system(`pause > NUL`);
				Shell.cli_output('\n');
			}
			if (Shell.is_linux || Shell.is_macintosh || Shell.is_android || Shell.is_iphone) {
				if (CoreX.FileSystem.exist_file(`/bin/bash`)) {
					cli_basic_output(`P ${los(`键入以继续 ...`)} `, true, 0, false);
					CoreX.Process.execute(`/bin/bash`, [`-c`, `read -s -n 1 _`], [], null, null, null);
					Shell.cli_output('\n');
				} else {
					cli_basic_output(`P ${los(`键入回车以继续 ...`)} `, true, 0, false);
					Shell.cli_input();
				}
			}
			cli_set_message_text_attribute('v');
		}
		if (Shell.is_gui) {
			Shell.gui_input_pause();
			gui_basic_output('t', `P ${los(`响应以继续 ...`)} `, []);
		}
		return;
	}

	// ------------------------------------------------

	export function confirm(
		checker: Check.CheckerX<boolean> | null,
	): boolean;

	export function confirm(
		checker: Check.CheckerX<boolean> | null,
		nullable: boolean,
	): boolean | null;

	export function confirm(
		checker: Check.CheckerX<boolean> | null,
		nullable: boolean = false,
	): boolean | null {
		let result: boolean | null = undefined as any;
		if (Shell.is_cli) {
			let input = cli_basic_input(
				'C',
				(value) => {
					let regexp_check_result = Check.enumeration_checker_x(['n', 'y'])(value);
					if (regexp_check_result !== null) {
						return los(`确认值格式非法，须为 n 或 y`);
					}
					return checker === null ? null : checker(parse_confirm_string(value));
				},
				nullable,
			);
			result = input === null ? null : input === 'y';
		}
		if (Shell.is_gui) {
			let input = gui_basic_input(
				() => {
					return Shell.gui_input_confirm();
				},
				'C',
				(value) => {
					return value;
				},
				(value) => {
					return checker === null ? null : checker(parse_confirm_string(value));
				},
				nullable,
			);
			result = input === null ? null : parse_confirm_string(input);
		}
		return result;
	}

	// ------------------------------------------------

	export function number(
		checker: Check.CheckerX<number> | null,
	): number;

	export function number(
		checker: Check.CheckerX<number> | null,
		nullable: boolean,
	): number | null;

	export function number(
		checker: Check.CheckerX<number> | null,
		nullable: boolean = false,
	): number | null {
		let result: number | null = undefined as any;
		if (Shell.is_cli) {
			let input = cli_basic_input(
				'N',
				(value) => {
					let regexp_check_result = Check.regexp_checker_x(/^(\+|\-)?(\d+)(\.\d*)?$/)(value);
					if (regexp_check_result !== null) {
						return los(`数字格式非法，{}`, regexp_check_result);
					}
					return checker === null ? null : checker(Number.parseFloat(value));
				},
				nullable,
			);
			result = input === null ? null : Number.parseFloat(input);
		}
		if (Shell.is_gui) {
			let input = gui_basic_input(
				() => {
					return Shell.gui_input_number();
				},
				'N',
				(value) => {
					return value;
				},
				(value) => {
					return checker === null ? null : checker(Number(value));
				},
				nullable,
			);
			result = input === null ? null : Number(input);
		}
		return result;
	}

	// ------------------------------------------------

	export function integer(
		checker: Check.CheckerX<bigint> | null,
	): bigint;

	export function integer(
		checker: Check.CheckerX<bigint> | null,
		nullable: boolean,
	): bigint | null;

	export function integer(
		checker: Check.CheckerX<bigint> | null,
		nullable: boolean = false,
	): bigint | null {
		let result: bigint | null = undefined as any;
		if (Shell.is_cli) {
			let input = cli_basic_input(
				'I',
				(value) => {
					let regexp_check_result = Check.regexp_checker_x(/^(\+|\-)?(\d+)$/)(value);
					if (regexp_check_result !== null) {
						return los(`整数格式非法，{}`, regexp_check_result);
					}
					return checker === null ? null : checker(BigInt(value));
				},
				nullable,
			);
			result = input === null ? null : BigInt(input);
		}
		if (Shell.is_gui) {
			let input = gui_basic_input(
				() => {
					return Shell.gui_input_integer();
				},
				'I',
				(value) => {
					return value;
				},
				(value) => {
					return checker === null ? null : checker(BigInt(value));
				},
				nullable,
			);
			result = input === null ? null : BigInt(input);
		}
		return result;
	}

	// ------------------------------------------------

	export function size(
		checker: Check.CheckerX<bigint> | null,
	): bigint;

	export function size(
		checker: Check.CheckerX<bigint> | null,
		nullable: boolean,
	): bigint | null;

	export function size(
		checker: Check.CheckerX<bigint> | null,
		nullable: boolean = false,
	): bigint | null {
		let result: bigint | null = undefined as any;
		if (Shell.is_cli) {
			let input = cli_basic_input(
				'Z',
				(value) => {
					let regexp_check_result = Check.regexp_checker_x(/^(\d+)(\.\d*)?(b|k|m|g)$/)(value);
					if (regexp_check_result !== null) {
						return los(`尺寸格式非法，{}`, regexp_check_result);
					}
					return checker === null ? null : checker(parse_size_string(value));
				},
				nullable,
			);
			result = input === null ? null : parse_size_string(input);
		}
		if (Shell.is_gui) {
			let input = gui_basic_input(
				() => {
					return Shell.gui_input_size();
				},
				'Z',
				(value) => {
					return value;
				},
				(value) => {
					return checker === null ? null : checker(parse_size_string(value));
				},
				nullable,
			);
			result = input === null ? null : parse_size_string(input);
		}
		return result;
	}

	// ------------------------------------------------

	export function string(
		checker: Check.CheckerX<string> | null,
	): string;

	export function string(
		checker: Check.CheckerX<string> | null,
		nullable: boolean,
	): string | null;

	export function string(
		checker: Check.CheckerX<string> | null,
		nullable: boolean = false,
	): string | null {
		let result: string | null = undefined as any;
		if (Shell.is_cli) {
			let input = cli_basic_input(
				'S',
				checker,
				nullable,
			);
			result = input;
		}
		if (Shell.is_gui) {
			let input = gui_basic_input(
				() => {
					return Shell.gui_input_string();
				},
				'S',
				(value) => {
					return value;
				},
				(value) => {
					return checker === null ? null : checker(String(value));
				},
				nullable,
			);
			result = input === null ? null : String(input);
		}
		return result;
	}

	// ------------------------------------------------

	export function option<Value>(
		option: Array<[Value, string?] | bigint | null>,
		checker: Check.CheckerX<Value> | null,
	): Value;

	export function option<Value>(
		option: Array<[Value, string?] | bigint | null>,
		checker: Check.CheckerX<Value> | null,
		nullable: boolean,
	): Value | null;

	export function option<Value>(
		option: Array<[Value, string?] | bigint | null>,
		checker: Check.CheckerX<Value> | null,
		nullable: boolean = false,
	): Value | null {
		let result: Value | null = undefined as any;
		let maximum_index_string_length = `${option.length}`.length;
		let option_index = new Array<number>();
		let option_index_discretized = new Map<bigint, number>();
		let option_message = new Array<string>();
		let current_option_index = 1n;
		option.forEach((e, i) => {
			if (e === null) {
				current_option_index += 1n;
			} else if (typeof e === 'bigint') {
				current_option_index = e;
			} else {
				option_index.push(i);
				option_index_discretized.set(current_option_index, i);
				option_message.push(`${make_prefix_padded_string(current_option_index, ' ', maximum_index_string_length)}. ${e[1] === undefined ? e[0] : e[1]}`);
				current_option_index += 1n;
			}
		});
		if (Shell.is_cli) {
			option_message.forEach((e) => {
				cli_basic_output(e, false, 1, true);
			});
			let input = cli_basic_input(
				'O',
				(value) => {
					let regexp_check_result = Check.regexp_checker_x(/^(\+|\-)?(\d+)$/)(value);
					if (regexp_check_result !== null) {
						return los(`整数格式非法，{}`, regexp_check_result);
					}
					let value_integer = BigInt(value);
					if (!option_index_discretized.has(value_integer)) {
						return los(`输入项不在可选项中`);
					}
					return checker === null ? null : checker((option[option_index_discretized.get(value_integer)!] as [Value, string?])[0]);
				},
				nullable,
			);
			result = input === null ? null : (option[option_index_discretized.get(BigInt(input))!] as [Value, string?])[0];
		}
		if (Shell.is_gui) {
			let input = gui_basic_input(
				() => {
					return Shell.gui_input_option(option_message);
				},
				'O',
				(value) => {
					return option_message[Number(BigInt(value) - 1n)];
				},
				(value) => {
					return checker === null ? null : checker((option[option_index[Number(value) - 1]] as [Value, string?])[0]);
				},
				nullable,
			);
			result = input === null ? null : (option[Number(option_index[Number(input) - 1])] as [Value, string?])[0];
		}
		return result;
	}

	// ------------------------------------------------

}