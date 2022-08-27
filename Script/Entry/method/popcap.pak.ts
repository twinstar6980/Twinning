/**
 * + popcap.pak.pack PopCap-PAK 打包
 * + popcap.pak.unpack PopCap-PAK 解包
 * + popcap.pak.pack_auto PopCap-PAK 自动打包
 * + popcap.pak.crypt PopCap-PAK 加解密
 */
namespace TwinKleS.Entry.method.popcap.pak {

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.pak.pack',
				description: 'PopCap-PAK 打包',
				worker(a: Entry.CFSA & {
					bundle_directory: string;
					data_file: string | '?default' | '?input';
					version_number: bigint | '?input';
					version_compress_resource_data: boolean | '?input';
					data_buffer_size: string | '?input';
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: bigint;
					let version_compress_resource_data: boolean;
					let data_buffer_size: bigint;
					{
						bundle_directory = a.bundle_directory;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: bundle_directory.replace(/((\.pak)(\.bundle))?$/i, '.pak'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（0）`, Check.enum_checkerx([0n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.version_compress_resource_data === '?input') {
							version_compress_resource_data = Input.yon(`是否启用资源数据压缩`)!;
						} else {
							version_compress_resource_data = a.version_compress_resource_data;
						}
						if (a.data_buffer_size === '?input') {
							data_buffer_size = Input.size(`请输入用于保存包数据输出的内存空间大小`)!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.PAK.pack_fs(data_file, manifest_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data }, data_buffer_size);
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
					version_compress_resource_data: '?input',
					data_buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pak)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.pak.unpack',
				description: 'PopCap-PAK 解包',
				worker(a: Entry.CFSA & {
					data_file: string;
					bundle_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
					version_compress_resource_data: boolean | '?input';
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: bigint;
					let version_compress_resource_data: boolean;
					{
						data_file = a.data_file;
						bundle_directory = ArgumentParser.path(a.bundle_directory, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.pak))?$/i, '.pak.bundle'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（0）`, Check.enum_checkerx([0n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.version_compress_resource_data === '?input') {
							version_compress_resource_data = Input.yon(`是否启用资源数据压缩`)!;
						} else {
							version_compress_resource_data = a.version_compress_resource_data;
						}
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.PAK.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data });
					Output.i(`输出路径：${bundle_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: '?input',
					version_compress_resource_data: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pak)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.pak.pack_auto',
				description: 'PopCap-PAK 自动打包',
				worker(a: Entry.CFSA & {
					resource_directory: string;
					data_file: string | '?default' | '?input';
					version_number: bigint | '?input';
					version_compress_resource_data: boolean | '?input';
				}) {
					let resource_directory: string;
					let data_file: string;
					let version_number: bigint;
					let version_compress_resource_data: boolean;
					{
						resource_directory = a.resource_directory;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: resource_directory.replace(/((\.pak)(\.resource))?$/i, '.pak'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（0）`, Check.enum_checkerx([0n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.version_compress_resource_data === '?input') {
							version_compress_resource_data = Input.yon(`是否启用资源数据压缩`)!;
						} else {
							version_compress_resource_data = a.version_compress_resource_data;
						}
					}
					let data = Support.PopCapPAK.ResourcePack.pack(resource_directory, version_number, version_compress_resource_data);
					CoreX.FileSystem.write_file(data_file, data[0].view().sub(Core.Size.value(0n), data[1]));
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					resource_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
					version_compress_resource_data: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'resource_directory',
			}),
			Executor.method_of({
				id: 'popcap.pak.crypt',
				description: 'PopCap-PAK 加解密',
				worker(a: Entry.CFSA & {
					plain_file: string;
					cipher_file: string | '?default' | '?input';
				}) {
					let plain_file: string;
					let cipher_file: string;
					{
						plain_file = a.plain_file;
						cipher_file = ArgumentParser.path(a.cipher_file, {
							input_message: '请输入输出路径',
							default_value: plain_file.replace(/((\.pak))?$/i, '.xor.pak'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					CoreX.Tool.Data.Encrypt.XOR.crypt_fs(plain_file, cipher_file, 0xF7n);
					Output.i(`输出路径：${cipher_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pak)$/i]]),
				input_forwarder: 'plain_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.pak._injector,
});