namespace TwinStar.Script.Entry.method.wwise.sound_bank {

	// ------------------------------------------------

	// encode *
	// decode *

	type Configuration = {
		version_number: Executor.RequestArgument<bigint, false>;
		encode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.sound_bank.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					bundle_directory: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						bundle_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.bnk)(\.bundle))?$/i, '.bnk')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.Wwise.SoundBank.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let embedded_media_directory = `${bundle_directory}/embedded_media`;
					CoreX.Tool.Wwise.SoundBank.encode_fs(data_file, manifest_file, embedded_media_directory, { number: version_number as any }, buffer_size);
					Console.success(los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: configuration.version_number,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.bnk)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'wwise.sound_bank.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.RequireArgument<string>;
					bundle_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: bigint;
					{
						data_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.bnk))?$/i, '.bnk.bundle')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.Wwise.SoundBank.VersionNumberE), null, null, initial)),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let embedded_media_directory = `${bundle_directory}/embedded_media`;
					CoreX.Tool.Wwise.SoundBank.decode_fs(data_file, manifest_file, embedded_media_directory, { number: version_number as any });
					Console.success(los(`执行成功`), [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: configuration.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.bnk)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'wwise.sound_bank.encode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					bundle_directory_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let bundle_directory_directory: string;
					let data_file_directory: string;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						bundle_directory_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'bundle_directory_directory'),
							a.bundle_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (bundle_directory_directory.replace(/$/i, '.encode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.Wwise.SoundBank.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
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
							let embedded_media_directory = `${bundle_directory}/embedded_media`;
							CoreX.Tool.Wwise.SoundBank.encode_fs(data_file, manifest_file, embedded_media_directory, { number: version_number as any }, data_buffer.view());
						},
					);
					Console.success(los(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					bundle_directory_directory: undefined!,
					data_file_directory: '?default',
					version_number: configuration.version_number,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'bundle_directory_directory',
			}),
			Executor.method_of({
				id: 'wwise.sound_bank.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file_directory: Executor.RequireArgument<string>;
					bundle_directory_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file_directory: string;
					let bundle_directory_directory: string;
					let version_number: bigint;
					{
						data_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						bundle_directory_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory_directory'),
							a.bundle_directory_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.Wwise.SoundBank.VersionNumberE), null, null, initial)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.bnk)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let bundle_directory = `${bundle_directory_directory}/${item}.bundle`;
							let manifest_file = `${bundle_directory}/manifest.json`;
							let embedded_media_directory = `${bundle_directory}/embedded_media`;
							CoreX.Tool.Wwise.SoundBank.decode_fs(data_file, manifest_file, embedded_media_directory, { number: version_number as any });
						},
					);
					Console.success(los(`执行成功`), [`${bundle_directory_directory}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					data_file_directory: undefined!,
					bundle_directory_directory: '?default',
					version_number: configuration.version_number,
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