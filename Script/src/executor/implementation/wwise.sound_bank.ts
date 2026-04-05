namespace Twinning.Script.Executor.Implementation.Wwise.SoundBank {

	// #region interface

	export function activate(
	): void {
		push_typical_method('wwise.sound_bank', [
			typical_method({
				identifier: 'encode',
				filter: ['directory', /(\.bnk\.bundle)$/i],
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
						automatic: (argument: {bundle_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.bundle_directory, /(\.bnk\.bundle)?$/i, '.bnk'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Wwise.SoundBank.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.bnk\.bundle)?$/i, '.bnk'),
					}),
				],
				worker: ({bundle_directory, data_file, version_number, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					let definition_file = bundle_directory.join('definition.json');
					let embedded_media_directory = bundle_directory.join('embedded_media');
					KernelX.Tool.Wwise.SoundBank.encode_fs(data_file, definition_file, embedded_media_directory, {number: version_number as any}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.bnk)$/i],
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.bnk)?$/i, '.bnk.bundle'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Wwise.SoundBank.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.bnk)?$/i, '.bnk.bundle'),
					}),
				],
				worker: ({data_file, bundle_directory, version_number}, store: {}) => {
					let definition_file = bundle_directory.join('definition.json');
					let embedded_media_directory = bundle_directory.join('embedded_media');
					KernelX.Tool.Wwise.SoundBank.decode_fs(data_file, definition_file, embedded_media_directory, {number: version_number as any});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
