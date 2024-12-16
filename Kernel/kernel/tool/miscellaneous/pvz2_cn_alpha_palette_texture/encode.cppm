module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.common;

export namespace Twinning::Kernel::Tool::Miscellaneous::PvZ2CNAlphaPaletteTexture {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image (
			OByteStreamView &          data,
			Image::CImageView const &  image,
			List<Image::Color> const & palette
		) -> Void {
			auto bit_count = test_palette(palette);
			auto index_data = OByteStreamView{data.forward_view(image.size().area() * bit_count / k_type_bit_count<Byte>)};
			auto pixel_row_stream = IStreamView<CListView<Image::Pixel>>{image.data()};
			auto pixel_row = IStreamView<Image::Pixel>{};
			auto index_table = Array<Optional<Size>>{0b1_sz << k_maximum_bit_count};
			for (auto & index : SizeRange{palette.size()}) {
				index_table[cbox<Size>(palette[index])].set(index);
			}
			auto bit_reserve = k_type_bit_count<Byte>;
			while (!index_data.full()) {
				if (pixel_row.full()) {
					pixel_row.set(pixel_row_stream.next());
				}
				auto & pixel = pixel_row.next();
				auto   alpha = Image::compress_color(pixel.alpha, k_maximum_bit_count);
				auto   index_value = cbox<Byte>(index_table[cbox<Size>(alpha)].get());
				if (bit_reserve > bit_count) {
					index_data.current() |= index_value << (bit_reserve - bit_count);
					bit_reserve -= bit_count;
				}
				else {
					if (bit_reserve == bit_count) {
						index_data.current() |= index_value << (bit_reserve - bit_count);
						index_data.forward();
						bit_reserve = k_type_bit_count<Byte>;
					}
					else {
						auto first_block_size = bit_reserve;
						auto second_block_size = bit_count - first_block_size;
						auto first_block = clip_bit(index_value, second_block_size, first_block_size);
						auto second_block = clip_bit(index_value, k_begin_index, second_block_size);
						index_data.current() |= first_block;
						index_data.forward();
						index_data.current() |= second_block << (k_type_bit_count<Byte> - second_block_size);
						bit_reserve = k_type_bit_count<Byte> - second_block_size;
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &          data_,
			Image::CImageView const &  image,
			List<Image::Color> const & palette
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, palette);
		}

	};

}
