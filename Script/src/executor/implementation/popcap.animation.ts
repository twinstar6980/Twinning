namespace Twinning.Script.Executor.Implementation.Popcap.Animation {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.animation', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.pam\.json)$/i],
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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.pam\.json)?$/i, '.pam'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
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
				batch: [
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /()?$/i, '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pam\.json)?$/i, '.pam'),
					}),
				],
				worker: ({definition_file, data_file, version_number, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Popcap.Animation.encode_fs(data_file, definition_file, {number: version_number as any}, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.pam)$/i],
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.pam)?$/i, '.pam.json'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pam)?$/i, '.pam.json'),
					}),
				],
				worker: ({data_file, definition_file, version_number}, store: {}) => {
					KernelX.Tool.Popcap.Animation.decode_fs(data_file, definition_file, {number: version_number as any});
					return;
				},
			}),
			typical_method({
				identifier: 'convert.flash.from',
				filter: ['file', /(\.pam\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /(\.pam\.json)?$/i, '.pam.xfl'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Animation.VersionNumberE,
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
						identifier: 'ripe_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.from'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pam\.json)?$/i, '.pam.xfl'),
					}),
				],
				worker: ({raw_file, ripe_directory, version_number}, store: {}) => {
					let raw = JsonHelper.decode_file(raw_file) as Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation;
					Support.Popcap.Animation.Convert.Flash.From.from_fsh(raw, ripe_directory, {number: version_number as any});
					Support.Popcap.Animation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw, null);
					Support.Popcap.Animation.Convert.Flash.create_xfl_content_file(ripe_directory);
					return;
				},
			}),
			typical_method({
				identifier: 'convert.flash.to',
				filter: ['directory', /(\.pam\.xfl)$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_directory, /(\.pam\.xfl)?$/i, '.pam.json'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_directory',
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
						automatic: (argument: {ripe_directory: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_directory, /()?$/i, '.to'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.pam\.xfl)?$/i, '.pam.json'),
					}),
				],
				worker: ({ripe_directory, raw_file, version_number}, store: {}) => {
					Support.Popcap.Animation.Convert.Flash.To.to_fs(raw_file, ripe_directory, {number: version_number as any});
					return;
				},
			}),
			typical_method({
				identifier: 'convert.flash.resize',
				filter: ['directory', /(\.pam\.xfl)$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'resolution',
						option: null,
						checker: (argument, value) => (value > 0n ? null : los('executor.implement:*.resolution_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_directory, resolution}, store: {}) => {
					Support.Popcap.Animation.Convert.Flash.SourceManager.resize_fs(target_directory, resolution);
					return;
				},
			}),
			typical_method({
				identifier: 'convert.flash.link_media',
				filter: ['directory', /(\.pam\.xfl)$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_directory}, store: {}) => {
					let media_directory = target_directory.join('LIBRARY').join('media');
					StorageHelper.remove_if(media_directory);
					StorageHelper.create_directory(media_directory);
					StorageHelper.list_directory(target_directory.join('..'), 1n, true, false, true, false)
						.filter((value) => (/(\.png)$/i.test(value.name()!)))
						.forEach((value) => {
							StorageHelper.create_link(media_directory.push(value), target_directory.join('..').push(value), false);
						});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
