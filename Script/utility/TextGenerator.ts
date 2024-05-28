namespace Twinning.Script.TextGenerator {

	// ------------------------------------------------

	export type ProgressStyle = 'percent' | 'fraction';

	export function progress(
		style: ProgressStyle,
		no_bar: boolean,
		length: number,
		limit: number,
		current: number,
	): Array<string> {
		let result = '';
		let current_length = Math.floor(current * length / limit);
		let number_text: string;
		switch (style) {
			case 'percent': {
				number_text = `${make_prefix_padded_string(Math.floor(current / limit), ' ', 3)}%`;
				break;
			}
			case 'fraction': {
				let padded_length = `${limit}`.length;
				number_text = `${make_prefix_padded_string(current, ' ', padded_length)}/${limit}`;
				break;
			}
		}
		result += `${no_bar ? '' : `[${'#'.repeat(current_length)}${'-'.repeat(length - current_length)}] `}${number_text}`;
		return [result];
	}

	// ------------------------------------------------

	export type Generator = {
		make(): Array<string>;
		toString(): string;
	};

	export class Progress implements Generator {

		// ------------------------------------------------

		private m_style: ProgressStyle;

		private m_no_bar: boolean;

		private m_length: number;

		private m_limit: number;

		private m_current: number;

		private m_step: number;

		// ------------------------------------------------

		constructor(
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
		}

		// ------------------------------------------------

		limit(
		): number {
			return this.m_limit;
		}

		current(
		): number {
			return this.m_current;
		}

		set_current(
			value: number,
		): Progress {
			this.m_current = value;
			return this;
		}

		increase(
			value: number = 1,
		): Progress {
			return this.set_current(this.m_current + this.m_step * value);
		}

		decrease(
			value: number = 1,
		): Progress {
			return this.set_current(this.m_current - this.m_step * value);
		}

		// ------------------------------------------------

		make() {
			return progress(this.m_style, this.m_no_bar, this.m_length, this.m_limit, this.m_current);
		}

		// ------------------------------------------------

		toString() {
			return this.make().join('\n');
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}