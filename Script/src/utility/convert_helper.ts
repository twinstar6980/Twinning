namespace Twinning.Script.ConvertHelper {

	// #region exception

	export function generate_exception_message(
		exception: any,
	): [string, Array<string>] {
		let title: string = '';
		let description: Array<string> = [];
		if (exception instanceof Error) {
			if (exception.name === 'NativeError') {
				title = `${exception.name}`;
				description.push(...exception.message.split('\n'));
			}
			else {
				title = `${exception.name}: ${exception.message}`;
			}
			if (exception.stack !== undefined) {
				description.push(...exception.stack.split('\n').slice(0, -1)
					.map((value) => (/^    at (.*) \((.*?)(?:\:(\d+)\:(\d+))?\)$/.exec(value)))
					.filter((value) => (value !== null))
					.map((value) => (`@ ${['native', 'missing', 'null'].includes(value[2]) ? `<${value[2]}>` : value[2]}:${value[3] === undefined ? '?' : value[3]}:${value[4] === undefined ? '?' : value[4]} ${value[1]}`))
				);
			}
		}
		else {
			title = `${exception}`;
		}
		return [title, description];
	}

	// #endregion

	// #region object

	export function object_clear_undefined<TTarget extends Object>(
		target: TTarget,
	): TTarget {
		for (let key in target) {
			let value = target[key];
			if (value === undefined) {
				delete target[key];
			}
		}
		return target;
	}

	// #endregion

	// #region record

	export function record_from_array<TElement extends any, TKey extends string, TValue extends any>(
		source: Array<TElement>,
		mapper: (index: number, element: TElement) => [TKey, TValue],
	): Record<TKey, TValue> {
		let destination = {} as Record<TKey, TValue>;
		for (let index in source) {
			let [key, value] = mapper(Number(index), source[index]);
			destination[key] = value;
		}
		return destination;
	}

	export function record_to_array<TKey extends string | number | symbol, TValue extends any, TElement extends any>(
		source: Record<TKey, TValue>,
		mapper: (key: TKey, value: TValue) => TElement,
	): Array<TElement> {
		let destination = [] as Array<TElement>;
		for (let key in source) {
			destination.push(mapper(key, source[key]));
		}
		return destination;
	}

	export function record_transform<TKey extends string | number | symbol, TValue extends any, TNewKey extends string | number | symbol, TNewValue extends any>(
		source: Record<TKey, TValue>,
		mapper: (key: TKey, value: TValue) => [TNewKey, TNewValue],
	): Record<TNewKey, TNewValue> {
		let destination = {} as Record<TNewKey, TNewValue>;
		for (let key in source) {
			let [new_key, new_value] = mapper(key, source[key]);
			destination[new_key] = new_value;
		}
		return destination;
	}

	// #endregion

	// #region boolean

	export function make_boolean_to_string(
		value: boolean,
	): string {
		return !value ? 'false' : 'true';
	}

	// ----------------

	export function make_boolean_to_string_of_confirmation_character(
		value: boolean,
	): string {
		return !value ? 'n' : 'y';
	}

	export function parse_boolean_from_string_of_confirmation_character(
		text: string,
	): boolean {
		if (text === 'n') {
			return false;
		}
		if (text === 'y') {
			return true;
		}
		throw new Error(`invalid confirmation boolean string`);
	}

	// #endregion

	// #region integer

	export function make_integer_to_string(
		value: bigint,
	): string {
		return `${value}`;
	}

	// ----------------

	export function make_integer_to_byte_array(
		value: bigint,
		size: null | number,
		endian: 'little' | 'big' | 'current' = 'current',
	): Array<bigint> {
		let array = [] as Array<bigint>;
		if (endian === 'current') {
			endian = Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value ? 'big' : 'little';
		}
		assert_test(value >= 0n);
		while (value !== 0n) {
			array.push(value & 0xFFn);
			value >>= 8n;
		}
		if (size !== null) {
			array = array.slice(0, Math.min(array.length, size));
			array.push(...new Array<bigint>(size - array.length).fill(0n));
		}
		if (endian === 'big') {
			array = array.reverse();
		}
		return array;
	}

	export function parse_integer_from_byte_array(
		array: Array<bigint>,
		size: null | number,
		endian: 'little' | 'big' | 'current' = 'current',
	): bigint {
		let value = 0n;
		if (endian === 'current') {
			endian = Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value ? 'big' : 'little';
		}
		if (endian === 'big') {
			array = array.reverse();
		}
		if (size !== null) {
			array = array.slice(0, Math.min(array.length, size));
		}
		for (let index in array) {
			value = value | (array[index] << BigInt(8 * Number(index)));
		}
		return value;
	}

	// #endregion

	// #region floater

	export function floater_is_equal(
		x: number,
		y: number,
		tolerance = Number.EPSILON,
	): boolean {
		return Math.abs(x - y) < tolerance;
	}

	// ----------------

	export function make_floater_to_string(
		value: number,
	): string {
		return `${value}${Number.isInteger(value) ? '.0' : ''}`;
	}

	// #endregion

	// #region character

	export function is_path_separator(
		value: string,
	): boolean {
		if (value.length !== 1) {
			return false;
		}
		return value === '/' || value === '\\';
	}

	export function is_letter(
		value: string,
	): boolean {
		if (value.length !== 1) {
			return false;
		}
		var code = value.charCodeAt(0) | 0x20;
		return 'a'.charCodeAt(0) <= code && code <= 'z'.charCodeAt(0);
	}

	// #endregion

	// #region string

	export function unquote_string(
		source: string,
	): string {
		let destination = source;
		if (destination.length >= 2 && destination.startsWith(`'`) && destination.endsWith(`'`)) {
			destination = destination.slice(1, -1);
		}
		else if (destination.length >= 2 && destination.startsWith(`"`) && destination.endsWith(`"`)) {
			destination = destination.slice(1, -1);
		}
		return destination;
	}

	// ----------------

	export function normalize_string_line_feed(
		source: string,
	): string {
		return source.replaceAll('\r\n', '\n');
	}

	export function split_string_by_line_feed(
		source: string,
		ignore_last_if_empty: boolean,
	): Array<string> {
		let destination = source.split('\n');
		if (ignore_last_if_empty) {
			if (destination.length > 0 && destination[destination.length - 1].length === 0) {
				destination.pop();
			}
		}
		return destination;
	}

	// ----------------

	export function make_string_to_byte_array(
		value: string,
	): Array<bigint> {
		value = value.replaceAll(' ', '');
		let array = [] as Array<bigint>;
		assert_test(value.length % 2 === 0);
		for (let key_index = 0; key_index < value.length / 2; key_index++) {
			array.push(BigInt(Number.parseInt(value.substring(key_index * 2, key_index * 2 + 2), 16)));
		}
		return array;
	}

	export function parse_string_from_byte_array(
		array: Array<bigint>,
		space: boolean,
	): string {
		return array.map((value) => (value.toString(16).padStart(2, '0'))).join(!space ? '' : ' ');
	}

	// ----------------

	export function check_string_data_maybe_utf16(
		source: ArrayBuffer,
	): boolean {
		return source.byteLength >= 2 && [0xFFFE, 0xFEFF].includes(new DataView(source).getUint16(0, true));
	}

	// #endregion

	// #region path

	export function replace_path_name(
		source: StoragePath,
		pattern: RegExp,
		replacement: string,
	): StoragePath {
		let destination = null as StoragePath | null;
		let name = source.name();
		if (name === null) {
			destination = new StoragePath(source);
		}
		else {
			name = name.replace(pattern, replacement);
			destination = source.parent()!.join(name);
		}
		return destination;
	}

	// #endregion

	// #region date

	export function make_date_to_string_simple(
		value: Date,
	): string {
		let p = (source: number, maximum_length: number) => (source.toString().padStart(maximum_length, '0'));
		return `${p(value.getFullYear() % 100, 2)}-${p(value.getMonth() + 1, 2)}-${p(value.getDate(), 2)}.${p(value.getHours(), 2)}-${p(value.getMinutes(), 2)}-${p(value.getSeconds(), 2)}.${p(value.getMilliseconds(), 3)}`;
	}

	// #endregion

}
