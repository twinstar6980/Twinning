#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/image.hpp"

namespace TwinStar::Core::Tool::Image::Texture {

	M_enumeration(
		M_wrap(Format),
		M_wrap(
			a_8,
			rgb_332,
			rgb_565,
			rgba_5551,
			rgba_4444,
			rgba_8888,
			argb_1555,
			argb_4444,
			argb_8888,
			l_8,
			la_44,
			la_88,
			al_44,
			al_88,
			rgb_888_o,
			rgba_8888_o,
		),
	);

	using FormatPackage = ValuePackage<
		Format::Constant::a_8(),
		Format::Constant::rgb_332(),
		Format::Constant::rgb_565(),
		Format::Constant::rgba_5551(),
		Format::Constant::rgba_4444(),
		Format::Constant::rgba_8888(),
		Format::Constant::argb_1555(),
		Format::Constant::argb_4444(),
		Format::Constant::argb_8888(),
		Format::Constant::l_8(),
		Format::Constant::la_44(),
		Format::Constant::la_88(),
		Format::Constant::al_44(),
		Format::Constant::al_88(),
		Format::Constant::rgb_888_o(),
		Format::Constant::rgba_8888_o()
	>;

	// TODO
	inline constexpr auto bpp_of (
		Format const & format
	) -> Size {
		auto result = Size{};
		switch (format.value) {
			case Format::Constant::a_8().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::rgb_332().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::rgb_565().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_5551().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_4444().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_8888().value : {
				result = 32_sz;
				break;
			}
			case Format::Constant::argb_1555().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_4444().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_8888().value : {
				result = 32_sz;
				break;
			}
			case Format::Constant::l_8().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::la_44().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::la_88().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::al_44().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::al_88().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgb_888_o().value : {
				result = 24_sz;
				break;
			}
			case Format::Constant::rgba_8888_o().value : {
				result = 32_sz;
				break;
			}
		}
		return result;
	}

	// ----------------

	template <auto format> requires (FormatPackage::has(format))
	struct EncodeCommon {

	protected:

	};

	template <auto format> requires (FormatPackage::has(format))
	struct Encode :
		EncodeCommon<format> {

	protected:

		static auto process_pixel (
			OByteStreamView &    data,
			Image::Pixel const & pixel
		) -> Void {
			if constexpr (format == Format::Constant::a_8()) {
				data.write(
					(cbw<IntegerU8>(Image::compress_color(pixel.alpha, 8_sz)) << 0_sz)
				);
			}
			if constexpr (format == Format::Constant::rgb_332()) {
				data.write(
					(cbw<IntegerU8>(Image::compress_color(pixel.blue, 3_sz)) << 0_sz) |
					(cbw<IntegerU8>(Image::compress_color(pixel.green, 3_sz)) << 3_sz) |
					(cbw<IntegerU8>(Image::compress_color(pixel.red, 2_sz)) << 6_sz)
				);
			}
			if constexpr (format == Format::Constant::rgb_565()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(pixel.blue, 5_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.green, 6_sz)) << 5_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.red, 5_sz)) << 11_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_5551()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(pixel.alpha, 1_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.blue, 5_sz)) << 1_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.green, 5_sz)) << 6_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.red, 5_sz)) << 11_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_4444()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(pixel.alpha, 4_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.blue, 4_sz)) << 4_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.green, 4_sz)) << 8_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.red, 4_sz)) << 12_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				data.write(
					(cbw<IntegerU32>(Image::compress_color(pixel.alpha, 8_sz)) << 0_sz) |
					(cbw<IntegerU32>(Image::compress_color(pixel.blue, 8_sz)) << 8_sz) |
					(cbw<IntegerU32>(Image::compress_color(pixel.green, 8_sz)) << 16_sz) |
					(cbw<IntegerU32>(Image::compress_color(pixel.red, 8_sz)) << 24_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_1555()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(pixel.blue, 5_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.green, 5_sz)) << 5_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.red, 5_sz)) << 10_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.alpha, 1_sz)) << 15_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_4444()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(pixel.blue, 4_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.green, 4_sz)) << 4_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.red, 4_sz)) << 8_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.alpha, 4_sz)) << 12_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_8888()) {
				data.write(
					(cbw<IntegerU32>(Image::compress_color(pixel.blue, 8_sz)) << 0_sz) |
					(cbw<IntegerU32>(Image::compress_color(pixel.green, 8_sz)) << 8_sz) |
					(cbw<IntegerU32>(Image::compress_color(pixel.red, 8_sz)) << 16_sz) |
					(cbw<IntegerU32>(Image::compress_color(pixel.alpha, 8_sz)) << 24_sz)
				);
			}
			if constexpr (format == Format::Constant::l_8()) {
				data.write(
					(cbw<IntegerU8>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 8_sz)) << 0_sz)
				);
			}
			if constexpr (format == Format::Constant::la_44()) {
				data.write(
					(cbw<IntegerU8>(Image::compress_color(pixel.alpha, 4_sz)) << 0_sz) |
					(cbw<IntegerU8>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 4_sz)) << 4_sz)
				);
			}
			if constexpr (format == Format::Constant::la_88()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(pixel.alpha, 8_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 8_sz)) << 8_sz)
				);
			}
			if constexpr (format == Format::Constant::al_44()) {
				data.write(
					(cbw<IntegerU8>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 4_sz)) << 0_sz) |
					(cbw<IntegerU8>(Image::compress_color(pixel.alpha, 4_sz)) << 4_sz)
				);
			}
			if constexpr (format == Format::Constant::al_88()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_color(convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue), 8_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_color(pixel.alpha, 8_sz)) << 8_sz)
				);
			}
			if constexpr (format == Format::Constant::rgb_888_o()) {
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.red, 8_sz)));
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.green, 8_sz)));
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.blue, 8_sz)));
			}
			if constexpr (format == Format::Constant::rgba_8888_o()) {
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.red, 8_sz)));
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.green, 8_sz)));
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.blue, 8_sz)));
				data.write(cbw<IntegerU8>(Image::compress_color(pixel.alpha, 8_sz)));
			}
			return;
		}

		static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			for (auto & row : image.data()) {
				for (auto & pixel : row) {
					process_pixel(data, pixel);
				}
			}
			return;
		}

	public:

		static auto do_process_pixel (
			OByteStreamView &    data_,
			Image::Pixel const & pixel
		) -> Void {
			M_use_zps_of(data);
			return process_pixel(data, pixel);
		}

		static auto do_process_image (
			OByteStreamView &         data_,
			Image::CImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

	template <auto format> requires (FormatPackage::has(format))
	struct Decode :
		EncodeCommon<format> {

	protected:

		static auto process_pixel (
			IByteStreamView & data,
			Image::Pixel &    pixel
		) -> Void {
			if constexpr (format == Format::Constant::a_8()) {
				auto block = data.read_of<IntegerU8>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::rgb_332()) {
				auto block = data.read_of<IntegerU8>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 3_sz)), 3_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 3_sz, 3_sz)), 3_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 6_sz, 2_sz)), 2_sz);
			}
			if constexpr (format == Format::Constant::rgb_565()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 5_sz, 6_sz)), 6_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 11_sz, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_5551()) {
				auto block = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 1_sz)), 1_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 1_sz, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 6_sz, 5_sz)), 5_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 11_sz, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_4444()) {
				auto block = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 12_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				auto block = data.read_of<IntegerU32>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 16_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 24_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::argb_1555()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 5_sz, 5_sz)), 5_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 10_sz, 5_sz)), 5_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 15_sz, 1_sz)), 1_sz);
			}
			if constexpr (format == Format::Constant::argb_4444()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 4_sz)), 4_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 12_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::argb_8888()) {
				auto block = data.read_of<IntegerU32>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 16_sz, 8_sz)), 8_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 24_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::l_8()) {
				auto block = data.read_of<IntegerU8>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::la_44()) {
				auto block = data.read_of<IntegerU8>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::la_88()) {
				auto block = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::al_44()) {
				auto block = data.read_of<IntegerU8>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 4_sz)), 4_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 4_sz, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::al_88()) {
				auto block = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 0_sz, 8_sz)), 8_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(block, 8_sz, 8_sz)), 8_sz);
			}
			if constexpr (format == Format::Constant::rgb_888_o()) {
				pixel.red = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
			}
			if constexpr (format == Format::Constant::rgba_8888_o()) {
				pixel.red = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(data.read_of<IntegerU8>()), 8_sz);
			}
			return;
		}

		static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			for (auto & row : image.data()) {
				for (auto & pixel : row) {
					process_pixel(data, pixel);
				}
			}
			return;
		}

	public:

		static auto do_process_pixel (
			IByteStreamView & data_,
			Image::Pixel &    pixel
		) -> Void {
			M_use_zps_of(data);
			return process_pixel(data, pixel);
		}

		static auto do_process_image (
			IByteStreamView &         data_,
			Image::VImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

}
