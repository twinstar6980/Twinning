namespace Twinning.Script.Executor.Implementation.Popcap.ResourceStreamGroup {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.resource_stream_group', [
			typical_method({
				identifier: 'pack',
				filter: ['directory', /(\.rsg\.bundle)$/i],
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
						automatic: (argument: {bundle_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.bundle_directory, /(\.rsg\.bundle)?$/i, '.rsg'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ResourceStreamGroup.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rsg\.bundle)?$/i, '.rsg'),
					}),
				],
				worker: ({bundle_directory, data_file, version_number, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					let definition_file = bundle_directory.join('definition.json');
					let resource_directory = bundle_directory.join('resource');
					KernelX.Tool.Popcap.ResourceStreamGroup.pack_fs(data_file, definition_file, resource_directory, {number: version_number as any}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'unpack',
				filter: ['file', /(\.rsg)$/i],
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.rsg)?$/i, '.rsg.bundle'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ResourceStreamGroup.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rsg)?$/i, '.rsg.bundle'),
					}),
				],
				worker: ({data_file, bundle_directory, version_number}, store: {}) => {
					let definition_file = bundle_directory.join('definition.json');
					let resource_directory = bundle_directory.join('resource');
					KernelX.Tool.Popcap.ResourceStreamGroup.unpack_fs(data_file, definition_file, resource_directory, {number: version_number as any});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
