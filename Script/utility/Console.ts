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

	/** 消息栏属性 */
	const k_cli_bar_text_attribute: Record<BarType, VirtualTerminalSequences.TextAttribute> = {
		v: {
			background: null,
			foreground: 'default',
			underline: false,
		},
		i: {
			background: null,
			foreground: ['blue', false],
			underline: null,
		},
		w: {
			background: null,
			foreground: ['yellow', false],
			underline: null,
		},
		e: {
			background: null,
			foreground: ['red', false],
			underline: null,
		},
		s: {
			background: null,
			foreground: ['green', false],
			underline: null,
		},
		t: {
			background: null,
			foreground: ['magenta', false],
			underline: false,
		},
	};

	/** 消息栏前导图标 */
	const k_cli_bar_icon: Record<BarType, string> = {
		v: '⚪',
		i: '🔵',
		w: '🟡',
		e: '🔴',
		s: '🟢',
		t: '🟣',
	};

	// ------------------------------------------------

	/** 是否禁用虚拟终端序列 */
	export let cli_disable_virtual_terminal_sequences = false;

	/**
	 * 设置分类化文本属性
	 * 
	 * 仅当 cli_disable_virtual_terminal_sequences == false 时输出控制序列，以避免在不支持虚拟终端序列的环境下将控制序列输出为文本
	 * 
	 * @param type 类型名
	 */
	export function cli_set_bar_text_attribute(
		type: BarType,
	): void {
		if (!cli_disable_virtual_terminal_sequences) {
			Shell.cli_output(VirtualTerminalSequences.text_attribute(k_cli_bar_text_attribute[type]));
		}
		return;
	}

	// ------------------------------------------------

	/**
	 * 通知
	 * @param type 类型
	 * @param title 标题
	 * @param description 描述
	 */
	export function notify(
		type: BarType,
		title: string,
		description: Array<string>,
		with_icon: boolean = true,
	): void {
		if (Shell.is_cli) {
			cli_set_bar_text_attribute(type);
			Shell.cli_output(`${with_icon ? k_cli_bar_icon[type] : '  '} ${title}\n`);
			cli_set_bar_text_attribute('v');
			for (let description_element of description) {
				Shell.cli_output(`     ${description_element}\n`);
			}
			cli_set_bar_text_attribute('v');
		}
		if (Shell.is_gui) {
			Shell.gui_output(type, title, description);
		}
		return;
	}

	/**
	 * 暂停
	 */
	export function pause(
	): void {
		if (Shell.is_cli) {
			cli_set_bar_text_attribute('t');
			if (Shell.is_windows) {
				Shell.cli_output(`${k_cli_bar_icon['t']} 键入以继续 ... `);
				CoreX.System.system(`pause > nul`);
				Shell.cli_output('\n');
			}
			if (Shell.is_linux || Shell.is_macos || Shell.is_android || Shell.is_ios) {
				if (CoreX.FileSystem.exist_file(`/bin/bash`)) {
					Shell.cli_output(`${k_cli_bar_icon['t']} 键入以继续 ... `);
					CoreX.System.process(`/bin/bash`, [`-c`, `read -s -n 1 _`]);
					Shell.cli_output('\n');
				} else {
					Shell.cli_output(`${k_cli_bar_icon['t']} 键入回车以继续 ... `);
					Shell.cli_input();
				}
			}
			cli_set_bar_text_attribute('v');
		}
		if (Shell.is_gui) {
			option([[1n, 'continue']], false);
		}
		return;
	}

	/**
	 * 输入字符串
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function string(
		checker: null | Check.CheckerX<string> = null,
		nullable: boolean = false,
	): string | null {
		let result: string | null = undefined as any;
		while (true) {
			let input: string = undefined as any;
			if (Shell.is_cli) {
				cli_set_bar_text_attribute('t');
				Shell.cli_output(`${k_cli_bar_icon['t']} `);
				input = Shell.cli_input();
				cli_set_bar_text_attribute('v');
			}
			if (Shell.is_gui) {
				input = Shell.cli_input();
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
			notify('e', `输入无效`, [`${check_result}`]);
		}
		return result;
	}

	/**
	 * 输入布尔值
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function boolean(
		nullable: boolean = false,
	): boolean | null {
		let result: boolean | null = undefined as any;
		if (Shell.is_cli) {
			let input = string(Check.enum_checkerx(['false', 'true']), nullable);
			result = input === null ? null : input === 'true';
		}
		if (Shell.is_gui) {
			let input = option([[1n, 'false'], [2n, 'true']], nullable);
			result = input === null ? null : input === 2n;
		}
		return result;
	}

	/**
	 * 输入布尔值，但以y(yes)和n(no)代替true、false
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function yon(
		nullable: boolean = false,
	): boolean | null {
		let result: boolean | null = undefined as any;
		if (Shell.is_cli) {
			let input = string(Check.enum_checkerx(['n', 'y']), nullable);
			result = input === null ? null : input === 'y';
		}
		if (Shell.is_gui) {
			let input = option([[1n, 'no'], [2n, 'yes']], nullable);
			result = input === null ? null : input === 2n;
		}
		return result;
	}

	/**
	 * 输入数字
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function number(
		checker: null | Check.CheckerX<number> = null,
		nullable: boolean = false,
	): number | null {
		let input = string((value) => {
			let regexp_check_result = Check.regexp_checkerx(/^[\\+\\-]?\d+(\.\d+)?$/)(value);
			if (regexp_check_result !== null) {
				return regexp_check_result;
			}
			return checker === null ? null : checker(Number.parseFloat(value));
		}, nullable);
		return input === null ? null : Number.parseFloat(input);
	}

	/**
	 * 输入整数
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function integer(
		checker: null | Check.CheckerX<bigint> = null,
		nullable: boolean = false,
	): bigint | null {
		let input = string((value) => {
			let regexp_check_result = Check.regexp_checkerx(/^[\\+\\-]?\d+$/)(value);
			if (regexp_check_result !== null) {
				return regexp_check_result;
			}
			return checker === null ? null : checker(BigInt(value));
		}, nullable);
		return input === null ? null : BigInt(input);
	}

	/**
	 * 输入表示size的字符串，并转为整数
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function size(
		checker: null | Check.CheckerX<bigint> = null,
		nullable: boolean = false,
	): bigint | null {
		let input = string((value) => {
			let regexp_check_result = Check.regexp_checkerx(/^\d+(\.\d+)?(b|k|m|g)$/)(value);
			if (regexp_check_result !== null) {
				return regexp_check_result;
			}
			return checker === null ? null : checker(parse_size_string(value));
		}, nullable);
		return input === null ? null : parse_size_string(input);
	}

	/**
	 * 输入选项
	 * @param option 选项索引及描述信息
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function option(
		option: Array<[bigint, string]>,
		nullable: boolean = false,
	): bigint | null {
		let result: bigint | null = undefined as any;
		if (Shell.is_cli) {
			let max_index_string_length = `${option[option.length - 1][0]}`.length;
			for (let e of option) {
				Shell.cli_output(`     ${make_prefix_padded_string(e[0], ' ', max_index_string_length)}. ${e[1]}\n`);
			}
			let input = integer(Check.enum_checkerx(option.map((e) => (e[0]))), nullable);
			result = input;
		}
		if (Shell.is_gui) {
			let input = Shell.gui_input_option([...option.map((e) => (e[1])), ...(!nullable ? [] : ['null'])]);
			result = option[Number(input)][0];
		}
		return result;
	}

	// ------------------------------------------------

}