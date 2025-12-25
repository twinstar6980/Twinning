namespace Twinning.Script.Executor.Implement.Popcap.Animation {

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
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.pam\.json)?$/i, '.pam')),
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
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam\.json)?$/i, '.pam')),
					}),
				],
				worker: ({ definition_file, data_file, version_number, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Popcap.Animation.encode_fs(data_file, definition_file, { number: version_number as any }, temporary.buffer.view());
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.pam)?$/i, '.pam.json')),
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
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam)?$/i, '.pam.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number }, temporary: {}) => {
					KernelX.Tool.Popcap.Animation.decode_fs(data_file, definition_file, { number: version_number as any });
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
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/(\.pam\.json)?$/i, '.pam.xfl')),
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
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.from'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam\.json)?$/i, '.pam.xfl')),
					}),
				],
				worker: ({ raw_file, ripe_directory, version_number }, temporary: {}) => {
					let raw = KernelX.Json.read_fs_js(raw_file) as Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation;
					Support.Popcap.Animation.Convert.Flash.From.from_fsh(raw, ripe_directory, { number: version_number as any });
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
						automatic: (argument: { ripe_directory: string; }) => (argument.ripe_directory.replace(/(\.pam\.xfl)?$/i, '.pam.json')),
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
						automatic: (argument: { ripe_directory: string; }) => (argument.ripe_directory + '.to'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam\.xfl)?$/i, '.pam.json')),
					}),
				],
				worker: ({ ripe_directory, raw_file, version_number }, temporary: {}) => {
					Support.Popcap.Animation.Convert.Flash.To.to_fs(raw_file, ripe_directory, { number: version_number as any });
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
				worker: ({ target_directory, resolution }, temporary: {}) => {
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
				worker: ({ target_directory }, temporary: {}) => {
					let media_directory = `${target_directory}/LIBRARY/media`;
					KernelX.Storage.remove_if(media_directory);
					KernelX.Storage.create_directory(media_directory);
					KernelX.Storage.list_directory(`${target_directory}/..`, 1n, true, false)
						.filter((value) => (/(\.png)$/i.test(value)))
						.forEach((value) => {
							KernelX.Storage.create_hard_link(`${media_directory}/${value}`, `${target_directory}/../${value}`);
						});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
