module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.file.png.read;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.file.png.common;
import twinning.kernel.third.libpng;

export namespace Twinning::Kernel::Tool::Texture::File::PNG {

	struct Read :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			auto png_struct = Third::libpng::$png_create_read_struct(Third::libpng::$PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			Third::libpng::$png_set_error_fn(png_struct, nullptr, &png_error, &png_warning);
			Third::libpng::$png_set_read_fn(png_struct, &data, &png_read_data);
			auto png_info = Third::libpng::$png_create_info_struct(png_struct);
			Third::libpng::$png_read_info(png_struct, png_info);
			assert_test(image.size() == Image::ImageSize{mbox<Size>(Third::libpng::$png_get_image_width(png_struct, png_info)), mbox<Size>(Third::libpng::$png_get_image_height(png_struct, png_info))});
			auto png_bit_depth = Third::libpng::$png_get_bit_depth(png_struct, png_info);
			switch (Third::libpng::$png_get_color_type(png_struct, png_info)) {
				case Third::libpng::$PNG_COLOR_TYPE_PALETTE : {
					Third::libpng::$png_set_add_alpha(png_struct, 0xFF, Third::libpng::$PNG_FILLER_AFTER);
					Third::libpng::$png_set_palette_to_rgb(png_struct);
					break;
				}
				case Third::libpng::$PNG_COLOR_TYPE_GRAY : {
					Third::libpng::$png_set_add_alpha(png_struct, 0xFF, Third::libpng::$PNG_FILLER_AFTER);
					if (png_bit_depth == 1 || png_bit_depth == 2 || png_bit_depth == 4) {
						Third::libpng::$png_set_expand_gray_1_2_4_to_8(png_struct);
					}
					[[fallthrough]];
				}
				case Third::libpng::$PNG_COLOR_TYPE_GRAY_ALPHA : {
					if (png_bit_depth == 16) {
						Third::libpng::$png_set_scale_16(png_struct);
					}
					Third::libpng::$png_set_gray_to_rgb(png_struct);
					break;
				}
				case Third::libpng::$PNG_COLOR_TYPE_RGB : {
					Third::libpng::$png_set_add_alpha(png_struct, 0xFF, Third::libpng::$PNG_FILLER_AFTER);
					[[fallthrough]];
				}
				case Third::libpng::$PNG_COLOR_TYPE_RGB_ALPHA : {
					if (png_bit_depth == 16) {
						Third::libpng::$png_set_scale_16(png_struct);
					}
					break;
				}
				default : {
					throw ImpossibleException{};
				}
			}
			auto png_trans_alpha = Third::libpng::$png_bytep{};
			auto png_num_trans = int{};
			auto png_trans_color = Third::libpng::$png_color_16p{};
			if (Third::libpng::$png_get_tRNS(png_struct, png_info, &png_trans_alpha, &png_num_trans, &png_trans_color) != 0) {
				Third::libpng::$png_set_tRNS_to_alpha(png_struct);
			}
			auto png_background = Third::libpng::$png_color_16p{};
			if (Third::libpng::$png_get_bKGD(png_struct, png_info, &png_background) != 0) {
				Third::libpng::$png_set_background(png_struct, png_background, Third::libpng::$PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
			}
			auto png_file_srgb_intent = int{};
			auto png_file_gamma = double{};
			if (Third::libpng::$png_get_sRGB(png_struct, png_info, &png_file_srgb_intent) != 0) {
				Third::libpng::$png_set_gamma(png_struct, Third::libpng::$PNG_DEFAULT_sRGB, Third::libpng::$PNG_DEFAULT_sRGB);
			}
			else if (Third::libpng::$png_get_gAMA(png_struct, png_info, &png_file_gamma) != 0) {
				Third::libpng::$png_set_gamma(png_struct, Third::libpng::$PNG_DEFAULT_sRGB, png_file_gamma);
			}
			else {
				Third::libpng::$png_set_gamma(png_struct, Third::libpng::$PNG_DEFAULT_sRGB, 0.45455);
			}
			for (auto & row : image.data()) {
				Third::libpng::$png_read_row(png_struct, reinterpret_cast<Third::libpng::$png_bytep>(row.begin().value), nullptr);
			}
			Third::libpng::$png_read_end(png_struct, png_info);
			Third::libpng::$png_destroy_read_struct(&png_struct, &png_info, nullptr);
			return;
		}

		// ----------------

		inline static auto estimate_image (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			auto stream = IByteStreamView{data};
			auto png_struct = Third::libpng::$png_create_read_struct(Third::libpng::$PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			Third::libpng::$png_set_error_fn(png_struct, nullptr, &png_error, &png_warning);
			Third::libpng::$png_set_read_fn(png_struct, &stream, &png_read_data);
			auto png_info = Third::libpng::$png_create_info_struct(png_struct);
			Third::libpng::$png_read_info(png_struct, png_info);
			image_size = Image::ImageSize{mbox<Size>(Third::libpng::$png_get_image_width(png_struct, png_info)), mbox<Size>(Third::libpng::$png_get_image_height(png_struct, png_info))};
			Third::libpng::$png_destroy_read_struct(&png_struct, &png_info, nullptr);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &         data_,
			Image::VImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

		inline static auto estimate (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			restruct(image_size);
			return estimate_image(data, image_size);
		}

	};

}
