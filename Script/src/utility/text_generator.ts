namespace Twinning.Script.TextGenerator {

	// #region common

	export type Generator = {
		toString(): string;
	};

	// #endregion

	// #region progress

	export type ProgressStyle = 'percent' | 'fraction';

	// ----------------

	export class Progress implements Generator {

		// #region constructor

		private m_style: ProgressStyle;

		private m_no_bar: boolean;

		private m_length: number;

		private m_limit: number;

		private m_current: number;

		private m_step: number;

		// ----------------

		public constructor(
			style: ProgressStyle,
			no_bar: boolean,
			length: number,
			limit: number,
			current: number = 0,
			step: number = 1,
		) {
			this.m_style = style;
			this.m_no_bar = no_bar;
			this.m_length = length;
			this.m_limit = limit;
			this.m_current = current;
			this.m_step = step;
			return;
		}

		// #endregion

		// #region access

		public limit(
		): number {
			return this.m_limit;
		}

		// ----------------

		public current(
		): number {
			return this.m_current;
		}

		public set_current(
			value: number,
		): Progress {
			this.m_current = value;
			return this;
		}

		// ----------------

		public increase(
			value: number = 1,
		): Progress {
			return this.set_current(this.m_current + this.m_step * value);
		}

		public decrease(
			value: number = 1,
		): Progress {
			return this.set_current(this.m_current - this.m_step * value);
		}

		// #endregion

		// #region generate

		public toString(
		): string {
			let result = '';
			let current_length = Math.floor(this.m_current * this.m_length / this.m_limit);
			let number_text: string;
			switch (this.m_style) {
				case 'percent': {
					number_text = `${Math.floor(this.m_current / this.m_limit).toString().padStart(3, ' ')}%`;
					break;
				}
				case 'fraction': {
					number_text = `${this.m_current.toString().padStart(`${this.m_limit}`.length, ' ')}/${this.m_limit}`;
					break;
				}
			}
			result += `${this.m_no_bar ? '' : `[${'#'.repeat(current_length)}${'-'.repeat(this.m_length - current_length)}] `}${number_text}`;
			return result;
		}

		// #endregion

	}

	// #endregion

}
