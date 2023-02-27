#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/image.hpp"

namespace TwinStar::Core::Tool::Image::Texture {

	M_enumeration(
		M_wrap(Format),
		M_wrap(
			a_8,
			rgb_565,
			rgba_5551,
			rgba_4444,
			rgba_8888,
			argb_4444,
			argb_8888,
		),
	);

	using FormatPackage = ValuePackage<
		Format::Constant::a_8(),
		Format::Constant::rgb_565(),
		Format::Constant::rgba_5551(),
		Format::Constant::rgba_4444(),
		Format::Constant::rgba_8888(),
		Format::Constant::argb_4444(),
		Format::Constant::argb_8888()
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
			case Format::Constant::argb_4444().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_8888().value : {
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
					(cbw<IntegerU8>(pixel.alpha) << 0_sz)
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
					(cbw<IntegerU32>(pixel.alpha) << 0_sz) |
					(cbw<IntegerU32>(pixel.blue) << 8_sz) |
					(cbw<IntegerU32>(pixel.green) << 16_sz) |
					(cbw<IntegerU32>(pixel.red) << 24_sz)
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
					(cbw<IntegerU32>(pixel.blue) << 0_sz) |
					(cbw<IntegerU32>(pixel.green) << 8_sz) |
					(cbw<IntegerU32>(pixel.red) << 16_sz) |
					(cbw<IntegerU32>(pixel.alpha) << 24_sz)
				);
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
				auto data_8 = data.read_of<IntegerU8>();
				pixel.alpha = cbw<Image::Color>(clip_bit(data_8, 1_ix, 8_sz));
			}
			if constexpr (format == Format::Constant::rgb_565()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 1_ix, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 6_ix, 6_sz)), 6_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 12_ix, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_5551()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 1_ix, 1_sz)), 1_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 2_ix, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 7_ix, 5_sz)), 5_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 12_ix, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_4444()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 1_ix, 4_sz)), 4_sz);
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 5_ix, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 9_ix, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 13_ix, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				auto data_32 = data.read_of<IntegerU32>();
				pixel.alpha = cbw<Image::Color>(clip_bit(data_32, 1_ix, 8_sz));
				pixel.blue = cbw<Image::Color>(clip_bit(data_32, 9_ix, 8_sz));
				pixel.green = cbw<Image::Color>(clip_bit(data_32, 17_ix, 8_sz));
				pixel.red = cbw<Image::Color>(clip_bit(data_32, 25_ix, 8_sz));
			}
			if constexpr (format == Format::Constant::argb_4444()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 1_ix, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 5_ix, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 9_ix, 4_sz)), 4_sz);
				pixel.alpha = Image::uncompress_color(cbw<Image::Color>(clip_bit(data_16, 13_ix, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::argb_8888()) {
				auto data_32 = data.read_of<IntegerU32>();
				pixel.blue = cbw<Image::Color>(clip_bit(data_32, 1_ix, 8_sz));
				pixel.green = cbw<Image::Color>(clip_bit(data_32, 9_ix, 8_sz));
				pixel.red = cbw<Image::Color>(clip_bit(data_32, 17_ix, 8_sz));
				pixel.alpha = cbw<Image::Color>(clip_bit(data_32, 25_ix, 8_sz));
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
