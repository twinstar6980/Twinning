/**
 * + marmalade.dzip.pack Marmalade-DZip 打包
 * + marmalade.dzip.unpack Marmalade-DZip 解包
 * + marmalade.dzip.pack_auto Marmalade-DZip 自动打包
 */
namespace TwinKleS.Entry.method.marmalade.dzip {

	// ------------------------------------------------

	type Config = {
		pack_buffer_size: Argument.Request<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'marmalade.dzip.pack',
				description: 'Marmalade-DZip 打包',
				worker(a: Entry.CFSA & {
					bundle_directory: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: [0n][number];
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
							() => (bundle_directory.replace(/((\.dz)(\.bundle))?$/i, '.dz')),
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
					CoreX.Tool.Marmalade.DZip.pack_fs(data_file, manifest_file, resource_directory, { number: version_number }, buffer_size);
					Console.notify('s', `执行成功`, [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: '?input',
					buffer_size: config.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.dz)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'marmalade.dzip.unpack',
				description: 'Marmalade-DZip 解包',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					bundle_directory: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: [0n][number];
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
							() => (data_file.replace(/((\.dz))?$/i, '.dz.bundle')),
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
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.Marmalade.DZip.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number });
					Console.notify('s', `执行成功`, [`${bundle_directory}`]);
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
					resource_directory: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
				}) {
					let resource_directory: string;
					let data_file: string;
					let version_number: [0n][number];
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
							() => (resource_directory.replace(/((\.dz)(\.resource))?$/i, '.dz')),
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
					}
					let data = Support.MarmaladeDZip.ResourcePack.pack(resource_directory, version_number);
					CoreX.FileSystem.write_file(data_file, data[0].view().sub(Core.Size.value(0n), data[1]));
					Console.notify('s', `执行成功`, [`${data_file}`]);
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