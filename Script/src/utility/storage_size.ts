namespace Twinning.Script {

	export enum StorageSizeExponent {
		b = 0,
		k = 1,
		m = 2,
		g = 3,
	}

	// ----------------

	export class StorageSize {

		// #region constructor

		private m_count: number;

		private m_exponent: StorageSizeExponent;

		// ----------------

		public constructor(
		);

		public constructor(
			other: StorageSize,
		);

		public constructor(
			text: string,
		);

		public constructor(
			argument?: StorageSize | string,
		) {
			this.m_count = 0;
			this.m_exponent = StorageSizeExponent.b;
			if (argument instanceof StorageSize) {
				this.m_count = argument.m_count;
				this.m_exponent = argument.m_exponent;
			}
			if (typeof argument === 'string') {
				this.parse(argument);
			}
			return;
		}

		// #endregion

		// #region query

		public count(
		): StoragePathType {
			return this.m_count;
		}

		public exponent(
		): StorageSizeExponent {
			return this.m_exponent;
		}

		// #endregion

		// #region value

		public value(
		): bigint {
			let count_big = Math.trunc(this.m_count);
			let count_little = this.m_count - count_big;
			return BigInt(count_big) * BigInt(1024 ** this.m_exponent) + BigInt(Math.trunc(count_little * (1024 ** this.m_exponent)));
		}

		// #endregion

		// #region convert

		public parse(
			text: string,
		): void {
			this.m_count = 0;
			this.m_exponent = StorageSizeExponent.b;
			let count = Number.parseFloat(text.slice(0, -1));
			let exponent = ['b', 'k', 'm', 'g'].indexOf(text.slice(-1));
			assert_test(exponent !== -1);
			this.m_count = count;
			this.m_exponent = exponent;
			return;
		}

		public emit(
		): string {
			return `${this.m_count.toFixed(1)}${['b', 'k', 'm', 'g'][this.m_exponent]}`;
		}

		// #endregion

	}

}
