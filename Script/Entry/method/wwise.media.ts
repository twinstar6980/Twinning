namespace TwinStar.Script.Entry.method.wwise.media {

	// ------------------------------------------------

	// decode *

	type Configuration = {
		tool_ffmpeg_program_file: Executor.Argument<string, false>;
		tool_ww2ogg_program_file: Executor.Argument<string, false>;
		tool_ww2ogg_code_book_file: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.media.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					tool_ffmpeg_program_file: Executor.Argument<string, false>;
					tool_ww2ogg_program_file: Executor.Argument<string, false>;
					tool_ww2ogg_code_book_file: Executor.Argument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let tool_ffmpeg_program_file: string;
					let tool_ww2ogg_program_file: string;
					let tool_ww2ogg_code_book_file: string;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/((\.wem))?$/i, '.wav')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						tool_ffmpeg_program_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'tool_ffmpeg_program_file'),
							a.tool_ffmpeg_program_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						tool_ww2ogg_program_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'tool_ww2ogg_program_file'),
							a.tool_ww2ogg_program_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						tool_ww2ogg_code_book_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'tool_ww2ogg_code_book_file'),
							a.tool_ww2ogg_code_book_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
					}
					let temporary_directpry = Home.new_temporary();
					CoreX.Tool.Wwise.Media.decode_fs(ripe_file, raw_file, tool_ffmpeg_program_file, tool_ww2ogg_program_file, tool_ww2ogg_code_book_file, temporary_directpry, {});
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file: undefined!,
					raw_file: '?default',
					tool_ffmpeg_program_file: configuration.tool_ffmpeg_program_file,
					tool_ww2ogg_program_file: configuration.tool_ww2ogg_program_file,
					tool_ww2ogg_code_book_file: configuration.tool_ww2ogg_code_book_file,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.wem)$/i]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'wwise.media.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file_directory: Executor.Argument<string, false>;
					raw_file_directory: Executor.Argument<string, true>;
					tool_ffmpeg_program_file: Executor.Argument<string, false>;
					tool_ww2ogg_program_file: Executor.Argument<string, false>;
					tool_ww2ogg_code_book_file: Executor.Argument<string, false>;
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let tool_ffmpeg_program_file: string;
					let tool_ww2ogg_program_file: string;
					let tool_ww2ogg_code_book_file: string;
					{
						ripe_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file_directory'),
							a.ripe_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						raw_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						tool_ffmpeg_program_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'tool_ffmpeg_program_file'),
							a.tool_ffmpeg_program_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						tool_ww2ogg_program_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'tool_ww2ogg_program_file'),
							a.tool_ww2ogg_program_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						tool_ww2ogg_code_book_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'tool_ww2ogg_code_book_file'),
							a.tool_ww2ogg_code_book_file,
							(value) => (Home.of(value)),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
					}
					let temporary_directpry = Home.new_temporary();
					simple_batch_execute(
						ripe_file_directory,
						['file', /.+(\.wem)$/i],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.wav`;
							CoreX.Tool.Wwise.Media.decode_fs(ripe_file, raw_file, tool_ffmpeg_program_file, tool_ww2ogg_program_file, tool_ww2ogg_code_book_file, temporary_directpry, {});
						},
					);
					return [`${raw_file_directory}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file_directory: undefined!,
					raw_file_directory: '?default',
					tool_ffmpeg_program_file: configuration.tool_ffmpeg_program_file,
					tool_ww2ogg_program_file: configuration.tool_ww2ogg_program_file,
					tool_ww2ogg_code_book_file: configuration.tool_ww2ogg_code_book_file,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'ripe_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.wwise.media._injector,
});