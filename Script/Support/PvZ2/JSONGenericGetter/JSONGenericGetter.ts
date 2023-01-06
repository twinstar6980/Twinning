/** PvZ2-JSON数据通用读取 */
namespace TwinStar.Support.PvZ2.JSONGenericGetter {

	// ------------------------------------------------

	/** PvZ2-JSON中可能出现的JS值类型 */
	export type ValueType = boolean | number | bigint | string | ValueType[] | { [key: string]: ValueType; };

	// ------------------------------------------------

	/**
	 * 获取布尔值
	 * @param source 源值
	 * @param default_value 当源值为undefined时所用的默认返回值，若不指定，则在此时抛出异常
	 * @returns boolean值
	 */
	export function boolean<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): boolean | T {
		if (source === undefined) {
			if (default_value !== undefined) {
				return default_value;
			} else {
				throw new Error(`type error`);
			}
		}
		if (typeof source === 'boolean') {
			return source;
		}
		if (typeof source === 'number') {
			return source === 0;
		}
		if (typeof source === 'bigint') {
			return source === 0n;
		}
		if (typeof source === 'string') {
			return source === '';
		}
		throw new Error(`type error`);
	}

	/**
	 * 获取数字
	 * @param source 源值
	 * @param default_value 当源值为undefined时所用的默认返回值，若不指定，则在此时抛出异常
	 * @returns number值
	 */
	export function number<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): number | T {
		if (source === undefined) {
			if (default_value !== undefined) {
				return default_value;
			} else {
				throw new Error(`type error`);
			}
		}
		if (typeof source === 'number') {
			return source;
		}
		if (typeof source === 'boolean') {
			return source ? 1 : 0;
		}
		if (typeof source === 'bigint') {
			return Number(source);
		}
		if (typeof source === 'string') {
			return Number.parseFloat(source);
		}
		throw new Error(`type error`);
	}

	/**
	 * 获取整数
	 * @param source 源值
	 * @param default_value 当源值为undefined时所用的默认返回值，若不指定，则在此时抛出异常
	 * @returns integer值
	 */
	export function integer<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): bigint | T {
		if (source === undefined) {
			if (default_value !== undefined) {
				return default_value;
			} else {
				throw new Error(`type error`);
			}
		}
		if (typeof source === 'bigint') {
			return source;
		}
		if (typeof source === 'boolean') {
			return source ? 1n : 0n;
		}
		if (typeof source === 'number') {
			if (Number.isInteger(source)) {
				return BigInt(source);
			} else {
				return BigInt(source.toFixed(0));
			}
		}
		if (typeof source === 'string') {
			return BigInt(source);
		}
		throw new Error(`type error`);
	}

	/**
	 * 获取字符串
	 * @param source 源值
	 * @param default_value 当源值为undefined时所用的默认返回值，若不指定，则在此时抛出异常
	 * @returns string值
	 */
	export function string<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): string | T {
		if (source === undefined) {
			if (default_value !== undefined) {
				return default_value;
			} else {
				throw new Error(`type error`);
			}
		}
		if (typeof source === 'string') {
			return source;
		}
		throw new Error(`type error`);
	}

	// ------------------------------------------------

}
