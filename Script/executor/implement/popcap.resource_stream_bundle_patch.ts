namespace Twinning.Script.Executor.Implement.PopCap.ResourceStreamBundlePatch {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.resource_stream_bundle_patch', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.rsb)$/i],
				argument: [
					typical_argument_path({
						identifier: 'after_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'patch_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { after_file: string; }) => (argument.after_file.replace(/(\.rsb)?$/i, '.rsbpatch')),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'before_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundlePatch.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'use_raw_packet',
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
				batch: null,
				worker: ({ after_file, patch_file, before_file, version_number, use_raw_packet, buffer_size }, temporary: {}) => {
					KernelX.Tool.PopCap.ResourceStreamBundlePatch.encode_fs(before_file, after_file, patch_file, use_raw_packet, { number: version_number as any }, buffer_size);
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.rsbpatch)$/i],
				argument: [
					typical_argument_path({
						identifier: 'patch_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'after_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { patch_file: string; }) => (argument.patch_file.replace(/(\.rsbpatch)?$/i, '.rsb')),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'before_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundlePatch.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'use_raw_packet',
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
				batch: null,
				worker: ({ patch_file, after_file, before_file, version_number, use_raw_packet, buffer_size }, temporary: {}) => {
					KernelX.Tool.PopCap.ResourceStreamBundlePatch.decode_fs(before_file, after_file, patch_file, use_raw_packet, { number: version_number as any }, buffer_size);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
