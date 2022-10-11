/**
 * + popcap.pak.pack PopCap-PAK 打包
 * + popcap.pak.unpack PopCap-PAK 解包
 * + popcap.pak.pack_auto PopCap-PAK 自动打包
 * + popcap.pak.crypt PopCap-PAK 加解密
 */
namespace TwinKleS.Entry.method.popcap.pak {

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: Argument.Request<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.pak.pack',
				description: 'PopCap-PAK 打包',
				worker(a: Entry.CFSA & {
					bundle_directory: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					version_compress_resource_data: Argument.Request<boolean, false>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: [0n][number];
					let version_compress_resource_data: boolean;
					let buffer_size: bigint;
					{
						bundle_directory = Argument.require(
							'捆绑目录', '',
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Argument.request(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.pak)(\.bundle))?$/i, '.pak')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([0n].includes(value) ? null : `版本不受支持`),
						);
						version_compress_resource_data = Argument.request(
							'启用资源数据压缩', '',
							a.version_compress_resource_data,
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
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.PAK.pack_fs(data_file, manifest_file, resource_directory, { number: version_number, compress_resource_data: version_compress_resource_data }, buffer_size);
					Console.notify('s', `执行成功`, [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
					version_compress_resource_data: '?input',
					buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pak)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.pak.unpack',
				description: 'PopCap-PAK 解包',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					bundle_directory: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					version_compress_resource_data: Argument.Request<boolean, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: [0n][number];
					let version_compress_resource_data: boolean;
					{
						data_file = Argument.require(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Argument.request(
							'捆绑目录', '',
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.pak))?$/i, '.pak.bundle')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([0n].includes(value) ? null : `版本不受支持`),
						);
						version_compress_resource_data = Argument.request(
							'启用资源数据压缩', '',
							a.version_compress_resource_data,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.PAK.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number, compress_resource_data: version_compress_resource_data });
					Console.notify('s', `执行成功`, [`${bundle_directory}`]);
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
					resource_directory: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					version_compress_resource_data: Argument.Request<boolean, false>;
				}) {
					let resource_directory: string;
					let data_file: string;
					let version_number: [0n][number];
					let version_compress_resource_data: boolean;
					{
						resource_directory = Argument.require(
							'资源目录', '',
							a.resource_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Argument.request(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							() => (resource_directory.replace(/((\.pak)(\.resource))?$/i, '.pak')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([0n].includes(value) ? null : `版本不受支持`),
						);
						version_compress_resource_data = Argument.request(
							'启用资源数据压缩', '',
							a.version_compress_resource_data,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					let data = Support.PopCapPAK.ResourcePack.pack(resource_directory, version_number, version_compress_resource_data);
					CoreX.FileSystem.write_file(data_file, data[0].view().sub(Core.Size.value(0n), data[1]));
					Console.notify('s', `执行成功`, [`${data_file}`]);
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
					plain_file: Argument.Require<string>;
					cipher_file: Argument.Request<string, true>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					{
						plain_file = Argument.require(
							'明文文件', '',
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Argument.request(
							'密文文件', '',
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/((\.pak))?$/i, '.xor.pak')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Encrypt.XOR.crypt_fs(plain_file, cipher_file, 0xF7n);
					Console.notify('s', `执行成功`, [`${cipher_file}`]);
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