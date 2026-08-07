namespace Twinning.Script.Executor.Implementation.Popcap.ParticleEffect {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.particle_effect', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.ppf\.json)$/i],
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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.ppf\.json)?$/i, '.ppf'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ParticleEffect.VersionNumberE,
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
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.ppf\.json)?$/i, '.ppf'),
					}),
				],
				worker: ({definition_file, data_file, version_number, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Popcap.ParticleEffect.encode_fs(data_file, definition_file, {number: version_number as any}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.ppf)$/i],
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.ppf)?$/i, '.ppf.json'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ParticleEffect.VersionNumberE,
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
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.ppf)?$/i, '.ppf.json'),
					}),
				],
				worker: ({data_file, definition_file, version_number}, store: {}) => {
					KernelX.Tool.Popcap.ParticleEffect.decode_fs(data_file, definition_file, {number: version_number as any});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
