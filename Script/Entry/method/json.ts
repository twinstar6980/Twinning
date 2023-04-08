namespace TwinStar.Script.Entry.method.json {

	// ------------------------------------------------

	// format *

	type Configuration = {
		disable_trailing_comma: Executor.RequestArgument<boolean, true>;
		disable_array_wrap_line: Executor.RequestArgument<boolean, true>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'json.format',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					source_file: Executor.RequireArgument<string>;
					destination_file: Executor.RequestArgument<string, true>;
					disable_trailing_comma: Executor.RequestArgument<boolean, true>;
					disable_array_wrap_line: Executor.RequestArgument<boolean, true>;
				}) {
					let source_file: string;
					let destination_file: string;
					let disable_trailing_comma: boolean;
					let disable_array_wrap_line: boolean;
					{
						source_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'source_file'),
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						destination_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'destination_file'),
							a.destination_file,
							(value) => (value),
							() => (source_file.replace(/((\.json))?$/i, '.format.json')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						disable_trailing_comma = Executor.request_argument(
							Executor.query_argument_name(this.id, 'disable_trailing_comma'),
							a.disable_trailing_comma,
							(value) => (value),
							() => (CoreX.JSON.g_format.disable_trailing_comma),
							(initial) => (Console.confirmation(null, null, initial)),
						);
						disable_array_wrap_line = Executor.request_argument(
							Executor.query_argument_name(this.id, 'disable_array_wrap_line'),
							a.disable_array_wrap_line,
							(value) => (value),
							() => (CoreX.JSON.g_format.disable_array_wrap_line),
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					let json = CoreX.JSON.read_fs(source_file);
					CoreX.JSON.write_fs(destination_file, json, disable_trailing_comma, disable_array_wrap_line);
					Console.success(los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					source_file: undefined!,
					destination_file: '?default',
					disable_trailing_comma: configuration.disable_trailing_comma,
					disable_array_wrap_line: configuration.disable_array_wrap_line,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'source_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'json.format.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					source_file_directory: Executor.RequireArgument<string>;
					destination_file_directory: Executor.RequestArgument<string, true>;
					disable_trailing_comma: Executor.RequestArgument<boolean, true>;
					disable_array_wrap_line: Executor.RequestArgument<boolean, true>;
				}) {
					let source_file_directory: string;
					let destination_file_directory: string;
					let disable_trailing_comma: boolean;
					let disable_array_wrap_line: boolean;
					{
						source_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'source_file_directory'),
							a.source_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						destination_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'destination_file_directory'),
							a.destination_file_directory,
							(value) => (value),
							() => (source_file_directory.replace(/$/i, '.format')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						disable_trailing_comma = Executor.request_argument(
							Executor.query_argument_name(this.id, 'disable_trailing_comma'),
							a.disable_trailing_comma,
							(value) => (value),
							() => (CoreX.JSON.g_format.disable_trailing_comma),
							(initial) => (Console.confirmation(null, null, initial)),
						);
						disable_array_wrap_line = Executor.request_argument(
							Executor.query_argument_name(this.id, 'disable_array_wrap_line'),
							a.disable_array_wrap_line,
							(value) => (value),
							() => (CoreX.JSON.g_format.disable_array_wrap_line),
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					simple_batch_execute(
						source_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let source_file = `${source_file_directory}/${item}`;
							let destination_file = `${destination_file_directory}/${item}`;
							let json = CoreX.JSON.read_fs(source_file);
							CoreX.JSON.write_fs(destination_file, json, disable_trailing_comma, disable_array_wrap_line);
						},
					);
					Console.success(los(`执行成功`), [`${destination_file_directory}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					source_file_directory: undefined!,
					destination_file_directory: '?default',
					disable_trailing_comma: configuration.disable_trailing_comma,
					disable_array_wrap_line: configuration.disable_array_wrap_line,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'source_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.json._injector,
});