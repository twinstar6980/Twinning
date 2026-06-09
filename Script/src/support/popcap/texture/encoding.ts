namespace Twinning.Script.Support.Popcap.Texture.Encoding {

	// #region common

	const FormatX = [
		'rgba_8888_o',
		'argb_8888',
		'rgba_4444',
		'rgb_565',
		'rgba_5551',
		'rgba_4444_tiled',
		'rgb_565_tiled',
		'rgba_5551_tiled',
		'rgba_pvrtc1_4bpp',
		'rgb_pvrtc1_4bpp_a_8',
		'rgb_etc1_a_8',
		'rgb_etc1_a_palette',
		'rgba_astc_4x4',
		'rgba_astc_5x5',
		'rgba_astc_6x6',
		'rgba_astc_8x8',
	] as const;

	export type Format = typeof FormatX[number];

	export const FormatE = FormatX as unknown as Format[];

	// ----------------

	export type EncodeOption = {
		rgb_etc1_a_palette: null | {
			palette: KernelX.Image.ColorList;
		};
	};

	// ----------------

	const k_standard_format_of_a8 = {endian: false, channel: [['alpha', 8n]]} as KernelX.Tool.Texture.Encoding.Format;

	function get_standard_format(
		format: Format,
	): KernelX.Tool.Texture.Encoding.Format | null {
		let result: KernelX.Tool.Texture.Encoding.Format | null = null;
		switch (format) {
			case 'rgba_8888_o': {
				result = {
					endian: true,
					channel: [
						['red', 8n],
						['green', 8n],
						['blue', 8n],
						['alpha', 8n],
					],
				};
				break;
			}
			case 'argb_8888': {
				result = {
					endian: false,
					channel: [
						['alpha', 8n],
						['red', 8n],
						['green', 8n],
						['blue', 8n],
					],
				};
				break;
			}
			case 'rgba_4444': {
				result = {
					endian: false,
					channel: [
						['red', 4n],
						['green', 4n],
						['blue', 4n],
						['alpha', 4n],
					],
				};
				break;
			}
			case 'rgb_565': {
				result = {
					endian: false,
					channel: [
						['red', 5n],
						['green', 6n],
						['blue', 5n],
					],
				};
				break;
			}
			case 'rgba_5551': {
				result = {
					endian: false,
					channel: [
						['red', 5n],
						['green', 5n],
						['blue', 5n],
						['alpha', 1n],
					],
				};
				break;
			}
		}
		return result;
	}

	// ----------------

	export function get_dummy_channel_filler(
		format: Format,
	): [null | bigint, null | bigint, null | bigint, null | bigint] {
		let channel: Array<string>;
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				channel = format.split('_')[0].split('');
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				channel = format.split('_')[0].split('');
				break;
			}
			case 'rgba_pvrtc1_4bpp':
			case 'rgb_pvrtc1_4bpp_a_8': {
				channel = format.split('_')[0].split('');
				if (format === 'rgb_pvrtc1_4bpp_a_8') {
					channel.push('a');
				}
				break;
			}
			case 'rgb_etc1_a_8':
			case 'rgb_etc1_a_palette': {
				channel = format.split('_')[0].split('');
				channel.push('a');
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				channel = format.split('_')[0].split('');
				break;
			}
		}
		return [
			channel.includes('r') ? null : 0n,
			channel.includes('g') ? null : 0n,
			channel.includes('b') ? null : 0n,
			channel.includes('a') ? null : 255n,
		];
	}

	export function get_block_size(
		format: Format,
	): KernelX.Image.ImageSize {
		let result: KernelX.Image.ImageSize;
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				result = [1n, 1n];
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				result = [32n, 32n];
				break;
			}
			case 'rgba_pvrtc1_4bpp':
			case 'rgb_pvrtc1_4bpp_a_8': {
				result = [4n, 4n];
				break;
			}
			case 'rgb_etc1_a_8':
			case 'rgb_etc1_a_palette': {
				result = [4n, 4n];
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				let block_size_expression = format.substring('rgba_astc_'.length).split('x');
				result = [BigInt(block_size_expression[0]), BigInt(block_size_expression[1])];
				break;
			}
		}
		return result;
	}

	export function compute_data_size(
		size: KernelX.Image.ImageSize,
		format: Format,
		option: EncodeOption,
	): bigint {
		let result = 0n;
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				let standard_format = get_standard_format(format);
				assert_test(standard_format !== null);
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, standard_format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				let standard_format = get_standard_format(format.slice(0, -6) as Format);
				assert_test(standard_format !== null);
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, standard_format);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				result += KernelX.Tool.Texture.Compression.compute_data_size(size, 'rgba_pvrtc1_4bpp');
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				result += KernelX.Tool.Texture.Compression.compute_data_size(size, 'rgb_pvrtc1_4bpp');
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, k_standard_format_of_a8);
				break;
			}
			case 'rgb_etc1_a_8': {
				result += KernelX.Tool.Texture.Compression.compute_data_size(size, 'rgb_etc1');
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, k_standard_format_of_a8);
				break;
			}
			case 'rgb_etc1_a_palette': {
				assert_test(option.rgb_etc1_a_palette !== null);
				result += KernelX.Tool.Texture.Compression.compute_data_size(size, 'rgb_etc1');
				result += KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.compute_data_size_with_palette(size, option.rgb_etc1_a_palette.palette.length);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				result += KernelX.Tool.Texture.Compression.compute_data_size(size, format);
				break;
			}
		}
		return result;
	}

	export function compute_padded_image_size(
		origin_size: KernelX.Image.ImageSize,
		format: Format,
	): KernelX.Image.ImageSize {
		let block_size = get_block_size(format);
		let padded_size: KernelX.Image.ImageSize = [
			ConvertHelper.compute_padded_size(origin_size[0], block_size[0]),
			ConvertHelper.compute_padded_size(origin_size[1], block_size[1]),
		];
		if (format.includes('etc') || format.includes('pvrtc')) {
			padded_size = [
				ConvertHelper.compute_padded_size_of_power_of_two(padded_size[0]),
				ConvertHelper.compute_padded_size_of_power_of_two(padded_size[1]),
			];
		}
		if (format.includes('pvrtc')) {
			let maximum_size = padded_size[0] > padded_size[1] ? padded_size[0] : padded_size[1];
			padded_size = [maximum_size, maximum_size];
		}
		return padded_size;
	}

	export function get_bpp_for_pitch(
		format: Format,
	): bigint {
		let result = 0n;
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				let standard_format = get_standard_format(format);
				assert_test(standard_format !== null);
				result += KernelX.Tool.Texture.Encoding.get_pixel_bit_count(standard_format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				let standard_format = get_standard_format(format.slice(0, -6) as Format);
				assert_test(standard_format !== null);
				result += KernelX.Tool.Texture.Encoding.get_pixel_bit_count(standard_format);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				let block_size = get_block_size(format);
				result += KernelX.Tool.Texture.Compression.get_block_bit_count('rgba_pvrtc1_4bpp') / (block_size[0] * block_size[1]);
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				// TODO
				let block_size = get_block_size(format);
				result += KernelX.Tool.Texture.Compression.get_block_bit_count('rgb_pvrtc1_4bpp') / (block_size[0] * block_size[1]);
				break;
			}
			case 'rgb_etc1_a_8':
			case 'rgb_etc1_a_palette': {
				let standard_format = get_standard_format('rgba_8888_o');
				assert_test(standard_format !== null);
				result += KernelX.Tool.Texture.Encoding.get_pixel_bit_count(standard_format);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				// TODO
				result += 0n;
				break;
			}
		}
		return result;
	}

	// #endregion

	// #region utility

	export function encode(
		data: Kernel.OutputByteStreamView,
		image: Kernel.Image.ConstantImageView,
		format: Format,
		option: EncodeOption,
	): void {
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				let standard_format = get_standard_format(format);
				assert_test(standard_format !== null);
				KernelX.Tool.Texture.Encoding.encode(data, image, standard_format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				let tiled_image = Kernel.Image.Image.allocate(image.size());
				let tiled_image_view = tiled_image.view();
				KernelX.Tool.Texture.Transformation.Tiling.encode(image, tiled_image_view, get_block_size(format));
				let standard_format = get_standard_format(format.slice(0, -6) as Format);
				assert_test(standard_format !== null);
				KernelX.Tool.Texture.Encoding.encode(data, tiled_image_view, standard_format);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				KernelX.Tool.Texture.Compression.compress(data, image, 'rgba_pvrtc1_4bpp');
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				KernelX.Tool.Texture.Compression.compress(data, image, 'rgb_pvrtc1_4bpp');
				KernelX.Tool.Texture.Encoding.encode(data, image, k_standard_format_of_a8);
				break;
			}
			case 'rgb_etc1_a_8': {
				KernelX.Tool.Texture.Compression.compress(data, image, 'rgb_etc1');
				KernelX.Tool.Texture.Encoding.encode(data, image, k_standard_format_of_a8);
				break;
			}
			case 'rgb_etc1_a_palette': {
				assert_test(option.rgb_etc1_a_palette !== null);
				KernelX.Tool.Texture.Compression.compress(data, image, 'rgb_etc1');
				KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.encode_with_palette(data, image, option.rgb_etc1_a_palette.palette);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				KernelX.Tool.Texture.Compression.compress(data, image, format);
				break;
			}
		}
		return;
	}

	export function decode(
		data: Kernel.InputByteStreamView,
		image: Kernel.Image.VariableImageView,
		format: Format,
		fill_dummy: Boolean,
	): void {
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				let standard_format = get_standard_format(format);
				assert_test(standard_format !== null);
				KernelX.Tool.Texture.Encoding.decode(data, image, standard_format, false);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				let original_image = Kernel.Image.Image.allocate(image.size());
				let original_image_view = original_image.view();
				let standard_format = get_standard_format(format.slice(0, -6) as Format);
				assert_test(standard_format !== null);
				KernelX.Tool.Texture.Encoding.decode(data, image, standard_format, false);
				KernelX.Tool.Texture.Transformation.Tiling.decode(original_image_view, image, get_block_size(format));
				image.draw(original_image_view);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				KernelX.Tool.Texture.Compression.uncompress(data, image, 'rgba_pvrtc1_4bpp', false);
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				KernelX.Tool.Texture.Compression.uncompress(data, image, 'rgb_pvrtc1_4bpp', false);
				KernelX.Tool.Texture.Encoding.decode(data, image, k_standard_format_of_a8, false);
				break;
			}
			case 'rgb_etc1_a_8': {
				KernelX.Tool.Texture.Compression.uncompress(data, image, 'rgb_etc1', false);
				KernelX.Tool.Texture.Encoding.decode(data, image, k_standard_format_of_a8, false);
				break;
			}
			case 'rgb_etc1_a_palette': {
				KernelX.Tool.Texture.Compression.uncompress(data, image, 'rgb_etc1', false);
				KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.decode_with_palette(data, image);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				KernelX.Tool.Texture.Compression.uncompress(data, image, format, false);
				break;
			}
		}
		if (fill_dummy) {
			let dummy_channel_filler = get_dummy_channel_filler(format);
			if (dummy_channel_filler.some((it) => it !== null)) {
				KernelX.Tool.Texture.Transformation.Filling.encode(image, image, dummy_channel_filler[0], dummy_channel_filler[1], dummy_channel_filler[2], dummy_channel_filler[3]);
			}
		}
		return;
	}

	// ----------------

	export function encode_fs(
		data_file: StoragePath,
		image_file: StoragePath,
		format: Format,
	): void {
		let image = KernelX.Tool.Texture.Conversion.Png.decode_fs_of(image_file, [false, 'dynamic', (size) => compute_padded_image_size(size, format)]);
		let image_view = image.view();
		let image_size = image_view.size().value;
		let option: EncodeOption = {
			rgb_etc1_a_palette: null,
		};
		if (format === 'rgb_etc1_a_palette') {
			option.rgb_etc1_a_palette = {
				palette: KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.evaluate_palette(image_view),
			};
		}
		let data_size = compute_data_size(image_size, format, option);
		let data = Kernel.ByteArray.allocate(Kernel.Size.value(data_size));
		let data_stream = Kernel.ByteStreamView.watch(data.view());
		encode(data_stream, image_view, format, option);
		StorageHelper.write_file(data_file, data_stream.stream_view());
		return;
	}

	export function decode_fs(
		data_file: StoragePath,
		image_file: StoragePath,
		image_size: KernelX.Image.ImageSize,
		format: Format,
	): void {
		let data = StorageHelper.read_file(data_file);
		let data_stream = Kernel.ByteStreamView.watch(data.view());
		let image_size_padded = compute_padded_image_size(image_size, format);
		let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size_padded));
		let image_view = image.view();
		decode(data_stream, image_view, format, true);
		KernelX.Tool.Texture.Conversion.Png.encode_fs(image_file, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
		return;
	}

	// #endregion

}
