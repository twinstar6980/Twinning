module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.encoding.common;

export namespace Twinning::Kernel::Tool::Texture::Encoding {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Format>)
		inline static auto process_pixel (
			IByteStreamView & data,
			Image::Pixel &    pixel
		) -> Void {
			if constexpr (format == Format::Constant::a_8()) {
				auto block = data.read_of<IntegerU8>();
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::rgb_332()) {
				auto block = data.read_of<IntegerU8>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 3_sz)), 3_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 3_sz, 3_sz)), 3_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 6_sz, 2_sz)), 2_sz);
			}
			if constexpr (format == Format::Constant::rgb_565()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 5_sz, 6_sz)), 6_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 11_sz, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_5551()) {
				auto block = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 1_sz)), 1_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 1_sz, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 6_sz, 5_sz)), 5_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 11_sz, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_4444()) {
				auto block = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 12_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				auto block = data.read_of<IntegerU32>();
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 16_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 24_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::argb_1555()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 5_sz, 5_sz)), 5_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 10_sz, 5_sz)), 5_sz);
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 15_sz, 1_sz)), 1_sz);
			}
			if constexpr (format == Format::Constant::argb_4444()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 4_sz)), 4_sz);
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 12_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::argb_8888()) {
				auto block = data.read_of<IntegerU32>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 16_sz, 8_sz)), 8_sz);
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 24_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::l_8()) {
				auto block = data.read_of<IntegerU8>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::la_44()) {
				auto block = data.read_of<IntegerU8>();
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::la_88()) {
				auto block = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::al_44()) {
				auto block = data.read_of<IntegerU8>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::al_88()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::rgb_888_o()) {
				pixel.red = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
			}
			if constexpr (format == Format::Constant::rgba_8888_o()) {
				pixel.red = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.green = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.blue = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.alpha = Image::uncompress_color(cbox<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
			}
			return;
		}

		// ----------------

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Format>)
		inline static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			for (auto & row : image.data()) {
				for (auto & pixel : row) {
					process_pixel<format>(data, pixel);
				}
			}
			return;
		}

		inline static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image,
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
			IByteStreamView &         data_,
			Image::VImageView const & image,
			Format const &            format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
