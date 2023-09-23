namespace TwinStar.Script.Executor.Implement.popcap.texture {

	// ------------------------------------------------

	// encode
	// decode

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.texture', [
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
						default: '?input',
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { image_file: string; }) => (argument.image_file.replace(/(\.png)?$/i, '.ptx')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'format',
						option: Support.PopCap.Texture.Encoding.FormatE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ image_file, data_file, format }) => {
					Support.PopCap.Texture.Encoding.encode_fs(image_file, data_file, format as any);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.ptx)$/i],
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
						id: 'image_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.ptx)?$/i, '.png')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'format',
						option: Support.PopCap.Texture.Encoding.FormatE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'image_width',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'image_height',
						option: null,
						checker: (argument: {}, value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ data_file, image_file, format, image_width, image_height }) => {
					Support.PopCap.Texture.Encoding.decode_fs(data_file, image_file, [image_width, image_height], format as any);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.texture.injector;