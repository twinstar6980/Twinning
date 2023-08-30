namespace TwinStar.Script.Executor.Implement.popcap.re_animation {

	// ------------------------------------------------

	// encode *
	// decode *

	export type Configuration = {
		version_platform: TypicalArgumentExpression<string>;
		version_variant_64: TypicalArgumentExpression<boolean>;
		encode_buffer_size: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.re_animation', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.reanim\.json)$/i],
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
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.reanim\.json)?$/i, '.reanim.compiled')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'version_platform',
						option: KernelX.Tool.PopCap.ReAnimation.VersionPlatformE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_platform,
					}),
					typical_argument_confirmation({
						id: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: (argument: { version_platform: string; }) => (argument.version_platform === 'mobile' ? null : false),
						default: configuration.version_variant_64,
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
					let { definition_file, data_file, version_platform, version_variant_64, buffer_size } = argument;
					KernelX.Tool.PopCap.ReAnimation.encode_fs(data_file, definition_file, { platform: version_platform as any, variant_64: version_variant_64 }, buffer_size);
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.reanim\.json)?$/i, '.reanim.compiled')),
					}),
				],
				batch_worker: (argument, temporary: { buffer: Kernel.ByteArray; }) => {
					let { definition_file, data_file, version_platform, version_variant_64, buffer_size } = argument;
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.ReAnimation.encode_fs(data_file, definition_file, { platform: version_platform as any, variant_64: version_variant_64 }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.reanim\.compiled)$/i],
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.reanim\.compiled)?$/i, '.reanim.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'version_platform',
						option: KernelX.Tool.PopCap.ReAnimation.VersionPlatformE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_platform,
					}),
					typical_argument_confirmation({
						id: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: (argument: { version_platform: string; }) => (argument.version_platform === 'mobile' ? null : false),
						default: configuration.version_variant_64,
					}),
				],
				worker: (argument) => {
					let { data_file, definition_file, version_platform, version_variant_64 } = argument;
					KernelX.Tool.PopCap.ReAnimation.decode_fs(data_file, definition_file, { platform: version_platform as any, variant_64: version_variant_64 });
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.reanim\.compiled)?$/i, '.reanim.json')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.re_animation.injector;