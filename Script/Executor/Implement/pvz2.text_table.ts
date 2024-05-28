namespace Twinning.Script.Executor.Implement.pvz2.text_table {

	// ------------------------------------------------

	// convert

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'pvz2.text_table', [
			typical_method({
				id: 'convert',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'source_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'destination_version',
						option: Support.PvZ2.TextTable.Convert.VersionE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; destination_version: string; }) => (argument.source_file.replace(/(\.(txt|json))?$/i, `.convert.${{ 'text': 'txt', 'json_map': 'json', 'json_list': 'json' }[argument.destination_version]}`)),
						condition: null,
					}),
				],
				worker: ({ source_file, destination_version, destination_file }) => {
					Support.PvZ2.TextTable.Convert.convert_fs(source_file, destination_file, 'automatic', destination_version as any);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.pvz2.text_table.injector;