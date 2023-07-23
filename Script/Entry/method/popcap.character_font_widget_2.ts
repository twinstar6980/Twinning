namespace TwinStar.Script.Entry.method.popcap.character_font_widget_2 {

	// ------------------------------------------------

	// encode *
	// decode *

	type Configuration = {
		encode_buffer_size: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.character_font_widget_2.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					definition_file: Executor.Argument<string, false>;
					data_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let definition_file: string;
					let data_file: string;
					let buffer_size: bigint;
					{
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (definition_file.replace(/((\.cfw2)(\.json))?$/i, '.cfw2')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					KernelX.Tool.PopCap.CharacterFontWidget2.encode_fs(data_file, definition_file, {}, buffer_size);
					return [`${data_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					definition_file: undefined!,
					data_file: '?default',
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.cfw2)(\.json)$/i]]),
				input_forwarder: 'definition_file',
			}),
			Executor.method_of({
				id: 'popcap.character_font_widget_2.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.Argument<string, false>;
					definition_file: Executor.Argument<string, true>;
				}) {
					let data_file: string;
					let definition_file: string;
					{
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							() => (data_file.replace(/((\.cfw2))?$/i, '.cfw2.json')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					KernelX.Tool.PopCap.CharacterFontWidget2.decode_fs(data_file, definition_file, {});
					return [`${definition_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file: undefined!,
					definition_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.cfw2)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.character_font_widget_2.encode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					definition_file_directory: Executor.Argument<string, false>;
					data_file_directory: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let definition_file_directory: string;
					let data_file_directory: string;
					let buffer_size: bigint;
					{
						definition_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file_directory'),
							a.definition_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (definition_file_directory.replace(/$/i, '.encode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					let data_buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					simple_batch_execute(
						definition_file_directory,
						['file', /.+(\.cfw2)(\.json)$/i],
						(item) => {
							let definition_file = `${definition_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -5)}`;
							KernelX.Tool.PopCap.CharacterFontWidget2.encode_fs(data_file, definition_file, {}, data_buffer.view());
						},
					);
					return [`${data_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					definition_file_directory: undefined!,
					data_file_directory: '?default',
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'definition_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.character_font_widget_2.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file_directory: Executor.Argument<string, false>;
					definition_file_directory: Executor.Argument<string, true>;
				}) {
					let data_file_directory: string;
					let definition_file_directory: string;
					{
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						definition_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file_directory'),
							a.definition_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.cfw2)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let definition_file = `${definition_file_directory}/${item}.json`;
							KernelX.Tool.PopCap.CharacterFontWidget2.decode_fs(data_file, definition_file, {});
						},
					);
					return [`${definition_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file_directory: undefined!,
					definition_file_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.character_font_widget_2._injector,
});