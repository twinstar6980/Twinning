namespace Twinning.Script.Executor.Implement.PopCap.zlib {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.zlib', [
			typical_method({
				identifier: 'compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file, version_variant_64 }, temporary: {}) => {
					KernelX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', { variant_64: version_variant_64 });
					return;
				},
			}),
			typical_method({
				identifier: 'uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file, version_variant_64 }, temporary: {}) => {
					KernelX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, { variant_64: version_variant_64 });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
