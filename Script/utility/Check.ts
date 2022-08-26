/** 校验器 */
namespace TwinKleS.Check {

	// ------------------------------------------------

	/** 值校验器 */
	export type Checker<T> = (value: T) => boolean;

	// ------------------------------------------------

	/** 返回失败信息的校验器 */
	export type CheckerX<T> = (value: T) => null | string;

	/**
	 * 生成枚举校验器
	 * @param source 枚举值
	 * @returns 校验器
	 */
	export function enum_checkerx<T>(
		source: Array<T>,
	): CheckerX<T> {
		return (value) => (source.includes(value) ? null : `需为其中之一：[ ${source.join(' , ')} ]`);
	}

	/**
	 * 生成正则校验器
	 * @param source 正则
	 * @returns 校验器
	 */
	export function regexp_checkerx(
		source: RegExp,
	): CheckerX<string> {
		return (value) => (source.test(value) ? null : `需匹配正则：${source.source}`);
	}

	// ------------------------------------------------

}