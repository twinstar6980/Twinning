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
					raw_file: string;
					ripe_file: string | '?default' | '?input';
					level: CoreX.Tool.Data.Compress.Deflate.CompressionLevel;
				}) {
					let raw_file: string;
					let ripe_file: string;
					let level: CoreX.Tool.Data.Compress.Deflate.CompressionLevel;
					{
						raw_file = a.raw_file;
						ripe_file = ArgumentParser.path(a.ripe_file, {
							input_message: '请输入输出路径',
							default_value: raw_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						level = a.level;
					}
					CoreX.Tool.Data.Compress.Deflate.compress_fs(raw_file, ripe_file, level, 15n, 9n, 'default_mode', 'none');
					Console.notify('s', `输出路径：${ripe_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
					level: 9n,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compress.deflate.uncompress',
				description: 'Deflate 解压',
				worker(a: Entry.CFSA & {
					ripe_file: string;
					raw_file: string | '?default' | '?input';
					raw_data_buffer_size: string | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					let raw_data_buffer_size: bigint;
					{
						ripe_file = a.ripe_file;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.raw_data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存原始数据的内存空间大小`, []);
							raw_data_buffer_size = Console.size()!;
						} else {
							raw_data_buffer_size = parse_size_string(a.raw_data_buffer_size);
						}
					}
					CoreX.Tool.Data.Compress.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'none', raw_data_buffer_size);
					Console.notify('s', `输出路径：${raw_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					raw_data_buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compress.zlib.compress',
				description: 'ZLib 压缩',
				worker(a: Entry.CFSA & {
					raw_file: string;
					ripe_file: string | '?default' | '?input';
					level: CoreX.Tool.Data.Compress.Deflate.CompressionLevel;
				}) {
					let raw_file: string;
					let ripe_file: string;
					let level: CoreX.Tool.Data.Compress.Deflate.CompressionLevel;
					{
						raw_file = a.raw_file;
						ripe_file = ArgumentParser.path(a.ripe_file, {
							input_message: '请输入输出路径',
							default_value: raw_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						level = a.level;
					}
					CoreX.Tool.Data.Compress.Deflate.compress_fs(raw_file, ripe_file, level, 15n, 9n, 'default_mode', 'zlib');
					Console.notify('s', `输出路径：${ripe_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
					level: 9n,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compress.zlib.uncompress',
				description: 'ZLib 解压',
				worker(a: Entry.CFSA & {
					ripe_file: string;
					raw_file: string | '?default' | '?input';
					raw_data_buffer_size: string | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					let raw_data_buffer_size: bigint;
					{
						ripe_file = a.ripe_file;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.raw_data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存原始数据的内存空间大小`, []);
							raw_data_buffer_size = Console.size()!;
						} else {
							raw_data_buffer_size = parse_size_string(a.raw_data_buffer_size);
						}
					}
					CoreX.Tool.Data.Compress.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'zlib', raw_data_buffer_size);
					Console.notify('s', `输出路径：${raw_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					raw_data_buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compress.gzip.compress',
				description: 'GZip 压缩',
				worker(a: Entry.CFSA & {
					raw_file: string;
					ripe_file: string | '?default' | '?input';
					level: CoreX.Tool.Data.Compress.Deflate.CompressionLevel;
				}) {
					let raw_file: string;
					let ripe_file: string;
					let level: CoreX.Tool.Data.Compress.Deflate.CompressionLevel;
					{
						raw_file = a.raw_file;
						ripe_file = ArgumentParser.path(a.ripe_file, {
							input_message: '请输入输出路径',
							default_value: raw_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						level = a.level;
					}
					CoreX.Tool.Data.Compress.Deflate.compress_fs(raw_file, ripe_file, level, 15n, 9n, 'default_mode', 'gzip');
					Console.notify('s', `输出路径：${ripe_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
					level: 9n,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.compress.gzip.uncompress',
				description: 'GZip 解压',
				worker(a: Entry.CFSA & {
					ripe_file: string;
					raw_file: string | '?default' | '?input';
					raw_data_buffer_size: string | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					let raw_data_buffer_size: bigint;
					{
						ripe_file = a.ripe_file;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.raw_data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存原始数据的内存空间大小`, []);
							raw_data_buffer_size = Console.size()!;
						} else {
							raw_data_buffer_size = parse_size_string(a.raw_data_buffer_size);
						}
					}
					CoreX.Tool.Data.Compress.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'gzip', raw_data_buffer_size);
					Console.notify('s', `输出路径：${raw_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					raw_data_buffer_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
			Executor.method_of({
				id: 'data.compress.bzip2.compress',
				description: 'BZip2 压缩',
				worker(a: Entry.CFSA & {
					raw_file: string;
					ripe_file: string | '?default' | '?input';
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = a.raw_file;
						ripe_file = ArgumentParser.path(a.ripe_file, {
							input_message: '请输入输出路径',
							default_value: raw_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					CoreX.Tool.Data.Compress.BZip2.compress_fs(raw_file, ripe_file, 9n);
					Console.notify('s', `输出路径：${ripe_file}`, []);
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
					ripe_file: string;
					raw_file: string | '?default' | '?input';
					raw_data_buffer_size: string | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					let raw_data_buffer_size: bigint;
					{
						ripe_file = a.ripe_file;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.raw_data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存原始数据的内存空间大小`, []);
							raw_data_buffer_size = Console.size()!;
						} else {
							raw_data_buffer_size = parse_size_string(a.raw_data_buffer_size);
						}
					}
					CoreX.Tool.Data.Compress.BZip2.uncompress_fs(ripe_file, raw_file, raw_data_buffer_size);
					Console.notify('s', `输出路径：${raw_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					raw_data_buffer_size: '?input',
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