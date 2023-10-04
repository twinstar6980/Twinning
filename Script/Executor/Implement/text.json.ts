namespace TwinStar.Script.Executor.Implement.text.json {

	// ------------------------------------------------

	// format *

	export type Configuration = {
		disable_trailing_comma: TypicalArgumentExpression<boolean>;
		disable_array_wrap_line: TypicalArgumentExpression<boolean>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('text.json', [
			typical_method({
				id: 'format',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'source_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.json)?$/i, '.format.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_boolean({
						id: 'disable_trailing_comma',
						checker: null,
						automatic: (argument: {}) => (KernelX.JSON.g_format.disable_trailing_comma),
						condition: null,
						default: configuration.disable_trailing_comma,
					}),
					typical_argument_boolean({
						id: 'disable_array_wrap_line',
						checker: null,
						automatic: (argument: {}) => (KernelX.JSON.g_format.disable_array_wrap_line),
						condition: null,
						default: configuration.disable_array_wrap_line,
					}),
				],
				worker: ({ source_file, destination_file, disable_trailing_comma, disable_array_wrap_line }) => {
					let data = KernelX.JSON.read_fs(source_file);
					KernelX.JSON.write_fs(destination_file, data, disable_trailing_comma, disable_array_wrap_line);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'source_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'destination_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.format'),
						condition: null,
						default: '?automatic',
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

TwinStar.Script.Executor.Implement.text.json.injector;