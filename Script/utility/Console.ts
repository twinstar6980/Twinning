/** 输出工具 */
namespace TwinKleS.Console {

	// ------------------------------------------------

	/**
	 * 消息栏分类
	 * + v 常规
	 * + i 消息
	 * + w 警告
	 * + e 错误
	 * + s 成功
	 * + t 输入
	 */
	export type BarType = 'v' | 'i' | 'w' | 'e' | 's' | 't';

	// ------------------------------------------------

	/** 是否禁用虚拟终端序列 */
	export let cli_disable_virtual_terminal_sequences = false;

	/** 消息栏属性 */
	const k_cli_bar_text_attribute: Record<BarType, VirtualTerminalSequences.TextAttribute> = {
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

	/**
	 * 设置分类化文本属性
	 * 
	 * 仅当 cli_disable_virtual_terminal_sequences == false 时输出控制序列，以避免在不支持虚拟终端序列的环境下输出控制序列
	 * 
	 * @param type 类型名
	 */
	function cli_set_bar_text_attribute(
		type: BarType,
	): void {
		if (!cli_disable_virtual_terminal_sequences) {
			Shell.cli_output(VirtualTerminalSequences.text_attribute(k_cli_bar_text_attribute[type]));
		}
		return;
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
		nullable: boolean = false,
	): string | null {
		let result: string | null = undefined as any;
		while (true) {
			let input: string = undefined as any;
			if (Shell.is_cli) {
				cli_set_bar_text_attribute('t');
				cli_basic_output(`${leading} `, true, 0, false);
				input = Shell.cli_input();
				cli_set_bar_text_attribute('v');
			}
			if (Shell.is_gui) {
				input = Shell.gui_input_string();
			}
			if (nullable && input === '') {
				result = null;
				break;
			}
			if (checker === null) {
				result = input;
				break;
			}
			let check_result = checker(input);
			if (check_result === null) {
				result = input;
				break;
			}
			notify('w', `输入无效，请重新输入`, [`${check_result}`]);
		}
		return result;
	}

	// ------------------------------------------------

	export function notify(
		type: BarType,
		title: string,
		description: Array<string>,
		with_icon: boolean = true, // for cli
	): void {
		if (Shell.is_cli) {
			cli_set_bar_text_attribute(type);
			cli_basic_output(title, with_icon, 0, true);
			cli_set_bar_text_attribute('v');
			for (let description_element of description) {
				cli_basic_output(description_element, false, 1, true);
			}
			cli_set_bar_text_attribute('v');
		}
		if (Shell.is_gui) {
			Shell.gui_output(type, title, description);
		}
		return;
	}

	// ------------------------------------------------

	export function pause(
	): void {
		if (Shell.is_cli) {
			cli_set_bar_text_attribute('t');
			if (Shell.is_windows) {
				cli_basic_output(`P 键入以继续 ... `, true, 0, false);
				CoreX.System.system(`pause > nul`);
				Shell.cli_output('\n');
			}
			if (Shell.is_linux || Shell.is_macos || Shell.is_android || Shell.is_ios) {
				if (CoreX.FileSystem.exist_file(`/bin/bash`)) {
					cli_basic_output(`P 键入以继续 ... `, true, 0, false);
					CoreX.System.process(`/bin/bash`, [`-c`, `read -s -n 1 _`]);
					Shell.cli_output('\n');
				} else {
					cli_basic_output(`P 键入回车以继续 ... `, true, 0, false);
					Shell.cli_input();
				}
			}
			cli_set_bar_text_attribute('v');
		}
		if (Shell.is_gui) {
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
			let input = cli_basic_input('C', (value) => {
				let regexp_check_result = Check.enum_checkerx(['n', 'y'])(value);
				if (regexp_check_result !== null) {
					return `确认值只识别 n 与 y`;
				}
				return checker === null ? null : checker(value === 'y');
			}, nullable);
			result = input === null ? null : input === 'y';
		}
		if (Shell.is_gui) {
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
			let input = cli_basic_input('N', (value) => {
				let regexp_check_result = Check.regexp_checkerx(/^[\\+\\-]?\d+(\.\d+)?$/)(value);
				if (regexp_check_result !== null) {
					return `数字格式非法，${regexp_check_result}`;
				}
				return checker === null ? null : checker(Number.parseFloat(value));
			}, nullable);
			result = input === null ? null : Number.parseFloat(input);
		}
		if (Shell.is_gui) {
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
			let input = cli_basic_input('I', (value) => {
				let regexp_check_result = Check.regexp_checkerx(/^[\\+\\-]?\d+$/)(value);
				if (regexp_check_result !== null) {
					return `整数格式非法，${regexp_check_result}`;
				}
				return checker === null ? null : checker(BigInt(value));
			}, nullable);
			result = input === null ? null : BigInt(input);
		}
		if (Shell.is_gui) {
		}
		return result;
	}

	// ------------------------------------------------

	export function binary_size(
		checker: Check.CheckerX<bigint> | null,
	): bigint;

	export function binary_size(
		checker: Check.CheckerX<bigint> | null,
		nullable: boolean,
	): bigint | null;

	export function binary_size(
		checker: Check.CheckerX<bigint> | null,
		nullable: boolean = false,
	): bigint | null {
		let result: bigint | null = undefined as any;
		if (Shell.is_cli) {
			let input = cli_basic_input('Z', (value) => {
				let regexp_check_result = Check.regexp_checkerx(/^\d+(\.\d+)?(b|k|m|g)$/)(value);
				if (regexp_check_result !== null) {
					return `二进制尺寸格式非法，${regexp_check_result}`;
				}
				return checker === null ? null : checker(parse_size_string(value));
			}, nullable);
			result = input === null ? null : parse_size_string(input);
		}
		if (Shell.is_gui) {
		}
		return result;
	}

	// ------------------------------------------------

	export function option<Value>(
		option: Array<[Value, string?] | null>,
		checker: Check.CheckerX<Value> | null,
	): Value;

	export function option<Value>(
		option: Array<[Value, string?] | null>,
		checker: Check.CheckerX<Value> | null,
		nullable: boolean,
	): Value | null;

	export function option<Value>(
		option: Array<[Value, string?] | null>,
		checker: Check.CheckerX<Value> | null,
		nullable: boolean = false,
	): Value | null {
		let result: Value | null = undefined as any;
		if (Shell.is_cli) {
			let max_index_string_length = `${option.length}`.length;
			let valid_option_index: Array<bigint> = [];
			option.forEach((e, i) => {
				if (e !== null) {
					cli_basic_output(`${make_prefix_padded_string(i + 1, ' ', max_index_string_length)}. ${e[1] === undefined ? e[0] : e[1]}\n`, false, 1, false);
					valid_option_index.push(BigInt(i) + 1n);
				}
			});
			let input = cli_basic_input('O', (value) => {
				let regexp_check_result = Check.regexp_checkerx(/^[\\+\\-]?\d+$/)(value);
				if (regexp_check_result !== null) {
					return `整数格式非法，${regexp_check_result}`;
				}
				let value_integer = BigInt(value);
				if (!valid_option_index.includes(value_integer)) {
					return `输入项不在可选项中`;
				}
				return checker === null ? null : checker(option[Number(value_integer - 1n)]![0]);
			}, nullable);
			result = input === null ? null : option[Number(BigInt(input) - 1n)]![0];
		}
		if (Shell.is_gui) {
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
			let input = cli_basic_input('S', checker, nullable);
			result = input;
		}
		if (Shell.is_gui) {
		}
		return result;
	}

	// ------------------------------------------------

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

}