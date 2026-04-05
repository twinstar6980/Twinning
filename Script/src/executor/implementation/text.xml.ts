namespace Twinning.Script.Executor.Implementation.Text.Xml {

	// #region interface

	export function activate(
	): void {
		push_typical_method('text.xml', [
			typical_method({
				identifier: 'format',
				filter: ['file', /(\.xml)$/i],
				argument: [
					typical_argument_path({
						identifier: 'source_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {source_file: StoragePath}) => ConvertHelper.replace_path_name(argument.source_file, /(\.xml)?$/i, '.format.xml'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'source_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'destination_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {source_file: StoragePath}) => ConvertHelper.replace_path_name(argument.source_file, /()?$/i, '.format'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.xml)?$/i, '.xml'),
					}),
				],
				worker: ({source_file, destination_file}, store: {}) => {
					let data = KernelX.Tool.Data.Serialization.Json.read_fs(source_file);
					KernelX.Tool.Data.Serialization.Json.write_fs(destination_file, data);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
