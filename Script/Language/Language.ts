/** 多语言 */
namespace TwinStar.Language {

	// ------------------------------------------------

	/** 单语言字符串表 */
	export type Map = Record<string, string | null>;

	/** 多语言字符串表 */
	export type Table = Record<string, Map>;

	/** 全局字符串表 */
	let g_table: Table = {};

	/** 目标语言 */
	let g_target: string = undefined!;

	// ------------------------------------------------

	/**
	 * 向全局表追加数据
	 * @param language 语言
	 * @param map 单语言字符串表
	 */
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

	/**
	 * 设置目标语言
	 * @param language 语言
	 */
	export function set_target(
		language: string,
	): void {
		g_target = language;
		return;
	}

	// ------------------------------------------------

	/**
	 * 从全局表中查询key对应的字符串
	 * @param key 搜索的键
	 * @param argumant 填充参数
	 * @returns 经过填充的相应字符串
	 */
	export function query(
		key: string,
		...argumant: any[]
	): string {
		let map = g_table[g_target];
		if (map === undefined) {
			throw new Error(`locale not found : ${g_target}`);
		}
		let format = map[key];
		if (format === undefined) {
			throw new Error(`key not found : ${key}`);
		}
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