namespace TwinStar.Script.Executor.Implement.popcap.resource_stream_bundle_patch {

	// ------------------------------------------------

	// encode
	// decode

	export type Configuration = {
		use_raw_packet: TypicalArgumentExpression<boolean>;
		version_number: TypicalArgumentExpression<bigint>;
		encode_buffer_size: TypicalArgumentExpression<string>;
		decode_buffer_size: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.resource_stream_bundle_patch', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.rsb)$/i],
				argument: [
					typical_argument_path({
						id: 'after_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'patch_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { after_file: string; }) => (argument.after_file.replace(/(\.rsb)?$/i, '.rsbpatch')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_path({
						id: 'before_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundlePatch.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'use_raw_packet',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.use_raw_packet,
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
					let { after_file, patch_file, before_file, version_number, use_raw_packet, buffer_size } = argument;
					KernelX.Tool.PopCap.ResourceStreamBundlePatch.encode_fs(before_file, after_file, patch_file, use_raw_packet, { number: version_number as any }, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.rsbpatch)$/i],
				argument: [
					typical_argument_path({
						id: 'patch_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'after_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { patch_file: string; }) => (argument.patch_file.replace(/(\.rsbpatch)?$/i, '.rsb')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_path({
						id: 'before_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundlePatch.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'use_raw_packet',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.use_raw_packet,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.decode_buffer_size,
					}),
				],
				worker: (argument) => {
					let { patch_file, after_file, before_file, version_number, use_raw_packet, buffer_size } = argument;
					KernelX.Tool.PopCap.ResourceStreamBundlePatch.decode_fs(before_file, after_file, patch_file, use_raw_packet, { number: version_number as any }, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.resource_stream_bundle_patch.injector;