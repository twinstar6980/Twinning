/**
 * + popcap.zlib.compress PopCap-ZLib 压缩
 * + popcap.zlib.uncompress PopCap-ZLib 解压
 * + popcap.zlib.compress.batch [批处理] PopCap-ZLib 压缩
 * + popcap.zlib.uncompress.batch [批处理] PopCap-ZLib 解压
 */
namespace TwinKleS.Entry.method.popcap.zlib {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.zlib.compress',
				description: 'PopCap-ZLib 压缩',
				worker(a: Entry.CFSA & {
					raw_file: Argument.Require<string>;
					ripe_file: Argument.Request<string, true>;
					variant_64: boolean | '?input';
				}) {
					let raw_file: string;
					let ripe_file: string;
					let variant_64: boolean;
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
						variant_64 = Argument.request(
							'使用64位变体', '',
							a.variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', variant_64);
					Console.notify('s', `执行成功`, [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
					variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'popcap.zlib.uncompress',
				description: 'PopCap-ZLib 解压',
				worker(a: Entry.CFSA & {
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
					variant_64: boolean | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					let variant_64: boolean;
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
						variant_64 = Argument.request(
							'使用64位变体', '',
							a.variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, variant_64);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.zlib.compress.batch',
				description: '[批处理] PopCap-ZLib 压缩',
				worker(a: Entry.CFSA & {
					raw_file_directory: Argument.Require<string>;
					ripe_file_directory: Argument.Request<string, true>;
					variant_64: boolean | '?input';
				}) {
					let raw_file_directory: string;
					let ripe_file_directory: string;
					let variant_64: boolean;
					{
						raw_file_directory = Argument.require(
							'原始文件目录', '',
							a.raw_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						ripe_file_directory = Argument.request(
							'成品文件目录', '',
							a.ripe_file_directory,
							(value) => (value),
							() => (raw_file_directory.replace(/$/i, '.popcap_zlib_compress')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						variant_64 = Argument.request(
							'使用64位变体', '',
							a.variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						raw_file_directory,
						['file', null],
						(item) => {
							let raw_file = `${raw_file_directory}/${item}`;
							let ripe_file = `${ripe_file_directory}/${item}`;
							CoreX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', variant_64);
						},
					);
					Console.notify('s', `执行成功`, [`${ripe_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file_directory: undefined!,
					ripe_file_directory: '?default',
					variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'raw_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.zlib.uncompress.batch',
				description: '[批处理] PopCap-ZLib 解压',
				worker(a: Entry.CFSA & {
					ripe_file_directory: Argument.Require<string>;
					raw_file_directory: Argument.Request<string, true>;
					variant_64: boolean | '?input';
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let variant_64: boolean;
					{
						ripe_file_directory = Argument.require(
							'成品文件目录', '',
							a.ripe_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Argument.request(
							'原始文件目录', '',
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_file_directory.replace(/$/i, '.popcap_zlib_uncompress')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						variant_64 = Argument.request(
							'使用64位变体', '',
							a.variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						ripe_file_directory,
						['file', null],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item}`;
							CoreX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, variant_64);
						},
					);
					Console.notify('s', `执行成功`, [`${raw_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file_directory: undefined!,
					raw_file_directory: '?default',
					variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'ripe_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.zlib._injector,
});