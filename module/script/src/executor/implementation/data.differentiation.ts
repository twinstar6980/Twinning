namespace Twinning.Script.Executor.Implementation.Data.Differentiation {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.differentiation', [
			typical_method({
				identifier: 'vcdiff.encode',
				filter: ['file', /()$/i],
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
						automatic: (argument: {after_file: StoragePath}) => ConvertHelper.replace_path_name(argument.after_file, /()?$/i, '.patch.bin'),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'before_file',
						rule: ['file', 'input'],
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
				worker: ({after_file, patch_file, before_file, buffer_size}, store: {}) => {
					KernelX.Tool.Data.Differentiation.Vcdiff.encode_fs(before_file, after_file, patch_file, false, buffer_size.value());
					return;
				},
			}),
			typical_method({
				identifier: 'vcdiff.decode',
				filter: ['file', /()$/i],
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
						automatic: (argument: {patch_file: StoragePath}) => ConvertHelper.replace_path_name(argument.patch_file, /()?$/i, '.after.bin'),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'before_file',
						rule: ['file', 'input'],
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
				worker: ({patch_file, after_file, before_file, buffer_size}, store: {}) => {
					KernelX.Tool.Data.Differentiation.Vcdiff.decode_fs(before_file, after_file, patch_file, 0x7FFFFFFFn, buffer_size.value());
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
