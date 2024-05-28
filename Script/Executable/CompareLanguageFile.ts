// 对比语言文本文件
namespace Twinning.Script.Executable.CompareLanguageFile {

	function compare(
		original_map: Language.StringMap,
		translation_map: Language.StringMap,
	): void {
		let original_key = Object.keys(original_map);
		let translation_key = Object.keys(translation_map);
		let missing_key = original_key.filter((key) => (!translation_key.includes(key)));
		let surplus_key = translation_key.filter((key) => (!original_key.includes(key)));
		let line_warning: Array<string> = [];
		let line_count = Math.min(original_key.length, translation_key.length);
		for (let line = 0; line < line_count; line++) {
			if (original_key[line] !== translation_key[line]) {
				line_warning.push(`line ${line} : ${original_key[line]} <==> ${translation_key[line]}`);
			}
		}
		Console.error(`missing key ${missing_key.length}`, missing_key);
		Console.warning(`surplus key ${surplus_key.length}`, surplus_key);
		Console.warning(`line warning ${line_warning.length}`, line_warning);
		return;
	}

	export function execute(
	): void {
		Console.information(los('executable.compare_language_file:please_input_original_file'), []);
		let original_file = Console.path('file', 'input', null, null);
		Console.information(los('executable.compare_language_file:please_input_translation_file'), []);
		let translation_file = Console.path('file', 'input', null, null);
		let original_map = KernelX.JSON.read_fs_js(original_file) as Language.StringMap;
		let translation_map = KernelX.JSON.read_fs_js(translation_file) as Language.StringMap;
		compare(original_map, translation_map);
		return;
	}

}

Twinning.Script.Executable.CompareLanguageFile.execute();