/**
 * + popcap.rsgp.pack PopCap-RSGP 打包
 * + popcap.rsgp.unpack PopCap-RSGP 解包
 */
namespace TwinKleS.Entry.method.popcap.rsgp {

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.rsgp.pack',
				description: 'PopCap-RSGP 打包',
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
							default_value: bundle_directory.replace(/((\.rsgp)(\.bundle))?$/i, '.rsgp'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（3~4）`, Check.enum_checkerx([3n, 4n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.data_buffer_size === '?input') {
							data_buffer_size = Input.size(`请输入用于保存包数据输出的内存空间大小`)!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.RSGP.pack_fs(data_file, manifest_file, resource_directory, { number: version_number as any }, data_buffer_size);
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					data_buffer_size: config.pack_buffer_size,
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.rsgp)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.rsgp.unpack',
				description: 'PopCap-RSGP 解包',
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
							default_value: data_file.replace(/((\.rsgp))?$/i, '.rsgp.bundle'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（3~4）`, Check.enum_checkerx([3n, 4n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.RSGP.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number as any });
					Output.i(`输出路径：${bundle_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rsgp)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.rsgp._injector,
});