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
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.xml)?$/i, '.format.xml')),
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.xml)?$/i, '.xml')),
					}),
				],
				worker: ({ source_file, destination_file }, temporary: {}) => {
					let data = KernelX.Xml.read_fs(source_file);
					KernelX.Xml.write_fs(destination_file, data);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
