namespace TwinStar.Script.Language {

	// ------------------------------------------------

	export type Map = Record<string, string>;

	export type Table = Record<string, Map>;

	// ------------------------------------------------

	let g_table: Table = {};

	let g_target: string | null = null;

	// ------------------------------------------------

	export function push_table(
		language: string,
		map: Map,
	): void {
		g_table[language] = {};
		for (let key in map) {
			g_table[language][key] = map[key];
		}
		return;
	}

	export function set_target(
		language: string | null,
	): void {
		g_target = language;
		return;
	}

	// ------------------------------------------------

	export function query(
		id: string,
		...argumant: any[]
	): string {
		assert_test(g_target !== null, `locale target not set`);
		let map = g_table[g_target];
		assert_test(map !== undefined, `locale target not found : ${g_target}`);
		let format = map[id];
		assert_test(format !== undefined, `locale id not found : ${id}`);
		let result = '';
		let argument_index = 0;
		for (let index = 0; index < format.length; ++index) {
			let character = format[index];
			if (format[index] === '{' && format[index + 1] === '}') {
				result += `${argumant[argument_index]}`;
				++argument_index;
				++index;
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