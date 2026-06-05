namespace Twinning.Script.Executor.Implementation.Popcap.CryptData {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.crypt_data', [
			typical_method({
				identifier: 'encrypt',
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
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.cdat'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'limit',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value ? null : los('executor.implementation:*.out_of_range')),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
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
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, '.cdat'),
					}),
				],
				worker: ({raw_file, ripe_file, limit, key}, store: {}) => {
					KernelX.Tool.Popcap.CryptData.encrypt_fs(raw_file, ripe_file, limit, key, {});
					return;
				},
			}),
			typical_method({
				identifier: 'decrypt',
				filter: ['file', /(\.cdat)$/i],
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
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /(\.cdat)?$/i, ''),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'limit',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value ? null : los('executor.implementation:*.out_of_range')),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
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
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.decrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.cdat)?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, limit, key}, store: {}) => {
					KernelX.Tool.Popcap.CryptData.decrypt_fs(raw_file, ripe_file, limit, key, {});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
