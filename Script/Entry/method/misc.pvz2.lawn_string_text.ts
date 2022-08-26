/**
 * + misc.pvz2.lawn_string_text.convert PvZ-2字符串表 版本转换
 */
namespace TwinKleS.Entry.method.misc.pvz2.lawn_string_text {

	// ------------------------------------------------

	function input_text_version(
		message: string,
	): Support.PvZ2.LawnStringText.Version {
		let format_list = Support.PvZ2.LawnStringText.VersionE;
		let index = Input.number([
			message,
			format_list.map((e, i) => (`${make_prefix_padded_string(i + 1, ' ', 2)}. ${e}`)),
		], Check.enum_checkerx(format_list.map((e, i) => (i + 1))))!;
		return format_list[index - 1];
	}

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'misc.pvz2.lawn_string_text.convert',
				description: 'PvZ-2字符串表 版本转换',
				worker(a: Entry.CFSA & {
					source_file: string;
					dest_file: string | '?default' | '?input';
					dest_version: Support.PvZ2.LawnStringText.Version | '?input';
				}) {
					let source_file: string;
					let dest_file: string;
					let dest_version: Support.PvZ2.LawnStringText.Version;
					{
						source_file = a.source_file;
						if (a.dest_version === '?input') {
							dest_version = input_text_version(`请输入目标文本版本`);
						} else {
							dest_version = a.dest_version;
						}
						dest_file = ArgumentParser.path(a.dest_file, {
							input_message: '请输入输出路径',
							default_value: source_file.replace(/((\.(txt|json)))?$/i, `.converted.${{ 'text': 'txt', 'json_map': 'map.json', 'json_list': 'list.json' }[dest_version]}`),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					Support.PvZ2.LawnStringText.convert_fs(source_file, dest_file, 'auto', dest_version);
					Output.i(`输出路径：${dest_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					dest_file: '?default',
					dest_version: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.*(LawnStrings).*(\.(txt|json))$/i]]),
				input_forwarder: 'source_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.misc.pvz2.lawn_string_text._injector,
});