namespace TwinStar.Script.Entry.method.wwise.media {

	// ------------------------------------------------

	// decode *

	type Config = {
		tool_ffmpeg_program_file: Executor.RequestArgument<string, false>;
		tool_ww2ogg_program_file: Executor.RequestArgument<string, false>;
		tool_ww2ogg_code_book_file: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.media.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					tool_ffmpeg_program_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_program_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_code_book_file: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let tool_ffmpeg_program_file: string;
					let tool_ww2ogg_program_file: string;
					let tool_ww2ogg_code_book_file: string;
					{
						ripe_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						raw_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/((\.wem))?$/i, '.wav')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						tool_ffmpeg_program_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ffmpeg_program_file'),
							a.tool_ffmpeg_program_file,
							(value) => (HomeDirectory.of(value)),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_program_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_program_file'),
							a.tool_ww2ogg_program_file,
							(value) => (HomeDirectory.of(value)),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_code_book_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_code_book_file'),
							a.tool_ww2ogg_code_book_file,
							(value) => (HomeDirectory.of(value)),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
					}
					let temporary_directpry = HomeDirectory.new_temporary();
					CoreX.Tool.Wwise.Media.decode_fs(ripe_file, raw_file, tool_ffmpeg_program_file, tool_ww2ogg_program_file, tool_ww2ogg_code_book_file, temporary_directpry);
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					tool_ffmpeg_program_file: config.tool_ffmpeg_program_file,
					tool_ww2ogg_program_file: config.tool_ww2ogg_program_file,
					tool_ww2ogg_code_book_file: config.tool_ww2ogg_code_book_file,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.wem)$/i]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'wwise.media.decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file_directory: Executor.RequireArgument<string>;
					raw_file_directory: Executor.RequestArgument<string, true>;
					tool_ffmpeg_program_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_program_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_code_book_file: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let tool_ffmpeg_program_file: string;
					let tool_ww2ogg_program_file: string;
					let tool_ww2ogg_code_book_file: string;
					{
						ripe_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'ripe_file_directory'),
							a.ripe_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_file_directory.replace(/$/i, '.decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						tool_ffmpeg_program_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ffmpeg_program_file'),
							a.tool_ffmpeg_program_file,
							(value) => (HomeDirectory.of(value)),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_program_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_program_file'),
							a.tool_ww2ogg_program_file,
							(value) => (HomeDirectory.of(value)),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_code_book_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_code_book_file'),
							a.tool_ww2ogg_code_book_file,
							(value) => (HomeDirectory.of(value)),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
					}
					let temporary_directpry = HomeDirectory.new_temporary();
					simple_batch_execute(
						ripe_file_directory,
						['file', /.+(\.wem)$/i],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.wav`;
							CoreX.Tool.Wwise.Media.decode_fs(ripe_file, raw_file, tool_ffmpeg_program_file, tool_ww2ogg_program_file, tool_ww2ogg_code_book_file, temporary_directpry);
						},
					);
					Console.notify('s', los(`执行成功`), [`${raw_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file_directory: undefined!,
					raw_file_directory: '?default',
					tool_ffmpeg_program_file: config.tool_ffmpeg_program_file,
					tool_ww2ogg_program_file: config.tool_ww2ogg_program_file,
					tool_ww2ogg_code_book_file: config.tool_ww2ogg_code_book_file,
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