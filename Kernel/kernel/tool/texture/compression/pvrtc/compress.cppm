module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.pvrtc.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.third.PVRTCCompressor;

export namespace Twinning::Kernel::Tool::Texture::Compression::PVRTC {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image_v1_4bpp_rgb (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			assert_test(is_padded_size(image.size().width, k_block_width));
			assert_test(is_padded_size(image.size().height, k_block_width));
			auto proxy_image = Third::PVRTCCompressor::RgbBitmap{static_cast<int>(image.size().width.value), static_cast<int>(image.size().height.value)};
			auto proxy_image_data = VListView<Third::PVRTCCompressor::ColorRgb<unsigned char>>{make_pointer(proxy_image.GetData()), image.size().area()};
			for (auto & pixel_y : SizeRange{image.size().height}) {
				for (auto & pixel_x : SizeRange{image.size().width}) {
					auto & pixel = image[pixel_y][pixel_x];
					auto & proxy_pixel = proxy_image_data[pixel_y * image.size().width + pixel_x];
					proxy_pixel.r = pixel.red.value;
					proxy_pixel.g = pixel.green.value;
					proxy_pixel.b = pixel.blue.value;
				}
			}
			Third::PVRTCCompressor::PvrTcEncoder::EncodeRgb4Bpp(data.current_pointer().value, proxy_image);
			data.forward(image.size().area() * k_bpp_4 / k_type_bit_count<Byte>);
			return;
		}

		inline static auto process_image_v1_4bpp_rgba (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			assert_test(is_padded_size(image.size().width, k_block_width));
			assert_test(is_padded_size(image.size().height, k_block_width));
			auto proxy_image = Third::PVRTCCompressor::RgbaBitmap{static_cast<int>(image.size().width.value), static_cast<int>(image.size().height.value)};
			auto proxy_image_data = VListView<Third::PVRTCCompressor::ColorRgba<unsigned char>>{make_pointer(proxy_image.GetData()), image.size().area()};
			for (auto & pixel_y : SizeRange{image.size().height}) {
				for (auto & pixel_x : SizeRange{image.size().width}) {
					auto & pixel = image[pixel_y][pixel_x];
					auto & proxy_pixel = proxy_image_data[pixel_y * image.size().width + pixel_x];
					proxy_pixel.r = pixel.red.value;
					proxy_pixel.g = pixel.green.value;
					proxy_pixel.b = pixel.blue.value;
					proxy_pixel.a = pixel.alpha.value;
				}
			}
			Third::PVRTCCompressor::PvrTcEncoder::EncodeRgba4Bpp(data.current_pointer().value, proxy_image);
			data.forward(image.size().area() * k_bpp_4 / k_type_bit_count<Byte>);
			return;
		}

		// ----------------

		inline static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image,
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
			OByteStreamView &         data_,
			Image::CImageView const & image,
			Format const &            format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
