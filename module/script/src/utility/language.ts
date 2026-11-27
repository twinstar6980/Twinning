namespace Twinning.Script.Language {

	// #region utility

	export type StringMap = Record<string, string>;

	// ----------------

	let g_map: StringMap = {};

	// ----------------

	export function imbue(
		source: StringMap,
	): void {
		g_map = ConvertHelper.record_transform(source, (key, value) => ([key, value]));
		return;
	}

	// ----------------

	export function query(
		key: string,
		...argument: any[]
	): string {
		let format = g_map[key];
		if (format === undefined) {
			throw new Error(`locale key not found '${key}'`);
		}
		return ConvertHelper.realize_format_string(
			ConvertHelper.parse_format_string(format, false),
			(index, name) => {
				assert_test(index < argument.length);
				return `${argument[index]}`;
			},
		);
	}

	// #endregion

}

namespace Twinning.Script {
	/** LOcalized String, @see {@link Language.query} */
	export const los = Language.query;
}
