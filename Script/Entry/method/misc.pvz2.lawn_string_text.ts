/**
 * + misc.pvz2.lawn_string_text.convert PvZ-2字符串表 版本转换
 */
namespace TwinKleS.Entry.method.misc.pvz2.lawn_string_text {

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
					source_file: Argument.Require<string>;
					dest_version: Argument.Request<string, false>;
					dest_file: Argument.Request<string, true>;
				}) {
					let source_file: string;
					let dest_version: Support.PvZ2.LawnStringText.Version;
					let dest_file: string;
					{
						source_file = Argument.require(
							'来源文件', '',
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						dest_version = Argument.request(
							'目标版本', '',
							a.dest_version,
							(value) => (value),
							null,
							() => (Console.option(Support.PvZ2.LawnStringText.VersionE.map((e) => ([e])), null)),
							(value) => (Support.PvZ2.LawnStringText.VersionE.includes(value as any) ? null : `版本不受支持`),
						);
						dest_file = Argument.request(
							'目标文件', '',
							a.dest_file,
							(value) => (value),
							() => (source_file.replace(/((\.(txt|json)))?$/i, `.converted.${{ 'text': 'txt', 'json_map': 'map.json', 'json_list': 'list.json' }[dest_version]}`)),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.PvZ2.LawnStringText.convert_fs(source_file, dest_file, 'auto', dest_version);
					Console.notify('s', `执行成功`, [`${dest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					dest_version: '?input',
					dest_file: '?default',
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