namespace Twinning.Script.Executor.Implement.Text.XML {

	// #region interface

	export function activate(
	): void {
		push_typical_method('text.xml', [
			typical_method({
				id: 'format',
				filter: ['file', /(\.xml)$/i],
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
				batch: [
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
				worker: ({ source_file, destination_file }, temporary: {}) => {
					let data = KernelX.XML.read_fs(source_file);
					KernelX.XML.write_fs(destination_file, data);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
