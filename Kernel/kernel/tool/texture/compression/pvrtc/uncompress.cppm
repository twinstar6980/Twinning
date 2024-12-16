module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.pvrtc.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.third.PVRTCCompressor;

export namespace Twinning::Kernel::Tool::Texture::Compression::PVRTC {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image_v1_4bpp_rgb (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			assert_test(is_padded_size(image.size().width, k_block_width));
			assert_test(is_padded_size(image.size().height, k_block_width));
			auto proxy_image_data = Array<Third::PVRTCCompressor::ColorRgb<unsigned char>>{image.size().area()};
			Third::PVRTCCompressor::PvrTcDecoder::DecodeRgb4Bpp(proxy_image_data.begin().value, Third::PVRTCCompressor::Point2<int>{static_cast<int>(image.size().width.value), static_cast<int>(image.size().height.value)}, data.current_pointer().value);
			for (auto & pixel_y : SizeRange{image.size().height}) {
				for (auto & pixel_x : SizeRange{image.size().width}) {
					auto & pixel = image[pixel_y][pixel_x];
					auto & proxy_pixel = proxy_image_data[pixel_y * image.size().width + pixel_x];
					pixel.red.value = proxy_pixel.r;
					pixel.green.value = proxy_pixel.g;
					pixel.blue.value = proxy_pixel.b;
				}
			}
			data.forward(image.size().area() * k_bpp_4 / k_type_bit_count<Byte>);
			return;
		}

		inline static auto process_image_v1_4bpp_rgba (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			assert_test(is_padded_size(image.size().width, k_block_width));
			assert_test(is_padded_size(image.size().height, k_block_width));
			auto proxy_image_data = Array<Third::PVRTCCompressor::ColorRgba<unsigned char>>{image.size().area()};
			Third::PVRTCCompressor::PvrTcDecoder::DecodeRgba4Bpp(proxy_image_data.begin().value, Third::PVRTCCompressor::Point2<int>{static_cast<int>(image.size().width.value), static_cast<int>(image.size().height.value)}, data.current_pointer().value);
			for (auto & pixel_y : SizeRange{image.size().height}) {
				for (auto & pixel_x : SizeRange{image.size().width}) {
					auto & pixel = image[pixel_y][pixel_x];
					auto & proxy_pixel = proxy_image_data[pixel_y * image.size().width + pixel_x];
					pixel.red.value = proxy_pixel.r;
					pixel.green.value = proxy_pixel.g;
					pixel.blue.value = proxy_pixel.b;
					pixel.alpha.value = proxy_pixel.a;
				}
			}
			data.forward(image.size().area() * k_bpp_4 / k_type_bit_count<Byte>);
			return;
		}

		// ----------------

		inline static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image,
			Format const &            format
		) -> Void {
			if (format == Format::Constant::v1_4bpp_rgb()) {
				process_image_v1_4bpp_rgb(data, image);
			}
			if (format == Format::Constant::v1_4bpp_rgba()) {
				process_image_v1_4bpp_rgba(data, image);
			}
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &         data_,
			Image::VImageView const & image,
			Format const &            format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
