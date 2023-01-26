/**
 * + pvz2.lawn_string_text.convert PvZ-2字符串表 版本转换
 */
namespace TwinStar.Entry.method.pvz2.lawn_string_text {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'pvz2.lawn_string_text.convert',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					source_file: Executor.RequireArgument<string>;
					destination_version: Executor.RequestArgument<string, false>;
					destination_file: Executor.RequestArgument<string, true>;
				}) {
					let source_file: string;
					let destination_version: Support.PvZ2.LawnStringText.Version;
					let destination_file: string;
					{
						source_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'source_file'),
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						destination_version = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'destination_version'),
							a.destination_version,
							(value) => (value),
							null,
							() => (Console.option(Support.PvZ2.LawnStringText.VersionE.map((e) => ([e])), null)),
							(value) => (Support.PvZ2.LawnStringText.VersionE.includes(value as any) ? null : los(`版本不受支持`)),
						);
						destination_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'destination_file'),
							a.destination_file,
							(value) => (value),
							() => (source_file.replace(/((\.(txt|json)))?$/i, `.converted.${{ 'text': 'txt', 'json_map': 'map.json', 'json_list': 'list.json' }[destination_version]}`)),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.PvZ2.LawnStringText.convert_fs(source_file, destination_file, 'auto', destination_version);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_version: '?input',
					destination_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.*(LawnStrings).*(\.(txt|json))$/i]]),
				input_forwarder: 'source_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Entry.method.pvz2.lawn_string_text._injector,
});