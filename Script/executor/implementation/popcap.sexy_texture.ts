namespace Twinning.Script.Executor.Implementation.Popcap.SexyTexture {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.sexy_texture', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.png)$/i],
				argument: [
					typical_argument_path({
						identifier: 'image_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { image_file: string; }) => (argument.image_file.replace(/(\.png)?$/i, '.tex')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.SexyTexture.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'format',
						option: KernelX.Tool.Popcap.SexyTexture.FormatE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'compress_texture_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'image_file',
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
						automatic: (argument: { image_file: string; }) => (argument.image_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.tex')),
					}),
				],
				worker: ({ image_file, data_file, version_number, format, compress_texture_data }, temporary: {}) => {
					KernelX.Tool.Popcap.SexyTexture.encode_fs(data_file, image_file, format as any, compress_texture_data, { number: version_number as any });
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.tex)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'image_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.tex)?$/i, '.png')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.SexyTexture.VersionNumberE,
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
						identifier: 'image_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.tex)?$/i, '.png')),
					}),
				],
				worker: ({ data_file, image_file, version_number }, temporary: {}) => {
					KernelX.Tool.Popcap.SexyTexture.decode_fs(data_file, image_file, { number: version_number as any });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
