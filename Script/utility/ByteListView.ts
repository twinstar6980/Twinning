namespace TwinStar {

	// ------------------------------------------------

	export class ByteListView {

		// ------------------------------------------------

		private m_view: DataView;

		private m_endian_little: boolean;

		// ------------------------------------------------

		constructor(
			data: ArrayBuffer,
			offset: number = 0,
		) {
			this.m_view = new DataView(data, offset);
			this.m_endian_little = true;
		}

		// ------------------------------------------------

		sub(
			begin: number,
			size: number,
		): ArrayBuffer {
			return Core.ByteListView.value(this.m_view.buffer).sub(Core.Size.value(BigInt(begin)), Core.Size.value(BigInt(size))).value;
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
			return (value === undefined) ? BigInt(this.m_view.getUint8(position)) : this.m_view.setUint8(position, Number(value));
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
			return (value === undefined) ? BigInt(this.m_view.getUint16(position, this.m_endian_little)) : this.m_view.setUint16(position, Number(value), this.m_endian_little);
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
			return (value === undefined) ? BigInt(this.m_view.getUint32(position, this.m_endian_little)) : this.m_view.setUint32(position, Number(value), this.m_endian_little);
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
			return (value === undefined) ? this.m_view.getBigInt64(position, this.m_endian_little) : this.m_view.setBigInt64(position, value, this.m_endian_little);
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
			return (value === undefined) ? BigInt(this.m_view.getInt8(position)) : this.m_view.setInt8(position, Number(value));
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
			return (value === undefined) ? BigInt(this.m_view.getInt16(position, this.m_endian_little)) : this.m_view.setInt16(position, Number(value), this.m_endian_little);
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
			return (value === undefined) ? BigInt(this.m_view.getInt32(position, this.m_endian_little)) : this.m_view.setInt32(position, Number(value), this.m_endian_little);
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
			return (value === undefined) ? this.m_view.getBigInt64(position, this.m_endian_little) : this.m_view.setBigInt64(position, value, this.m_endian_little);
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
			return (value === undefined) ? this.m_view.getFloat32(position, this.m_endian_little) : this.m_view.setFloat32(position, value, this.m_endian_little);
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
			return (value === undefined) ? this.m_view.getFloat64(position, this.m_endian_little) : this.m_view.setFloat64(position, value, this.m_endian_little);
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}