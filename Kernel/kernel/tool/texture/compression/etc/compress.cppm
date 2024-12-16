module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.etc.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.third.etcpak;

export namespace Twinning::Kernel::Tool::Texture::Compression::ETC {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image_v1_rgb (
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
					data.forward(k_block_width * k_block_width * k_bpp_rgb / k_type_bit_count<Byte>);
				}
			}
			return;
		}

		inline static auto process_image_v2_rgb (
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
					Third::etcpak::CompressEtc2Rgb(cast_pointer<std::uint32_t>(image_block.begin()).value, cast_pointer<std::uint64_t>(data.current_pointer()).value, 1, k_block_width.value, true);
					data.forward(k_block_width * k_block_width * k_bpp_rgb / k_type_bit_count<Byte>);
				}
			}
			return;
		}

		inline static auto process_image_v2_rgba (
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
							block_pixel.alpha = pixel.alpha;
						}
					}
					Third::etcpak::CompressEtc2Rgba(cast_pointer<std::uint32_t>(image_block.begin()).value, cast_pointer<std::uint64_t>(data.current_pointer()).value, 1, k_block_width.value, true);
					data.forward(k_block_width * k_block_width * k_bpp_rgba / k_type_bit_count<Byte>);
				}
			}
			return;
		}

		// ----------------

		inline static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image,
			Format const &            format
		) -> Void {
			if (format == Format::Constant::v1_rgb()) {
				process_image_v1_rgb(data, image);
			}
			if (format == Format::Constant::v2_rgb()) {
				process_image_v2_rgb(data, image);
			}
			if (format == Format::Constant::v2_rgba()) {
				process_image_v2_rgba(data, image);
			}
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &         data_,
			Image::CImageView const & image,
			Format const &            format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
