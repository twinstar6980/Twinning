/**
 * + data.compress.deflate.compress Deflate 压缩
 * + data.compress.deflate.uncompress Deflate 解压
 * + data.compress.zlib.compress ZLib 压缩
 * + data.compress.zlib.uncompress ZLib 解压
 * + data.compress.gzip.compress GZip 压缩
 * + data.compress.gzip.uncompress GZip 解压
 * + data.compress.bzip2.compress BZip2 压缩
 * + data.compress.bzip2.uncompress BZip2 解压
 */
namespace TwinKleS.Entry.method.data.compress {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.compress.deflate.compress',
				description: 'Deflate 压缩',
				worker(a: Entry.CFSA & {
					raw_file: Argument.Require<string>;
					ripe_file: Argument.Request<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Argument.require(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Argument.request(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compress.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'none');
					Console.notify('s', `执行成功`, [`${ripe_file}`]);
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
				id: 'data.compress.deflate.uncompress',
				description: 'Deflate 解压',
				worker(a: Entry.CFSA & {
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.Data.Compress.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'none', buffer_size);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compress.zlib.compress',
				description: 'ZLib 压缩',
				worker(a: Entry.CFSA & {
					raw_file: Argument.Require<string>;
					ripe_file: Argument.Request<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Argument.require(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Argument.request(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compress.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'zlib');
					Console.notify('s', `执行成功`, [`${ripe_file}`]);
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
				id: 'data.compress.zlib.uncompress',
				description: 'ZLib 解压',
				worker(a: Entry.CFSA & {
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.Data.Compress.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'zlib', buffer_size);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compress.gzip.compress',
				description: 'GZip 压缩',
				worker(a: Entry.CFSA & {
					raw_file: Argument.Require<string>;
					ripe_file: Argument.Request<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Argument.require(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Argument.request(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compress.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'gzip');
					Console.notify('s', `执行成功`, [`${ripe_file}`]);
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
				id: 'data.compress.gzip.uncompress',
				description: 'GZip 解压',
				worker(a: Entry.CFSA & {
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.Data.Compress.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'gzip', buffer_size);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compress.bzip2.compress',
				description: 'BZip2 压缩',
				worker(a: Entry.CFSA & {
					raw_file: Argument.Require<string>;
					ripe_file: Argument.Request<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Argument.require(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Argument.request(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Compress.BZip2.compress_fs(raw_file, ripe_file, 9n);
					Console.notify('s', `执行成功`, [`${ripe_file}`]);
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
				id: 'data.compress.bzip2.uncompress',
				description: 'BZip2 解压',
				worker(a: Entry.CFSA & {
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let buffer_size: bigint;
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
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.Data.Compress.BZip2.uncompress_fs(ripe_file, raw_file, buffer_size);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.data.compress._injector,
});