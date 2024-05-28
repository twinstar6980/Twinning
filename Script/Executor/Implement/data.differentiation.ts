namespace Twinning.Script.Executor.Implement.data.differentiation {

	// ------------------------------------------------

	// vcdiff.encode
	// vcdiff.decode

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'data.differentiation', [
			typical_method({
				id: 'vcdiff.encode',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'after_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'patch_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { after_file: string; }) => (argument.after_file.replace(/()?$/i, '.patch.bin')),
						condition: null,
					}),
					typical_argument_path({
						id: 'before_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ after_file, patch_file, before_file, buffer_size }) => {
					KernelX.Tool.Data.Differentiation.VCDiff.encode_fs(before_file, after_file, patch_file, false, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'vcdiff.decode',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'patch_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'after_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { patch_file: string; }) => (argument.patch_file.replace(/()?$/i, '.after.bin')),
						condition: null,
					}),
					typical_argument_path({
						id: 'before_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ patch_file, after_file, before_file, buffer_size }) => {
					KernelX.Tool.Data.Differentiation.VCDiff.decode_fs(before_file, after_file, patch_file, 0x7FFFFFFFn, buffer_size);
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

Twinning.Script.Executor.Implement.data.differentiation.injector;