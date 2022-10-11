/**
 * + wwise.encoded_media.decode Wwise-Encoded-Media 解码
 * + wwise.encoded_media.decode.batch [批处理] Wwise-Encoded-Media 解码
 */
namespace TwinKleS.Entry.method.wwise.encoded_media {

	// ------------------------------------------------

	type Config = {
		tool: {
			ffmpeg: string;
			ww2ogg: string;
			ww2ogg_pcb: string;
		};
		temp_directpry: Argument.Require<string>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.encoded_media.decode',
				description: 'Wwise-Encoded-Media 解码',
				worker(a: Entry.CFSA & {
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
					tool_ffmpeg_file: Argument.Request<string, false>;
					tool_ww2ogg_file: Argument.Request<string, false>;
					tool_ww2ogg_pcb_file: Argument.Request<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let tool_ffmpeg_file: string;
					let tool_ww2ogg_file: string;
					let tool_ww2ogg_pcb_file: string;
					{
						ripe_file = Argument.require(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						raw_file = Argument.request(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/((\.wem))?$/i, '.wav')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						tool_ffmpeg_file = Argument.request(
							'FFMPEG程序文件', '',
							a.tool_ffmpeg_file,
							(value) => (value),
							null,
							...Argument.requester_for_path('file', [true]),
						);
						tool_ww2ogg_file = Argument.request(
							'WW2OGG程序文件', '',
							a.tool_ww2ogg_file,
							(value) => (value),
							null,
							...Argument.requester_for_path('file', [true]),
						);
						tool_ww2ogg_pcb_file = Argument.request(
							'WW2OGG-PCB文件', '',
							a.tool_ww2ogg_pcb_file,
							(value) => (value),
							null,
							...Argument.requester_for_path('file', [true]),
						);
					}
					CoreX.Tool.Wwise.EncodedMedia.decode_fs(ripe_file, raw_file, tool_ffmpeg_file, tool_ww2ogg_file, tool_ww2ogg_pcb_file, Main.path_at_home(config.temp_directpry));
					Console.notify('s', `执行成功`, [`${raw_file}`]);
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
				description: '[批处理] Wwise-Encoded-Media 解码',
				worker(a: Entry.CFSA & {
					ripe_file_directory: Argument.Require<string>;
					raw_file_directory: Argument.Request<string, true>;
					tool_ffmpeg_file: Argument.Request<string, false>;
					tool_ww2ogg_file: Argument.Request<string, false>;
					tool_ww2ogg_pcb_file: Argument.Request<string, false>;
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let tool_ffmpeg_file: string;
					let tool_ww2ogg_file: string;
					let tool_ww2ogg_pcb_file: string;
					{
						ripe_file_directory = Argument.require(
							'成品文件目录', '',
							a.ripe_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Argument.request(
							'原始文件目录', '',
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_file_directory.replace(/$/i, '.wem_decode')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						tool_ffmpeg_file = Argument.request(
							'FFMPEG程序文件', '',
							a.tool_ffmpeg_file,
							(value) => (value),
							null,
							...Argument.requester_for_path('file', [true]),
						);
						tool_ww2ogg_file = Argument.request(
							'WW2OGG程序文件', '',
							a.tool_ww2ogg_file,
							(value) => (value),
							null,
							...Argument.requester_for_path('file', [true]),
						);
						tool_ww2ogg_pcb_file = Argument.request(
							'WW2OGG-PCB文件', '',
							a.tool_ww2ogg_pcb_file,
							(value) => (value),
							null,
							...Argument.requester_for_path('file', [true]),
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
					Console.notify('s', `执行成功`, [`${raw_file_directory}`]);
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
	injector: TwinKleS.Entry.method.wwise.encoded_media._injector,
});