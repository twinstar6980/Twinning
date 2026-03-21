namespace Twinning.Script.Support.Pvz2.JsonGenericGetter {

	// #region utility

	export type ValueType = boolean | number | bigint | string | ValueType[] | {[key: string]: ValueType};

	// ----------------

	export function boolean<TDefault = never>(
		source: ValueType | undefined,
		default_value?: TDefault,
	): boolean | TDefault {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (CheckHelper.is_boolean(source)) {
			return source;
		}
		if (CheckHelper.is_bigint(source)) {
			return source !== 0n;
		}
		if (CheckHelper.is_number(source)) {
			return source !== 0;
		}
		if (CheckHelper.is_string(source)) {
			return source !== '';
		}
		throw new Error(`type error`);
	}

	export function number<TDefault = never>(
		source: ValueType | undefined,
		default_value?: TDefault,
	): number | TDefault {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (CheckHelper.is_number(source)) {
			return source;
		}
		if (CheckHelper.is_boolean(source)) {
			return source ? 1 : 0;
		}
		if (CheckHelper.is_bigint(source)) {
			return Number(source);
		}
		if (CheckHelper.is_string(source)) {
			return Number(source);
		}
		throw new Error(`type error`);
	}

	export function integer<TDefault = never>(
		source: ValueType | undefined,
		default_value?: TDefault,
	): bigint | TDefault {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (CheckHelper.is_bigint(source)) {
			return source;
		}
		if (CheckHelper.is_boolean(source)) {
			return source ? 1n : 0n;
		}
		if (CheckHelper.is_number(source)) {
			return BigInt(Math.trunc(source));
		}
		if (CheckHelper.is_string(source)) {
			return BigInt(Math.trunc(Number(source)));
		}
		throw new Error(`type error`);
	}

	export function string<TDefault = never>(
		source: ValueType | undefined,
		default_value?: TDefault,
	): string | TDefault {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (CheckHelper.is_string(source)) {
			return source;
		}
		throw new Error(`type error`);
	}

	// #endregion

}
