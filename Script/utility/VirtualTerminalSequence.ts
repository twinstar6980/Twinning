namespace Twinning.Script.VirtualTerminalSequence {

	// ------------------------------------------------

	const k_color_value = {
		black: 0,
		red: 1,
		green: 2,
		yellow: 3,
		blue: 4,
		magenta: 5,
		cyan: 6,
		white: 7,
	} as const;

	export type ColorName = keyof typeof k_color_value;

	// ------------------------------------------------

	export type TextAttribute = {
		background: null | 'default' | [ColorName, boolean];
		foreground: null | 'default' | [ColorName, boolean];
		underline: null | boolean;
		bold: null | boolean;
	};

	export function text_attribute(
		attribute: TextAttribute,
	): string {
		let get_color_code = (
			color: 'default' | [ColorName, boolean],
		): number => (
			color === 'default'
				? 9
				: k_color_value[color[0]] + (color[1] ? 60 : 0)
		);
		let result = '';
		if (attribute.background !== null) {
			result += `${40 + get_color_code(attribute.background)};`;
		}
		if (attribute.foreground !== null) {
			result += `${30 + get_color_code(attribute.foreground)};`;
		}
		if (attribute.underline !== null) {
			result += `${attribute.underline ? 4 : 24};`;
		}
		if (attribute.bold !== null) {
			result += `${attribute.bold ? 1 : 22};`;
		}
		return result.length === 0 ? '' : `\u001b[${result.slice(0, -1)}m`;
	}

	// ------------------------------------------------

}