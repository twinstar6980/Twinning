namespace Twinning.Script {

	export class ByteListView {

		// #region structor

		private m_view: DataView<ArrayBuffer>;

		private m_endian_little: boolean;

		// ----------------

		public constructor(
			data: ArrayBuffer,
			offset?: number,
			size?: number,
		) {
			this.m_view = new DataView(data, offset, size);
			this.m_endian_little = !Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value;
			return;
		}

		// #endregion

		// #region action

		public size(
		): number {
			return this.m_view.byteLength;
		}

		public sub(
			begin: number,
			size: number,
		): ArrayBuffer {
			return Kernel.ByteListView.value(this.m_view.buffer).sub(Kernel.Size.value(BigInt(this.m_view.byteOffset + begin)), Kernel.Size.value(BigInt(size))).value;
		}

		// #endregion

		// #region access

		public u8(
			position: number,
		): bigint;

		public u8(
			position: number,
			value: bigint,
		): void;

		public u8(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getUint8(position)) : this.m_view.setUint8(position, Number(value));
		}

		// ----------------

		public u16(
			position: number,
		): bigint;

		public u16(
			position: number,
			value: bigint,
		): void;

		public u16(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getUint16(position, this.m_endian_little)) : this.m_view.setUint16(position, Number(value), this.m_endian_little);
		}

		// ----------------

		public u32(
			position: number,
		): bigint;

		public u32(
			position: number,
			value: bigint,
		): void;

		public u32(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getUint32(position, this.m_endian_little)) : this.m_view.setUint32(position, Number(value), this.m_endian_little);
		}

		// ----------------

		public u64(
			position: number,
		): bigint;

		public u64(
			position: number,
			value: bigint,
		): void;

		public u64(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? this.m_view.getBigUint64(position, this.m_endian_little) : this.m_view.setBigInt64(position, value, this.m_endian_little);
		}

		// ----------------

		public i8(
			position: number,
		): bigint;

		public i8(
			position: number,
			value: bigint,
		): void;

		public i8(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getInt8(position)) : this.m_view.setInt8(position, Number(value));
		}

		// ----------------

		public i16(
			position: number,
		): bigint;

		public i16(
			position: number,
			value: bigint,
		): void;

		public i16(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getInt16(position, this.m_endian_little)) : this.m_view.setInt16(position, Number(value), this.m_endian_little);
		}

		// ----------------

		public i32(
			position: number,
		): bigint;

		public i32(
			position: number,
			value: bigint,
		): void;

		public i32(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? BigInt(this.m_view.getInt32(position, this.m_endian_little)) : this.m_view.setInt32(position, Number(value), this.m_endian_little);
		}

		// ----------------

		public i64(
			position: number,
		): bigint;

		public i64(
			position: number,
			value: bigint,
		): void;

		public i64(
			position: number,
			value?: bigint,
		): void | bigint {
			return value === undefined ? this.m_view.getBigInt64(position, this.m_endian_little) : this.m_view.setBigInt64(position, value, this.m_endian_little);
		}

		// ----------------

		public f32(
			position: number,
		): number;

		public f32(
			position: number,
			value: number,
		): void;

		public f32(
			position: number,
			value?: number,
		): void | number {
			return value === undefined ? this.m_view.getFloat32(position, this.m_endian_little) : this.m_view.setFloat32(position, value, this.m_endian_little);
		}

		// ----------------

		public f64(
			position: number,
		): number;

		public f64(
			position: number,
			value: number,
		): void;

		public f64(
			position: number,
			value?: number,
		): void | number {
			return value === undefined ? this.m_view.getFloat64(position, this.m_endian_little) : this.m_view.setFloat64(position, value, this.m_endian_little);
		}

		// #endregion

	}

}