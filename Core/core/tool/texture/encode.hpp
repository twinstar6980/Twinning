#pragma once

// TODO : format is decide by endian, should or not ?

#include "core/utility/utility.hpp"
#include "core/tool/common/version_utility.hpp"

namespace TwinKleS::Core::Tool::Texture {

	inline namespace CommonOfEncode {

		namespace Detail {

			#pragma region format

			enum class Format : ZIntegerU8 {
				a_8,
				rgb_888,
				rgba_8888,
				rgb_565_l,
				rgba_4444_l,
				rgba_5551_l,
				argb_4444_l,
				argb_8888_l,
			};

			using FormatEnum = ValuePackage<
				Format::a_8,
				Format::rgb_888,
				Format::rgba_8888,
				Format::rgb_565_l,
				Format::rgba_4444_l,
				Format::rgba_5551_l,
				Format::argb_4444_l,
				Format::argb_8888_l
			>;

			// ----------------

			inline auto bpp_of (
				Format const & format
			) -> Size {
				auto result = Size{};
				switch (format) {
					case Format::a_8 : {
						result = 8_sz;
						break;
					}
					case Format::rgb_888 : {
						result = 24_sz;
						break;
					}
					case Format::rgba_8888 : {
						result = 32_sz;
						break;
					}
					case Format::rgb_565_l : {
						result = 16_sz;
						break;
					}
					case Format::rgba_4444_l : {
						result = 16_sz;
						break;
					}
					case Format::rgba_5551_l : {
						result = 16_sz;
						break;
					}
					case Format::argb_4444_l : {
						result = 16_sz;
						break;
					}
					case Format::argb_8888_l : {
						result = 32_sz;
						break;
					}
				}
				return result;
			}

			#pragma endregion

		}

		using Detail::Format;

		using Detail::FormatEnum;

		using Detail::bpp_of;

	}

	namespace Encode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region process

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process_pixel (
				OByteStreamView &    data,
				Image::Pixel const & pixel
			) -> Void {
				if constexpr (format == Format::a_8) {
					data.write(pixel.alpha);
				} else if constexpr (format == Format::rgb_888) {
					data.write(pixel.red);
					data.write(pixel.green);
					data.write(pixel.blue);
				} else if constexpr (format == Format::rgba_8888) {
					data.write(pixel.red);
					data.write(pixel.green);
					data.write(pixel.blue);
					data.write(pixel.alpha);
				} else if constexpr (format == Format::rgb_565_l) {
					data.write(
						(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 5_sz)) << 0_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.green, 6_sz)) << 5_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.red, 5_sz)) << 11_sz)
					);
				} else if constexpr (format == Format::rgba_4444_l) {
					data.write(
						(cbw<IntegerU16>(Image::compress_channel(pixel.alpha, 4_sz)) << 0_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 4_sz)) << 4_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.green, 4_sz)) << 8_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.red, 4_sz)) << 12_sz)
					);
				} else if constexpr (format == Format::rgba_5551_l) {
					data.write(
						(cbw<IntegerU16>(Image::compress_channel(pixel.alpha, 1_sz)) << 0_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 5_sz)) << 1_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.green, 5_sz)) << 6_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.red, 5_sz)) << 11_sz)
					);
				} else if constexpr (format == Format::argb_4444_l) {
					data.write(
						(cbw<IntegerU16>(Image::compress_channel(pixel.blue, 4_sz)) << 0_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.green, 4_sz)) << 4_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.red, 4_sz)) << 8_sz) |
						(cbw<IntegerU16>(Image::compress_channel(pixel.alpha, 4_sz)) << 12_sz)
					);
				} else if constexpr (format == Format::argb_8888_l) {
					data.write(
						(cbw<IntegerU32>(pixel.blue) << 0_sz) |
						(cbw<IntegerU32>(pixel.green) << 8_sz) |
						(cbw<IntegerU32>(pixel.red) << 16_sz) |
						(cbw<IntegerU32>(pixel.alpha) << 24_sz)
					);
				}
				return;
			}

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process_image (
				OByteStreamView &          data,
				Image::CBitmapView const & image
			) -> Void {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						process_pixel<format>(data, pixel);
					}
				}
				return;
			}

			// ----------------

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process (
				OByteStreamView &    data,
				Image::Pixel const & pixel
			) -> Void {
				process_pixel<format>(data, pixel);
				return;
			}

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process (
				OByteStreamView &          data,
				Image::CBitmapView const & image
			) -> Void {
				process_image<format>(data, image);
				return;
			}

			inline auto process (
				OByteStreamView &    data,
				Image::Pixel const & pixel,
				Format const &       format
			) -> Void {
				Generalization::execute<FormatEnum>(
					format,
					[&] <auto index, auto format> (ValuePackage<index, format>) {
						process<format>(data, pixel);
					}
				);
				return;
			}

			inline auto process (
				OByteStreamView &          data,
				Image::CBitmapView const & image,
				Format const &             format
			) -> Void {
				Generalization::execute<FormatEnum>(
					format,
					[&] <auto index, auto format> (ValuePackage<index, format>) {
						process<format>(data, image);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Decode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region process

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process_pixel (
				IByteStreamView & data,
				Image::Pixel &    pixel
			) -> Void {
				if constexpr (format == Format::a_8) {
					pixel.alpha = data.read_of<IntegerU8>();
				} else if constexpr (format == Format::rgb_888) {
					pixel.red = data.read_of<IntegerU8>();
					pixel.green = data.read_of<IntegerU8>();
					pixel.blue = data.read_of<IntegerU8>();
				} else if constexpr (format == Format::rgba_8888) {
					pixel.red = data.read_of<IntegerU8>();
					pixel.green = data.read_of<IntegerU8>();
					pixel.blue = data.read_of<IntegerU8>();
					pixel.alpha = data.read_of<IntegerU8>();
				} else if constexpr (format == Format::rgb_565_l) {
					auto data_16 = data.read_of<IntegerU16>();
					pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 5_sz)), 5_sz);
					pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 6_ix, 6_sz)), 6_sz);
					pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 12_ix, 5_sz)), 5_sz);
				} else if constexpr (format == Format::rgba_4444_l) {
					auto data_16 = data.read_of<IntegerU16>();
					pixel.alpha = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 4_sz)), 4_sz);
					pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 5_ix, 4_sz)), 4_sz);
					pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 9_ix, 4_sz)), 4_sz);
					pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 13_ix, 4_sz)), 4_sz);
				} else if constexpr (format == Format::rgba_5551_l) {
					auto data_16 = data.read_of<IntegerU16>();
					pixel.alpha = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 1_sz)), 1_sz);
					pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 2_ix, 5_sz)), 5_sz);
					pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 7_ix, 5_sz)), 5_sz);
					pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 12_ix, 5_sz)), 5_sz);
				} else if constexpr (format == Format::argb_4444_l) {
					auto data_16 = data.read_of<IntegerU16>();
					pixel.blue = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 1_ix, 4_sz)), 4_sz);
					pixel.green = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 5_ix, 4_sz)), 4_sz);
					pixel.red = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 9_ix, 4_sz)), 4_sz);
					pixel.alpha = Image::uncompress_channel(cbw<Image::Channel>(clip_bit(data_16, 13_ix, 4_sz)), 4_sz);
				} else if constexpr (format == Format::argb_8888_l) {
					auto data_32 = data.read_of<IntegerU32>();
					pixel.blue = cbw<Image::Channel>(clip_bit(data_32, 1_ix, 8_sz));
					pixel.green = cbw<Image::Channel>(clip_bit(data_32, 9_ix, 8_sz));
					pixel.red = cbw<Image::Channel>(clip_bit(data_32, 17_ix, 8_sz));
					pixel.alpha = cbw<Image::Channel>(clip_bit(data_32, 25_ix, 8_sz));
				}
				return;
			}

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process_image (
				IByteStreamView &          data,
				Image::VBitmapView const & image
			) -> Void {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						process_pixel<format>(data, pixel);
					}
				}
				return;
			}

			// ----------------

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process (
				IByteStreamView & data,
				Image::Pixel &    pixel
			) -> Void {
				process_pixel<format>(data, pixel);
				return;
			}

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Format>)
			inline auto process (
				IByteStreamView &          data,
				Image::VBitmapView const & image
			) -> Void {
				process_image<format>(data, image);
				return;
			}

			inline auto process (
				IByteStreamView & data,
				Image::Pixel &    pixel,
				Format const &    format
			) -> Void {
				Generalization::execute<FormatEnum>(
					format,
					[&] <auto index, auto format> (ValuePackage<index, format>) {
						process<format>(data, pixel);
					}
				);
				return;
			}

			inline auto process (
				IByteStreamView &          data,
				Image::VBitmapView const & image,
				Format const &             format
			) -> Void {
				Generalization::execute<FormatEnum>(
					format,
					[&] <auto index, auto format> (ValuePackage<index, format>) {
						process<format>(data, image);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}

namespace TwinKleS::Core::Trait::Reflection {

	M_reflection_enum(
		M_wrap(Tool::Texture::Format),
		M_wrap(Format),
		M_wrap(a_8, rgb_888, rgba_8888, rgb_565_l, rgba_4444_l, rgba_5551_l, argb_4444_l, argb_8888_l),
	);

}
