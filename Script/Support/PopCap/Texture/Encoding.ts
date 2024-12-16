namespace Twinning.Script.Support.PopCap.Texture.Encoding {

	// ------------------------------------------------

	const FormatX = [
		'rgba_8888_o',
		'argb_8888',
		'rgba_4444',
		'rgb_565',
		'rgba_5551',
		'rgba_4444_tiled',
		'rgb_565_tiled',
		'rgba_5551_tiled',
		'rgba_pvrtc4',
		'rgb_pvrtc4_a_8',
		'rgb_etc1_a_8',
		'rgb_etc1_a_palette',
	] as const;

	export type Format = typeof FormatX[number];

	export const FormatE = FormatX as unknown as Format[];

	// ------------------------------------------------

	export type EncodeOption = {
		rgb_etc1_a_palette: null | {
			palette: KernelX.Image.ColorList;
		};
	};

	// ------------------------------------------------

	export function is_opacity_format(
		format: Format,
	): boolean {
		return [
			'rgb_565',
			'rgb_565_tiled',
		].includes(format);
	}

	// ------------------------------------------------

	export function compute_padded_image_size(
		origin_size: KernelX.Image.ImageSize,
		format: Format,
	): KernelX.Image.ImageSize {
		let compute = (t: bigint) => {
			let r = 0b1n << 1n;
			while (r < t) {
				r <<= 1n;
			}
			return r;
		};
		let padded_size: KernelX.Image.ImageSize;
		if (format.includes('etc1')) {
			padded_size = [compute(origin_size[0]), compute(origin_size[1])];
		}
		else if (format.includes('pvrtc')) {
			let padded_width = compute(origin_size[0]);
			let padded_height = compute(origin_size[1]);
			let maximum_size = padded_width > padded_height ? padded_width : padded_height;
			padded_size = [maximum_size, maximum_size];
		}
		else {
			padded_size = [origin_size[0], origin_size[1]];
		}
		return padded_size;
	}

	export function compute_data_size(
		size: KernelX.Image.ImageSize,
		format: Format,
		option: EncodeOption,
	): bigint {
		let data_size = 0n;
		switch (format) {
			case 'rgba_8888_o':
			case 'argb_8888':
			case 'rgba_4444':
			case 'rgb_565':
			case 'rgba_5551': {
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, format.slice(0, -6) as KernelX.Tool.Texture.Encoding.CompositeFormat);
				break;
			}
			case 'rgba_pvrtc4': {
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgba_pvrtc4');
				break;
			}
			case 'rgb_pvrtc4_a_8': {
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_pvrtc4');
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'a_8');
				break;
			}
			case 'rgb_etc1_a_8': {
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_etc1');
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'a_8');
				break;
			}
			case 'rgb_etc1_a_palette': {
				assert_test(option.rgb_etc1_a_palette !== null, `option is null`);
				data_size += KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_etc1');
				data_size += KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.compute_data_size_with_palette(size, option.rgb_etc1_a_palette.palette.length);
				break;
			}
		}
		return data_size;
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
				result += KernelX.Tool.Texture.Encoding.get_bpp(format);
				break;
			}
			case 'rgba_4444_tiled':
			case 'rgb_565_tiled':
			case 'rgba_5551_tiled': {
				result += KernelX.Tool.Texture.Encoding.get_bpp(format.slice(0, -6) as KernelX.Tool.Texture.Encoding.CompositeFormat);
				break;
			}
			case 'rgba_pvrtc4': {
				result += KernelX.Tool.Texture.Encoding.get_bpp('rgba_pvrtc4');
				break;
			}
			case 'rgb_pvrtc4_a_8': {
				// TODO
				result += KernelX.Tool.Texture.Encoding.get_bpp('rgb_pvrtc4');
				break;
			}
			case 'rgb_etc1_a_8':
			case 'rgb_etc1_a_palette': {
				result += KernelX.Tool.Texture.Encoding.get_bpp('rgba_8888_o');
				break;
			}
		}
		return result;
	}

	// ------------------------------------------------

	export function encode(
		image: Kernel.Image.CImageView,
		data: Kernel.OByteStreamView,
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
			case 'rgba_pvrtc4': {
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgba_pvrtc4');
				break;
			}
			case 'rgb_pvrtc4_a_8': {
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_pvrtc4');
				KernelX.Tool.Texture.Encoding.encode(data, image, 'a_8');
				break;
			}
			case 'rgb_etc1_a_8': {
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_etc1');
				KernelX.Tool.Texture.Encoding.encode(data, image, 'a_8');
				break;
			}
			case 'rgb_etc1_a_palette': {
				assert_test(option.rgb_etc1_a_palette !== null, `option is null`);
				KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_etc1');
				KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.encode_with_palette(data, image, option.rgb_etc1_a_palette.palette);
				break;
			}
		}
		return;
	}

	export function decode(
		data: Kernel.IByteStreamView,
		image: Kernel.Image.VImageView,
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
			case 'rgba_pvrtc4': {
				KernelX.Tool.Texture.Encoding.decode(data, image, 'rgba_pvrtc4');
				break;
			}
			case 'rgb_pvrtc4_a_8': {
				KernelX.Tool.Texture.Encoding.decode(data, image, 'rgb_pvrtc4');
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
				KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.decode_with_palette(data, image);
				break;
			}
		}
		return;
	}

	// ------------------------------------------------

	export function encode_fs(
		image_file: string,
		data_file: string,
		format: Format,
	): void {
		let image_data = KernelX.Storage.read_file(image_file);
		let image_stream = Kernel.ByteStreamView.watch(image_data.view());
		let image_size = KernelX.Image.File.PNG.size(image_stream.view());
		let padded_image_size = compute_padded_image_size(image_size, format);
		let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(padded_image_size));
		let image_view = image.view();
		KernelX.Image.File.PNG.read(image_stream, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
		let option: EncodeOption = {
			rgb_etc1_a_palette: null,
		};
		if (format === 'rgb_etc1_a_palette') {
			option.rgb_etc1_a_palette = {
				palette: KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.evaluate_palette(image_view),
			};
		}
		let data_size = compute_data_size(padded_image_size, format, option);
		let data = Kernel.ByteArray.allocate(Kernel.Size.value(data_size));
		let stream = Kernel.ByteStreamView.watch(data.view());
		encode(image_view, stream, format, option);
		KernelX.Storage.write_file(data_file, stream.stream_view());
		return;
	}

	export function decode_fs(
		data_file: string,
		image_file: string,
		image_size: KernelX.Image.ImageSize,
		format: Format,
	): void {
		let data = KernelX.Storage.read_file(data_file);
		let stream = Kernel.ByteStreamView.watch(data.view());
		let padded_image_size = compute_padded_image_size(image_size, format);
		let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(padded_image_size));
		let image_view = image.view();
		decode(stream, image_view, format);
		KernelX.Image.File.PNG.write_fs(image_file, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
		return;
	}

	// ------------------------------------------------

}