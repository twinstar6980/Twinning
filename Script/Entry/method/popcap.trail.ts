namespace TwinStar.Script.Entry.method.popcap.trail {

	// ------------------------------------------------

	// encode *
	// decode *

	type Configuration = {
		version_platform: Executor.RequestArgument<string, false>;
		version_variant_64: Executor.RequestArgument<boolean, false>;
		encode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.trail.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					manifest_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_platform: string;
					let version_variant_64: boolean;
					let buffer_size: bigint;
					{
						manifest_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.trail)(\.json))?$/i, '.trail.compiled')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_platform = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(CoreX.Tool.PopCap.Trail.VersionPlatformE), null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.Trail.encode_fs(data_file, manifest_file, { platform: version_platform as any, variant_64: version_variant_64 }, buffer_size);
					Console.success(los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					manifest_file: undefined!,
					data_file: '?default',
					version_platform: configuration.version_platform,
					version_variant_64: configuration.version_variant_64,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.trail)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.trail.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.RequireArgument<string>;
					manifest_file: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_platform: string;
					let version_variant_64: boolean;
					{
						data_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						manifest_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							() => (data_file.replace(/((\.trail)(\.compiled))?$/i, '.trail.json')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_platform = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(CoreX.Tool.PopCap.Trail.VersionPlatformE), null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.Trail.decode_fs(data_file, manifest_file, { platform: version_platform as any, variant_64: version_variant_64 });
					Console.success(los(`执行成功`), [`${manifest_file}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					data_file: undefined!,
					manifest_file: '?default',
					version_platform: configuration.version_platform,
					version_variant_64: configuration.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.trail)(\.compiled)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.trail.encode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					manifest_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_platform: string;
					let version_variant_64: boolean;
					let buffer_size: bigint;
					{
						manifest_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.encode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_platform = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(CoreX.Tool.PopCap.Trail.VersionPlatformE), null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
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
						manifest_file_directory,
						['file', /.+(\.trail)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -5)}.compiled`;
							CoreX.Tool.PopCap.Trail.encode_fs(data_file, manifest_file, { platform: version_platform as any, variant_64: version_variant_64 }, data_buffer.view());
						},
					);
					Console.success(los(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_platform: configuration.version_platform,
					version_variant_64: configuration.version_variant_64,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.trail.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file_directory: Executor.RequireArgument<string>;
					manifest_file_directory: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_platform: string;
					let version_variant_64: boolean;
					{
						data_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_platform = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(CoreX.Tool.PopCap.Trail.VersionPlatformE), null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.trail)(\.compiled)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let manifest_file = `${manifest_file_directory}/${item.slice(0, -9)}.json`;
							CoreX.Tool.PopCap.Trail.decode_fs(data_file, manifest_file, { platform: version_platform as any, variant_64: version_variant_64 });
						},
					);
					Console.success(los(`执行成功`), [`${manifest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_common_argument,
					data_file_directory: undefined!,
					manifest_file_directory: '?default',
					version_platform: configuration.version_platform,
					version_variant_64: configuration.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.trail._injector,
});