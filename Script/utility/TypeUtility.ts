namespace TwinStar {

	// ------------------------------------------------

	export function equal_or<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value: R,
	): U | R {
		return value === target_value ? value as U : default_value;
	}

	export function not_equal_or<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value: R,
	): T | R {
		return value !== target_value ? value : default_value;
	}

	// ------------------------------------------------

	export function equal_or_by<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value_generator: () => R,
	): U | R {
		return value === target_value ? value as U : default_value_generator();
	}

	export function not_equal_or_by<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value_generator: () => R,
	): T | R {
		return value !== target_value ? value : default_value_generator();
	}

	// ------------------------------------------------

	export function undefined_or<T, R>(
		value: T | undefined,
		default_value: R,
	): undefined | R {
		return equal_or(value, undefined, default_value);
	}

	export function defined_or<T, R>(
		value: T | undefined,
		default_value: R,
	): T | R {
		return not_equal_or(value, undefined, default_value) as T | R;
	}

	export function null_or<T, R>(
		value: T | null,
		default_value: R,
	): null | R {
		return equal_or(value, null, default_value);
	}

	export function nonnull_or<T, R>(
		value: T | null,
		default_value: R,
	): T | R {
		return not_equal_or(value, null, default_value) as T | R;
	}

	// ------------------------------------------------

	export function undefined_or_by<T, R>(
		value: T | undefined,
		default_value_generator: () => R,
	): undefined | R {
		return equal_or_by(value, undefined, default_value_generator);
	}

	export function defined_or_by<T, R>(
		value: T | undefined,
		default_value_generator: () => R,
	): T | R {
		return not_equal_or_by(value, undefined, default_value_generator) as T | R;
	}

	export function null_or_by<T, R>(
		value: T | null,
		default_value_generator: () => R,
	): null | R {
		return equal_or_by(value, null, default_value_generator);
	}

	export function nonnull_or_by<T, R>(
		value: T | null,
		default_value_generator: () => R,
	): T | R {
		return not_equal_or_by(value, null, default_value_generator) as T | R;
	}

	// ------------------------------------------------

	export function record_from_array<ElementT extends any, KeyT extends string, ValueT extends any>(
		array: Array<ElementT>,
		mapper: (index: number, element: ElementT) => [KeyT, ValueT],
	): Record<KeyT, ValueT> {
		let record: Record<KeyT, ValueT> = {} as any;
		for (let index in array) {
			let [key, value] = mapper(Number(index), array[index]);
			record[key] = value;
		}
		return record;
	}

	export function record_to_array<KeyT extends string, ValueT extends any, ElementT extends any>(
		record: Record<KeyT, ValueT>,
		mapper: (key: string, value: ValueT) => ElementT,
	): Array<ElementT> {
		let array: Array<ElementT> = [];
		for (let key in record) {
			array.push(mapper(key, record[key]));
		}
		return array;
	}

	export function record_transform<KeyT extends string, ValueT extends any, NewKeyT extends string, NewValueT extends any>(
		record: Record<KeyT, ValueT>,
		mapper: (key: string, value: ValueT) => [NewKeyT, NewValueT],
	): Record<NewKeyT, NewValueT> {
		let new_record: Record<NewKeyT, NewValueT> = {} as any;
		for (let key in record) {
			let [new_key, new_value] = mapper(key, record[key]);
			new_record[new_key] = new_value;
		}
		return new_record;
	}

	// ------------------------------------------------

	export function make_prefix_padded_string(
		source: any,
		prefix: string,
		maximum_length: number,
	): string {
		let source_string = `${source}`;
		return `${prefix.repeat(Math.max(0, maximum_length - source_string.length) / prefix.length)}${source_string}`;
	}

	// ------------------------------------------------

	export function parse_confirm_string(
		string: string,
	): boolean {
		if (string === 'n') {
			return false;
		}
		if (string === 'y') {
			return true;
		}
		assert(false, `invalid confirm string`);
	}

	export function parse_size_string(
		string: string,
	): bigint {
		const k_unit_map = {
			b: 1,
			k: 1024,
			m: 1024 * 1024,
			g: 1024 * 1024 * 1024,
		};
		return BigInt((Number.parseFloat(string.slice(0, -1)) * k_unit_map[string.slice(-1) as 'b' | 'k' | 'm' | 'g']).toFixed(0));
	}

	// ------------------------------------------------

	export function date_to_simple_string(
		date: Date,
	): string {
		let p = (source: number, maximum_length: number) => (make_prefix_padded_string(source, '0', maximum_length));
		return `${p(date.getFullYear() % 100, 2)}-${p(date.getMonth() + 1, 2)}-${p(date.getDate(), 2)}.${p(date.getHours(), 2)}-${p(date.getMinutes(), 2)}-${p(date.getSeconds(), 2)}.${p(date.getMilliseconds(), 3)}`;
	}

	// ------------------------------------------------

	export function integer_from_byte(
		byte_array: ArrayBuffer,
	): bigint {
		let integer_array = new Uint8Array(byte_array);
		let result = 0n;
		for (let index in integer_array) {
			result |= BigInt(integer_array[index]) << BigInt(8 * Number(index));
		}
		return result;
	}

	// ------------------------------------------------

}