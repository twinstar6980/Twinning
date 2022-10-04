/**
 * + popcap.reanim.encode PopCap-Reanim 编码
 * + popcap.reanim.decode PopCap-Reanim 解码
 * + popcap.reanim.encode.batch [批处理] PopCap-Reanim 编码
 * + popcap.reanim.decode.batch [批处理] PopCap-Reanim 解码
 */
namespace TwinKleS.Entry.method.popcap.reanim {

	// ------------------------------------------------

	type Config = {
		encode_buffer_size: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.reanim.encode',
				description: 'PopCap-Reanim 编码',
				worker(a: Entry.CFSA & {
					manifest_file: string;
					data_file: string | '?default' | '?input';
					version_platform: 'desktop' | 'phone' | '?input';
					version_variant_64: boolean | '?input';
					data_buffer_size: string | '?input';
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_platform: 'desktop' | 'phone';
					let version_variant_64: boolean;
					let data_buffer_size: bigint;
					{
						manifest_file = a.manifest_file;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: manifest_file.replace(/((\.reanim)(\.json))?$/i, '.reanim.compiled'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_platform === '?input') {
							Console.notify('i', `请选择版本平台`, [`1. desktop 适用于桌面端（windows）`, `2. phone 适用于移动端（android、ios）`]);
							version_platform = (['desktop', 'phone'] as const)[Number(Console.integer(Check.enum_checkerx([1n, 2n]))! - 1n)];
						} else {
							version_platform = a.version_platform;
						}
						if (a.version_variant_64 === '?input') {
							Console.notify('i', `平台是否为64位？（仅适用于移动端）`, []);
							version_variant_64 = Console.yon()!;
						} else {
							version_variant_64 = a.version_variant_64;
						}
						if (a.data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存Reanim数据的内存空间大小`, []);
							data_buffer_size = Console.size()!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					CoreX.Tool.PopCap.Reanim.encode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 }, data_buffer_size);
					Console.notify('s', `输出路径：${data_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
					data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.reanim)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.reanim.decode',
				description: 'PopCap-Reanim 解码',
				worker(a: Entry.CFSA & {
					data_file: string;
					manifest_file: string | '?default' | '?input';
					version_platform: 'desktop' | 'phone' | '?input';
					version_variant_64: boolean | '?input';
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_platform: 'desktop' | 'phone';
					let version_variant_64: boolean;
					{
						data_file = a.data_file;
						manifest_file = ArgumentParser.path(a.manifest_file, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.reanim)(\.compiled))?$/i, '.reanim.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_platform === '?input') {
							Console.notify('i', `请选择版本平台`, [`1. desktop 适用于桌面端（windows）`, `2. phone 适用于移动端（android、ios）`]);
							version_platform = (['desktop', 'phone'] as const)[Number(Console.integer(Check.enum_checkerx([1n, 2n]))! - 1n)];
						} else {
							version_platform = a.version_platform;
						}
						if (a.version_variant_64 === '?input') {
							Console.notify('i', `平台是否为64位？（仅适用于移动端）`, []);
							version_variant_64 = Console.yon()!;
						} else {
							version_variant_64 = a.version_variant_64;
						}
					}
					CoreX.Tool.PopCap.Reanim.decode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 });
					Console.notify('s', `输出路径：${manifest_file}`, []);
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
					manifest_file_directory: string;
					data_file_directory: string | '?default' | '?input';
					version_platform: 'desktop' | 'phone' | '?input';
					version_variant_64: boolean | '?input';
					data_buffer_size: string | '?input';
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_platform: 'desktop' | 'phone';
					let version_variant_64: boolean;
					let data_buffer_size: bigint;
					{
						manifest_file_directory = a.manifest_file_directory;
						data_file_directory = ArgumentParser.path(a.data_file_directory, {
							input_message: '请输入输出路径',
							default_value: manifest_file_directory.replace(/$/i, '.reanim_encode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_platform === '?input') {
							Console.notify('i', `请选择版本平台`, [`1. desktop 适用于桌面端（windows）`, `2. phone 适用于移动端（android、ios）`]);
							version_platform = (['desktop', 'phone'] as const)[Number(Console.integer(Check.enum_checkerx([1n, 2n]))! - 1n)];
						} else {
							version_platform = a.version_platform;
						}
						if (a.version_variant_64 === '?input') {
							Console.notify('i', `平台是否为64位？（仅适用于移动端）`, []);
							version_variant_64 = Console.yon()!;
						} else {
							version_variant_64 = a.version_variant_64;
						}
						if (a.data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存Reanim数据的内存空间大小`, []);
							data_buffer_size = Console.size()!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(data_buffer_size));
					simple_batch_execute(
						manifest_file_directory,
						['file', /.+(\.reanim)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -9)}.compiled`;
							CoreX.Tool.PopCap.Reanim.encode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 }, data_buffer.view());
						},
					);
					Console.notify('s', `输出路径：${data_file_directory}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
					data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.reanim.decode.batch',
				description: '[批处理] PopCap-Reanim 解码',
				worker(a: Entry.CFSA & {
					data_file_directory: string;
					manifest_file_directory: string | '?default' | '?input';
					version_platform: 'desktop' | 'phone' | '?input';
					version_variant_64: boolean | '?input';
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_platform: 'desktop' | 'phone';
					let version_variant_64: boolean;
					{
						data_file_directory = a.data_file_directory;
						manifest_file_directory = ArgumentParser.path(a.manifest_file_directory, {
							input_message: '请输入输出路径',
							default_value: data_file_directory.replace(/$/i, '.reanim_decode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_platform === '?input') {
							Console.notify('i', `请选择版本平台`, [`1. desktop 适用于桌面端（windows）`, `2. phone 适用于移动端（android、ios）`]);
							version_platform = (['desktop', 'phone'] as const)[Number(Console.integer(Check.enum_checkerx([1n, 2n]))! - 1n)];
						} else {
							version_platform = a.version_platform;
						}
						if (a.version_variant_64 === '?input') {
							Console.notify('i', `平台是否为64位？（仅适用于移动端）`, []);
							version_variant_64 = Console.yon()!;
						} else {
							version_variant_64 = a.version_variant_64;
						}
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
					Console.notify('s', `输出路径：${manifest_file_directory}`, []);
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