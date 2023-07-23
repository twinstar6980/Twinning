namespace TwinStar.Script.Entry.method.popcap.resource_stream_group {

	// ------------------------------------------------

	// pack
	// unpack

	type Configuration = {
		version_number: Executor.Argument<bigint, false>;
		pack_buffer_size: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.resource_stream_group.pack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					bundle_directory: Executor.Argument<string, false>;
					data_file: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.rsg)(\.bundle))?$/i, '.rsg')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamGroup.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.PopCap.ResourceStreamGroup.pack_fs(data_file, definition_file, resource_directory, { number: version_number as any }, buffer_size);
					return [`${data_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: configuration.version_number,
					buffer_size: configuration.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsg)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_group.unpack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.Argument<string, false>;
					bundle_directory: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: bigint;
					{
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.rsg))?$/i, '.rsg.bundle')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.ResourceStreamGroup.VersionNumberE), null, null, initial)),
						);
					}
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.PopCap.ResourceStreamGroup.unpack_fs(data_file, definition_file, resource_directory, { number: version_number as any });
					return [`${bundle_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: configuration.version_number,
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