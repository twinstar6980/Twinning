namespace Twinning.Script.Executor.Implementation.Text.Json {

	// #region interface

	export function activate(
	): void {
		push_typical_method('text.json', [
			typical_method({
				identifier: 'format',
				filter: ['file', /(\.json)$/i],
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
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.json)?$/i, '.format.json')),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'disable_array_trailing_comma',
						checker: null,
						automatic: (argument: {}) => (KernelX.Json.g_format.disable_array_trailing_comma),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'disable_array_line_breaking',
						checker: null,
						automatic: (argument: {}) => (KernelX.Json.g_format.disable_array_line_breaking),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'disable_object_trailing_comma',
						checker: null,
						automatic: (argument: {}) => (KernelX.Json.g_format.disable_object_trailing_comma),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'disable_object_line_breaking',
						checker: null,
						automatic: (argument: {}) => (KernelX.Json.g_format.disable_object_line_breaking),
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.format'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.json)?$/i, '.json')),
					}),
				],
				worker: ({ source_file, destination_file, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking }, temporary: {}) => {
					let data = KernelX.Json.read_fs(source_file);
					KernelX.Json.write_fs(destination_file, data, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
