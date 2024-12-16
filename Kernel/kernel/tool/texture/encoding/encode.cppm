module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.encoding.common;

export namespace Twinning::Kernel::Tool::Texture::Encoding {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Format>)
		inline static auto process_pixel (
			OByteStreamView &    data,
			Image::Pixel const & pixel
		) -> Void {
			if constexpr (format == Format::Constant::a_8()) {
				data.write(
					(cbox<IntegerU8>(Image::compress_color(pixel.alpha, 8_sz)) << 0_sz)
				);
			}
			if constexpr (format == Format::Constant::rgb_332()) {
				data.write(
					(cbox<IntegerU8>(Image::compress_color(pixel.blue, 3_sz)) << 0_sz) |
					(cbox<IntegerU8>(Image::compress_color(pixel.green, 3_sz)) << 3_sz) |
					(cbox<IntegerU8>(Image::compress_color(pixel.red, 2_sz)) << 6_sz)
				);
			}
			if constexpr (format == Format::Constant::rgb_565()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(pixel.blue, 5_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.green, 6_sz)) << 5_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.red, 5_sz)) << 11_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_5551()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(pixel.alpha, 1_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.blue, 5_sz)) << 1_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.green, 5_sz)) << 6_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.red, 5_sz)) << 11_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_4444()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(pixel.alpha, 4_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.blue, 4_sz)) << 4_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.green, 4_sz)) << 8_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.red, 4_sz)) << 12_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				data.write(
					(cbox<IntegerU32>(Image::compress_color(pixel.alpha, 8_sz)) << 0_sz) |
					(cbox<IntegerU32>(Image::compress_color(pixel.blue, 8_sz)) << 8_sz) |
					(cbox<IntegerU32>(Image::compress_color(pixel.green, 8_sz)) << 16_sz) |
					(cbox<IntegerU32>(Image::compress_color(pixel.red, 8_sz)) << 24_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_1555()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(pixel.blue, 5_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.green, 5_sz)) << 5_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.red, 5_sz)) << 10_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.alpha, 1_sz)) << 15_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_4444()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(pixel.blue, 4_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.green, 4_sz)) << 4_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.red, 4_sz)) << 8_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.alpha, 4_sz)) << 12_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_8888()) {
				data.write(
					(cbox<IntegerU32>(Image::compress_color(pixel.blue, 8_sz)) << 0_sz) |
					(cbox<IntegerU32>(Image::compress_color(pixel.green, 8_sz)) << 8_sz) |
					(cbox<IntegerU32>(Image::compress_color(pixel.red, 8_sz)) << 16_sz) |
					(cbox<IntegerU32>(Image::compress_color(pixel.alpha, 8_sz)) << 24_sz)
				);
			}
			if constexpr (format == Format::Constant::l_8()) {
				data.write(
					(cbox<IntegerU8>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 8_sz)) << 0_sz)
				);
			}
			if constexpr (format == Format::Constant::la_44()) {
				data.write(
					(cbox<IntegerU8>(Image::compress_color(pixel.alpha, 4_sz)) << 0_sz) |
					(cbox<IntegerU8>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 4_sz)) << 4_sz)
				);
			}
			if constexpr (format == Format::Constant::la_88()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(pixel.alpha, 8_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 8_sz)) << 8_sz)
				);
			}
			if constexpr (format == Format::Constant::al_44()) {
				data.write(
					(cbox<IntegerU8>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 4_sz)) << 0_sz) |
					(cbox<IntegerU8>(Image::compress_color(pixel.alpha, 4_sz)) << 4_sz)
				);
			}
			if constexpr (format == Format::Constant::al_88()) {
				data.write(
					(cbox<IntegerU16>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 8_sz)) << 0_sz) |
					(cbox<IntegerU16>(Image::compress_color(pixel.alpha, 8_sz)) << 8_sz)
				);
			}
			if constexpr (format == Format::Constant::rgb_888_o()) {
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.red, 8_sz)));
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.green, 8_sz)));
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.blue, 8_sz)));
			}
			if constexpr (format == Format::Constant::rgba_8888_o()) {
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.red, 8_sz)));
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.green, 8_sz)));
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.blue, 8_sz)));
				data.write(cbox<IntegerU8>(Image::compress_color(pixel.alpha, 8_sz)));
			}
			return;
		}

		// ----------------

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Format>)
		inline static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			for (auto & row : image.data()) {
				for (auto & pixel : row) {
					process_pixel<format>(data, pixel);
				}
			}
			return;
		}

		inline static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image,
			Format const &            format
		) -> Void {
			Generalization::match<FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					process_image<format>(data, image);
				}
			);
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &         data_,
			Image::CImageView const & image,
			Format const &            format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
