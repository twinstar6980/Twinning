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
		temp_directpry: string;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.encoded_media.decode',
				description: 'Wwise-Encoded-Media 解码',
				worker(a: Entry.CFSA & {
					ripe_file: string;
					raw_file: string | '?default' | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					{
						ripe_file = a.ripe_file;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_file.replace(/((\.wem))?$/i, '.wav'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					CoreX.Tool.Wwise.EncodedMedia.decode_fs(ripe_file, raw_file, Main.path_at_home(config.tool.ffmpeg), Main.path_at_home(config.tool.ww2ogg), Main.path_at_home(config.tool.ww2ogg_pcb), Main.path_at_home(config.temp_directpry));
					Output.i(`输出路径：${raw_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
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
					ripe_file_directory: string;
					raw_file_directory: string | '?default' | '?input';
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					{
						ripe_file_directory = a.ripe_file_directory;
						raw_file_directory = ArgumentParser.path(a.raw_file_directory, {
							input_message: '请输入输出路径',
							default_value: ripe_file_directory.replace(/$/i, '.wem_decode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					simple_batch_execute(
						ripe_file_directory,
						['file', /.+(\.wem)$/i],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.wav`;
							CoreX.Tool.Wwise.EncodedMedia.decode_fs(ripe_file, raw_file, Main.path_at_home(config.tool.ffmpeg), Main.path_at_home(config.tool.ww2ogg), Main.path_at_home(config.tool.ww2ogg_pcb), Main.path_at_home(config.temp_directpry));
						},
					);
					Output.i(`输出路径：${raw_file_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file_directory: undefined!,
					raw_file_directory: '?default',
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