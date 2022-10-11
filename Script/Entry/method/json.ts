/**
 * + json.format JSON 格式化
 * + json.format.batch [批处理] JSON 格式化
 */
namespace TwinKleS.Entry.method.json {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'json.format',
				description: 'JSON 格式化',
				worker(a: Entry.CFSA & {
					source_file: Argument.Require<string>;
					dest_file: Argument.Request<string, true>;
					disable_trailing_comma: Argument.Request<boolean | null, true>;
					disable_array_wrap_line: Argument.Request<boolean | null, true>;
				}) {
					let source_file: string;
					let dest_file: string;
					let disable_trailing_comma: boolean | null;
					let disable_array_wrap_line: boolean | null;
					{
						source_file = Argument.require(
							'来源文件', '',
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						dest_file = Argument.request(
							'目标文件', '',
							a.dest_file,
							(value) => (value),
							() => (source_file.replace(/((\.json))?$/i, '.formatted.json')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						disable_trailing_comma = Argument.request(
							'是否禁用尾随逗号', '',
							a.disable_trailing_comma,
							(value) => (value),
							() => (null),
							() => (Console.confirm(null, true)),
							(value) => (null),
						);
						disable_array_wrap_line = Argument.request(
							'是否禁用尾随逗号', '',
							a.disable_array_wrap_line,
							(value) => (value),
							() => (null),
							() => (Console.confirm(null, true)),
							(value) => (null),
						);
					}
					let json = CoreX.JSON.read_fs(source_file);
					CoreX.JSON.write_fs(dest_file, json, disable_trailing_comma === null ? undefined : disable_trailing_comma, disable_array_wrap_line === null ? undefined : disable_array_wrap_line);
					Console.notify('s', `执行成功`, [`${dest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					dest_file: '?default',
					disable_trailing_comma: '?input',
					disable_array_wrap_line: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'source_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'json.format.batch',
				description: '[批处理] JSON 格式化',
				worker(a: Entry.CFSA & {
					source_file_directory: Argument.Require<string>;
					dest_file_directory: Argument.Request<string, true>;
					disable_trailing_comma: Argument.Request<boolean | null, true>;
					disable_array_wrap_line: Argument.Request<boolean | null, true>;
				}) {
					let source_file_directory: string;
					let dest_file_directory: string;
					let disable_trailing_comma: boolean | null;
					let disable_array_wrap_line: boolean | null;
					{
						source_file_directory = Argument.require(
							'来源文件目录', '',
							a.source_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						dest_file_directory = Argument.request(
							'目标文件目录', '',
							a.dest_file_directory,
							(value) => (value),
							() => (source_file_directory.replace(/$/i, '.json_format')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						disable_trailing_comma = Argument.request(
							'是否禁用尾随逗号', '',
							a.disable_trailing_comma,
							(value) => (value),
							() => (null),
							() => (Console.confirm(null, true)),
							(value) => (null),
						);
						disable_array_wrap_line = Argument.request(
							'是否禁用尾随逗号', '',
							a.disable_array_wrap_line,
							(value) => (value),
							() => (null),
							() => (Console.confirm(null, true)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						source_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let source_file = `${source_file_directory}/${item}`;
							let dest_file = `${dest_file_directory}/${item}`;
							let json = CoreX.JSON.read_fs(source_file);
							CoreX.JSON.write_fs(dest_file, json, disable_trailing_comma === null ? undefined : disable_trailing_comma, disable_array_wrap_line === null ? undefined : disable_array_wrap_line);
						},
					);
					Console.notify('s', `执行成功`, [`${dest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file_directory: undefined!,
					dest_file_directory: '?default',
					disable_trailing_comma: '?input',
					disable_array_wrap_line: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'source_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.json._injector,
});