namespace Twinning.Script.ConvertHelper {

	// #region exception

	export function generate_exception_message(
		exception: any,
	): Array<string> {
		let result: Array<string> = [];
		if (!(exception instanceof Error)) {
			result.push(`# <${typeof exception}>`);
			result.push(`$ message: ${exception}`);
		}
		else {
			result.push(`# ${exception.name}`);
			if (exception.name === 'NativeException') {
				result.push(...exception.message.split('\n'));
			}
			else {
				result.push(`$ message: ${exception.message}`);
				if (exception instanceof SuppressedError) {
					result.push(...generate_exception_message(exception.error));
				}
			}
			if (exception.stack !== undefined) {
				for (let frame of exception.stack.split('\n').slice(0, -1)) {
					let frame_match = /^    at (.*) \((.*?)(?:\:(\d+)\:(\d+))?\)$/.exec(frame);
					if (frame_match === null) {
						continue;
					}
					let frame_part = [
						['native', 'missing', 'null'].includes(frame_match[2]) ? `<${frame_match[2]}>` : frame_match[2],
						frame_match[3] === undefined ? '?' : frame_match[3],
						frame_match[4] === undefined ? '?' : frame_match[4],
						frame_match[1],
					];
					result.push(`@ ${frame_part[0]}:${frame_part[1]}:${frame_part[2]} ${frame_part[3]}`);
				}
			}
		}
		return result;
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

	export function parse_boolean_from_string(
		text: string,
	): boolean {
		if (text === 'false') {
			return false;
		}
		if (text === 'true') {
			return true;
		}
		throw new Error(`invalid boolean string`);
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

	// #region size

	export function is_padded_size(
		value: bigint,
		unit: bigint,
	): boolean {
		return value % unit === 0n;
	}

	export function compute_padded_size(
		value: bigint,
		unit: bigint,
	): bigint {
		return is_padded_size(value, unit) ? (value) : ((value / unit + 1n) * unit);
	}

	// ----------------

	export function is_padded_size_of_power_of_two(
		value: bigint,
	): boolean {
		return(value !== 0n) && ((value & (value - 1n)) === 0n);
	}

	export function compute_padded_size_of_power_of_two(
		value: bigint,
	): bigint {
		let result = 0b1n << 1n;
		while (result < value) {
			result <<= 1n;
		}
		return result;
	}

	// #endregion

	// #region character

	export function is_letter(
		value: string,
	): boolean {
		if (value.length !== 1) {
			return false;
		}
		let code = value.charCodeAt(0) | 0x20;
		return 'a'.charCodeAt(0) <= code && code <= 'z'.charCodeAt(0);
	}

	// ----------------

	export function is_path_dot(
		value: string,
	): boolean {
		if (value.length !== 1) {
			return false;
		}
		return value === '.';
	}

	export function is_path_directory_separator(
		value: string,
	): boolean {
		if (value.length !== 1) {
			return false;
		}
		return value === '/' || value === '\\';
	}

	export function is_path_volume_separator(
		value: string,
	): boolean {
		if (value.length !== 1) {
			return false;
		}
		return value === ':';
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

	export function make_date_to_string(
		value: Date,
		format: string,
	): string {
		let value_map = {
			'Y': value.getFullYear(),
			'M': value.getMonth() + 1,
			'D': value.getDate(),
			'h': value.getHours(),
			'm': value.getMinutes(),
			's': value.getSeconds(),
			'S': value.getMilliseconds(),
		} as Record<string, number>;
		return format.replaceAll(
			/Y+|M+|D+|h+|m+|s+|S+/g,
			(match) => {
				let part = value_map[match[0]].toString();
				return part.substring(Math.max(0, part.length - match.length)).padStart(match.length, '0');
			},
		);
	}

	// #endregion

	// #region stream

	export function write_utf8_character(
		stream: ByteStreamView,
		value: bigint,
	): bigint {
		let extra_size = 0;
		if (value < 0x80n) {
			stream.u8(value);
			extra_size = 0;
		}
		else if (value < 0x800n) {
			stream.u8(0b11000000n | ((value >> (6n * 1n)) & ~(~0n << 6n)));
			extra_size = 1;
		}
		else if (value < 0x10000n) {
			stream.u8(0b11100000n | ((value >> (6n * 2n)) & ~(~0n << 6n)));
			extra_size = 2;
		}
		else if (value < 0x110000) {
			stream.u8(0b11110000n | ((value >> (6n * 3n)) & ~(~0n << 6n)));
			extra_size = 3;
		}
		else {
			throw new Error(`invalid unicode character`);
		}
		while (extra_size > 0) {
			--extra_size;
			stream.u8(0b10000000n | ((value >> (6n * BigInt(extra_size))) & ~(~0n << 6n)));
		}
		return value;
	}

	export function read_utf8_character(
		stream: ByteStreamView,
		size: {value: number},
	): bigint {
		let character = 0n;
		let current = stream.u8();
		let extra_size: number;
		if (current < 0b10000000n) {
			character = current;
			extra_size = 0;
		}
		else if (current < 0b11000000n) {
			throw new Error(`data@${stream.p().toString(16)}h: invalid utf-8 first character`);
		}
		else if (current < 0b11100000n) {
			character = current & 0b00011111n;
			extra_size = 1;
		}
		else if (current < 0b11110000n) {
			character = current & 0b00001111n;
			extra_size = 2;
		}
		else if (current < 0b11111000n) {
			character = current & 0b00000111n;
			extra_size = 3;
		}
		else {
			throw new Error(`data@${stream.p().toString(16)}h: invalid utf-8 first character`);
		}
		size.value = 1 + extra_size;
		while (extra_size > 0) {
			--extra_size;
			current = stream.u8();
			if ((current & 0b11000000n) !== 0b10000000n) {
				throw new Error(`data@${stream.p().toString(16)}h: invalid utf-8 extra character`);
			}
			character = character << 6n | (current & 0b00111111n);
		}
		return character;
	}

	export function compute_utf8_character_size(
		value: bigint,
	): number {
		let extra_size = 0;
		if (value < 0x80n) {
			extra_size = 0;
		}
		else if (value < 0x800n) {
			extra_size = 1;
		}
		else if (value < 0x10000n) {
			extra_size = 2;
		}
		else if (value < 0x110000) {
			extra_size = 3;
		}
		else {
			throw new Error(`invalid unicode character`);
		}
		return 1 + extra_size;
	}

	// ----------------

	export function write_utf8_string(
		stream: ByteStreamView,
		value: string,
	): void {
		for (let current of [...value]) {
			write_utf8_character(stream, BigInt(current.codePointAt(0) ?? 0));
		}
		return;
	}

	export function read_utf8_string(
		stream: ByteStreamView,
		length: bigint,
		size: {value: number},
	): string {
		let value = ``;
		let character_size = {value: undefined!};
		size.value = 0;
		for (let index = 0n; index < length; index++) {
			let current = read_utf8_character(stream, character_size);
			value += String.fromCodePoint(Number(current));
			size.value += character_size.value;
		}
		return value;
	}

	export function read_utf8_string_by_size(
		stream: ByteStreamView,
		size: bigint,
	): string {
		let value = ``;
		let character_size = {value: undefined!};
		let count = 0;
		while (count < Number(size)) {
			let current = read_utf8_character(stream, character_size);
			value += String.fromCodePoint(Number(current));
			count += character_size.value;
		}
		if (count > Number(size)) {
			throw new Error(`data@${stream.p().toString(16)}h: utf-8 string too long`);
		}
		return value;
	}

	export function compute_utf8_string_size(
		value: string,
	): number {
		let size = 0;
		for (let current of [...value]) {
			size += compute_utf8_character_size(BigInt(current.codePointAt(0) ?? 0));
		}
		return size;
	}

	// #endregion

}
