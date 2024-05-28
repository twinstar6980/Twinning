namespace Twinning.Script.Executor.Implement.popcap.zlib {

	// ------------------------------------------------

	// compress *
	// uncompress *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'popcap.zlib', [
			typical_method({
				id: 'compress',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ raw_file, ripe_file, version_variant_64 }) => {
					KernelX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', { variant_64: version_variant_64 });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'uncompress',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_variant_64',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ ripe_file, raw_file, version_variant_64 }) => {
					KernelX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, { variant_64: version_variant_64 });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.popcap.zlib.injector;