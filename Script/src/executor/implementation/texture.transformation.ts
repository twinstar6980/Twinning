namespace Twinning.Script.Executor.Implementation.Texture.Transformation {

	// #region interface

	export function activate(
	): void {
		push_typical_method('texture.transformation', [
			typical_method({
				identifier: 'flipping.encode',
				filter: ['file', /(\.png)$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.png)?$/i, '.flipped.png'),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'flip_horizontal',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'flip_vertical',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.flipped'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.png)?$/i, '.png'),
					}),
				],
				worker: ({raw_file, ripe_file, flip_horizontal, flip_vertical}, store: {}) => {
					KernelX.Tool.Texture.Transformation.Flipping.encode_fs(raw_file, ripe_file, flip_horizontal, flip_vertical);
					return;
				},
			}),
			typical_method({
				identifier: 'scaling.encode',
				filter: ['file', /(\.png)$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.png)?$/i, '.scaled.png'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'target_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implementation:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'target_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implementation:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.scaled'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.png)?$/i, '.png'),
					}),
				],
				worker: ({raw_file, ripe_file, target_width, target_height}, store: {}) => {
					KernelX.Tool.Texture.Transformation.Scaling.encode_fs(raw_file, ripe_file, [target_width, target_height]);
					return;
				},
			}),
			typical_method({
				identifier: 'scaling.encode_rate',
				filter: ['file', /(\.png)$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.png)?$/i, '.scaled.png'),
						condition: null,
					}),
					typical_argument_floater({
						identifier: 'target_rate',
						option: null,
						checker: (argument: {}, value) => ((0.0 < value) ? null : los('executor.implementation:*.rate_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.scaled'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.png)?$/i, '.png'),
					}),
				],
				worker: ({raw_file, ripe_file, target_rate}, store: {}) => {
					KernelX.Tool.Texture.Transformation.Scaling.encode_rate_fs(raw_file, ripe_file, target_rate);
					return;
				},
			}),
			typical_method({
				identifier: 'tiling.encode',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.png)?$/i, '.tiled.png'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'tile_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implementation:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'tile_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implementation:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.tiled'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file, tile_width, tile_height}, store: {}) => {
					KernelX.Tool.Texture.Transformation.Tiling.encode_fs(raw_file, ripe_file, [tile_width, tile_height]);
					return;
				},
			}),
			typical_method({
				identifier: 'tiling.decode',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /(\.png)?$/i, '.detiled.png'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'tile_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implementation:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'tile_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implementation:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.detiled'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, tile_width, tile_height}, store: {}) => {
					KernelX.Tool.Texture.Transformation.Tiling.decode_fs(raw_file, ripe_file, [tile_width, tile_height]);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
