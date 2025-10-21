namespace Twinning.Script.Executor.Implement.Texture.Encoding {

	// #region interface

	export function activate(
	): void {
		push_typical_method('texture.encoding', [
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
						automatic: (argument: { image_file: string; }) => (argument.image_file.replace(/(\.png)?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_string({
						id: 'format',
						option: KernelX.Tool.Texture.Encoding.CompositeFormatE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ image_file, data_file, format }, temporary: {}) => {
					KernelX.Tool.Texture.Encoding.encode_fs(data_file, image_file, format as any);
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.bin)$/i],
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.bin)?$/i, '.png')),
						condition: null,
					}),
					typical_argument_string({
						id: 'format',
						option: KernelX.Tool.Texture.Encoding.CompositeFormatE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'image_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implement:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'image_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implement:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ data_file, image_file, format, image_width, image_height }, temporary: {}) => {
					KernelX.Tool.Texture.Encoding.decode_fs(data_file, image_file, [image_width, image_height], format as any);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
