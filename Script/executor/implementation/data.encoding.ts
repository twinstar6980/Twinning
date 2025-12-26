namespace Twinning.Script.Executor.Implementation.Data.Encoding {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.encoding', [
			typical_method({
				identifier: 'base64.encode',
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
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					KernelX.Tool.Data.Encoding.Base64.encode_fs(raw_file, ripe_file);
					return;
				},
			}),
			typical_method({
				identifier: 'base64.decode',
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
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file }, temporary: {}) => {
					KernelX.Tool.Data.Encoding.Base64.decode_fs(ripe_file, raw_file);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
