namespace Twinning.Script.Executor.Implement.Texture.Transformation {

	// #region interface

	export function activate(
	): void {
		push_typical_method('texture.transformation', [
			typical_method({
				id: 'flip',
				filter: ['file', /(\.png)$/i],
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
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.png)?$/i, '.flip.png')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'horizontal',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'vertical',
						checker: null,
						automatic: null,
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.scale'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.png')),
					}),
				],
				worker: ({ source_file, destination_file, horizontal, vertical }, temporary: {}) => {
					KernelX.Tool.Texture.Transformation.flip_fs(source_file, destination_file, horizontal, vertical);
					return;
				},
			}),
			typical_method({
				id: 'scale',
				filter: ['file', /(\.png)$/i],
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
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.png)?$/i, '.scale.png')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'size_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implement:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'size_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implement:*.size_should_be_bigger_then_zero')),
						automatic: null,
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.scale'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.png')),
					}),
				],
				worker: ({ source_file, destination_file, size_width, size_height }, temporary: {}) => {
					KernelX.Tool.Texture.Transformation.scale_fs(source_file, destination_file, [size_width, size_height]);
					return;
				},
			}),
			typical_method({
				id: 'scale_rate',
				filter: ['file', /(\.png)$/i],
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
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.png)?$/i, '.scale.png')),
						condition: null,
					}),
					typical_argument_floater({
						id: 'size_rate',
						option: null,
						checker: (argument: {}, value) => ((0.0 < value) ? null : los('executor.implement:*.rate_should_be_bigger_then_zero')),
						automatic: null,
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.scale'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.png')),
					}),
				],
				worker: ({ source_file, destination_file, size_rate }, temporary: {}) => {
					KernelX.Tool.Texture.Transformation.scale_rate_fs(source_file, destination_file, size_rate);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
