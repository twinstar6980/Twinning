namespace TwinStar.Script.Support.PvZ2.JSONGenericGetter {

	// ------------------------------------------------

	export type ValueType = boolean | number | bigint | string | ValueType[] | { [key: string]: ValueType; };

	// ------------------------------------------------

	export function boolean<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): boolean | T {
		if (source === undefined) {
			assert_test(default_value !== undefined, `type error`);
			return default_value;
		}
		if (typeof source === 'boolean') {
			return source;
		}
		if (typeof source === 'number') {
			return source !== 0;
		}
		if (typeof source === 'bigint') {
			return source !== 0n;
		}
		if (typeof source === 'string') {
			return source !== '';
		}
		assert_fail(`type error`);
	}

	export function number<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): number | T {
		if (source === undefined) {
			assert_test(default_value !== undefined, `type error`);
			return default_value;
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
		assert_fail(`type error`);
	}

	export function integer<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): bigint | T {
		if (source === undefined) {
			assert_test(default_value !== undefined, `type error`);
			return default_value;
		}
		if (typeof source === 'bigint') {
			return source;
		}
		if (typeof source === 'boolean') {
			return source ? 1n : 0n;
		}
		if (typeof source === 'number') {
			return BigInt(Number.isInteger(source) ? source : source.toFixed(0));
		}
		if (typeof source === 'string') {
			return BigInt(Number.parseFloat(source).toFixed(0));
		}
		assert_fail(`type error`);
	}

	export function string<T = never>(
		source: ValueType | undefined,
		default_value?: T,
	): string | T {
		if (source === undefined) {
			assert_test(default_value !== undefined, `type error`);
			return default_value;
		}
		if (typeof source === 'string') {
			return source;
		}
		assert_fail(`type error`);
	}

	// ------------------------------------------------

}
