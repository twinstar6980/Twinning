#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/ETCPACK.hpp"
#include "core/third_party/rg_etc1.hpp"

static_assert(sizeof(unsigned) == 4);

namespace TwinStar::Core::Tool::Texture::Compress::ETC1 {

	struct CompressCommon {

	protected:

		inline static constexpr auto k_bpp = Size{4_sz};

		inline static constexpr auto k_block_width = Size{4_sz};

	};

	struct Compress :
		CompressCommon {

	protected:

		// TODO : low quality
		static auto process_image_unused (
			OByteStreamView &          data,
			Image::CBitmapView const & image
		) -> Void {
			assert_condition(is_padded_size(image.size().width, k_block_width));
			assert_condition(is_padded_size(image.size().height, k_block_width));
			auto image_block = Array<Image::PixelRGB>{k_block_width * k_block_width};
			auto decompression_buffer = Array<Byte>{k_block_width * k_block_width * bs_static_size<Image::PixelRGB>()};
			for (auto & block_y : SizeRange{image.size().height / k_block_width}) {
				for (auto & block_x : SizeRange{image.size().width / k_block_width}) {
					for (auto & pixel_y : SizeRange{k_block_width}) {
						for (auto & pixel_x : SizeRange{k_block_width}) {
							auto & pixel = image[block_y * k_block_width + pixel_y][block_x * k_block_width + pixel_x];
							auto & block_pixel = image_block[pixel_y * k_block_width + pixel_x];
							block_pixel.red = pixel.red;
							block_pixel.green = pixel.green;
							block_pixel.blue = pixel.blue;
						}
					}
					auto block_part_1 = IntegerU32{};
					auto block_part_2 = IntegerU32{};
					Range::assign(decompression_buffer, 0x00_b);
					ThirdParty::ETCPACK::compressBlockETC1Exhaustive(
						cast_pointer<ThirdParty::ETCPACK::uint8>(image_block.begin()).value,
						cast_pointer<ThirdParty::ETCPACK::uint8>(decompression_buffer.begin()).value,
						static_cast<int>(k_block_width.value),
						static_cast<int>(k_block_width.value),
						static_cast<int>(k_begin_index.value),
						static_cast<int>(k_begin_index.value),
						self_cast<unsigned>(block_part_1),
						self_cast<unsigned>(block_part_2)
					);
					data.write(reverse_endian(block_part_1));
					data.write(reverse_endian(block_part_2));
				}
			}
			return;
		}

		static auto process_image (
			OByteStreamView &          data,
			Image::CBitmapView const & image
		) -> Void {
			assert_condition(is_padded_size(image.size().width, k_block_width));
			assert_condition(is_padded_size(image.size().height, k_block_width));
			auto image_block = Array<Image::PixelRGBA>{k_block_width * k_block_width};
			auto etc1_pack_params = ThirdParty::rg_etc1::etc1_pack_params{};
			ThirdParty::rg_etc1::pack_etc1_block_init();
			for (auto & block_y : SizeRange{image.size().height / k_block_width}) {
				M_log_no_line("\r## etc1-encode : {:>5.1f}% "_sf((cbw<Floating>(block_y) * 4.0_f / cbw<Floating>(image.size().height)) * 100.0_f));
				for (auto & block_x : SizeRange{image.size().width / k_block_width}) {
					for (auto & pixel_y : SizeRange{k_block_width}) {
						for (auto & pixel_x : SizeRange{k_block_width}) {
							auto & pixel = image[block_y * k_block_width + pixel_y][block_x * k_block_width + pixel_x];
							auto & block_pixel = image_block[pixel_y * k_block_width + pixel_x];
							block_pixel.red = pixel.red;
							block_pixel.green = pixel.green;
							block_pixel.blue = pixel.blue;
							block_pixel.alpha = Image::k_opaque_alpha;
						}
					}
					ThirdParty::rg_etc1::pack_etc1_block(data.current_pointer().value, cast_pointer<unsigned>(image_block.begin()).value, etc1_pack_params);
					data.forward(k_block_width * k_block_width * k_bpp / k_type_bit_count<Byte>);
				}
			}
			M_log("\r## etc1-encode : 100.0% "_s);
			return;
		}

	public:

		static auto do_process_image (
			OByteStreamView &          data_,
			Image::CBitmapView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

	struct Uncompress :
		CompressCommon {

	protected:

		static auto process_image (
			IByteStreamView &          data,
			Image::VBitmapView const & image
		) -> Void {
			assert_condition(is_padded_size(image.size().width, k_block_width));
			assert_condition(is_padded_size(image.size().height, k_block_width));
			auto image_block = Array<Image::PixelRGB>{k_block_width * k_block_width};
			for (auto & block_y : SizeRange{image.size().height / k_block_width}) {
				for (auto & block_x : SizeRange{image.size().width / k_block_width}) {
					auto block_part_1 = reverse_endian(data.read_of<IntegerU32>());
					auto block_part_2 = reverse_endian(data.read_of<IntegerU32>());
					ThirdParty::ETCPACK::decompressBlockETC2(
						self_cast<unsigned>(block_part_1),
						self_cast<unsigned>(block_part_2),
						cast_pointer<ThirdParty::ETCPACK::uint8>(image_block.begin()).value,
						static_cast<int>(k_block_width.value),
						static_cast<int>(k_block_width.value),
						static_cast<int>(k_begin_index.value),
						static_cast<int>(k_begin_index.value)
					);
					for (auto & pixel_y : SizeRange{k_block_width}) {
						for (auto & pixel_x : SizeRange{k_block_width}) {
							auto & pixel = image[block_y * k_block_width + pixel_y][block_x * k_block_width + pixel_x];
							auto & block_pixel = image_block[pixel_y * k_block_width + pixel_x];
							pixel.red = block_pixel.red;
							pixel.green = block_pixel.green;
							pixel.blue = block_pixel.blue;
						}
					}
				}
			}
			return;
		}

	public:

		static auto do_process_image (
			IByteStreamView &          data_,
			Image::VBitmapView const & image
		) -> Void {
			M_use_zps_of(data);
			// TODO : reset image
			return process_image(data, image);
		}

	};

}
