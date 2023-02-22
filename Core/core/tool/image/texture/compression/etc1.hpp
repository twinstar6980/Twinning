#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/image.hpp"
#include "core/third/ETCPACK.hpp"
#include "core/third/etcpak.hpp"

namespace TwinStar::Core::Tool::Image::Texture::Compression::ETC1 {

	struct CompressCommon {

	protected:

		inline static constexpr auto k_block_width = Size{4_sz};

		inline static constexpr auto k_bpp = Size{4_sz};

	};

	struct Compress :
		CompressCommon {

	protected:

		static auto process_image_rgb (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			assert_test(is_padded_size(image.size().width, k_block_width));
			assert_test(is_padded_size(image.size().height, k_block_width));
			auto image_block = Array<Image::Pixel>{k_block_width * k_block_width};
			for (auto & block_y : SizeRange{image.size().height / k_block_width}) {
				for (auto & block_x : SizeRange{image.size().width / k_block_width}) {
					for (auto & pixel_y : SizeRange{k_block_width}) {
						for (auto & pixel_x : SizeRange{k_block_width}) {
							auto & pixel = image[block_y * k_block_width + pixel_y][block_x * k_block_width + pixel_x];
							auto & block_pixel = image_block[pixel_y * k_block_width + pixel_x];
							block_pixel.red = pixel.blue;
							block_pixel.green = pixel.green;
							block_pixel.blue = pixel.red;
							block_pixel.alpha = Image::k_color_maximum;
						}
					}
					Third::etcpak::CompressEtc1Rgb(cast_pointer<std::uint32_t>(image_block.begin()).value, cast_pointer<std::uint64_t>(data.current_pointer()).value, 1, k_block_width.value);
					data.forward(k_block_width * k_block_width * k_bpp / k_type_bit_count<Byte>);
				}
			}
			return;
		}

		// ----------------

		static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			process_image_rgb(data, image);
			return;
		}

	public:

		static auto do_process_image (
			OByteStreamView &         data_,
			Image::CImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

	struct Uncompress :
		CompressCommon {

	protected:

		static auto process_image_rgb (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			assert_test(is_padded_size(image.size().width, k_block_width));
			assert_test(is_padded_size(image.size().height, k_block_width));
			auto image_block = Array<Image::Pixel>{k_block_width * k_block_width};
			for (auto & block_y : SizeRange{image.size().height / k_block_width}) {
				for (auto & block_x : SizeRange{image.size().width / k_block_width}) {
					auto block_part_1 = reverse_endian(data.read_of<IntegerU32>());
					auto block_part_2 = reverse_endian(data.read_of<IntegerU32>());
					Third::ETCPACK::decompressBlockETC2c(
						self_cast<unsigned>(block_part_1),
						self_cast<unsigned>(block_part_2),
						cast_pointer<Third::ETCPACK::uint8>(image_block.begin()).value,
						static_cast<int>(k_block_width.value),
						static_cast<int>(k_block_width.value),
						static_cast<int>(k_begin_index.value),
						static_cast<int>(k_begin_index.value),
						4
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

		// ----------------

		static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			process_image_rgb(data, image);
			return;
		}

	public:

		static auto do_process_image (
			IByteStreamView &         data_,
			Image::VImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

}
