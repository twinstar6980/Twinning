module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.common;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnAlphaPaletteTexture {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			InputBinaryStreamView &          data,
			Image::VariableImageView const & image,
			List<Image::Color> const &       palette
		) -> Void {
			auto bit_count = test_palette(palette);
			auto index_data = data.split_forward(image.size().area() * bit_count / type_bit_size_of<Byte>());
			auto bit_reserve = type_bit_size_of<Byte>();
			auto current_byte = k_null_byte;
			auto need_read = k_true;
			for (auto & y : SizeRange{image.size().height}) {
				for (auto & x : SizeRange{image.size().width}) {
					auto & pixel = image[x, y];
					auto   index_value = Byte{};
					if (need_read) {
						index_data.read(current_byte);
						need_read = k_false;
					}
					if (bit_reserve > bit_count) {
						index_value = Bitwise::extract(current_byte, bit_reserve - bit_count, bit_count);
						bit_reserve -= bit_count;
					}
					else {
						if (bit_reserve == bit_count) {
							index_value = Bitwise::extract(current_byte, bit_reserve - bit_count, bit_count);
							need_read = k_true;
							bit_reserve = type_bit_size_of<Byte>();
						}
						else {
							auto first_block_size = bit_reserve;
							auto second_block_size = bit_count - first_block_size;
							auto first_block = Bitwise::extract(current_byte, 0_sz, first_block_size);
							index_data.read(current_byte);
							auto second_block = Bitwise::extract(current_byte, type_bit_size_of<Byte>() - second_block_size, second_block_size);
							index_value = (first_block << second_block_size) | second_block;
							bit_reserve = type_bit_size_of<Byte>() - second_block_size;
						}
					}
					pixel.alpha = Image::uncompress_color(palette[cast_box<Size>(index_value)], k_maximum_bit_count);
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &          data_,
			Image::VariableImageView const & image,
			List<Image::Color> const &       palette
		) -> Void {
			M_use_split_stream_of(data);
			return process_image(data, image, palette);
		}

	};

}
