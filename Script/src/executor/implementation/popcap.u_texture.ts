namespace Twinning.Script.Executor.Implementation.Popcap.UTexture {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.u_texture', [
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
						automatic: (argument: {image_file: StoragePath}) => ConvertHelper.replace_path_name(argument.image_file, /(\.png)?$/i, '.tex'),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_compress_texture_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'format',
						option: KernelX.Tool.Popcap.UTexture.FormatE,
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
						automatic: (argument: {image_file: StoragePath}) => ConvertHelper.replace_path_name(argument.image_file, /()?$/i, '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.png)?$/i, '.tex'),
					}),
				],
				worker: ({image_file, data_file, version_compress_texture_data, format}, store: {}) => {
					KernelX.Tool.Popcap.UTexture.encode_fs(data_file, image_file, format as any, {compress_texture_data: version_compress_texture_data});
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.tex)?$/i, '.png'),
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_compress_texture_data',
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.tex)?$/i, '.png'),
					}),
				],
				worker: ({data_file, image_file, version_compress_texture_data}, store: {}) => {
					KernelX.Tool.Popcap.UTexture.decode_fs(data_file, image_file, {compress_texture_data: version_compress_texture_data});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
