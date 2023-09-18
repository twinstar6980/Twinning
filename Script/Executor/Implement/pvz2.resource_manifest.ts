namespace TwinStar.Script.Executor.Implement.pvz2.resource_manifest {

	// ------------------------------------------------

	// new_type_object_notation.encode
	// new_type_object_notation.decode
	// regular.from
	// regular.to

	export type Configuration = {
		official_use_array_style_path: TypicalArgumentExpression<boolean>;
		new_type_object_notation_encode_buffer_size: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('pvz2.resource_manifest', [
			typical_method({
				id: 'new_type_object_notation.encode',
				filter: ['file', /(\.json)$/i],
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
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.newton')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.new_type_object_notation_encode_buffer_size,
					}),
				],
				worker: ({ definition_file, data_file, buffer_size }) => {
					Support.PvZ2.ResourceManifest.NewTypeObjectNotation.Encode.process_fs(data_file, definition_file, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'new_type_object_notation.decode',
				filter: ['file', /(\.newton)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.newton)?$/i, '.json')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: ({ data_file, definition_file }) => {
					Support.PvZ2.ResourceManifest.NewTypeObjectNotation.Decode.process_fs(data_file, definition_file);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'regular.from',
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
					let raw = KernelX.JSON.read_fs_js<Support.PvZ2.ResourceManifest.Package>(raw_file);
					let ripe = Support.PvZ2.RegularResourceManifest.Convert.from_official(raw);
					KernelX.JSON.write_fs_js(ripe_file, ripe);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'regular.to',
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
					let ripe = KernelX.JSON.read_fs_js<Support.PvZ2.RegularResourceManifest.Package>(ripe_file);
					let raw = Support.PvZ2.RegularResourceManifest.Convert.to_official(ripe, use_array_style_path);
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