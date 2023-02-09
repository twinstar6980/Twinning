namespace TwinStar.Language {

	// ------------------------------------------------

	export type Map = Record<string, string | null>;

	export type Table = Record<string, Map>;

	// ------------------------------------------------

	let g_table: Table = {};

	let g_target: string = undefined!;

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
		language: string,
	): void {
		g_target = language;
		return;
	}

	// ------------------------------------------------

	export function query(
		key: string,
		...argumant: any[]
	): string {
		let map = g_table[g_target];
		assert_test(map !== undefined, `locale target not found : ${g_target}`);
		let format = map[key];
		assert_test(format !== undefined, `locale key not found : ${key}`);
		if (format === null) {
			format = key;
		}
		let result = '';
		let argument_index = 0;
		for (let i = 0; i < format.length; ++i) {
			let c = format[i];
			if (format[i] === '{' && format[i + 1] === '}') {
				result += `${argumant[argument_index]}`;
				++argument_index;
				++i;
			} else {
				result += c;
			}
		}
		return result;
	}

	// ------------------------------------------------

}
namespace TwinStar {
	/** LOcalized String, @see {@link Language.query} */
	export const los = Language.query;
}