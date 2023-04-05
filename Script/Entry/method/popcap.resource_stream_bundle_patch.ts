namespace TwinStar.Script.Entry.method.popcap.resource_stream_bundle_patch {

	// ------------------------------------------------

	// encode
	// decode

	type Config = {
		use_raw_packet: Executor.RequestArgument<boolean, false>;
		version_number: Executor.RequestArgument<bigint, false>;
		encode_buffer_size: Executor.RequestArgument<string, false>;
		decode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.resource_stream_bundle_patch.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					before_file: Executor.RequireArgument<string>;
					after_file: Executor.RequestArgument<string, false>;
					patch_file: Executor.RequestArgument<string, true>;
					use_raw_packet: Executor.RequestArgument<boolean, false>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let before_file: string;
					let after_file: string;
					let patch_file: string;
					let use_raw_packet: boolean;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						before_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'before_file'),
							a.before_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						after_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'after_file'),
							a.after_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', [true], null, null, initial)),
						);
						patch_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'patch_file'),
							a.patch_file,
							(value) => (value),
							() => (after_file.replace(/((\.rsb))?$/i, '.rsbpatch')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						let x = true as boolean;
						use_raw_packet = Executor.request_argument(
							Executor.query_argument_name(this.id, 'use_raw_packet'),
							a.use_raw_packet,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundlePatch.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.ResourceStreamBundlePatch.encode_fs(before_file, after_file, patch_file, use_raw_packet, { number: version_number as any }, buffer_size);
					Console.message('s', los(`执行成功`), [`${patch_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					before_file: undefined!,
					after_file: '?input',
					patch_file: '?default',
					use_raw_packet: config.use_raw_packet,
					version_number: config.version_number,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'before_file',
			}),
			Executor.method_of({
				id: 'popcap.resource_stream_bundle_patch.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					before_file: Executor.RequireArgument<string>;
					patch_file: Executor.RequestArgument<string, false>;
					after_file: Executor.RequestArgument<string, true>;
					use_raw_packet: Executor.RequestArgument<boolean, false>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let before_file: string;
					let patch_file: string;
					let after_file: string;
					let use_raw_packet: boolean;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						before_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'before_file'),
							a.before_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						patch_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'patch_file'),
							a.patch_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', [true], null, null, initial)),
						);
						after_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'after_file'),
							a.after_file,
							(value) => (value),
							() => (patch_file.replace(/((\.rsbpatch))?$/i, '.rsb')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						use_raw_packet = Executor.request_argument(
							Executor.query_argument_name(this.id, 'use_raw_packet'),
							a.use_raw_packet,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.ResourceStreamBundlePatch.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.ResourceStreamBundlePatch.decode_fs(before_file, after_file, patch_file, use_raw_packet, { number: version_number as any }, buffer_size);
					Console.message('s', los(`执行成功`), [`${after_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					before_file: undefined!,
					patch_file: '?input',
					after_file: '?default',
					use_raw_packet: config.use_raw_packet,
					version_number: config.version_number,
					buffer_size: config.decode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsb)$/i]]),
				input_forwarder: 'before_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.resource_stream_bundle_patch._injector,
});