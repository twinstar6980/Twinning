namespace TwinStar.Script.Executor.Implement.texture.atlas {

	// ------------------------------------------------

	// pack
	// unpack
	// pack_automatic

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('texture.atlas', [
			typical_method({
				id: 'pack',
				filter: ['file', /(\.atlas\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'sprite_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.atlas\.json)?$/i, '.sprite')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_path({
						id: 'atlas_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.atlas\.json)?$/i, '.atlas.png')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: (argument) => {
					let { definition_file, sprite_directory, atlas_file } = argument;
					Support.Atlas.Pack.pack_fs(definition_file, atlas_file, sprite_directory);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'unpack',
				filter: ['file', /(\.atlas\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'atlas_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.atlas\.json)?$/i, '.atlas.png')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_path({
						id: 'sprite_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.atlas\.json)?$/i, '.sprite')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: (argument) => {
					let { definition_file, atlas_file, sprite_directory } = argument;
					Support.Atlas.Pack.unpack_fs(definition_file, atlas_file, sprite_directory);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'pack_automatic',
				filter: ['directory', /(\.sprite)$/i],
				argument: [
					typical_argument_path({
						id: 'sprite_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'atlas_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { sprite_directory: string; }) => (argument.sprite_directory.replace(/(\.sprite)?$/i, '.atlas.png')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { sprite_directory: string; }) => (argument.sprite_directory.replace(/(\.sprite)?$/i, '.atlas.json')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: (argument) => {
					let { sprite_directory, atlas_file, definition_file } = argument;
					Support.Atlas.Pack.pack_automatic_fs(definition_file, atlas_file, sprite_directory);
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

TwinStar.Script.Executor.Implement.texture.atlas.injector;