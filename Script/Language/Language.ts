namespace TwinStar.Script.Language {

	// ------------------------------------------------

	export type StringMap = Record<string, string>;

	// ------------------------------------------------

	let g_map: StringMap = {};

	// ------------------------------------------------

	export function imbue(
		source: StringMap,
	): void {
		for (let key in source) {
			g_map[key] = source[key];
		}
		return;
	}

	// ------------------------------------------------

	export function query(
		id: string,
		...argumant: any[]
	): string {
		let format = g_map[id];
		assert_test(format !== undefined, `locale id not found : ${id}`);
		let result = '';
		let argument_index = 0;
		for (let index = 0; index < format.length; index++) {
			let character = format[index];
			if (format[index] === '{' && format[index + 1] === '}') {
				result += `${argumant[argument_index]}`;
				argument_index++;
				index++;
			} else if (format[index] === '{' && format[index + 1] === '{') {
				result += character;
				index++;
			} else {
				result += character;
			}
		}
		return result;
	}

	// ------------------------------------------------

}
namespace TwinStar.Script {
	/** LOcalized String, @see {@link Language.query} */
	export const los = Language.query;
}