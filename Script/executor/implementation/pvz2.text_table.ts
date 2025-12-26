namespace Twinning.Script.Executor.Implementation.PvZ2.TextTable {

	// #region interface

	export function activate(
	): void {
		push_typical_method('pvz2.text_table', [
			typical_method({
				identifier: 'convert',
				filter: ['file', /(\.(txt|json))$/i],
				argument: [
					typical_argument_path({
						identifier: 'source_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'destination_version',
						option: Support.Pvz2.TextTable.Convert.VersionE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; destination_version: string; }) => (argument.source_file.replace(/(\.(txt|json))?$/i, `.convert.${{ 'text': 'txt', 'json_map': 'json', 'json_list': 'json' }[argument.destination_version]}`)),
						condition: null,
					}),
				],
				batch: null,
				worker: ({ source_file, destination_version, destination_file }, temporary: {}) => {
					Support.Pvz2.TextTable.Convert.convert_fs(source_file, destination_file, 'automatic', destination_version as any);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
