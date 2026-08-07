namespace Twinning.Script.Executor.Implementation.Popcap.Trail {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.trail', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.trail\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.trail\.json)?$/i, '.trail.compiled'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'version_platform',
						option: KernelX.Tool.Popcap.Trail.VersionPlatformE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: (argument: {version_platform: string}) => (['mobile'].includes(argument.version_platform) ? null : false),
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
						identifier: 'definition_file',
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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /()?$/i, '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.trail\.json)?$/i, '.trail.compiled'),
					}),
				],
				worker: ({definition_file, data_file, version_platform, version_variant_64, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Popcap.Trail.encode_fs(data_file, definition_file, {platform: version_platform as any, variant_64: version_variant_64}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.trail\.compiled)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.trail\.compiled)?$/i, '.trail.json'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'version_platform',
						option: KernelX.Tool.Popcap.Trail.VersionPlatformE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: (argument: {version_platform: string}) => (['mobile'].includes(argument.version_platform) ? null : false),
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
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.trail\.compiled)?$/i, '.trail.json'),
					}),
				],
				worker: ({data_file, definition_file, version_platform, version_variant_64}, store: {}) => {
					KernelX.Tool.Popcap.Trail.decode_fs(data_file, definition_file, {platform: version_platform as any, variant_64: version_variant_64});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
