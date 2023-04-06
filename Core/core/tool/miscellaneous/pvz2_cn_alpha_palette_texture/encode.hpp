#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Miscellaneous::PvZ2CNAlphaPaletteTexture {

	struct EncodeCommon {

	protected:

		inline static constexpr auto k_maximum_bit_count = Size{4_sz};

		// ----------------

		static auto test_palette (
			List<Image::Color> const & palette
		) -> Size {
			assert_test(0b1_sz < palette.size() && palette.size() <= 0b1_sz << k_maximum_bit_count);
			auto bit_count = k_none_size;
			for (auto & index : SizeRange{k_maximum_bit_count}) {
				++bit_count;
				if (palette.size() <= 0b1_sz << bit_count) {
					break;
				}
			}
			auto maximum_value = cbw<Image::Color>(0b1_iu8 << k_maximum_bit_count);
			assert_test(Range::all_of(palette, [&] (auto & element) { return element <= maximum_value; }));
			return bit_count;
		}

	};

	struct Encode :
		EncodeCommon {

	protected:

		static auto process_image (
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
				index_table[cbw<Size>(palette[index])].set(index);
			}
			auto bit_reserve = k_type_bit_count<Byte>;
			while (!index_data.full()) {
				if (pixel_row.full()) {
					pixel_row.set(pixel_row_stream.next());
				}
				auto & pixel = pixel_row.next();
				auto   alpha = Image::compress_color(pixel.alpha, k_maximum_bit_count);
				auto   index_value = cbw<Byte>(index_table[cbw<Size>(alpha)].get());
				if (bit_reserve > bit_count) {
					index_data.current() |= index_value << (bit_reserve - bit_count);
					bit_reserve -= bit_count;
				} else {
					if (bit_reserve == bit_count) {
						index_data.current() |= index_value << (bit_reserve - bit_count);
						index_data.forward();
						bit_reserve = k_type_bit_count<Byte>;
					} else {
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

	public:

		static auto do_process_image (
			OByteStreamView &          data_,
			Image::CImageView const &  image,
			List<Image::Color> const & palette
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, palette);
		}

	};

	struct Decode :
		EncodeCommon {

	protected:

		static auto process_image (
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
				} else {
					if (bit_reserve == bit_count) {
						index_value = clip_bit(index_data.current(), bit_reserve - bit_count, bit_count);
						index_data.forward();
						bit_reserve = k_type_bit_count<Byte>;
					} else {
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
				pixel.alpha = Image::uncompress_color(palette[cbw<Size>(index_value)], k_maximum_bit_count);
			}
			return;
		}

	public:

		static auto do_process_image (
			IByteStreamView &          data_,
			Image::VImageView const &  image,
			List<Image::Color> const & palette
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, palette);
		}

	};

}
