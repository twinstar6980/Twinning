namespace Twinning.Script.Executor.Implement.Popcap.Texture {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.texture', [
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
						automatic: (argument: { image_file: string; }) => (argument.image_file.replace(/(\.png)?$/i, '.ptx')),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'format',
						option: Support.Popcap.Texture.Encoding.FormatE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ image_file, data_file, format }, temporary: {}) => {
					Support.Popcap.Texture.Encoding.encode_fs(image_file, data_file, format as any);
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.ptx)$/i],
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.ptx)?$/i, '.png')),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'format',
						option: Support.Popcap.Texture.Encoding.FormatE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'image_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implement:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'image_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los('executor.implement:*.size_should_be_bigger_then_zero')),
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ data_file, image_file, format, image_width, image_height }, temporary: {}) => {
					Support.Popcap.Texture.Encoding.decode_fs(data_file, image_file, [image_width, image_height], format as any);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
