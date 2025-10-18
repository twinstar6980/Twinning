namespace Twinning.Script.Executor.Implement.Wwise.SoundBank {

	// #region interface

	export function activate(
	): void {
		push_typical_method('wwise.sound_bank', [
			typical_method({
				id: 'encode',
				filter: ['directory', /(\.bnk\.bundle)$/i],
				argument: [
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory.replace(/(\.bnk\.bundle)?$/i, '.bnk')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.Wwise.SoundBank.VersionNumberE,
						checker: null,
						automatic: null,
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
				batch: [
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory + '.pack'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.bnk\.bundle)?$/i, '.bnk')),
					}),
				],
				worker: ({ bundle_directory, data_file, version_number, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					let definition_file = `${bundle_directory}/definition.json`;
					let embedded_media_directory = `${bundle_directory}/embedded_media`;
					KernelX.Tool.Wwise.SoundBank.encode_fs(data_file, definition_file, embedded_media_directory, { number: version_number as any }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.bnk)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.bnk)?$/i, '.bnk.bundle')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.Wwise.SoundBank.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.unpack'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.bnk)?$/i, '.bnk.bundle')),
					}),
				],
				worker: ({ data_file, bundle_directory, version_number }, temporary: {}) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let embedded_media_directory = `${bundle_directory}/embedded_media`;
					KernelX.Tool.Wwise.SoundBank.decode_fs(data_file, definition_file, embedded_media_directory, { number: version_number as any });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}