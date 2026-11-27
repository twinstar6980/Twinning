namespace Twinning.Script {

	export type Endian = 'little' | 'big';

	export function query_endian(
	): Endian {
		return 'little'; // TODO
	}

	// ----------------

	export class BinaryStreamView {

		// #region constructor

		private m_view: DataView<ArrayBuffer>;

		private m_position: number;

		private m_flag: { endian: Endian };

		// ----------------

		public constructor(
			data: DataView<ArrayBuffer>,
			position?: number,
			flag?: { endian: Endian },
		) {
			this.m_view = data;
			this.m_position = position !== undefined ? position : 0;
			this.m_flag = flag !== undefined ? {...flag} : { endian: KernelX.g_binary_stream_endian };
			return;
		}

		// #endregion

		// #region action

		public size(
		): number {
			return this.m_view.byteLength;
		}

		// ----------------

		public p(
		): number;

		public p(
			value: number,
		): void;

		public p(
			value?: number,
		): void | number {
			return value === undefined ? this.m_position : (this.m_position = value, undefined);
		}

		public pr(
			value: number,
		): void {
			this.m_position += value;
			return;
		}

		// ----------------

		public split_sub(
			begin: number,
			size: number,
		): BinaryStreamView {
			return new BinaryStreamView(new DataView(this.m_view.buffer, this.m_view.byteOffset + begin, size), 0, {...this.m_flag});
		}

		// TODO
		public sub_buffer(
			begin: number,
			size: number,
		): ArrayBuffer {
			return Kernel.BinaryView.value(this.m_view.buffer).sub(Kernel.Size.value(BigInt(this.m_view.byteOffset + begin)), Kernel.Size.value(BigInt(size))).value;
		}

		// TODO
		private _endian(
		): boolean {
			return this.m_flag.endian === 'little';
		}

		// #endregion

		// #region access

		public u8(
		): bigint;

		public u8(
			value: bigint,
		): void;

		public u8(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 1;
			return value === undefined ? BigInt(this.m_view.getUint8(position)) : this.m_view.setUint8(position, Number(value));
		}

		// ----------------

		public u16(
		): bigint;

		public u16(
			value: bigint,
		): void;

		public u16(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 2;
			return value === undefined ? BigInt(this.m_view.getUint16(position, this._endian())) : this.m_view.setUint16(position, Number(value), this._endian());
		}

		// ----------------

		public u32(
		): bigint;

		public u32(
			value: bigint,
		): void;

		public u32(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 4;
			return value === undefined ? BigInt(this.m_view.getUint32(position, this._endian())) : this.m_view.setUint32(position, Number(value), this._endian());
		}

		// ----------------

		public u64(
		): bigint;

		public u64(
			value: bigint,
		): void;

		public u64(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 8;
			return value === undefined ? this.m_view.getBigUint64(position, this._endian()) : this.m_view.setBigUint64(position, value, this._endian());
		}

		// ----------------

		public i8(
		): bigint;

		public i8(
			value: bigint,
		): void;

		public i8(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 1;
			return value === undefined ? BigInt(this.m_view.getInt8(position)) : this.m_view.setInt8(position, Number(value));
		}

		// ----------------

		public i16(
		): bigint;

		public i16(
			value: bigint,
		): void;

		public i16(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 2;
			return value === undefined ? BigInt(this.m_view.getInt16(position, this._endian())) : this.m_view.setInt16(position, Number(value), this._endian());
		}

		// ----------------

		public i32(
		): bigint;

		public i32(
			value: bigint,
		): void;

		public i32(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 4;
			return value === undefined ? BigInt(this.m_view.getInt32(position, this._endian())) : this.m_view.setInt32(position, Number(value), this._endian());
		}

		// ----------------

		public i64(
		): bigint;

		public i64(
			value: bigint,
		): void;

		public i64(
			value?: bigint,
		): void | bigint {
			let position = this.m_position;
			this.m_position += 8;
			return value === undefined ? this.m_view.getBigInt64(position, this._endian()) : this.m_view.setBigInt64(position, value, this._endian());
		}

		// ----------------

		public f32(
		): number;

		public f32(
			value: number,
		): void;

		public f32(
			value?: number,
		): void | number {
			let position = this.m_position;
			this.m_position += 4;
			return value === undefined ? this.m_view.getFloat32(position, this._endian()) : this.m_view.setFloat32(position, value, this._endian());
		}

		// ----------------

		public f64(
		): number;

		public f64(
			value: number,
		): void;

		public f64(
			value?: number,
		): void | number {
			let position = this.m_position;
			this.m_position += 8;
			return value === undefined ? this.m_view.getFloat64(position, this._endian()) : this.m_view.setFloat64(position, value, this._endian());
		}

		// #endregion

	}

}
