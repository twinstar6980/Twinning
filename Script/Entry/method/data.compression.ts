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
	// lzma.compress
	// lzma.uncompress

	type Configuration = {
		uncompress_buffer_size: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.compression.deflate.compress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file: Executor.Argument<string, false>;
					ripe_file: Executor.Argument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'none');
					return [`${ripe_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.deflate.uncompress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'none', buffer_size);
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: configuration.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.zlib.compress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file: Executor.Argument<string, false>;
					ripe_file: Executor.Argument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'zlib');
					return [`${ripe_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.zlib.uncompress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'zlib', buffer_size);
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: configuration.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.gzip.compress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file: Executor.Argument<string, false>;
					ripe_file: Executor.Argument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'gzip');
					return [`${ripe_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.gzip.uncompress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'gzip', buffer_size);
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: configuration.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.bzip2.compress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file: Executor.Argument<string, false>;
					ripe_file: Executor.Argument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.BZip2.compress_fs(raw_file, ripe_file, 9n);
					return [`${ripe_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.bzip2.uncompress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.BZip2.uncompress_fs(ripe_file, raw_file, buffer_size);
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: configuration.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compression.lzma.compress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file: Executor.Argument<string, false>;
					ripe_file: Executor.Argument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Lzma.compress_fs(raw_file, ripe_file, 9n);
					return [`${ripe_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compression.lzma.uncompress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Compression.Lzma.uncompress_fs(ripe_file, raw_file, buffer_size);
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: configuration.uncompress_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.data.compression._injector,
});