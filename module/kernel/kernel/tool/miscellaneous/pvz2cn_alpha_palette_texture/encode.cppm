module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.common;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnAlphaPaletteTexture {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			OutputBinaryStreamView &         data,
			Image::ConstantImageView const & image,
			List<Image::Color> const &       palette
		) -> Void {
			auto bit_count = test_palette(palette);
			auto index_data = data.split_forward(image.size().area() * bit_count / type_bit_size_of<Byte>());
			auto index_table = List<Optional<Size>>{0b1_sz << k_maximum_bit_count};
			for (auto & index : SizeRange{palette.size()}) {
				index_table[cast_box<Size>(palette[index])].set(index);
			}
			auto bit_reserve = type_bit_size_of<Byte>();
			auto current_byte = k_null_byte;
			for (auto & y : SizeRange{image.size().height}) {
				for (auto & x : SizeRange{image.size().width}) {
					auto & pixel = image[x, y];
					auto   alpha = Image::compress_color(pixel.alpha, k_maximum_bit_count);
					auto   index_value = cast_box<Byte>(index_table[cast_box<Size>(alpha)].get());
					if (bit_reserve > bit_count) {
						current_byte |= index_value << (bit_reserve - bit_count);
						bit_reserve -= bit_count;
					}
					else {
						if (bit_reserve == bit_count) {
							current_byte |= index_value << (bit_reserve - bit_count);
							index_data.write(current_byte);
							current_byte = k_null_byte;
							bit_reserve = type_bit_size_of<Byte>();
						}
						else {
							auto first_block_size = bit_reserve;
							auto second_block_size = bit_count - first_block_size;
							auto first_block = Bitwise::extract(index_value, second_block_size, first_block_size);
							auto second_block = Bitwise::extract(index_value, 0_sz, second_block_size);
							current_byte |= first_block;
							index_data.write(current_byte);
							current_byte = k_null_byte;
							current_byte |= second_block << (type_bit_size_of<Byte>() - second_block_size);
							bit_reserve = type_bit_size_of<Byte>() - second_block_size;
						}
					}
				}
			}
			if (bit_reserve != type_bit_size_of<Byte>()) {
				index_data.write(current_byte);
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView &         data_,
			Image::ConstantImageView const & image,
			List<Image::Color> const &       palette
		) -> Void {
			M_use_split_stream_of(data);
			return process_image(data, image, palette);
		}

	};

}
