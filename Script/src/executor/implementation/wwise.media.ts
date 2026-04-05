namespace Twinning.Script.Executor.Implementation.Wwise.Media {

	// #region interface

	export function activate(
	): void {
		push_typical_method('wwise.media', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.wav)$/i],
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
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.wav)?$/i, '.wem'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'format',
						option: Support.Wwise.Media.FormatE,
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
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.wav)?$/i, '.wem'),
					}),
				],
				worker: ({raw_file, ripe_file, format}, store: {}) => {
					Support.Wwise.Media.Encode.encode_fs(raw_file, ripe_file, format as any);
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.wem)$/i],
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
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /(\.wem)?$/i, '.wav'),
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
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.wem)?$/i, '.wav'),
					}),
				],
				worker: ({ripe_file, raw_file}, store: {}) => {
					Support.Wwise.Media.Decode.decode_fs(ripe_file, raw_file);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
