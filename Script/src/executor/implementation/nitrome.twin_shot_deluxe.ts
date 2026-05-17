namespace Twinning.Script.Executor.Implementation.Nitrome.TwinShotDeluxe {

	// #region interface

	export function activate(
	): void {
		push_typical_method('nitrome.twin_shot_deluxe', [
			typical_method({
				identifier: 'record.encode',
				filter: ['file', /(\.dat\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.dat\.json)?$/i, '.dat'),
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
				batch: [
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /()?$/i, '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.reanim\.json)?$/i, '.reanim.compiled'),
					}),
				],
				worker: ({definition_file, data_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					Support.Nitrome.TwinShotDeluxe.Record.Encode.process_fs(data_file, definition_file, store.buffer);
					return;
				},
			}),
			typical_method({
				identifier: 'record.decode',
				filter: ['file', /(\.dat)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.dat)?$/i, '.dat.json'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.reanim\.compiled)?$/i, '.reanim.json'),
					}),
				],
				worker: ({data_file, definition_file}, store: {}) => {
					Support.Nitrome.TwinShotDeluxe.Record.Decode.process_fs(data_file, definition_file);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
