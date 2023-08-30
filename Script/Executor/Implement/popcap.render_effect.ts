namespace TwinStar.Script.Executor.Implement.popcap.render_effect {

	// ------------------------------------------------

	// encode *
	// decode *

	export type Configuration = {
		version_number: TypicalArgumentExpression<bigint>;
		version_variant: TypicalArgumentExpression<bigint>;
		encode_buffer_size: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
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
						default: '?input',
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.popfx\.json)?$/i, '.popfx')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.RenderEffect.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_integer({
						id: 'version_variant',
						option: KernelX.Tool.PopCap.RenderEffect.VersionVariantE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_variant,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.encode_buffer_size,
					}),
				],
				worker: (argument) => {
					let { definition_file, data_file, version_number, version_variant, buffer_size } = argument;
					KernelX.Tool.PopCap.RenderEffect.encode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any }, buffer_size);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.popfx\.json)?$/i, '.popfx')),
					}),
				],
				batch_worker: (argument, temporary: { buffer: Kernel.ByteArray; }) => {
					let { definition_file, data_file, version_number, version_variant, buffer_size } = argument;
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
						default: '?input',
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.popfx)?$/i, '.popfx.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.RenderEffect.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_integer({
						id: 'version_variant',
						option: KernelX.Tool.PopCap.RenderEffect.VersionVariantE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_variant,
					}),
				],
				worker: (argument) => {
					let { data_file, definition_file, version_number, version_variant } = argument;
					KernelX.Tool.PopCap.RenderEffect.decode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.popfx)?$/i, '.popfx.json')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.render_effect.injector;