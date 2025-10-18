namespace Twinning.Script.Executor.Implement.PopCap.UTexture {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.u_texture', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.png)$/i],
				argument: [
					typical_argument_path({
						id: 'image_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { image_file: string; }) => (argument.image_file.replace(/(\.png)?$/i, '.tex')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_compress_texture_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'format',
						option: KernelX.Tool.PopCap.UTexture.FormatE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'image_file',
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
						automatic: (argument: { image_file: string; }) => (argument.image_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.png)?$/i, '.tex')),
					}),
				],
				worker: ({ image_file, data_file, version_compress_texture_data, format }, temporary: {}) => {
					KernelX.Tool.PopCap.UTexture.encode_fs(data_file, image_file, format as any, { compress_texture_data: version_compress_texture_data });
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.tex)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'image_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.tex)?$/i, '.png')),
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_compress_texture_data',
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
						id: 'image_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.tex)?$/i, '.png')),
					}),
				],
				worker: ({ data_file, image_file, version_compress_texture_data }, temporary: {}) => {
					KernelX.Tool.PopCap.UTexture.decode_fs(data_file, image_file, { compress_texture_data: version_compress_texture_data });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}