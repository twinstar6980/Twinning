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

		template <auto t_size> requires
			CategoryConstraint<>
			&& (IsSameOf<t_size, Size>)
		inline static auto process_pixel(
			OutputBinaryStreamView & data,
			Image::Pixel const &     pixel,
			Format const &           format
		) -> Void {
			auto block = IntegerU32{};
			for (auto & channel : format.channel) {
				auto color = Image::Color{};
				switch (channel.get<1_ix>().value) {
					case Channel::Constant::minimum().value: {
						color = Image::k_color_minimum;
						break;
					}
					case Channel::Constant::maximum().value: {
						color = Image::k_color_maximum;
						break;
					}
					case Channel::Constant::red().value: {
						color = pixel.red;
						break;
					}
					case Channel::Constant::green().value: {
						color = pixel.green;
						break;
					}
					case Channel::Constant::blue().value: {
						color = pixel.blue;
						break;
					}
					case Channel::Constant::alpha().value: {
						color = pixel.alpha;
						break;
					}
					case Channel::Constant::luminance().value: {
						color = convert_luminance_from_rgb(pixel.red, pixel.green, pixel.blue);
						break;
					}
					default: throw UnreachableException{};
				}
				auto part = cast_box_unsafe<IntegerU32>(Image::compress_color(color, channel.get<2_ix>()));
				block <<= channel.get<2_ix>();
				block |= Bitwise::infuse(part, 0_sz, channel.get<2_ix>());
			}
			if (!format.endian) {
				if constexpr (t_size == 1_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
				if constexpr (t_size == 2_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 2_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
				if constexpr (t_size == 3_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 2_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 3_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
				if constexpr (t_size == 4_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 2_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 3_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 4_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
			}
			else {
				if constexpr (t_size == 1_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
				if constexpr (t_size == 2_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 2_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
				if constexpr (t_size == 3_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 3_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 2_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
				if constexpr (t_size == 4_sz) {
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 4_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 3_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 2_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
					data.write(cast_box_unsafe<IntegerU8>(Bitwise::extract(block, 1_ix * type_bit_size_of<Byte>(), type_bit_size_of<Byte>())));
				}
			}
			return;
		}

		// ----------------

		inline static auto process_image(
			OutputBinaryStreamView &         data,
			Image::ConstantImageView const & image,
			Format const &                   format
		) -> Void {
			assert_test(is_valid_format(format));
			auto byte_count = get_pixel_byte_count(format);
			if (byte_count == 1_sz) {
				for (auto & y : SizeRange{image.size().height}) {
					for (auto & x : SizeRange{image.size().width}) {
						auto & pixel = image[x, y];
						process_pixel<1_sz>(data, pixel, format);
					}
				}
			}
			if (byte_count == 2_sz) {
				for (auto & y : SizeRange{image.size().height}) {
					for (auto & x : SizeRange{image.size().width}) {
						auto & pixel = image[x, y];
						process_pixel<2_sz>(data, pixel, format);
					}
				}
			}
			if (byte_count == 3_sz) {
				for (auto & y : SizeRange{image.size().height}) {
					for (auto & x : SizeRange{image.size().width}) {
						auto & pixel = image[x, y];
						process_pixel<3_sz>(data, pixel, format);
					}
				}
			}
			if (byte_count == 4_sz) {
				for (auto & y : SizeRange{image.size().height}) {
					for (auto & x : SizeRange{image.size().width}) {
						auto & pixel = image[x, y];
						process_pixel<4_sz>(data, pixel, format);
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView &         data_,
			Image::ConstantImageView const & image,
			Format const &                   format
		) -> Void {
			M_use_split_stream_of(data);
			return process_image(data, image, format);
		}

	};

}
