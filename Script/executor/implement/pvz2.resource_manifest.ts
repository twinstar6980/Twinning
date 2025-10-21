namespace Twinning.Script.Executor.Implement.PvZ2.ResourceManifest {

	// #region interface

	export function activate(
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
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.newton')),
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
				batch: null,
				worker: ({ definition_file, data_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					Support.PvZ2.ResourceManifest.NewTypeObjectNotation.Encode.process_fs(data_file, definition_file, temporary.buffer.view());
					return;
				},
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
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.newton)?$/i, '.json')),
						condition: null,
					}),
				],
				batch: null,
				worker: ({ data_file, definition_file }, temporary: {}) => {
					Support.PvZ2.ResourceManifest.NewTypeObjectNotation.Decode.process_fs(data_file, definition_file);
					return;
				},
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
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/(\.json)?$/i, '.regular.json')),
						condition: null,
					}),
				],
				batch: null,
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					let raw = KernelX.JSON.read_fs_js(raw_file) as Support.PvZ2.ResourceManifest.Package;
					let ripe = Support.PvZ2.RegularResourceManifest.Convert.from_official(raw);
					KernelX.JSON.write_fs_js(ripe_file, ripe);
					return;
				},
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
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/(\.json)?$/i, '.official.json')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'use_array_style_path',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ ripe_file, raw_file, use_array_style_path }, temporary: {}) => {
					let ripe = KernelX.JSON.read_fs_js(ripe_file) as Support.PvZ2.RegularResourceManifest.Package;
					let raw = Support.PvZ2.RegularResourceManifest.Convert.to_official(ripe, use_array_style_path);
					KernelX.JSON.write_fs_js(raw_file, raw);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
