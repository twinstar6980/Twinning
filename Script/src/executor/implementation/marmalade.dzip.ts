namespace Twinning.Script.Executor.Implementation.Marmalade.Dzip {

	// #region interface

	export function activate(
	): void {
		push_typical_method('marmalade.dzip', [
			typical_method({
				identifier: 'pack',
				filter: ['directory', /(\.dz\.bundle)$/i],
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
						automatic: (argument: {bundle_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.bundle_directory, /(\.dz\.bundle)?$/i, '.dz'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Marmalade.Dzip.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.dz\.bundle)?$/i, '.dz'),
					}),
				],
				worker: ({bundle_directory, data_file, version_number, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					let definition_file = bundle_directory.join('definition.json');
					let resource_directory = bundle_directory.join('resource');
					KernelX.Tool.Marmalade.Dzip.pack_fs(data_file, definition_file, resource_directory, {number: version_number as any}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'unpack',
				filter: ['file', /(\.dz)$/i],
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.dz)?$/i, '.dz.bundle'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Marmalade.Dzip.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.dz)?$/i, '.dz.bundle'),
					}),
				],
				worker: ({data_file, bundle_directory, version_number}, store: {}) => {
					let definition_file = bundle_directory.join('definition.json');
					let resource_directory = bundle_directory.join('resource');
					KernelX.Tool.Marmalade.Dzip.unpack_fs(data_file, definition_file, resource_directory, {number: version_number as any});
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
						automatic: (argument: {resource_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.resource_directory, /()?$/i, '.dz'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Marmalade.Dzip.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({resource_directory, data_file, version_number}, store: {}) => {
					let [data, data_size] = Support.Marmalade.Dzip.PackAutomatic.pack(resource_directory, version_number as any);
					StorageHelper.write_file(data_file, data.view().sub(Kernel.Size.value(0n), data_size));
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
