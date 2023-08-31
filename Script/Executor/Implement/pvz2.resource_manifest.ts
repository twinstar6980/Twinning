namespace TwinStar.Script.Executor.Implement.pvz2.resource_manifest {

	// ------------------------------------------------

	// convert.from
	// convert.to

	export type Configuration = {
		official_use_array_style_path: TypicalArgumentExpression<boolean>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('pvz2.resource_manifest', [
			typical_method({
				id: 'convert.from',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/(\.json)?$/i, '.regular.json')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: ({ raw_file, ripe_file }) => {
					let raw = KernelX.JSON.read_fs_js<Support.PvZ2.ResourceStreamBundle.OfficialResourceManifest.Package>(raw_file);
					let ripe = Support.PvZ2.ResourceStreamBundle.ResourceManifest.Convert.from_official(raw);
					KernelX.JSON.write_fs_js(ripe_file, ripe);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'convert.to',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/(\.json)?$/i, '.official.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_confirmation({
						id: 'use_array_style_path',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.official_use_array_style_path,
					}),
				],
				worker: ({ ripe_file, raw_file, use_array_style_path }) => {
					let ripe = KernelX.JSON.read_fs_js<Support.PvZ2.ResourceStreamBundle.ResourceManifest.Package>(ripe_file);
					let raw = Support.PvZ2.ResourceStreamBundle.ResourceManifest.Convert.to_official(ripe, use_array_style_path);
					KernelX.JSON.write_fs_js(raw_file, raw);
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

TwinStar.Script.Executor.Implement.pvz2.resource_manifest.injector;