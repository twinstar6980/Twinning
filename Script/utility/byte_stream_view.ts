namespace Twinning.Script {

	export class ByteStreamView {

		// #region constructor

		private m_view: ByteListView;

		private m_position: number;

		// ----------------

		public constructor(
			data: ArrayBuffer,
			offset?: number,
			size?: number,
		) {
			this.m_view = new ByteListView(data, offset, size);
			this.m_position = 0;
			return;
		}

		// #endregion

		// #region action

		public list(
		): ByteListView {
			return this.m_view;
		}

		// ----------------

		public size(
		): number {
			return this.m_view.size();
		}

		public sub(
			begin: number,
			size: number,
		): ArrayBuffer {
			return this.m_view.sub(begin, size);
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
			return (this.m_position += value, undefined);
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
			return this.m_view.u8(position, value!);
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
			return this.m_view.u16(position, value!);
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
			return this.m_view.u32(position, value!);
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
			return this.m_view.u64(position, value!);
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
			return this.m_view.i8(position, value!);
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
			return this.m_view.i16(position, value!);
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
			return this.m_view.i32(position, value!);
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
			return this.m_view.i64(position, value!);
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
			return this.m_view.f32(position, value!);
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
			return this.m_view.f64(position, value!);
		}

		// #endregion

	}

}
