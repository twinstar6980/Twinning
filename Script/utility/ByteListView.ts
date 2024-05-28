namespace Twinning.Script {

	// ------------------------------------------------

	export class ByteListView {

		// ------------------------------------------------

		private m_view: DataView;

		private m_endian_little: boolean;

		// ------------------------------------------------

		constructor(
			data: ArrayBuffer,
			offset?: number,
			size?: number,
		) {
			this.m_view = new DataView(data, offset, size);
			this.m_endian_little = !Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value;
		}

		// ------------------------------------------------

		size(
		): number {
			return this.m_view.byteLength;
		}

		sub(
			begin: number,
			size: number,
		): ArrayBuffer {
			return Kernel.ByteListView.value(this.m_view.buffer).sub(Kernel.Size.value(BigInt(this.m_view.byteOffset + begin)), Kernel.Size.value(BigInt(size))).value;
		}

		// ------------------------------------------------

		u8(
			position: number,
		): bigint;

		u8(
			position: number,
			value: bigint,
		): void;

		u8(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getUint8(position)) : this.m_view.setUint8(position, Number(value));
		}

		// ------------------------------------------------

		u16(
			position: number,
		): bigint;

		u16(
			position: number,
			value: bigint,
		): void;

		u16(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getUint16(position, this.m_endian_little)) : this.m_view.setUint16(position, Number(value), this.m_endian_little);
		}

		// ------------------------------------------------

		u32(
			position: number,
		): bigint;

		u32(
			position: number,
			value: bigint,
		): void;

		u32(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getUint32(position, this.m_endian_little)) : this.m_view.setUint32(position, Number(value), this.m_endian_little);
		}

		// ------------------------------------------------

		u64(
			position: number,
		): bigint;

		u64(
			position: number,
			value: bigint,
		): void;

		u64(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? this.m_view.getBigUint64(position, this.m_endian_little) : this.m_view.setBigInt64(position, value, this.m_endian_little);
		}

		// ------------------------------------------------

		i8(
			position: number,
		): bigint;

		i8(
			position: number,
			value: bigint,
		): void;

		i8(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getInt8(position)) : this.m_view.setInt8(position, Number(value));
		}

		// ------------------------------------------------

		i16(
			position: number,
		): bigint;

		i16(
			position: number,
			value: bigint,
		): void;

		i16(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getInt16(position, this.m_endian_little)) : this.m_view.setInt16(position, Number(value), this.m_endian_little);
		}

		// ------------------------------------------------

		i32(
			position: number,
		): bigint;

		i32(
			position: number,
			value: bigint,
		): void;

		i32(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getInt32(position, this.m_endian_little)) : this.m_view.setInt32(position, Number(value), this.m_endian_little);
		}

		// ------------------------------------------------

		i64(
			position: number,
		): bigint;

		i64(
			position: number,
			value: bigint,
		): void;

		i64(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? this.m_view.getBigInt64(position, this.m_endian_little) : this.m_view.setBigInt64(position, value, this.m_endian_little);
		}

		// ------------------------------------------------

		f32(
			position: number,
		): number;

		f32(
			position: number,
			value: number,
		): void;

		f32(
			position: number,
			value?: number,
		): void | number {
			return value === undefined ? this.m_view.getFloat32(position, this.m_endian_little) : this.m_view.setFloat32(position, value, this.m_endian_little);
		}

		// ------------------------------------------------

		f64(
			position: number,
		): number;

		f64(
			position: number,
			value: number,
		): void;

		f64(
			position: number,
			value?: number,
		): void | number {
			return value === undefined ? this.m_view.getFloat64(position, this.m_endian_little) : this.m_view.setFloat64(position, value, this.m_endian_little);
		}

		// ------------------------------------------------

	}

	export class ByteStreamView {

		// ------------------------------------------------

		private m_view: ByteListView;

		private m_position: number;

		// ------------------------------------------------

		constructor(
			data: ArrayBuffer,
			offset?: number,
			size?: number,
		) {
			this.m_view = new ByteListView(data, offset, size);
			this.m_position = 0;
		}

		// ------------------------------------------------

		list(
		): ByteListView {
			return this.m_view;
		}

		// ------------------------------------------------

		size(
		): number {
			return this.m_view.size();
		}

		sub(
			begin: number,
			size: number,
		): ArrayBuffer {
			return this.m_view.sub(begin, size);
		}

		// ------------------------------------------------

		p(
		): number;

		p(
			value: number,
		): void;

		p(
			value?: number,
		): void | number {
			return value === undefined ? this.m_position : (this.m_position = value, undefined);
		}

		pr(
			value: number,
		): void {
			return (this.m_position += value, undefined);
		}

		// ------------------------------------------------

		u8(
		): bigint;

		u8(
			value: bigint,
		): void;

		u8(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 1;
			return this.m_view.u8(position, value!);
		}

		// ------------------------------------------------

		u16(
		): bigint;

		u16(
			value: bigint,
		): void;

		u16(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 2;
			return this.m_view.u16(position, value!);
		}

		// ------------------------------------------------

		u32(
		): bigint;

		u32(
			value: bigint,
		): void;

		u32(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 4;
			return this.m_view.u32(position, value!);
		}

		// ------------------------------------------------

		u64(
		): bigint;

		u64(
			value: bigint,
		): void;

		u64(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 8;
			return this.m_view.u64(position, value!);
		}

		// ------------------------------------------------

		i8(
		): bigint;

		i8(
			value: bigint,
		): void;

		i8(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 1;
			return this.m_view.i8(position, value!);
		}

		// ------------------------------------------------

		i16(
		): bigint;

		i16(
			value: bigint,
		): void;

		i16(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 2;
			return this.m_view.i16(position, value!);
		}

		// ------------------------------------------------

		i32(
		): bigint;

		i32(
			value: bigint,
		): void;

		i32(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 4;
			return this.m_view.i32(position, value!);
		}

		// ------------------------------------------------

		i64(
		): bigint;

		i64(
			value: bigint,
		): void;

		i64(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 8;
			return this.m_view.i64(position, value!);
		}

		// ------------------------------------------------

		f32(
		): number;

		f32(
			value: number,
		): void;

		f32(
			value?: number,
		): void | number {
			let position = this.m_position;
			this.m_position += 4;
			return this.m_view.f32(position, value!);
		}

		// ------------------------------------------------

		f64(
		): number;

		f64(
			value: number,
		): void;

		f64(
			value?: number,
		): void | number {
			let position = this.m_position;
			this.m_position += 8;
			return this.m_view.f64(position, value!);
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}