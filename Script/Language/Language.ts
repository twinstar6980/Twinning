/** 多语言 */
namespace TwinKleS.Language {

	// ------------------------------------------------

	/** 多语言字符串表 */
	export type Table = Record<string, Record<string, string>>;

	/** 全局字符串表 */
	let table: Table = {};

	/**
	 * 向全局表追加数据
	 * @param another_table 追加的多语言字符串表，若需追加的键已存在于全局表中，则覆盖原有值
	 */
	export function push(
		another_table: Table,
	): void {
		for (let language in another_table) {
			if (table[language] === undefined) {
				table[language] = {};
			}
			for (let key in another_table[language]) {
				table[language][key] = another_table[language][key];
			}
		}
		return;
	}

	// ------------------------------------------------

	/** 目标语言 */
	export let target: string = '';

	/**
	 * 从全局表中获取key对应的字符串
	 * @param key 搜索的键
	 * @param argumant 填充参数
	 * @returns 经过填充的相应字符串
	 */
	export function of(
		key: string,
		...argumant: any[]
	): string {
		let map = table[target];
		if (map === undefined) {
			throw new MyError(`locale not found : ${target}`);
		}
		let format = map[key];
		if (format === undefined) {
			throw new MyError(`key not found : ${key}`);
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

namespace TwinKleS {
	/** @see {@link Language.of} */
	export const langof = Language.of;
}

({
	injector: (
		config: TwinKleS.Language.Table,
	) => {
		TwinKleS.Language.push(config);
	}
});