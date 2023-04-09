namespace TwinStar.Script {

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

	export function parse_confirmation_string(
		string: string,
	): boolean {
		if (string === 'n') {
			return false;
		}
		if (string === 'y') {
			return true;
		}
		assert_test(false, `invalid confirmation string`);
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

	export function integer_from_byte_le(
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

	export function unquote(
		source: string,
	): string {
		let destination = source;
		if (false) {
		} else if (destination.startsWith(`'`) && destination.endsWith(`'`)) {
			destination = destination.substring(1, destination.length - 1);
		} else if (destination.startsWith(`"`) && destination.endsWith(`"`)) {
			destination = destination.substring(1, destination.length - 1);
		}
		return destination;
	}

	// ------------------------------------------------

	export function normalize_string_line_feed(
		source: string,
	): string {
		return source.replaceAll('\r\n', '\n');
	}

	export function split_string_by_line_feed(
		source: string,
		ignore_last_if_empry: boolean = true,
	): Array<string> {
		let destination = source.split('\n');
		if (ignore_last_if_empry) {
			if (destination.length > 0 && destination[destination.length - 1].length === 0) {
				destination.pop();
			}
		}
		return destination;
	}

	// ------------------------------------------------

	export function split_error_stack(
		string: string | undefined,
	): Array<string> {
		let list: Array<string>;
		if (string === undefined) {
			list = [`@ ?`];
		} else {
			list = string.split('\n').slice(0, -1).map((e) => {
				let result: string;
				let regexp_result = /    at (.*) \((.*)\)/.exec(e);
				if (regexp_result !== null) {
					result = `@ ${regexp_result[2] === 'native' ? ('<native>:?') : (regexp_result[2])} ${regexp_result[1]}`;
				} else {
					result = '@ ?';
				}
				return result;
			});
		}
		return list;
	}

	export function parse_error_message(
		error: any,
	): [string, Array<string>] {
		let title: string = '';
		let description: Array<string> = [];
		if (error instanceof Error) {
			if (error.name === 'NativeError') {
				title = `${error.name}`;
				description.push(...error.message.split('\n'));
			} else {
				title = `${error.name}: ${error.message}`;
			}
			description.push(...split_error_stack(error.stack));
		} else {
			title = `${error}`;
		}
		return [title, description];
	}

	// ------------------------------------------------

}