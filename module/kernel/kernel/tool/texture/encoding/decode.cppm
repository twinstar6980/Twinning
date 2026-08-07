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

		template <auto t_size> requires
			CategoryConstraint<>
			&& (IsSameOf<t_size, Size>)
		inline static auto process_pixel(
			InputByteStreamView & data,
			Image::Pixel &        pixel,
			Format const &        format
		) -> Void {
			auto block = IntegerU32{};
			if (!format.endian) {
				if constexpr (t_size == 1_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
				if constexpr (t_size == 2_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 2_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
				if constexpr (t_size == 3_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 2_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 3_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
				if constexpr (t_size == 4_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 2_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 3_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 4_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
			}
			else {
				if constexpr (t_size == 1_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
				if constexpr (t_size == 2_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 2_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
				if constexpr (t_size == 3_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 3_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 2_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
				if constexpr (t_size == 4_sz) {
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 4_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 3_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 2_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
					block |= Bitwise::infuse(cast_box_unsafe<IntegerU32>(data.read_of<IntegerU8>()), 1_ix * k_type_bit_count<Byte>, k_type_bit_count<Byte>);
				}
			}
			for (auto & channel : Range::make_reverse_range_of(format.channel)) {
				auto part = Bitwise::extract(block, 0_sz, channel.get<2_ix>());
				block >>= channel.get<2_ix>();
				auto color = Image::uncompress_color(cast_box_unsafe<Image::Color>(part), channel.get<2_ix>());
				switch (channel.get<1_ix>().value) {
					case Channel::Constant::minimum().value: {
						break;
					}
					case Channel::Constant::maximum().value: {
						break;
					}
					case Channel::Constant::red().value: {
						pixel.red = color;
						break;
					}
					case Channel::Constant::green().value: {
						pixel.green = color;
						break;
					}
					case Channel::Constant::blue().value: {
						pixel.blue = color;
						break;
					}
					case Channel::Constant::alpha().value: {
						pixel.alpha = color;
						break;
					}
					case Channel::Constant::luminance().value: {
						pixel.red = color;
						pixel.green = color;
						pixel.blue = color;
						break;
					}
					default: throw UnreachableException{};
				}
			}
			return;
		}

		// ----------------

		inline static auto process_image(
			InputByteStreamView &            data,
			Image::VariableImageView const & image,
			Format const &                   format
		) -> Void {
			assert_test(is_valid_format(format));
			auto byte_count = get_pixel_byte_count(format);
			if (byte_count == 1_sz) {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						process_pixel<1_sz>(data, pixel, format);
					}
				}
			}
			if (byte_count == 2_sz) {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						process_pixel<2_sz>(data, pixel, format);
					}
				}
			}
			if (byte_count == 3_sz) {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						process_pixel<3_sz>(data, pixel, format);
					}
				}
			}
			if (byte_count == 4_sz) {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						process_pixel<4_sz>(data, pixel, format);
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &            data_,
			Image::VariableImageView const & image,
			Format const &                   format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
