namespace Twinning.Script.Executor.Implementation.Popcap.Package {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.package', [
			typical_method({
				identifier: 'pack',
				filter: ['directory', /(\.pak\.bundle)$/i],
				argument: [
					typical_argument_path({
						identifier: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {bundle_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.bundle_directory, /(\.pak\.bundle)?$/i, '.pak'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Package.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_compress_resource_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {bundle_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.bundle_directory, /()?$/i, '.pack'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pak\.bundle)?$/i, '.pak'),
					}),
				],
				worker: ({bundle_directory, data_file, version_number, version_compress_resource_data, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					let definition_file = bundle_directory.join('definition.json');
					let resource_directory = bundle_directory.join('resource');
					KernelX.Tool.Popcap.Package.pack_fs(data_file, definition_file, resource_directory, {number: version_number as any, compress_resource_data: version_compress_resource_data}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'unpack',
				filter: ['file', /(\.pak)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.pak)?$/i, '.pak.bundle'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Package.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_compress_resource_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.unpack'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pak)?$/i, '.pak.bundle'),
					}),
				],
				worker: ({data_file, bundle_directory, version_number, version_compress_resource_data}, store: {}) => {
					let definition_file = bundle_directory.join('definition.json');
					let resource_directory = bundle_directory.join('resource');
					KernelX.Tool.Popcap.Package.unpack_fs(data_file, definition_file, resource_directory, {number: version_number as any, compress_resource_data: version_compress_resource_data});
					return;
				},
			}),
			typical_method({
				identifier: 'pack_automatic',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'resource_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {resource_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.resource_directory, /()?$/i, '.pak'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Package.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_compress_resource_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({resource_directory, data_file, version_number, version_compress_resource_data}, store: {}) => {
					let [data, data_size] = Support.Popcap.Package.PackAutomatic.pack(resource_directory, version_number as any, version_compress_resource_data);
					StorageHelper.write_file(data_file, data.view().sub(Kernel.Size.value(0n), data_size));
					return;
				},
			}),
			typical_method({
				identifier: 'encrypt',
				filter: ['file', /(\.pak)$/i],
				argument: [
					typical_argument_path({
						identifier: 'plain_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {plain_file: StoragePath}) => ConvertHelper.replace_path_name(argument.plain_file, /(\.pak)?$/i, '.cipher.pak'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'key',
						option: null,
						checker: (argument: {}, value) => ((0n < value && value < 256n) ? null : los('executor.implement:*.illegal_key')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'plain_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {plain_file: StoragePath}) => ConvertHelper.replace_path_name(argument.plain_file, /()?$/i, '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pak)?$/i, '.pak'),
					}),
				],
				worker: ({plain_file, cipher_file, key}, store: {}) => {
					KernelX.Tool.Data.Encryption.Exor.encrypt_fs(plain_file, cipher_file, [key]);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
