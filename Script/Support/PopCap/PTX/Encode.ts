namespace TwinStar.Script.Support.PopCap.PTX.Encode {

	// ------------------------------------------------

	export const BaseFormatE = [
		'abgr_8888',
		'argb_8888',
		'rgba_4444',
		'rgb_565',
		'rgba_5551',
		'rgb_etc1',
		'rgb_etc1_a_8',
		'rgb_etc2',
		'rgb_etc2_a_8',
		'rgba_etc2',
		'rgb_pvrtc4',
		'rgb_pvrtc4_a_8',
		'rgba_pvrtc4',
	] as const;

	export const SpecialFormatE = [
		'rgba_4444_tiled',
		'rgb_565_tiled',
		'rgba_5551_tiled',
		'rgb_etc1_a_palette',
	] as const;

	export const FormatE = [
		...BaseFormatE,
		...SpecialFormatE,
	] as const;

	export type BaseFormat = typeof BaseFormatE[number];

	export type SpecialFormat = typeof SpecialFormatE[number];

	export type Format = typeof FormatE[number];

	// ------------------------------------------------

	const k_base_format: Record<BaseFormat, Array<CoreX.Tool.Image.Texture.CompositeFormat>> = {
		abgr_8888: [
			'rgba_8888',
		],
		argb_8888: [
			'argb_8888',
		],
		rgba_4444: [
			'rgba_4444',
		],
		rgb_565: [
			'rgb_565',
		],
		rgba_5551: [
			'rgba_5551',
		],
		rgb_etc1: [
			'rgb_etc1',
		],
		rgb_etc1_a_8: [
			'rgb_etc1',
			'a_8',
		],
		rgb_etc2: [
			'rgb_etc2',
		],
		rgb_etc2_a_8: [
			'rgb_etc2',
			'a_8',
		],
		rgba_etc2: [
			'rgba_etc2',
		],
		rgb_pvrtc4: [
			'rgb_pvrtc4',
		],
		rgb_pvrtc4_a_8: [
			'rgb_pvrtc4',
			'a_8',
		],
		rgba_pvrtc4: [
			'rgba_pvrtc4',
		],
	};

	// ------------------------------------------------

	type EncodeOption = {
		rgb_etc1_a_palette: null | {
			palette: CoreX.Image.ColorList;
		};
	};

	// ------------------------------------------------

	export function is_opacity_format(
		format: Format,
	): boolean {
		return [
			'rgb_565',
			'rgb_etc1',
			'rgb_etc2',
			'rgb_pvrtc4',
			'rgb_565_tiled',
		].includes(format);
	}

	// ------------------------------------------------

	export function compute_padded_image_size(
		origin_size: CoreX.Image.ImageSize,
		format: Format,
	): CoreX.Image.ImageSize {
		let compute = (t: bigint) => {
			let r = 0b1n << 1n;
			while (r < t) {
				r <<= 1n;
			}
			return r;
		};
		let padded_size: CoreX.Image.ImageSize;
		if (format.includes('etc1')) {
			padded_size = [compute(origin_size[0]), compute(origin_size[1])];
		} else if (format.includes('pvrtc')) {
			let padded_width = compute(origin_size[0]);
			let padded_height = compute(origin_size[1]);
			let maximum_size = padded_width > padded_height ? padded_width : padded_height;
			padded_size = [maximum_size, maximum_size];
		} else {
			padded_size = [origin_size[0], origin_size[1]];
		}
		return padded_size;
	}

	export function compute_data_size(
		size: CoreX.Image.ImageSize,
		format: Format,
		option: EncodeOption,
	): bigint {
		let data_size = 0n;
		if (BaseFormatE.includes(format as BaseFormat)) {
			data_size = CoreX.Tool.Image.Texture.compute_data_size_n(size, k_base_format[format as BaseFormat]);
		} else {
			switch (format) {
				case 'rgba_4444_tiled':
				case 'rgb_565_tiled':
				case 'rgba_5551_tiled': {
					data_size = CoreX.Tool.Image.Texture.compute_data_size_n(size, k_base_format[format.slice(0, -6) as BaseFormat]);
					break;
				}
				case 'rgb_etc1_a_palette': {
					assert_test(option.rgb_etc1_a_palette !== null, `option is null`);
					data_size = CoreX.Tool.Image.Texture.compute_data_size(size, 'rgb_etc1') + CoreX.Tool.Miscellaneous.PvZ2ChineseAndroidAlphaPaletteTexture.compute_data_size_with_palette(size, option.rgb_etc1_a_palette.palette.length);
					break;
				}
			}
		}
		return data_size;
	}

	export function encode(
		image: Core.Image.CImageView,
		data: Core.OByteStreamView,
		format: Format,
		option: EncodeOption,
	): void {
		if (BaseFormatE.includes(format as BaseFormat)) {
			if (['abgr_8888'].includes(format)) {
				Core.Miscellaneous.g_byte_stream_use_big_endian.value = !Core.Miscellaneous.g_byte_stream_use_big_endian.value;
			}
			CoreX.Tool.Image.Texture.encode_n(data, image, k_base_format[format as BaseFormat]);
			if (['abgr_8888'].includes(format)) {
				Core.Miscellaneous.g_byte_stream_use_big_endian.value = !Core.Miscellaneous.g_byte_stream_use_big_endian.value;
			}
		} else {
			switch (format) {
				case 'rgba_4444_tiled':
				case 'rgb_565_tiled':
				case 'rgba_5551_tiled': {
					CoreX.Tool.Miscellaneous.XboxTiledTexture.encode(data, image, k_base_format[format.slice(0, -6) as BaseFormat][0] as typeof Core.Tool.Image.Texture.Format.Value);
					break;
				}
				case 'rgb_etc1_a_palette': {
					assert_test(option.rgb_etc1_a_palette !== null, `option is null`);
					CoreX.Tool.Image.Texture.encode(data, image, 'rgb_etc1');
					CoreX.Tool.Miscellaneous.PvZ2ChineseAndroidAlphaPaletteTexture.encode_with_palette(data, image, option.rgb_etc1_a_palette.palette);
					break;
				}
			}
		}
		return;
	}

	export function decode(
		data: Core.IByteStreamView,
		image: Core.Image.VImageView,
		format: Format,
	): void {
		if (is_opacity_format(format)) {
			image.fill(Core.Image.Pixel.value([0xFFn, 0xFFn, 0xFFn, 0xFFn]));
		}
		if (BaseFormatE.includes(format as BaseFormat)) {
			if (['abgr_8888'].includes(format)) {
				Core.Miscellaneous.g_byte_stream_use_big_endian.value = !Core.Miscellaneous.g_byte_stream_use_big_endian.value;
			}
			CoreX.Tool.Image.Texture.decode_n(data, image, k_base_format[format as BaseFormat]);
			if (['abgr_8888'].includes(format)) {
				Core.Miscellaneous.g_byte_stream_use_big_endian.value = !Core.Miscellaneous.g_byte_stream_use_big_endian.value;
			}
		} else {
			switch (format) {
				case 'rgba_4444_tiled':
				case 'rgb_565_tiled':
				case 'rgba_5551_tiled': {
					CoreX.Tool.Miscellaneous.XboxTiledTexture.decode(data, image, k_base_format[format.slice(0, -6) as BaseFormat][0] as typeof Core.Tool.Image.Texture.Format.Value);
					break;
				}
				case 'rgb_etc1_a_palette': {
					CoreX.Tool.Image.Texture.decode(data, image, 'rgb_etc1');
					CoreX.Tool.Miscellaneous.PvZ2ChineseAndroidAlphaPaletteTexture.decode_with_palette(data, image);
					break;
				}
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
		let image_data = CoreX.FileSystem.read_file(image_file);
		let image_stream = Core.ByteStreamView.watch(image_data.view());
		let image_size = CoreX.Image.File.PNG.size(image_stream.view());
		let padded_image_size = compute_padded_image_size(image_size, format);
		let image = Core.Image.Image.allocate(Core.Image.ImageSize.value(padded_image_size));
		let image_view = image.view();
		CoreX.Image.File.PNG.read(image_stream, image_view.sub(Core.Image.ImagePosition.value([0n, 0n]), Core.Image.ImageSize.value(image_size)));
		let option: EncodeOption = {
			rgb_etc1_a_palette: null,
		};
		if (format === 'rgb_etc1_a_palette') {
			option.rgb_etc1_a_palette = {
				palette: CoreX.Tool.Miscellaneous.PvZ2ChineseAndroidAlphaPaletteTexture.evaluate_palette(image_view),
			};
		}
		let data_size = compute_data_size(padded_image_size, format, option);
		let data = Core.ByteArray.allocate(Core.Size.value(data_size));
		let stream = Core.ByteStreamView.watch(data.view());
		encode(image_view, stream, format, option);
		CoreX.FileSystem.write_file(data_file, stream.stream_view());
		return;
	}

	export function decode_fs(
		data_file: string,
		image_file: string,
		image_size: CoreX.Image.ImageSize,
		format: Format,
	): void {
		let data = CoreX.FileSystem.read_file(data_file);
		let stream = Core.ByteStreamView.watch(data.view());
		let padded_image_size = compute_padded_image_size(image_size, format);
		let image = Core.Image.Image.allocate(Core.Image.ImageSize.value(padded_image_size));
		let image_view = image.view();
		decode(stream, image_view, format);
		CoreX.Image.File.PNG.write_fs(image_file, image_view.sub(Core.Image.ImagePosition.value([0n, 0n]), Core.Image.ImageSize.value(image_size)));
		return;
	}

	// ------------------------------------------------

}