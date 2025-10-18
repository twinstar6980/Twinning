namespace Twinning.Script.Executor.Implement.PopCap.RenderEffect {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.render_effect', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.popfx\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.popfx\.json)?$/i, '.popfx')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.RenderEffect.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_variant',
						option: KernelX.Tool.PopCap.RenderEffect.VersionVariantE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.popfx\.json)?$/i, '.popfx')),
					}),
				],
				worker: ({ definition_file, data_file, version_number, version_variant, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.RenderEffect.encode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.popfx)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.popfx)?$/i, '.popfx.json')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.RenderEffect.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_variant',
						option: KernelX.Tool.PopCap.RenderEffect.VersionVariantE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.popfx)?$/i, '.popfx.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_variant }, temporary: {}) => {
					KernelX.Tool.PopCap.RenderEffect.decode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}