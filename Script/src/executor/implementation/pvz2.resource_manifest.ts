namespace Twinning.Script.Executor.Implementation.PvZ2.ResourceManifest {

	// #region interface

	export function activate(
	): void {
		push_typical_method('pvz2.resource_manifest', [
			typical_method({
				identifier: 'new_type_object_notation.encode',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.json)?$/i, '.newton'),
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({definition_file, data_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					Support.Pvz2.ResourceManifest.NewTypeObjectNotation.Encode.process_fs(data_file, definition_file, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'new_type_object_notation.decode',
				filter: ['file', /(\.newton)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.newton)?$/i, '.json'),
						condition: null,
					}),
				],
				batch: null,
				worker: ({data_file, definition_file}, store: {}) => {
					Support.Pvz2.ResourceManifest.NewTypeObjectNotation.Decode.process_fs(data_file, definition_file);
					return;
				},
			}),
			typical_method({
				identifier: 'regular.from',
				filter: ['file', /(\.json)$/i],
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
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.json)?$/i, '.regular.json'),
						condition: null,
					}),
				],
				batch: null,
				worker: ({raw_file, ripe_file}, store: {}) => {
					let raw = JsonHelper.decode_file(raw_file) as Support.Pvz2.ResourceManifest.Package;
					let ripe = Support.Pvz2.RegularResourceManifest.Convert.from_official(raw);
					JsonHelper.encode_file(ripe_file, ripe);
					return;
				},
			}),
			typical_method({
				identifier: 'regular.to',
				filter: ['file', /(\.json)$/i],
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
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /(\.json)?$/i, '.official.json'),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'use_array_style_path',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ripe_file, raw_file, use_array_style_path}, store: {}) => {
					let ripe = JsonHelper.decode_file(ripe_file) as Support.Pvz2.RegularResourceManifest.Package;
					let raw = Support.Pvz2.RegularResourceManifest.Convert.to_official(ripe, use_array_style_path);
					JsonHelper.encode_file(raw_file, raw);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
