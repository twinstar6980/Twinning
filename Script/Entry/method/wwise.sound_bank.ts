/**
 * + wwise.sound_bank.pack Wwise-Sound-Bank 打包
 * + wwise.sound_bank.unpack Wwise-Sound-Bank 解包
 * + wwise.sound_bank.pack.batch [批处理] Wwise-Sound-Bank 打包
 * + wwise.sound_bank.unpack.batch [批处理] Wwise-Sound-Bank 解包
 */
namespace TwinKleS.Entry.method.wwise.sound_bank {

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'wwise.sound_bank.pack',
				description: 'Wwise-Sound-Bank 打包',
				worker(a: Entry.CFSA & {
					bundle_directory: string;
					data_file: string | '?default' | '?input';
					version_number: bigint | '?input';
					data_buffer_size: string | '?input';
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: bigint;
					let data_buffer_size: bigint;
					{
						bundle_directory = a.bundle_directory;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: bundle_directory.replace(/((\.bnk)(\.bundle))?$/i, '.bnk'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（112）`, []);
							version_number = Console.integer(Check.enum_checkerx([112n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存包数据输出的内存空间大小`, []);
							data_buffer_size = Console.size()!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
					CoreX.Tool.Wwise.SoundBank.pack_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number as any }, data_buffer_size);
					Console.notify('s', `输出路径：${data_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
					data_buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.bnk)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'wwise.sound_bank.unpack',
				description: 'Wwise-Sound-Bank 解包',
				worker(a: Entry.CFSA & {
					data_file: string;
					bundle_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: bigint;
					{
						data_file = a.data_file;
						bundle_directory = ArgumentParser.path(a.bundle_directory, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.bnk))?$/i, '.bnk.bundle'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（112）`, []);
							version_number = Console.integer(Check.enum_checkerx([112n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
					CoreX.Tool.Wwise.SoundBank.unpack_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number as any });
					Console.notify('s', `输出路径：${bundle_directory}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.bnk)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'wwise.sound_bank.pack.batch',
				description: '[批处理] Wwise-Sound-Bank 打包',
				worker(a: Entry.CFSA & {
					bundle_directory_directory: string;
					data_file_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
					data_buffer_size: string | '?input';
				}) {
					let bundle_directory_directory: string;
					let data_file_directory: string;
					let version_number: bigint;
					let data_buffer_size: bigint;
					{
						bundle_directory_directory = a.bundle_directory_directory;
						data_file_directory = ArgumentParser.path(a.data_file_directory, {
							input_message: '请输入输出路径',
							default_value: bundle_directory_directory.replace(/$/i, '.bnk_pack'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（112）`, []);
							version_number = Console.integer(Check.enum_checkerx([112n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.data_buffer_size === '?input') {
							Console.notify('i', `请输入用于保存包数据输出的内存空间大小`, []);
							data_buffer_size = Console.size()!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(data_buffer_size));
					simple_batch_execute(
						bundle_directory_directory,
						['directory', /.+(\.bnk)(\.bundle)$/i],
						(item) => {
							let bundle_directory = `${bundle_directory_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -8)}`;
							let manifest_file = `${bundle_directory}/manifest.json`;
							let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
							CoreX.Tool.Wwise.SoundBank.pack_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number as any }, data_buffer.view());
						},
					);
					Console.notify('s', `输出路径：${data_file_directory}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory_directory: undefined!,
					data_file_directory: '?default',
					version_number: '?input',
					data_buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'bundle_directory_directory',
			}),
			Executor.method_of({
				id: 'wwise.sound_bank.unpack.batch',
				description: '[批处理] Wwise-Sound-Bank 解包',
				worker(a: Entry.CFSA & {
					data_file_directory: string;
					bundle_directory_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
				}) {
					let data_file_directory: string;
					let bundle_directory_directory: string;
					let version_number: bigint;
					{
						data_file_directory = a.data_file_directory;
						bundle_directory_directory = ArgumentParser.path(a.bundle_directory_directory, {
							input_message: '请输入输出路径',
							default_value: data_file_directory.replace(/$/i, '.bnk_unpack'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							Console.notify('i', `请输入版本编号（112）`, []);
							version_number = Console.integer(Check.enum_checkerx([112n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.bnk)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let bundle_directory = `${bundle_directory_directory}/${item}.bundle`;
							let manifest_file = `${bundle_directory}/manifest.json`;
							let embedded_audio_directory = `${bundle_directory}/embedded_audio`;
							CoreX.Tool.Wwise.SoundBank.unpack_fs(data_file, manifest_file, embedded_audio_directory, { number: version_number as any });
						},
					);
					Console.notify('s', `输出路径：${bundle_directory_directory}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					bundle_directory_directory: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.wwise.sound_bank._injector,
});