/**
 * + marmalade.dzip.pack Marmalade-DZip 打包
 * + marmalade.dzip.unpack Marmalade-DZip 解包
 * + marmalade.dzip.pack_auto Marmalade-DZip 自动打包
 */
namespace TwinKleS.Entry.method.marmalade.dzip {

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'marmalade.dzip.pack',
				description: 'Marmalade-DZip 打包',
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
							default_value: bundle_directory.replace(/((\.dz)(\.bundle))?$/i, '.dz'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（0）`, Check.enum_checkerx([0n]))!;
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
					CoreX.Tool.Marmalade.DZip.pack_fs(data_file, manifest_file, resource_directory, { number: version_number as any }, data_buffer_size);
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
					data_buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.dz)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'marmalade.dzip.unpack',
				description: 'Marmalade-DZip 解包',
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
							default_value: data_file.replace(/((\.dz))?$/i, '.dz.bundle'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（0）`, Check.enum_checkerx([0n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.Marmalade.DZip.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number as any });
					Output.i(`输出路径：${bundle_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.dz)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'marmalade.dzip.pack_auto',
				description: 'Marmalade-DZip 自动打包',
				worker(a: Entry.CFSA & {
					resource_directory: string;
					data_file: string | '?default' | '?input';
					version_number: bigint | '?input';
				}) {
					let resource_directory: string;
					let data_file: string;
					let version_number: bigint;
					{
						resource_directory = a.resource_directory;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: resource_directory.replace(/((\.dz)(\.resource))?$/i, '.dz'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（0）`, Check.enum_checkerx([0n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					let data = Support.MarmaladeDZip.ResourcePack.pack(resource_directory, version_number);
					CoreX.FileSystem.write_file(data_file, data[0].view().sub(Core.Size.value(0n), data[1]));
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					resource_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'resource_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.marmalade.dzip._injector,
});