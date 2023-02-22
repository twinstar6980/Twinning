/**
 * + wwise.sound_bank.encode Wwise-Sound-Bank 编码
 * + wwise.sound_bank.decode Wwise-Sound-Bank 解码
 * + wwise.sound_bank.encode.batch [批处理] Wwise-Sound-Bank 编码
 * + wwise.sound_bank.decode.batch [批处理] Wwise-Sound-Bank 解码
 */
namespace TwinStar.Script.Entry.method.wwise.sound_bank {

	// ------------------------------------------------

	type Config = {
		version_number: Executor.RequestArgument<bigint, false>;
		encode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.sound_bank.encode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					bundle_directory: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: [88n, 112n, 140n][number];
					let buffer_size: bigint;
					{
						bundle_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.bnk)(\.bundle))?$/i, '.bnk')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([88n, [88n, ''], 112n, [112n, ''], 140n, [140n, '']], null)),
							(value) => ([88n, 112n, 140n].includes(value) ? null : los(`版本不受支持`)),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
					CoreX.Tool.Wwise.SoundBank.encode_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number }, buffer_size);
					Console.notify('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: config.version_number,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.bnk)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'wwise.sound_bank.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					bundle_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: [88n, 112n, 140n][number];
					{
						data_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.bnk))?$/i, '.bnk.bundle')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([88n, [88n, ''], 112n, [112n, ''], 140n, [140n, '']], null)),
							(value) => ([88n, 112n, 140n].includes(value) ? null : los(`版本不受支持`)),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
					CoreX.Tool.Wwise.SoundBank.decode_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number });
					Console.notify('s', los(`执行成功`), [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.bnk)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'wwise.sound_bank.encode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					bundle_directory_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let bundle_directory_directory: string;
					let data_file_directory: string;
					let version_number: [88n, 112n, 140n][number];
					let buffer_size: bigint;
					{
						bundle_directory_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'bundle_directory_directory'),
							a.bundle_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (bundle_directory_directory.replace(/$/i, '.bnk_encode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([88n, [88n, ''], 112n, [112n, ''], 140n, [140n, '']], null)),
							(value) => ([88n, 112n, 140n].includes(value) ? null : los(`版本不受支持`)),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						bundle_directory_directory,
						['directory', /.+(\.bnk)(\.bundle)$/i],
						(item) => {
							let bundle_directory = `${bundle_directory_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -7)}`;
							let manifest_file = `${bundle_directory}/manifest.json`;
							let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
							CoreX.Tool.Wwise.SoundBank.encode_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number }, data_buffer.view());
						},
					);
					Console.notify('s', los(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory_directory: undefined!,
					data_file_directory: '?default',
					version_number: config.version_number,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'bundle_directory_directory',
			}),
			Executor.method_of({
				id: 'wwise.sound_bank.decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					bundle_directory_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file_directory: string;
					let bundle_directory_directory: string;
					let version_number: [88n, 112n, 140n][number];
					{
						data_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						bundle_directory_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'bundle_directory_directory'),
							a.bundle_directory_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.bnk_decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([88n, [88n, ''], 112n, [112n, ''], 140n, [140n, '']], null)),
							(value) => ([88n, 112n, 140n].includes(value) ? null : los(`版本不受支持`)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.bnk)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let bundle_directory = `${bundle_directory_directory}/${item}.bundle`;
							let manifest_file = `${bundle_directory}/manifest.json`;
							let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
							CoreX.Tool.Wwise.SoundBank.decode_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number });
						},
					);
					Console.notify('s', los(`执行成功`), [`${bundle_directory_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					bundle_directory_directory: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.wwise.sound_bank._injector,
});