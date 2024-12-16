module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.common;

export namespace Twinning::Kernel::Tool::Miscellaneous::PvZ2CNAlphaPaletteTexture {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image (
			IByteStreamView &          data,
			Image::VImageView const &  image,
			List<Image::Color> const & palette
		) -> Void {
			auto bit_count = test_palette(palette);
			auto index_data = IByteStreamView{data.forward_view(image.size().area() * bit_count / k_type_bit_count<Byte>)};
			auto pixel_row_stream = IStreamView<VListView<Image::Pixel>>{image.data()};
			auto pixel_row = OStreamView<Image::Pixel>{};
			auto bit_reserve = k_type_bit_count<Byte>;
			while (!(pixel_row_stream.full() && pixel_row.full())) {
				auto index_value = Byte{};
				if (pixel_row.full()) {
					pixel_row.set(pixel_row_stream.next());
				}
				if (bit_reserve > bit_count) {
					index_value = clip_bit(index_data.current(), bit_reserve - bit_count, bit_count);
					bit_reserve -= bit_count;
				}
				else {
					if (bit_reserve == bit_count) {
						index_value = clip_bit(index_data.current(), bit_reserve - bit_count, bit_count);
						index_data.forward();
						bit_reserve = k_type_bit_count<Byte>;
					}
					else {
						auto first_block_size = bit_reserve;
						auto second_block_size = bit_count - first_block_size;
						auto first_block = clip_bit(index_data.current(), k_begin_index, first_block_size);
						index_data.forward();
						auto second_block = clip_bit(index_data.current(), k_type_bit_count<Byte> - second_block_size, second_block_size);
						index_value = (first_block << second_block_size) | second_block;
						bit_reserve = k_type_bit_count<Byte> - second_block_size;
					}
				}
				auto & pixel = pixel_row.next();
				pixel.alpha = Image::uncompress_color(palette[cbox<Size>(index_value)], k_maximum_bit_count);
			}
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &          data_,
			Image::VImageView const &  image,
			List<Image::Color> const & palette
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, palette);
		}

	};

}
