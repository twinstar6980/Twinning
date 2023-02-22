#pragma once

// TODO : format is decide by endian, should or not ?

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Texture {

	M_enumeration(
		M_wrap(Format),
		M_wrap(
			a_8,
			rgb_888,
			rgba_8888,
			rgb_565_l,
			rgba_4444_l,
			rgba_5551_l,
			argb_4444_l,
			argb_8888_l,
		),
	);

	using FormatPackage = ValuePackage<
		Format::Constant::a_8(),
		Format::Constant::rgb_888(),
		Format::Constant::rgba_8888(),
		Format::Constant::rgb_565_l(),
		Format::Constant::rgba_4444_l(),
		Format::Constant::rgba_5551_l(),
		Format::Constant::argb_4444_l(),
		Format::Constant::argb_8888_l()
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
			case Format::Constant::rgb_888().value : {
				result = 24_sz;
				break;
			}
			case Format::Constant::rgba_8888().value : {
				result = 32_sz;
				break;
			}
			case Format::Constant::rgb_565_l().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_4444_l().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_5551_l().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_4444_l().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_8888_l().value : {
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
				data.write(pixel.alpha);
			}
			if constexpr (format == Format::Constant::rgb_888()) {
				data.write(pixel.red);
				data.write(pixel.green);
				data.write(pixel.blue);
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				data.write(pixel.red);
				data.write(pixel.green);
				data.write(pixel.blue);
				data.write(pixel.alpha);
			}
			if constexpr (format == Format::Constant::rgb_565_l()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 5_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.green, 6_sz)) << 5_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.red, 5_sz)) << 11_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_4444_l()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_channel(pixel.alpha, 4_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 4_sz)) << 4_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.green, 4_sz)) << 8_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.red, 4_sz)) << 12_sz)
				);
			}
			if constexpr (format == Format::Constant::rgba_5551_l()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_channel(pixel.alpha, 1_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 5_sz)) << 1_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.green, 5_sz)) << 6_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.red, 5_sz)) << 11_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_4444_l()) {
				data.write(
					(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 4_sz)) << 0_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.green, 4_sz)) << 4_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.red, 4_sz)) << 8_sz) |
					(cbw<IntegerU16>(Image::compress_channel(pixel.alpha, 4_sz)) << 12_sz)
				);
			}
			if constexpr (format == Format::Constant::argb_8888_l()) {
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
			OByteStreamView &          data,
			Image::CBitmapView const & image
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
			OByteStreamView &          data_,
			Image::CBitmapView const & image
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
				pixel.alpha = data.read_of<IntegerU8>();
			}
			if constexpr (format == Format::Constant::rgb_888()) {
				pixel.red = data.read_of<IntegerU8>();
				pixel.green = data.read_of<IntegerU8>();
				pixel.blue = data.read_of<IntegerU8>();
			}
			if constexpr (format == Format::Constant::rgba_8888()) {
				pixel.red = data.read_of<IntegerU8>();
				pixel.green = data.read_of<IntegerU8>();
				pixel.blue = data.read_of<IntegerU8>();
				pixel.alpha = data.read_of<IntegerU8>();
			}
			if constexpr (format == Format::Constant::rgb_565_l()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 6_ix, 6_sz)), 6_sz);
				pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 12_ix, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::rgba_4444_l()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 4_sz)), 4_sz);
				pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 5_ix, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 9_ix, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 13_ix, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::rgba_5551_l()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.alpha = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 1_sz)), 1_sz);
				pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 2_ix, 5_sz)), 5_sz);
				pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 7_ix, 5_sz)), 5_sz);
				pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 12_ix, 5_sz)), 5_sz);
			}
			if constexpr (format == Format::Constant::argb_4444_l()) {
				auto data_16 = data.read_of<IntegerU16>();
				pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 4_sz)), 4_sz);
				pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 5_ix, 4_sz)), 4_sz);
				pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 9_ix, 4_sz)), 4_sz);
				pixel.alpha = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 13_ix, 4_sz)), 4_sz);
			}
			if constexpr (format == Format::Constant::argb_8888_l()) {
				auto data_32 = data.read_of<IntegerU32>();
				pixel.blue = cbw<Image::Channel>(clip_bit(data_32, 1_ix, 8_sz));
				pixel.green = cbw<Image::Channel>(clip_bit(data_32, 9_ix, 8_sz));
				pixel.red = cbw<Image::Channel>(clip_bit(data_32, 17_ix, 8_sz));
				pixel.alpha = cbw<Image::Channel>(clip_bit(data_32, 25_ix, 8_sz));
			}
			return;
		}

		static auto process_image (
			IByteStreamView &          data,
			Image::VBitmapView const & image
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
			IByteStreamView &          data_,
			Image::VBitmapView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

}
