namespace Twinning.Script.Executor.Implement.text.xml {

	// ------------------------------------------------

	// format *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'text.xml', [
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
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.xml)?$/i, '.format.xml')),
						condition: null,
					}),
				],
				worker: ({ source_file, destination_file }) => {
					let data = KernelX.XML.read_fs(source_file);
					KernelX.XML.write_fs(destination_file, data);
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.xml)?$/i, '.xml')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.text.xml.injector;