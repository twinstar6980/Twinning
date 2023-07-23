namespace TwinStar.Script.Support.PopCap.Texture.Encode {

	// ------------------------------------------------

	const BasicFormatX = [
		'a_8',
		'rgb_332',
		'rgb_565',
		'rgba_5551',
		'rgba_4444',
		'rgba_8888',
		'argb_1555',
		'argb_4444',
		'argb_8888',
		'l_8',
		'la_44',
		'la_88',
		'al_44',
		'al_88',
		'rgb_888_o',
		'rgba_8888_o',
		'rgb_etc1',
		'rgb_etc1_a_8',
		'rgb_etc2',
		'rgb_etc2_a_8',
		'rgba_etc2',
		'rgb_pvrtc4',
		'rgb_pvrtc4_a_8',
		'rgba_pvrtc4',
	] as const;

	const SpecialFormatX = [
		'a_8_tiled',
		'rgb_332_tiled',
		'rgb_565_tiled',
		'rgba_5551_tiled',
		'rgba_4444_tiled',
		'rgba_8888_tiled',
		'argb_1555_tiled',
		'argb_4444_tiled',
		'argb_8888_tiled',
		'l_8_tiled',
		'la_44_tiled',
		'la_88_tiled',
		'al_44_tiled',
		'al_88_tiled',
		'rgb_888_o_tiled',
		'rgba_8888_o_tiled',
		'rgb_etc1_a_palette',
	] as const;

	const FormatX = [
		...BasicFormatX,
		...SpecialFormatX,
	] as const;

	export type BasicFormat = typeof BasicFormatX[number];

	export const BasicFormatE = BasicFormatX as unknown as BasicFormat[];

	export type SpecialFormat = typeof SpecialFormatX[number];

	export const SpecialFormatE = SpecialFormatX as unknown as SpecialFormat[];

	export type Format = typeof FormatX[number];

	export const FormatE = FormatX as unknown as Format[];

	// ------------------------------------------------

	const k_basic_format: Record<BasicFormat, Array<KernelX.Tool.Texture.Encoding.CompositeFormat>> = {
		a_8: [
			'a_8',
		],
		rgb_332: [
			'rgb_332',
		],
		rgb_565: [
			'rgb_565',
		],
		rgba_5551: [
			'rgba_5551',
		],
		rgba_4444: [
			'rgba_4444',
		],
		rgba_8888: [
			'rgba_8888',
		],
		argb_1555: [
			'argb_1555',
		],
		argb_4444: [
			'argb_4444',
		],
		argb_8888: [
			'argb_8888',
		],
		l_8: [
			'l_8',
		],
		la_44: [
			'la_44',
		],
		la_88: [
			'la_88',
		],
		al_44: [
			'al_44',
		],
		al_88: [
			'al_88',
		],
		rgb_888_o: [
			'rgb_888_o',
		],
		rgba_8888_o: [
			'rgba_8888_o',
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
			palette: KernelX.Image.ColorList;
		};
	};

	// ------------------------------------------------

	export function is_opacity_format(
		format: Format,
	): boolean {
		return [
			'rgb_332',
			'rgb_565',
			'l_8',
			'rgb_888_o',
			'rgb_etc1',
			'rgb_etc2',
			'rgb_pvrtc4',
			'rgb_332_tiled',
			'rgb_565_tiled',
			'l_8_tiled',
			'rgb_888_o_tiled',
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
		size: KernelX.Image.ImageSize,
		format: Format,
		option: EncodeOption,
	): bigint {
		let data_size = 0n;
		if (BasicFormatE.includes(format as BasicFormat)) {
			data_size = KernelX.Tool.Texture.Encoding.compute_data_size_n(size, k_basic_format[format as BasicFormat]);
		} else {
			if (format.endsWith('_tiled')) {
				data_size = KernelX.Tool.Texture.Encoding.compute_data_size_n(size, k_basic_format[format.slice(0, -6) as BasicFormat]);
			} else {
				switch (format) {
					case 'rgb_etc1_a_palette': {
						assert_test(option.rgb_etc1_a_palette !== null, `option is null`);
						data_size = KernelX.Tool.Texture.Encoding.compute_data_size(size, 'rgb_etc1') + KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.compute_data_size_with_palette(size, option.rgb_etc1_a_palette.palette.length);
						break;
					}
				}
			}
		}
		return data_size;
	}

	export function encode(
		image: Kernel.Image.CImageView,
		data: Kernel.OByteStreamView,
		format: Format,
		option: EncodeOption,
	): void {
		if (BasicFormatE.includes(format as BasicFormat)) {
			KernelX.Tool.Texture.Encoding.encode_n(data, image, k_basic_format[format as BasicFormat]);
		} else {
			if (format.endsWith('_tiled')) {
				KernelX.Tool.Miscellaneous.XboxTiledTexture.encode(data, image, k_basic_format[format.slice(0, -6) as BasicFormat][0] as typeof Kernel.Tool.Texture.Encoding.Format.Value);
			} else {
				switch (format) {
					case 'rgb_etc1_a_palette': {
						assert_test(option.rgb_etc1_a_palette !== null, `option is null`);
						KernelX.Tool.Texture.Encoding.encode(data, image, 'rgb_etc1');
						KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.encode_with_palette(data, image, option.rgb_etc1_a_palette.palette);
						break;
					}
				}
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
		if (BasicFormatE.includes(format as BasicFormat)) {
			KernelX.Tool.Texture.Encoding.decode_n(data, image, k_basic_format[format as BasicFormat]);
		} else {
			if (format.endsWith('_tiled')) {
				KernelX.Tool.Miscellaneous.XboxTiledTexture.decode(data, image, k_basic_format[format.slice(0, -6) as BasicFormat][0] as typeof Kernel.Tool.Texture.Encoding.Format.Value);
			} else {
				switch (format) {
					case 'rgb_etc1_a_palette': {
						KernelX.Tool.Texture.Encoding.decode(data, image, 'rgb_etc1');
						KernelX.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.decode_with_palette(data, image);
						break;
					}
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
		let image_data = KernelX.FileSystem.read_file(image_file);
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
		KernelX.FileSystem.write_file(data_file, stream.stream_view());
		return;
	}

	export function decode_fs(
		data_file: string,
		image_file: string,
		image_size: KernelX.Image.ImageSize,
		format: Format,
	): void {
		let data = KernelX.FileSystem.read_file(data_file);
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