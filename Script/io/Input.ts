/** 输入工具 */
namespace TwinKleS.Input {

	// ------------------------------------------------

	/**
	 * 暂停，等待用户响应
	 * @param message 提示信息
	 */
	export function pause(
		message: null | string = null,
	): void {
		if (message !== null) {
			Output.w(message);
		}
		Shell.pause('');
		Output.normal(`\u001b[1A\u001b[2K`);
		return;
	}

	// ------------------------------------------------

	/**
	 * 常规输入文本
	 */
	export function normal(
	): string {
		return Shell.input('');
	}

	// ------------------------------------------------

	type Message = string | [string, Array<string>];

	// ------------------------------------------------

	/**
	 * 输入字符串
	 * @param message 提示信息
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function string(
		message: null | Message,
		checker: null | Check.CheckerX<string> = null,
		nullable: boolean = false,
	): string | null {
		if (message !== null) {
			if (typeof message === 'string') {
				Output.i(message, 0, '?');
			} else {
				Output.i(message[0], 0, '?');
				for (let e of message[1]) {
					Output.v(e, +1);
				}
			}
		}
		while (true) {
			Output.set_classify_attribute('input');
			Output.normal(`> `);
			let input = normal();
			Output.set_classify_attribute('default');
			if (nullable && input === '') {
				return null;
			}
			if (checker === null) {
				return input;
			}
			let check_result = checker(input);
			if (check_result === null) {
				return input;
			}
			Output.e(`输入无效：${check_result}`);
		}
	}

	// ------------------------------------------------

	/**
	 * 输入布尔值
	 * @param message 提示信息
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function boolean(
		message: null | Message,
		nullable: boolean = false,
	): boolean | null {
		let input = string(message, Check.enum_checkerx(['true', 'false']), nullable);
		return input === null ? null : input === 'true';
	}

	/**
	 * 输入布尔值，但以y(yes)和n(no)代替true、false
	 * @param message 提示信息
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function yon(
		message: null | Message,
		nullable: boolean = false,
	): boolean | null {
		let input = string(message, Check.enum_checkerx(['y', 'n']), nullable);
		return input === null ? null : input === 'y';
	}

	// ------------------------------------------------

	/**
	 * 输入数字
	 * @param message 提示信息
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function number(
		message: null | Message,
		checker: null | Check.CheckerX<number> = null,
		nullable: boolean = false,
	): number | null {
		let input = string(message, (value) => {
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
	 * @param message 提示信息
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function integer(
		message: null | Message,
		checker: null | Check.CheckerX<bigint> = null,
		nullable: boolean = false,
	): bigint | null {
		let input = string(message, (value) => {
			let regexp_check_result = Check.regexp_checkerx(/^[\\+\\-]?\d+$/)(value);
			if (regexp_check_result !== null) {
				return regexp_check_result;
			}
			return checker === null ? null : checker(BigInt(value));
		}, nullable);
		return input === null ? null : BigInt(input);
	}

	// ------------------------------------------------

	/**
	 * 输入表示size的字符串，并转为整数
	 * @param message 提示信息
	 * @param checker 输入值校验器
	 * @param nullable 若为真，则当输入值为空串时返回null，不触发校验器
	 */
	export function size(
		message: null | Message,
		checker: null | Check.CheckerX<bigint> = null,
		nullable: boolean = false,
	): bigint | null {
		let input = string(message, (value) => {
			let regexp_check_result = Check.regexp_checkerx(/^\d+(\.\d+)?(b|k|m|g)$/)(value);
			if (regexp_check_result !== null) {
				return regexp_check_result;
			}
			return checker === null ? null : checker(parse_size_string(value));
		}, nullable);
		return input === null ? null : parse_size_string(input);
	}

	// ------------------------------------------------

}