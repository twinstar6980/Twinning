/** 输出工具 */
namespace TwinKleS.Output {

	// ------------------------------------------------

	/**
	 * 常规输出文本
	 * @param text 文本
	 */
	export function normal(
		text: string,
	): void {
		Shell.output(text);
		return;
	}

	// ------------------------------------------------

	/** 是否启用虚拟终端序列 */
	export let g_enable_virtual_terminal_sequences = false;

	/**
	 * 设置文本属性
	 * 
	 * 仅当 g_enable_virtual_terminal_sequences == true 时输出控制序列，以避免在不支持虚拟终端序列的环境下将控制序列输出为文本
	 * 
	 * @param attribute 文本属性
	 */
	export function set_text_attribute(
		attribute: VirtualTerminalSequences.TextAttribute,
	): void {
		if (g_enable_virtual_terminal_sequences) {
			normal(VirtualTerminalSequences.text_attribute(attribute));
		}
		return;
	}

	// ------------------------------------------------

	/**
	 * 分类化文本类型名
	 * + default 默认
	 * + debug 调试
	 * + verbose 常规
	 * + information 信息
	 * + error 错误
	 * + input 输入
	 */
	export type ClassifyTextTypeName = 'default' | 'debug' | 'verbose' | 'information' | 'warning' | 'error' | 'input';

	const k_classify_text_attribute: Record<ClassifyTextTypeName, VirtualTerminalSequences.TextAttribute> = {
		default: {
			background: 'default',
			foreground: 'default',
			underline: false,
		},
		debug: {
			background: null,
			foreground: ['magenta', false],
			underline: null,
		},
		verbose: {
			background: null,
			foreground: 'default',
			underline: null,
		},
		information: {
			background: null,
			foreground: ['blue', false],
			underline: null,
		},
		warning: {
			background: null,
			foreground: ['yellow', false],
			underline: null,
		},
		error: {
			background: null,
			foreground: ['red', false],
			underline: null,
		},
		input: {
			background: null,
			foreground: ['green', false],
			underline: null,
		},
	};

	/**
	 * 设置分类化文本属性
	 * @param type 类型名
	 */
	export function set_classify_attribute(
		type: ClassifyTextTypeName,
	): void {
		return set_text_attribute(k_classify_text_attribute[type]);
	}

	// ------------------------------------------------

	/**
	 * 输出分类化文本
	 * @param type 类型
	 * @param text 文本
	 * @param indent 缩进
	 * @param leading 前导符
	 * @param tailing_newline 是否附加尾随换行符
	 */
	export function classify(
		type: ClassifyTextTypeName,
		text: string,
		indent: number = 0,
		leading: null | string = null,
		tailing_newline: boolean = true,
	): void {
		set_classify_attribute(type);
		let processed_text = make_indented_string(text, indent);
		if (leading !== null) {
			processed_text = `${leading} ${processed_text}`;
		}
		if (tailing_newline) {
			processed_text = `${processed_text}\n`;
		}
		normal(processed_text);
		set_classify_attribute('default');
		return;
	}

	// ------------------------------------------------

	/**
	 * 输出分类化文本：调试级
	 * @param text 文本
	 * @param indent 缩进
	 * @param leading 前导符
	 * @param tailing_newline 是否附加尾随换行符
	 */
	export function debug(
		text: string,
		indent: number = 0,
		leading: null | string = null,
		tailing_newline: boolean = true,
	): void {
		return classify('debug', text, indent, leading, tailing_newline);
	}

	/**
	 * 输出分类化文本：常规级
	 * @param text 文本
	 * @param indent 缩进
	 * @param leading 前导符
	 * @param tailing_newline 是否附加尾随换行符
	 */
	export function verbose(
		text: string,
		indent: number = 0,
		leading: null | string = null,
		tailing_newline: boolean = true,
	): void {
		return classify('verbose', text, indent, leading, tailing_newline);
	}

	/**
	 * 输出分类化文本：信息级
	 * @param text 文本
	 * @param indent 缩进
	 * @param leading 前导符
	 * @param tailing_newline 是否附加尾随换行符
	 */
	export function information(
		text: string,
		indent: number = 0,
		leading: null | string = null,
		tailing_newline: boolean = true,
	): void {
		return classify('information', text, indent, leading, tailing_newline);
	}

	/**
	 * 输出分类化文本：警告级
	 * @param text 文本
	 * @param indent 缩进
	 * @param leading 前导符
	 * @param tailing_newline 是否附加尾随换行符
	 */
	export function warning(
		text: string,
		indent: number = 0,
		leading: null | string = null,
		tailing_newline: boolean = true,
	): void {
		return classify('warning', text, indent, leading, tailing_newline);
	}

	/**
	 * 输出分类化文本：错误级
	 * @param text 文本
	 * @param indent 缩进
	 * @param leading 前导符
	 * @param tailing_newline 是否附加尾随换行符
	 */
	export function error(
		text: string,
		indent: number = 0,
		leading: null | string = null,
		tailing_newline: boolean = true,
	): void {
		return classify('error', text, indent, leading, tailing_newline);
	}

	// ------------------------------------------------

	export const c = classify;

	export const d = debug;

	export const v = verbose;

	export const i = information;

	export const w = warning;

	export const e = error;

	// ------------------------------------------------

}