/** 文本生成器 */
namespace TwinStar.TextGenerator {

	// ------------------------------------------------

	/**
	 * 生成普通文本
	 * @param text 文本
	 */
	export function text(
		text: string,
	): Array<string> {
		return [text];
	}

	// ------------------------------------------------

	/**
	 * 进度条文本中的数值显示样式
	 * + percent 百分比
	 * + fraction 分数
	 */
	export type ProgressStyle = 'percent' | 'fraction';

	/**
	 * 生成进度条文本
	 * @param style 数值显示样式
	 * @param length 字符进度条长度
	 * @param limit 进度上限
	 * @param current 进度值
	 */
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

	/** 文本生成器接口 */
	export type Generator = {
		make(): Array<string>;
		toString(): string;
	};

	/** 普通文本生成器 */
	export class Text implements Generator {

		// ------------------------------------------------

		private m_text: string;

		// ------------------------------------------------

		/**
		 * @param text 文本
		 */
		constructor(
			text: string,
		) {
			this.m_text = text;
		}

		// ------------------------------------------------

		/** 生成文本 */
		make() {
			return text(this.m_text);
		}

		// ------------------------------------------------

		toString() {
			return this.make().join('\n');
		}

		// ------------------------------------------------

	}

	/** 进度条文本生成器 */
	export class Progress implements Generator {

		// ------------------------------------------------

		private m_style: ProgressStyle;

		private m_no_bar: boolean;

		private m_length: number;

		private m_limit: number;

		private m_current: number;

		private m_step: number;

		// ------------------------------------------------

		/**
		 * @param style 数值显示样式
		 * @param length 字符进度条长度
		 * @param no_bar 不显示进度条
		 * @param limit 进度上限
		 * @param current 进度值
		 * @param step 进度增减步长
		 */
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

		/**
		 * 获取进度上限
		 * @returns 进度上限
		 */
		limit(
		): number {
			return this.m_limit;
		}

		/**
		 * 获取进度值
		 * @returns 进度值
		 */
		current(
		): number {
			return this.m_current;
		}

		/**
		 * 设置进度值
		 * @param value 进度值
		 * @returns this
		 */
		set_current(
			value: number,
		): Progress {
			this.m_current = value;
			return this;
		}

		/**
		 * 增长进度值
		 * @param value 增长值
		 * @returns this
		 */
		increase(
			value: number = 1,
		): Progress {
			return this.set_current(this.m_current + this.m_step * value);
		}

		/**
		 * 减少进度值
		 * @param value 减少值
		 * @returns this
		 */
		decrease(
			value: number = 1,
		): Progress {
			return this.set_current(this.m_current - this.m_step * value);
		}

		// ------------------------------------------------

		/** 生成文本 */
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