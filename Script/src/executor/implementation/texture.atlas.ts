namespace Twinning.Script.Executor.Implementation.Texture.Atlas {

	// #region interface

	export function activate(
	): void {
		push_typical_method('texture.atlas', [
			typical_method({
				identifier: 'pack',
				filter: ['file', /(\.atlas\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'sprite_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.atlas\.json)?$/i, '.sprite'),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'atlas_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.atlas\.json)?$/i, '.atlas.png'),
						condition: null,
					}),
				],
				batch: null,
				worker: ({definition_file, sprite_directory, atlas_file}, store: {}) => {
					Support.Atlas.Pack.pack_fs(definition_file, atlas_file, sprite_directory);
					return;
				},
			}),
			typical_method({
				identifier: 'unpack',
				filter: ['file', /(\.atlas\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'atlas_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.atlas\.json)?$/i, '.atlas.png'),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'sprite_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.atlas\.json)?$/i, '.sprite'),
						condition: null,
					}),
				],
				batch: null,
				worker: ({definition_file, atlas_file, sprite_directory}, store: {}) => {
					Support.Atlas.Pack.unpack_fs(definition_file, atlas_file, sprite_directory);
					return;
				},
			}),
			typical_method({
				identifier: 'pack_automatic',
				filter: ['directory', /(\.sprite)$/i],
				argument: [
					typical_argument_path({
						identifier: 'sprite_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'atlas_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {sprite_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.sprite_directory, /(\.sprite)?$/i, '.atlas.png'),
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {sprite_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.sprite_directory, /(\.sprite)?$/i, '.atlas.json'),
						condition: null,
					}),
				],
				batch: null,
				worker: ({sprite_directory, atlas_file, definition_file}, store: {}) => {
					Support.Atlas.Pack.pack_automatic_fs(definition_file, atlas_file, sprite_directory, 'exponent_of_2');
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
