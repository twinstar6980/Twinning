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

	export function is_opacity_format(
		format: Format,
	): boolean {
		return [
			'rgb_565',
			'rgb_565_tiled',
		].includes(format);
	}

	// ----------------

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
				// TODO
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
				if (format === 'rgba_astc_4x4') {
					result = [4n, 4n];
				}
				if (format === 'rgba_astc_5x5') {
					result = [5n, 5n];
				}
				if (format === 'rgba_astc_6x6') {
					result = [6n, 6n];
				}
				if (format === 'rgba_astc_8x8') {
					result = [8n, 8n];
				}
				result = result!;
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
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, format.slice(0, -6) as KernelX.Tool.Texture.Encoding.CompositeFormat);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgba_pvrtc1_4bpp');
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_pvrtc1_4bpp');
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'a_8');
				break;
			}
			case 'rgb_etc1_a_8': {
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_etc1');
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'a_8');
				break;
			}
			case 'rgb_etc1_a_palette': {
				assert_test(option.rgb_etc1_a_palette !== null);
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_etc1');
				result += KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.compute_data_size_with_palette(size, option.rgb_etc1_a_palette.palette.length);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				result += KernelX.Tool.Texture.Encoding.compute_data_size(size, format);
				break;
			}
		}
		return result;
	}

	export function compute_padded_image_size(
		origin_size: KernelX.Image.ImageSize,
		format: Format,
	): KernelX.Image.ImageSize {
		let compute_block = (t: bigint, n: bigint) => {
			return t % n === 0n ? (t) : ((t / n + 1n) * n);
		};
		let compute_exponent_of_2 = (t: bigint) => {
			let r = 0b1n << 1n;
			while (r < t) {
				r <<= 1n;
			}
			return r;
		};
		let block_size = get_block_size(format);
		let padded_size: KernelX.Image.ImageSize = [compute_block(origin_size[0], block_size[0]), compute_block(origin_size[1], block_size[1])];
		if (format.includes('etc')) {
			padded_size = [compute_exponent_of_2(padded_size[0]), compute_exponent_of_2(padded_size[1])];
		}
		else if (format.includes('pvrtc')) {
			let padded_width = compute_exponent_of_2(padded_size[0]);
			let padded_height = compute_exponent_of_2(padded_size[1]);
			let maximum_size = padded_width > padded_height ? padded_width : padded_height;
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
				result += KernelX.Tool.Texture.Encoding.get_block_bit_count(format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				result += KernelX.Tool.Texture.Encoding.get_block_bit_count(format.slice(0, -6) as KernelX.Tool.Texture.Encoding.CompositeFormat);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				let block_size = get_block_size(format);
				result += KernelX.Tool.Texture.Encoding.get_block_bit_count('rgba_pvrtc1_4bpp') / (block_size[0] * block_size[1]);
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				// TODO
				let block_size = get_block_size(format);
				result += KernelX.Tool.Texture.Encoding.get_block_bit_count('rgb_pvrtc1_4bpp') / (block_size[0] * block_size[1]);
				break;
			}
			case 'rgb_etc1_a_8':
			case 'rgb_etc1_a_palette': {
				result += KernelX.Tool.Texture.Encoding.get_block_bit_count('rgba_8888_o');
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
				KernelX.Tool.Texture.Encoding.encode(data, image, format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				KernelX.Tool.Miscellaneous.XboxTiledTexture.encode(data, image, format.slice(0, -6) as KernelX.Tool.Texture.Encoding.Format);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgba_pvrtc1_4bpp');
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_pvrtc1_4bpp');
				KernelX.Tool.Texture.Encoding.encode(data, image, 'a_8');
				break;
			}
			case 'rgb_etc1_a_8': {
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_etc1');
				KernelX.Tool.Texture.Encoding.encode(data, image, 'a_8');
				break;
			}
			case 'rgb_etc1_a_palette': {
				assert_test(option.rgb_etc1_a_palette !== null);
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_etc1');
				KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.encode_with_palette(data, image, option.rgb_etc1_a_palette.palette);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				KernelX.Tool.Texture.Encoding.encode(data, image, format);
				break;
			}
		}
		return;
	}

	export function decode(
		data: Kernel.InputByteStreamView,
		image: Kernel.Image.VariableImageView,
		format: Format,
	): void {
		if (is_opacity_format(format)) {
			image.fill(Kernel.Image.Pixel.value([0xFFn, 0xFFn, 0xFFn, 0xFFn]));
		}
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				KernelX.Tool.Texture.Encoding.decode(data, image, format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				KernelX.Tool.Miscellaneous.XboxTiledTexture.decode(data, image, format.slice(0, -6) as KernelX.Tool.Texture.Encoding.Format);
				break;
			}
			case 'rgba_pvrtc1_4bpp': {
				KernelX.Tool.Texture.Encoding.decode(data, image, 'rgba_pvrtc1_4bpp');
				break;
			}
			case 'rgb_pvrtc1_4bpp_a_8': {
				KernelX.Tool.Texture.Encoding.decode(data, image, 'rgb_pvrtc1_4bpp');
				KernelX.Tool.Texture.Encoding.decode(data, image, 'a_8');
				break;
			}
			case 'rgb_etc1_a_8': {
				KernelX.Tool.Texture.Encoding.decode(data, image, 'rgb_etc1');
				KernelX.Tool.Texture.Encoding.decode(data, image, 'a_8');
				break;
			}
			case 'rgb_etc1_a_palette': {
				KernelX.Tool.Texture.Encoding.decode(data, image, 'rgb_etc1');
				KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.decode_with_palette(data, image);
				break;
			}
			case 'rgba_astc_4x4':
			case 'rgba_astc_5x5':
			case 'rgba_astc_6x6':
			case 'rgba_astc_8x8': {
				KernelX.Tool.Texture.Encoding.decode(data, image, format);
				break;
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
		let image_data = StorageHelper.read_file(image_file);
		let image_stream = Kernel.ByteStreamView.watch(image_data.view());
		let image_size = KernelX.Tool.Texture.File.Png.size(image_stream.view());
		let image_size_padded = compute_padded_image_size(image_size, format);
		let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size_padded));
		let image_view = image.view();
		KernelX.Tool.Texture.File.Png.read(image_stream, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
		let option: EncodeOption = {
			rgb_etc1_a_palette: null,
		};
		if (format === 'rgb_etc1_a_palette') {
			option.rgb_etc1_a_palette = {
				palette: KernelX.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.evaluate_palette(image_view),
			};
		}
		let data_size = compute_data_size(image_size_padded, format, option);
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
		decode(data_stream, image_view, format);
		KernelX.Tool.Texture.File.Png.write_fs(image_file, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
		return;
	}

	// #endregion

}
