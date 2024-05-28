namespace Twinning.Script.Executor.Implement.text.json {

	// ------------------------------------------------

	// format *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'text.json', [
			typical_method({
				id: 'format',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'source_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.json)?$/i, '.format.json')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'disable_array_trailing_comma',
						checker: null,
						automatic: (argument: {}) => (KernelX.JSON.g_format.disable_array_trailing_comma),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'disable_array_line_breaking',
						checker: null,
						automatic: (argument: {}) => (KernelX.JSON.g_format.disable_array_line_breaking),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'disable_object_trailing_comma',
						checker: null,
						automatic: (argument: {}) => (KernelX.JSON.g_format.disable_object_trailing_comma),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'disable_object_line_breaking',
						checker: null,
						automatic: (argument: {}) => (KernelX.JSON.g_format.disable_object_line_breaking),
						condition: null,
					}),
				],
				worker: ({ source_file, destination_file, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking }) => {
					let data = KernelX.JSON.read_fs(source_file);
					KernelX.JSON.write_fs(destination_file, data, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'source_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'destination_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.format'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.json)?$/i, '.json')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.text.json.injector;