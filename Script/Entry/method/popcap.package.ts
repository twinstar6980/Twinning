namespace TwinStar.Script.Entry.method.popcap.package_ {

	// ------------------------------------------------

	// pack
	// unpack
	// pack_automatic
	// encrypt

	type Configuration = {
		version_number: Executor.RequestArgument<bigint, false>;
		version_compress_resource_data: Executor.RequestArgument<boolean, false>;
		pack_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.package.pack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					bundle_directory: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					version_compress_resource_data: Executor.RequestArgument<boolean, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let bundle_directory: string;
					let data_file: string;
					let version_number: bigint;
					let version_compress_resource_data: boolean;
					let buffer_size: bigint;
					{
						bundle_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (bundle_directory.replace(/((\.pak)(\.bundle))?$/i, '.pak')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.PopCap.Package.VersionNumberE), null, null, initial)),
						);
						version_compress_resource_data = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_compress_resource_data'),
							a.version_compress_resource_data,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.Package.pack_fs(data_file, manifest_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data }, buffer_size);
					Console.success(los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					bundle_directory: undefined!,
					data_file: '?default',
					version_number: configuration.version_number,
					version_compress_resource_data: configuration.version_compress_resource_data,
					buffer_size: configuration.pack_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pak)(\.bundle)$/i]]),
				input_forwarder: 'bundle_directory',
			}),
			Executor.method_of({
				id: 'popcap.package.unpack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					bundle_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					version_compress_resource_data: Executor.RequestArgument<boolean, false>;
				}) {
					let data_file: string;
					let bundle_directory: string;
					let version_number: bigint;
					let version_compress_resource_data: boolean;
					{
						data_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						bundle_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'bundle_directory'),
							a.bundle_directory,
							(value) => (value),
							() => (data_file.replace(/((\.pak))?$/i, '.pak.bundle')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.PopCap.Package.VersionNumberE), null, null, initial)),
						);
						version_compress_resource_data = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_compress_resource_data'),
							a.version_compress_resource_data,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					let manifest_file = `${bundle_directory}/manifest.json`;
					let resource_directory = `${bundle_directory}/resource`;
					CoreX.Tool.PopCap.Package.unpack_fs(data_file, manifest_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data });
					Console.success(los(`执行成功`), [`${bundle_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					bundle_directory: '?default',
					version_number: configuration.version_number,
					version_compress_resource_data: configuration.version_compress_resource_data,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pak)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.package.pack_automatic',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					resource_directory: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					version_compress_resource_data: Executor.RequestArgument<boolean, false>;
				}) {
					let resource_directory: string;
					let data_file: string;
					let version_number: bigint;
					let version_compress_resource_data: boolean;
					{
						resource_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'resource_directory'),
							a.resource_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (resource_directory.replace(/((\.pak)(\.resource))?$/i, '.pak')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(CoreX.Tool.PopCap.Package.VersionNumberE), null, null, initial)),
						);
						version_compress_resource_data = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_compress_resource_data'),
							a.version_compress_resource_data,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					let data = Support.PopCap.Package.PackAutomatic.pack(resource_directory, version_number as any, version_compress_resource_data);
					CoreX.FileSystem.write_file(data_file, data[0].view().sub(Core.Size.value(0n), data[1]));
					Console.success(los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					resource_directory: undefined!,
					data_file: '?default',
					version_number: configuration.version_number,
					version_compress_resource_data: configuration.version_compress_resource_data,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'resource_directory',
			}),
			Executor.method_of({
				id: 'popcap.package.encrypt',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file: Executor.RequireArgument<string>;
					cipher_file: Executor.RequestArgument<string, true>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					{
						plain_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/((\.pak))?$/i, '.cipher.pak')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Encryption.XOR.encrypt_fs(plain_file, cipher_file, [0xF7n]);
					Console.success(los(`执行成功`), [`${cipher_file}`]);
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
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.package_._injector,
});