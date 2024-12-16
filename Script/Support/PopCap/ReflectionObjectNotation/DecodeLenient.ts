namespace Twinning.Script.Support.PopCap.ReflectionObjectNotation.DecodeLenient {

	// ------------------------------------------------

	export function read_pb_varint_unsigned(
		data: ByteStreamView,
	): bigint {
		let value = 0n;
		for (let shift_count = 0n; shift_count < 64n; shift_count += 7n) {
			let byte = data.u8();
			if ((byte & 0b10000000n) != 0b00000000n) {
				value |= (byte & 0b01111111n) << shift_count;
			}
			else {
				value |= (byte) << shift_count;
				break;
			}
		}
		return value;
	}

	export function read_pb_varint_signed(
		data: ByteStreamView,
	): bigint {
		let value = read_pb_varint_unsigned(data);
		return (value >> 1n) ^ -(value & 0b1n);
	}

	// ------------------------------------------------

	export function read_utf8_character(
		data: ByteStreamView,
		size: { value: number; },
	): bigint {
		let value = 0n;
		let current = data.u8();
		let extra_size: number;
		if (current < 0b10000000n) {
			value = current;
			extra_size = 0;
		}
		else if (current < 0b11000000n) {
			throw new Error(`data:${data.p().toString(16)}h : invalid utf-8 first character`);
		}
		else if (current < 0b11100000n) {
			value = current & 0b00011111n;
			extra_size = 1;
		}
		else if (current < 0b11110000n) {
			value = current & 0b00001111n;
			extra_size = 2;
		}
		else if (current < 0b11111000n) {
			value = current & 0b00000111n;
			extra_size = 3;
		}
		else {
			throw new Error(`data:${data.p().toString(16)}h : invalid utf-8 first character`);
		}
		size.value = 1 + extra_size;
		while (extra_size > 0) {
			--extra_size;
			current = data.u8();
			if ((current & 0b11000000n) != 0b10000000n) {
				throw new Error(`data:${data.p().toString(16)}h : invalid utf-8 extra character`);
			}
			value = value << 6n | (current & 0b00111111n);
		}
		return value;
	}

	export function read_utf8_string(
		data: ByteStreamView,
		length: bigint,
		size: { value: number; },
	): string {
		let value = ``;
		let character_size = { value: undefined! };
		size.value = 0;
		for (let index = 0n; index < length; index++) {
			let character = read_utf8_character(data, character_size);
			value += String.fromCodePoint(Number(character));
			size.value += character_size.value;
		}
		return value;
	}

	export function read_utf8_string_by_size(
		data: ByteStreamView,
		size: bigint,
	): string {
		let value = ``;
		let character_size = { value: undefined! };
		let count = 0;
		while (count < Number(size)) {
			let character = read_utf8_character(data, character_size);
			value += String.fromCodePoint(Number(character));
			count += character_size.value;
		}
		if (count > Number(size)) {
			throw new Error(`data:${data.p().toString(16)}h : utf-8 string too long`);
		}
		return value;
	}

	export function read_eascii_string(
		data: ByteStreamView,
		length: bigint,
	): string {
		let value = ``;
		for (let index = 0n; index < length; index++) {
			let character = data.u8();
			value += String.fromCodePoint(Number(character));
		}
		return value;
	}

	// ------------------------------------------------

	export function process_unit(
		data: ByteStreamView,
		native_string_index: Array<string>,
		unicode_string_index: Array<string>,
		type_identifier: bigint,
		version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
	): Kernel.JSON.JS_Value {
		let value: Kernel.JSON.JS_Value;
		switch (type_identifier) {
			case 0x00n: {
				value = false;
				break;
			}
			case 0x01n: {
				value = true;
				break;
			}
			case 0x08n: {
				value = data.i8();
				break;
			}
			case 0x09n: {
				value = 0n;
				break;
			}
			case 0x0An: {
				value = data.u8();
				break;
			}
			case 0x0Bn: {
				value = 0n;
				break;
			}
			case 0x10n: {
				value = data.i16();
				break;
			}
			case 0x11n: {
				value = 0n;
				break;
			}
			case 0x12n: {
				value = data.u16();
				break;
			}
			case 0x13n: {
				value = 0n;
				break;
			}
			case 0x20n: {
				value = data.i32();
				break;
			}
			case 0x21n: {
				value = 0n;
				break;
			}
			case 0x26n: {
				value = data.u32();
				break;
			}
			case 0x27n: {
				value = 0n;
				break;
			}
			case 0x40n: {
				value = data.i64();
				break;
			}
			case 0x41n: {
				value = 0n;
				break;
			}
			case 0x46n: {
				value = data.u64();
				break;
			}
			case 0x47n: {
				value = 0n;
				break;
			}
			case 0x22n: {
				value = data.f32();
				break;
			}
			case 0x23n: {
				value = 0.0;
				break;
			}
			case 0x42n: {
				value = data.f64();
				break;
			}
			case 0x43n: {
				value = 0.0;
				break;
			}
			case 0x24n:
			case 0x28n: {
				value = read_pb_varint_unsigned(data);
				break;
			}
			case 0x25n: {
				value = read_pb_varint_signed(data);
				break;
			}
			case 0x44n:
			case 0x48n: {
				value = read_pb_varint_unsigned(data);
				break;
			}
			case 0x45n: {
				value = read_pb_varint_signed(data);
				break;
			}
			case 0x81n: {
				let size = read_pb_varint_unsigned(data);
				let content: string;
				if (!version.native_string_encoding_use_utf8) {
					content = read_eascii_string(data, size);
				}
				else {
					content = read_utf8_string_by_size(data, size);
				}
				value = content;
				break;
			}
			case 0x90n: {
				let size = read_pb_varint_unsigned(data);
				let content: string;
				if (!version.native_string_encoding_use_utf8) {
					content = read_eascii_string(data, size);
				}
				else {
					content = read_utf8_string_by_size(data, size);
				}
				value = content;
				native_string_index.push(value);
				break;
			}
			case 0x91n: {
				let index = read_pb_varint_unsigned(data);
				if (Number(index) >= native_string_index.length) {
					throw new Error(`data:${data.p().toString(16)}h : invalid string index`);
				}
				value = native_string_index[Number(index)];
				break;
			}
			case 0x82n: {
				let length = read_pb_varint_unsigned(data);
				let size = read_pb_varint_unsigned(data);
				let actual_size = { value: undefined! };
				let content = read_utf8_string(data, length, actual_size);
				if (actual_size.value !== Number(size)) {
					Console.warning(`data:${data.p().toString(16)}h : invalid utf-8 string size : except ${size} but actual ${actual_size.value}`, []);
				}
				value = content;
				break;
			}
			case 0x92n: {
				let length = read_pb_varint_unsigned(data);
				let size = read_pb_varint_unsigned(data);
				let actual_size = { value: undefined! };
				let content = read_utf8_string(data, length, actual_size);
				if (actual_size.value !== Number(size)) {
					Console.warning(`data:${data.p().toString(16)}h : invalid utf-8 string size : except ${size} but actual ${actual_size.value}`, []);
				}
				value = content;
				unicode_string_index.push(value);
				break;
			}
			case 0x93n: {
				let index = read_pb_varint_unsigned(data);
				if (Number(index) >= unicode_string_index.length) {
					throw new Error(`data:${data.p().toString(16)}h : invalid string index`);
				}
				value = unicode_string_index[Number(index)];
				break;
			}
			case 0x83n: {
				switch (data.u8()) {
					case 0x00n: {
						value = `RTID(0)`;
						break;
					}
					case 0x02n: {
						let sheet_length = read_pb_varint_unsigned(data);
						let sheet_size = read_pb_varint_unsigned(data);
						let sheet_actual_size = { value: undefined! };
						let sheet_content = read_utf8_string(data, sheet_length, sheet_actual_size);
						if (sheet_actual_size.value !== Number(sheet_size)) {
							Console.warning(`data:${data.p().toString(16)}h : invalid utf-8 string size : except ${sheet_size} but actual ${sheet_actual_size.value}`, []);
						}
						let uid_middle = read_pb_varint_unsigned(data);
						let uid_first = read_pb_varint_unsigned(data);
						let uid_last = data.u32();
						value = `RTID(${uid_first}.${uid_middle}.${uid_last}@${sheet_content})`;
						break;
					}
					case 0x03n: {
						let sheet_length = read_pb_varint_unsigned(data);
						let sheet_size = read_pb_varint_unsigned(data);
						let sheet_actual_size = { value: undefined! };
						let sheet_content = read_utf8_string(data, sheet_length, sheet_actual_size);
						if (sheet_actual_size.value !== Number(sheet_size)) {
							Console.warning(`data:${data.p().toString(16)}h : invalid utf-8 string size : except ${sheet_size} but actual ${sheet_actual_size.value}`, []);
						}
						let alias_length = read_pb_varint_unsigned(data);
						let alias_size = read_pb_varint_unsigned(data);
						let alias_actual_size = { value: undefined! };
						let alias_content = read_utf8_string(data, alias_length, alias_actual_size);
						if (alias_actual_size.value !== Number(alias_size)) {
							Console.warning(`data:${data.p().toString(16)}h : invalid utf-8 string size : except ${alias_size} but actual ${alias_actual_size.value}`, []);
						}
						value = `RTID(${alias_content}@${sheet_content})`;
						break;
					}
					default: {
						throw new Error(`data:${data.p().toString(16)}h : invalid rtid type identifier`);
					}
				}
				break;
			}
			case 0x84n: {
				value = `RTID(0)`;
				break;
			}
			case 0x86n: {
				value = [];
				if (data.u8() !== 0xFDn) {
					throw new Error(`data:${data.p().toString(16)}h : invalid array size identifier`);
				}
				let size = read_pb_varint_unsigned(data);
				while (true) {
					let value_type_identifier = data.u8();
					if (value_type_identifier == 0xFEn) {
						break;
					}
					let element_value = process_unit(data, native_string_index, unicode_string_index, value_type_identifier, version);
					value.push(element_value);
				}
				if (value.length !== Number(size)) {
					Console.warning(`data:${data.p().toString(16)}h : invalid array size : except ${size} but actual ${value.length}`, []);
				}
				break;
			}
			case 0x85n: {
				value = {};
				while (true) {
					let key_type_identifier = data.u8();
					if (key_type_identifier == 0xFFn) {
						break;
					}
					let member_key = process_unit(data, native_string_index, unicode_string_index, key_type_identifier, version);
					let value_type_identifier = data.u8();
					let member_value = process_unit(data, native_string_index, unicode_string_index, value_type_identifier, version);
					value[member_key as string] = member_value;
				}
				break;
			}
			default: {
				throw new Error(`data:${data.p().toString(16)}h : invalid type identifier`);
			}
		}
		return value;
	}

	export function process_whole(
		data: ByteStreamView,
		version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
	): Kernel.JSON.JS_Value {
		if (data.u32() !== 0x4E4F5452n) {
			Console.warning(`data:${data.p().toString(16)}h : invalid magic`, []);
		}
		if (data.u32() !== version.number) {
			Console.warning(`data:${data.p().toString(16)}h : invalid version`, []);
		}
		let definition = process_unit(data, [], [], 0x85n, version);
		if (data.u32() !== 0x454E4F44n) {
			Console.warning(`data:${data.p().toString(16)}h : invalid done`, []);
		}
		return definition;
	}

	// ------------------------------------------------

	export function process(
		data: ByteStreamView,
		version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
	): Kernel.JSON.JS_Value {
		return process_whole(data, version);
	}

	// ------------------------------------------------

	export function process_fs(
		data_file: string,
		definition_file: string,
		version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
	): void {
		let data = KernelX.Storage.read_file(data_file);
		let definition = process(new ByteStreamView(data.view().value), version);
		KernelX.JSON.write_fs_js(definition_file, definition);
		return;
	}

	// ------------------------------------------------

}