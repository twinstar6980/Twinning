/**
 * + popcap.reanim.encode PopCap-Reanim 编码
 * + popcap.reanim.decode PopCap-Reanim 解码
 * + popcap.reanim.encode.batch [批处理] PopCap-Reanim 编码
 * + popcap.reanim.decode.batch [批处理] PopCap-Reanim 解码
 */
namespace TwinKleS.Entry.method.popcap.reanim {

	// ------------------------------------------------

	type Config = {
		encode_buffer_size: Argument.Request<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.reanim.encode',
				description: 'PopCap-Reanim 编码',
				worker(a: Entry.CFSA & {
					manifest_file: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					version_platform: Argument.Request<string, false>;
					version_variant_64: boolean | '?input';
					buffer_size: Argument.Request<string, false>;
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					let buffer_size: bigint;
					{
						manifest_file = Argument.require(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Argument.request(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.reanim)(\.json))?$/i, '.reanim.compiled')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Argument.request(
							'版本平台', '',
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option([[`desktop 适用于桌面端（windows）`], [`phone 适用于移动端（android、ios）`]], null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : `版本不受支持`),
						);
						version_variant_64 = Argument.request(
							'使用64位变体', '',
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
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
					CoreX.Tool.PopCap.Reanim.encode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 }, buffer_size);
					Console.notify('s', `执行成功`, [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.reanim)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.reanim.decode',
				description: 'PopCap-Reanim 解码',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					manifest_file: Argument.Request<string, true>;
					version_platform: Argument.Request<string, false>;
					version_variant_64: boolean | '?input';
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					{
						data_file = Argument.require(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						manifest_file = Argument.request(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							() => (data_file.replace(/((\.reanim)(\.compiled))?$/i, '.reanim.json')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Argument.request(
							'版本平台', '',
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option([[`desktop 适用于桌面端（windows）`], [`phone 适用于移动端（android、ios）`]], null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : `版本不受支持`),
						);
						version_variant_64 = Argument.request(
							'使用64位变体', '',
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.Reanim.decode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 });
					Console.notify('s', `执行成功`, [`${manifest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					manifest_file: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.reanim)(\.compiled)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.reanim.encode.batch',
				description: '[批处理] PopCap-Reanim 编码',
				worker(a: Entry.CFSA & {
					manifest_file_directory: Argument.Require<string>;
					data_file_directory: Argument.Request<string, true>;
					version_platform: Argument.Request<string, false>;
					version_variant_64: boolean | '?input';
					buffer_size: Argument.Request<string, false>;
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					let buffer_size: bigint;
					{
						manifest_file_directory = Argument.require(
							'清单文件目录', '',
							a.manifest_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Argument.request(
							'数据文件目录', '',
							a.data_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.reanim_encode')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Argument.request(
							'版本平台', '',
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option([[`desktop 适用于桌面端（windows）`], [`phone 适用于移动端（android、ios）`]], null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : `版本不受支持`),
						);
						version_variant_64 = Argument.request(
							'使用64位变体', '',
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
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
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						manifest_file_directory,
						['file', /.+(\.reanim)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -9)}.compiled`;
							CoreX.Tool.PopCap.Reanim.encode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 }, data_buffer.view());
						},
					);
					Console.notify('s', `执行成功`, [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.reanim.decode.batch',
				description: '[批处理] PopCap-Reanim 解码',
				worker(a: Entry.CFSA & {
					data_file_directory: Argument.Require<string>;
					manifest_file_directory: Argument.Request<string, true>;
					version_platform: Argument.Request<string, false>;
					version_variant_64: boolean | '?input';
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					{
						data_file_directory = Argument.require(
							'数据文件目录', '',
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file_directory = Argument.request(
							'清单文件目录', '',
							a.manifest_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.reanim_decode')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Argument.request(
							'版本平台', '',
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option([[`desktop 适用于桌面端（windows）`], [`phone 适用于移动端（android、ios）`]], null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : `版本不受支持`),
						);
						version_variant_64 = Argument.request(
							'使用64位变体', '',
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.reanim)(\.compiled)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let manifest_file = `${manifest_file_directory}/${item.slice(0, -9)}.json`;
							CoreX.Tool.PopCap.Reanim.decode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 });
						},
					);
					Console.notify('s', `执行成功`, [`${manifest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					manifest_file_directory: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.reanim._injector,
});