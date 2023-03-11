namespace TwinStar.Script.Entry.method.data.compression {

	// ------------------------------------------------

	// deflate.compress
	// deflate.uncompress
	// zlib.compress
	// zlib.uncompress
	// gzip.compress
	// gzip.uncompress
	// bzip2.compress
	// bzip2.uncompress

	type Config = {
		uncompress_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.compression.deflate.compress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'none');
					Console.notify('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.deflate.uncompress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
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
					CoreX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'none', buffer_size);
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: config.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.zlib.compress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'zlib');
					Console.notify('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.zlib.uncompress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
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
					CoreX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'zlib', buffer_size);
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: config.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.gzip.compress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'gzip');
					Console.notify('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.gzip.uncompress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
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
					CoreX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'gzip', buffer_size);
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: config.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.bzip2.compress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compression.BZip2.compress_fs(raw_file, ripe_file, 9n);
					Console.notify('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.bzip2.uncompress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
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
					CoreX.Tool.Data.Compression.BZip2.uncompress_fs(ripe_file, raw_file, buffer_size);
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: config.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.data.compression._injector,
});