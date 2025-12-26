namespace Twinning.Script.Executor.Implementation.Popcap.Particle {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.particle', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.particle\.json)$/i],
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
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.particle\.json)?$/i, '.xml.compiled')),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'version_platform',
						option: KernelX.Tool.Popcap.Particle.VersionPlatformE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: (argument: { version_platform: string; }) => (['mobile'].includes(argument.version_platform) ? null : false),
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
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.particle\.json)?$/i, '.xml.compiled')),
					}),
				],
				worker: ({ definition_file, data_file, version_platform, version_variant_64, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Popcap.Particle.encode_fs(data_file, definition_file, { platform: version_platform as any, variant_64: version_variant_64 }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.xml\.compiled)$/i],
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.xml\.compiled)?$/i, '.particle.json')),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'version_platform',
						option: KernelX.Tool.Popcap.Particle.VersionPlatformE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: (argument: { version_platform: string; }) => (['mobile'].includes(argument.version_platform) ? null : false),
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
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.xml\.compiled)?$/i, '.particle.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_platform, version_variant_64 }, temporary: {}) => {
					KernelX.Tool.Popcap.Particle.decode_fs(data_file, definition_file, { platform: version_platform as any, variant_64: version_variant_64 });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
