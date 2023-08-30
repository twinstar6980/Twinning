namespace TwinStar.Script.Executor.Implement.texture.transformation {

	// ------------------------------------------------

	// flip *
	// scale *
	// scale_rate *

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
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
						default: '?input',
					}),
					typical_argument_path({
						id: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.png)?$/i, '.flip.png')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_confirmation({
						id: 'horizontal',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_confirmation({
						id: 'vertical',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: (argument) => {
					let { source_file, destination_file, horizontal, vertical } = argument;
					KernelX.Tool.Texture.Transformation.flip_fs(source_file, destination_file, horizontal, vertical);
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.scale'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.png')),
					}),
				],
				batch_worker: null,
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
						default: '?input',
					}),
					typical_argument_path({
						id: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.png)?$/i, '.scale.png')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'size_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'size_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: (argument) => {
					let { source_file, destination_file, size_width, size_height } = argument;
					KernelX.Tool.Texture.Transformation.scale_fs(source_file, destination_file, [size_width, size_height]);
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.scale'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.png')),
					}),
				],
				batch_worker: null,
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
						default: '?input',
					}),
					typical_argument_path({
						id: 'destination_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { source_file: string; }) => (argument.source_file.replace(/(\.png)?$/i, '.scale.png')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_number({
						id: 'size_rate',
						option: null,
						checker: (argument: {}, value) => ((0.0 < value) ? null : los(`比例应大于零`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: (argument) => {
					let { source_file, destination_file, size_rate } = argument;
					KernelX.Tool.Texture.Transformation.scale_rate_fs(source_file, destination_file, size_rate);
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
						automatic: (argument: { source_file: string; }) => (argument.source_file + '.scale'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.png')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.texture.transformation.injector;