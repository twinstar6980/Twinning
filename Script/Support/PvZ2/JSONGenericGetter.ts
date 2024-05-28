namespace Twinning.Script.Support.PvZ2.JSONGenericGetter {

	// ------------------------------------------------

	export type ValueType = boolean | number | bigint | string | ValueType[] | { [key: string]: ValueType; };

	// ------------------------------------------------

	export function boolean<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): boolean | T {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (is_boolean(source)) {
			return source;
		}
		if (is_bigint(source)) {
			return source !== 0n;
		}
		if (is_number(source)) {
			return source !== 0;
		}
		if (is_string(source)) {
			return source !== '';
		}
		assert_fail(`type error`);
	}

	export function number<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): number | T {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (is_number(source)) {
			return source;
		}
		if (is_boolean(source)) {
			return source ? 1 : 0;
		}
		if (is_bigint(source)) {
			return Number(source);
		}
		if (is_string(source)) {
			return Number(source);
		}
		assert_fail(`type error`);
	}

	export function integer<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): bigint | T {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (is_bigint(source)) {
			return source;
		}
		if (is_boolean(source)) {
			return source ? 1n : 0n;
		}
		if (is_number(source)) {
			return BigInt(Math.trunc(source));
		}
		if (is_string(source)) {
			return BigInt(Math.trunc(Number(source)));
		}
		assert_fail(`type error`);
	}

	export function string<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): string | T {
		if (source === undefined && default_value !== undefined) {
			return default_value;
		}
		if (is_string(source)) {
			return source;
		}
		assert_fail(`type error`);
	}

	// ------------------------------------------------

}
