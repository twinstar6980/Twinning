/** 工具集 */
namespace TwinStar {

	// ------------------------------------------------

	/**
	 * 若value等于target_value，则返回value，否则返回default_value
	 */
	export function equal_or<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value: R,
	): U | R {
		return value === target_value ? value as U : default_value;
	}

	/**
	 * 若value不等于target_value，则返回value，否则返回default_value
	 */
	export function not_equal_or<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value: R,
	): T | R {
		return value !== target_value ? value : default_value;
	}

	// ------------------------------------------------

	/**
	 * 若value等于target_value，则返回value，否则返回default_value_generator的调用结果
	 */
	export function equal_or_by<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value_generator: () => R,
	): U | R {
		return value === target_value ? value as U : default_value_generator();
	}

	/**
	 * 若value不等于target_value，则返回value，否则返回default_value_generator的调用结果
	 */
	export function not_equal_or_by<T, U extends T, R>(
		value: T,
		target_value: U,
		default_value_generator: () => R,
	): T | R {
		return value !== target_value ? value : default_value_generator();
	}

	// ------------------------------------------------

	/**
	 * 若value不为undefined，则返回default_value
	 */
	export function undefined_or<T, R>(
		value: T | undefined,
		default_value: R,
	): undefined | R {
		return equal_or(value, undefined, default_value);
	}

	/**
	 * 若value为undefined，则返回default_value
	 */
	export function defined_or<T, R>(
		value: T | undefined,
		default_value: R,
	): T | R {
		return not_equal_or(value, undefined, default_value) as T | R;
	}

	/**
	 * 若value不为null，则返回default_value
	 */
	export function null_or<T, R>(
		value: T | null,
		default_value: R,
	): null | R {
		return equal_or(value, null, default_value);
	}

	/**
	 * 若value为null，则返回default_value
	 */
	export function nonnull_or<T, R>(
		value: T | null,
		default_value: R,
	): T | R {
		return not_equal_or(value, null, default_value) as T | R;
	}

	// ------------------------------------------------

	/**
	 * 若value不为undefined，则返回default_value
	 */
	export function undefined_or_by<T, R>(
		value: T | undefined,
		default_value_generator: () => R,
	): undefined | R {
		return equal_or_by(value, undefined, default_value_generator);
	}

	/**
	 * 若value为undefined，则返回default_value
	 */
	export function defined_or_by<T, R>(
		value: T | undefined,
		default_value_generator: () => R,
	): T | R {
		return not_equal_or_by(value, undefined, default_value_generator) as T | R;
	}

	/**
	 * 若value不为null，则返回default_value
	 */
	export function null_or_by<T, R>(
		value: T | null,
		default_value_generator: () => R,
	): null | R {
		return equal_or_by(value, null, default_value_generator);
	}

	/**
	 * 若value为null，则返回default_value
	 */
	export function nonnull_or_by<T, R>(
		value: T | null,
		default_value_generator: () => R,
	): T | R {
		return not_equal_or_by(value, null, default_value_generator) as T | R;
	}

	// ------------------------------------------------

	/**
	 * 将数组转为记录
	 * @param array 数组
	 * @param mapper 转换函数，接受元素索引与其值，返回键值对
	 * @returns 记录
	 */
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

	/**
	 * 将记录转为数组
	 * @param record 记录
	 * @param mapper 转换函数，接受键值对，返回元素值
	 * @returns 数组
	 */
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

	/**
	 * 将记录转为新记录
	 * @param record 记录
	 * @param mapper 转换函数，接受键值对，返回新键值对
	 * @returns 新记录
	 */
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

	/**
	 * 生成填充了前缀的字符串
	 * @param source 源串
	 * @param prefix 前缀
	 * @param max_length 结果串的最大长度，有空余则以前缀填充
	 * @returns 填充了前缀的字符串
	 */
	export function make_prefix_padded_string(
		source: any,
		prefix: string,
		max_length: number,
	): string {
		let source_string = `${source}`;
		return `${prefix.repeat(Math.max(0, max_length - source_string.length) / prefix.length)}${source_string}`;
	}

	// ------------------------------------------------

	/**
	 * 生成用于缩进的字符串
	 * @param indent 缩进值；正为进，负为退
	 * @returns 用于缩进的字符串
	 */
	export function make_indent_string(
		indent: number,
	): string {
		return indent < 0 ? '\b\b\b\b'.repeat(-indent) : '    '.repeat(indent);
	}

	/**
	 * 生成缩进后的字符串
	 * @param string 源串
	 * @param indent 缩进值；正为进，负为退
	 * @returns 缩进后的字符串
	 */
	export function make_indented_string(
		string: string,
		indent: number,
	): string {
		return `${make_indent_string(indent)}${string}`;
	}

	// ------------------------------------------------

	/**
	 * 解析用于表示确认值的字符串
	 * @param string 字符串
	 * @returns 解析值
	 */
	export function parse_confirm_string(
		string: string,
	): boolean {
		if (string === 'n') {
			return false;
		}
		if (string === 'y') {
			return true;
		}
		throw Error('invalid confirm string');
	}

	/**
	 * 解析用于表示尺寸的字符串
	 * @param string 字符串
	 * @returns 解析值
	 */
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

	/**
	 * 以UTF16编码将字符串转换为二进制数据
	 * @param source 字符串
	 * @param endian 端序
	 * @param bom 是否添加BOM
	 * @returns 二进制数据
	 */
	export function string_to_utf16_data(
		source: string,
		endian: 'big' | 'little',
		bom: boolean,
	): ArrayBuffer {
		let is_little_endian = endian === 'little';
		let buffer = new ArrayBuffer(source.length * 2 + (bom ? 2 : 0));
		let view = new DataView(buffer);
		let position = 0;
		if (bom) {
			view.setUint16(0, 0xFEFF, is_little_endian);
			++position;
		}
		for (let i = 0; i < source.length; ++i) {
			view.setUint16(position * 2, source.charCodeAt(i), is_little_endian);
			++position;
		}
		return buffer;
	}

	/**
	 * 以UTF16编码将二进制数据转换为字符串
	 * @param source 二进制数据
	 * @param endian 端序，若指定为 parse ，则取第一个编码单元作为BOM
	 * @returns 字符串
	 */
	export function string_from_utf16_data(
		source: DataView,
		endian: 'big' | 'little' | 'parse',
	): string {
		if (source.byteLength < 2 || source.byteLength % 2 !== 0) {
			throw new Error(`invalid source size`);
		}
		let position = 0;
		let is_little_endian: boolean;
		if (endian === 'parse') {
			let bom = parse_utf16_bom(source);
			if (bom === null) {
				throw new Error(`invalid bom`);
			}
			is_little_endian = bom;
			++position;
		} else {
			is_little_endian = endian === 'little';
		}
		let result = new Array<string>(source.byteLength / 2);
		while (position * 2 < source.byteLength) {
			result[position] = String.fromCharCode(source.getUint16(position * 2, is_little_endian));
			++position;
		}
		return result.join('');
	}

	/**
	 * 解析UTF16-BOM
	 * @param data UTF16数据
	 * @returns BOM是否表征为小端，若BOM无效，返回null
	 */
	export function parse_utf16_bom(
		data: DataView,
	): boolean | null {
		if (data.byteLength < 2) {
			return null;
		}
		let bom_first = data.getUint8(0);
		let bom_second = data.getUint8(1);
		if (bom_first === 0xFF && bom_second === 0xFE) {
			return true;
		} else if (bom_first === 0xFE && bom_second === 0xFF) {
			return false;
		} else {
			return null;
		}
	}

	// ------------------------------------------------

	/**
	 * 将日期转换为简易表示的字符串 yy-mm-dd.hh-mm-ss.ss
	 * @param date 日期
	 * @returns 简易表示的字符串
	 */
	export function date_to_simple_string(
		date: Date,
	): string {
		let p = (source: number, max_length: number) => (make_prefix_padded_string(source, '0', max_length));
		return `${p(date.getFullYear() % 100, 2)}-${p(date.getMonth() + 1, 2)}-${p(date.getDate(), 2)}.${p(date.getHours(), 2)}-${p(date.getMinutes(), 2)}-${p(date.getSeconds(), 2)}.${p(date.getMilliseconds(), 3)}`;
	}

	// ------------------------------------------------

	/**
	 * 从字节数组中解析一个整数（小端序）
	 * @param byte_array 字节数组
	 * @returns 整数值
	 */
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