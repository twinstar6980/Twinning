namespace Twinning.Script.Executor.Implement.PopCap.CharacterFontWidget2 {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.character_font_widget_2', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.cfw2\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.cfw2\.json)?$/i, '.cfw2')),
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.cfw2\.json)?$/i, '.cfw2')),
					}),
				],
				worker: ({ definition_file, data_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.CharacterFontWidget2.encode_fs(data_file, definition_file, {}, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.cfw2)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.cfw2)?$/i, '.cfw2.json')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.cfw2)?$/i, '.cfw2.json')),
					}),
				],
				worker: ({ data_file, definition_file }, temporary: {}) => {
					KernelX.Tool.PopCap.CharacterFontWidget2.decode_fs(data_file, definition_file, {});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}