namespace TwinStar.Script.Executor.Implement.popcap.animation {

	// ------------------------------------------------

	// encode *
	// decode *
	// convert.flash.from *
	// convert.flash.to *
	// convert.flash.resize *
	// convert.flash.link_media *

	export type Configuration = {
		version_number: TypicalArgumentExpression<bigint>;
		encode_buffer_size: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.animation', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.pam\.json)$/i],
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
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.pam\.json)?$/i, '.pam')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.encode_buffer_size,
					}),
				],
				worker: ({ definition_file, data_file, version_number, buffer_size }) => {
					KernelX.Tool.PopCap.Animation.encode_fs(data_file, definition_file, { number: version_number as any }, buffer_size);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam\.json)?$/i, '.pam')),
					}),
				],
				batch_worker: ({ definition_file, data_file, version_number, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.Animation.encode_fs(data_file, definition_file, { number: version_number as any }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.pam)$/i],
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.pam)?$/i, '.pam.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.Animation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
				],
				worker: ({ data_file, definition_file, version_number }) => {
					KernelX.Tool.PopCap.Animation.decode_fs(data_file, definition_file, { number: version_number as any });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam)?$/i, '.pam.json')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'convert.flash.from',
				filter: ['file', /(\.pam\.json)$/i],
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
						id: 'ripe_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/(\.pam\.json)?$/i, '.pam.xfl')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: ({ raw_file, ripe_directory }) => {
					let raw = KernelX.JSON.read_fs_js<Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation>(raw_file);
					Support.PopCap.Animation.Convert.Flash.From.from_fsh(raw, ripe_directory);
					Support.PopCap.Animation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
					Support.PopCap.Animation.Convert.Flash.create_xfl_content_file(ripe_directory);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.convert_flash_from'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam\.json)?$/i, '.pam.xfl')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'convert.flash.to',
				filter: ['directory', /(\.pam\.xfl)$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_directory: string; }) => (argument.ripe_directory.replace(/(\.pam\.xfl)?$/i, '.pam.json')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: ({ ripe_directory, raw_file }) => {
					Support.PopCap.Animation.Convert.Flash.To.to_fs(raw_file, ripe_directory);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'ripe_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_directory: string; }) => (argument.ripe_directory + '.convert_flash_to'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.pam\.xfl)?$/i, '.pam.json')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'convert.flash.resize',
				filter: ['directory', /(\.pam\.xfl)$/i],
				argument: [
					typical_argument_path({
						id: 'target_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'resolution',
						option: null,
						checker: (argument, value) => (value > 0n ? null : los(`分辨率应大于零`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ target_directory, resolution }) => {
					Support.PopCap.Animation.Convert.Flash.SourceManager.resize_fs(target_directory, resolution);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'target_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'convert.flash.link_media',
				filter: ['directory', /(\.pam\.xfl)$/i],
				argument: [
					typical_argument_path({
						id: 'target_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ target_directory }) => {
					let media_directory = `${target_directory}/LIBRARY/media`;
					if (KernelX.FileSystem.exist(media_directory)) {
						KernelX.FileSystem.remove(media_directory);
					}
					KernelX.FileSystem.create_directory(media_directory);
					KernelX.FileSystem.list_file(`${target_directory}/..`, 1n)
						.filter((e) => (/(\.png)$/i.test(e)))
						.forEach((e) => {
							KernelX.FileSystem.create_hard_link(`${media_directory}/${e}`, `${target_directory}/../${e}`);
						});
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'target_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.animation.injector;