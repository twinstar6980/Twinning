/**
 * + wwise.encoded_media.decode Wwise-Encoded-Media 解码
 * + wwise.encoded_media.decode.batch [批处理] Wwise-Encoded-Media 解码
 */
namespace TwinStar.Entry.method.wwise.encoded_media {

	// ------------------------------------------------

	type Config = {
		tool: {
			ffmpeg: string;
			ww2ogg: string;
			ww2ogg_pcb: string;
		};
		temp_directpry: Executor.RequireArgument<string>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.encoded_media.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					tool_ffmpeg_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_pcb_file: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let tool_ffmpeg_file: string;
					let tool_ww2ogg_file: string;
					let tool_ww2ogg_pcb_file: string;
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
						tool_ffmpeg_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ffmpeg_file'),
							a.tool_ffmpeg_file,
							(value) => (value),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_file'),
							a.tool_ww2ogg_file,
							(value) => (value),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_pcb_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_pcb_file'),
							a.tool_ww2ogg_pcb_file,
							(value) => (value),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
					}
					CoreX.Tool.Wwise.EncodedMedia.decode_fs(ripe_file, raw_file, tool_ffmpeg_file, tool_ww2ogg_file, tool_ww2ogg_pcb_file, Main.path_at_home(config.temp_directpry));
					Console.notify('s', localized(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					tool_ffmpeg_file: Main.path_at_home(config.tool.ffmpeg),
					tool_ww2ogg_file: Main.path_at_home(config.tool.ww2ogg),
					tool_ww2ogg_pcb_file: Main.path_at_home(config.tool.ww2ogg_pcb),
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.wem)$/i]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'wwise.encoded_media.decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file_directory: Executor.RequireArgument<string>;
					raw_file_directory: Executor.RequestArgument<string, true>;
					tool_ffmpeg_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_file: Executor.RequestArgument<string, false>;
					tool_ww2ogg_pcb_file: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let tool_ffmpeg_file: string;
					let tool_ww2ogg_file: string;
					let tool_ww2ogg_pcb_file: string;
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
							() => (ripe_file_directory.replace(/$/i, '.wem_decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						tool_ffmpeg_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ffmpeg_file'),
							a.tool_ffmpeg_file,
							(value) => (value),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_file'),
							a.tool_ww2ogg_file,
							(value) => (value),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
						tool_ww2ogg_pcb_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'tool_ww2ogg_pcb_file'),
							a.tool_ww2ogg_pcb_file,
							(value) => (value),
							null,
							...Executor.argument_requester_for_path('file', [true]),
						);
					}
					simple_batch_execute(
						ripe_file_directory,
						['file', /.+(\.wem)$/i],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.wav`;
							CoreX.Tool.Wwise.EncodedMedia.decode_fs(ripe_file, raw_file, tool_ffmpeg_file, tool_ww2ogg_file, tool_ww2ogg_pcb_file, Main.path_at_home(config.temp_directpry));
						},
					);
					Console.notify('s', localized(`执行成功`), [`${raw_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file_directory: undefined!,
					raw_file_directory: '?default',
					tool_ffmpeg_file: Main.path_at_home(config.tool.ffmpeg),
					tool_ww2ogg_file: Main.path_at_home(config.tool.ww2ogg),
					tool_ww2ogg_pcb_file: Main.path_at_home(config.tool.ww2ogg_pcb),
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'ripe_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Entry.method.wwise.encoded_media._injector,
});