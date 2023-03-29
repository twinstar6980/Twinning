namespace TwinStar.Script.Entry.method.popcap.resource_stream_group {

	// ------------------------------------------------

	// pack
	// unpack

	type Config = {
		version_number: Executor.RequestArgument<bigint, false>;
		pack_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.resource_stream_group.pack',
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
					let version_number: bigint;
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
							() => (bundle_directory.replace(/((\.rsg)(\.bundle))?$/i, '.rsg')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([0n, null, null, null, [3n, ''], [4n, '']], null)),
							(value) => (CoreX.Tool.PopCap.ResourceStreamGroup.VersionNumberE.includes(value as any) ? null : los(`版本不受支持`)),
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
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.ResourceStreamGroup.pack_fs(data_file, manifest_file, resource_directory, { number: version_number as any }, buffer_size);
					Console.notify('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: config.version_number,
					buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsg)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_group.unpack',
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
					let version_number: bigint;
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
							() => (data_file.replace(/((\.rsg))?$/i, '.rsg.bundle')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([0n, null, null, null, [3n, ''], [4n, '']], null)),
							(value) => (CoreX.Tool.PopCap.ResourceStreamGroup.VersionNumberE.includes(value as any) ? null : los(`版本不受支持`)),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.ResourceStreamGroup.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number as any });
					Console.notify('s', los(`执行成功`), [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsg)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.resource_stream_group._injector,
});