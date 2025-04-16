module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/libpng/png.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.libpng;

export namespace Twinning::Kernel::Third::libpng {

	using $size_t = ::size_t;

	using $png_structp = ::png_structp;

	using $png_bytep = ::png_bytep;

	using $png_const_bytep = ::png_const_bytep;

	using $png_const_charp = ::png_const_charp;

	using $png_color_16p = ::png_color_16p;

	using $png_uint_32 = ::png_uint_32;

	inline constexpr auto $png_error = ::png_error;

	inline constexpr auto $png_set_error_fn = ::png_set_error_fn;

	inline constexpr auto $png_set_read_fn = ::png_set_read_fn;

	inline constexpr auto $png_set_write_fn = ::png_set_write_fn;

	inline constexpr auto $png_get_io_ptr = ::png_get_io_ptr;

	inline constexpr auto $png_create_read_struct = ::png_create_read_struct;

	inline constexpr auto $png_destroy_read_struct = ::png_destroy_read_struct;

	inline constexpr auto $png_create_write_struct = ::png_create_write_struct;

	inline constexpr auto $png_destroy_write_struct = ::png_destroy_write_struct;

	inline constexpr auto $png_create_info_struct = ::png_create_info_struct;

	inline constexpr auto $png_read_info = ::png_read_info;

	inline constexpr auto $png_read_row = ::png_read_row;

	inline constexpr auto $png_read_end = ::png_read_end;

	inline constexpr auto $png_write_info = ::png_write_info;

	inline constexpr auto $png_write_row = ::png_write_row;

	inline constexpr auto $png_write_end = ::png_write_end;

	inline constexpr auto $png_get_image_width = ::png_get_image_width;

	inline constexpr auto $png_get_image_height = ::png_get_image_height;

	inline constexpr auto $png_get_color_type = ::png_get_color_type;

	inline constexpr auto $png_get_bit_depth = ::png_get_bit_depth;

	inline constexpr auto $png_set_IHDR = ::png_set_IHDR;

	inline constexpr auto $png_get_sRGB = ::png_get_sRGB;

	inline constexpr auto $png_get_gAMA = ::png_get_gAMA;

	inline constexpr auto $png_get_tRNS = ::png_get_tRNS;

	inline constexpr auto $png_get_bKGD = ::png_get_bKGD;

	inline constexpr auto $png_set_background = ::png_set_background;

	inline constexpr auto $png_set_add_alpha = ::png_set_add_alpha;

	inline constexpr auto $png_set_tRNS_to_alpha = ::png_set_tRNS_to_alpha;

	inline constexpr auto $png_set_palette_to_rgb = ::png_set_palette_to_rgb;

	inline constexpr auto $png_set_expand_gray_1_2_4_to_8 = ::png_set_expand_gray_1_2_4_to_8;

	inline constexpr auto $png_set_gray_to_rgb = ::png_set_gray_to_rgb;

	inline constexpr auto $png_set_scale_16 = ::png_set_scale_16;

	inline constexpr auto $png_set_gamma = ::png_set_gamma;

	inline constexpr auto $PNG_LIBPNG_VER_STRING = PNG_LIBPNG_VER_STRING;

	inline constexpr auto $PNG_DEFAULT_sRGB = PNG_DEFAULT_sRGB;

	inline constexpr auto $PNG_FILLER_AFTER = PNG_FILLER_AFTER;

	inline constexpr auto $PNG_BACKGROUND_GAMMA_FILE = PNG_BACKGROUND_GAMMA_FILE;

	inline constexpr auto $PNG_COLOR_TYPE_PALETTE = PNG_COLOR_TYPE_PALETTE;

	inline constexpr auto $PNG_COLOR_TYPE_GRAY = PNG_COLOR_TYPE_GRAY;

	inline constexpr auto $PNG_COLOR_TYPE_GRAY_ALPHA = PNG_COLOR_TYPE_GRAY_ALPHA;

	inline constexpr auto $PNG_COLOR_TYPE_RGB = PNG_COLOR_TYPE_RGB;

	inline constexpr auto $PNG_COLOR_TYPE_RGB_ALPHA = PNG_COLOR_TYPE_RGB_ALPHA;

	inline constexpr auto $PNG_INTERLACE_NONE = PNG_INTERLACE_NONE;

	inline constexpr auto $PNG_COMPRESSION_TYPE_DEFAULT = PNG_COMPRESSION_TYPE_DEFAULT;

	inline constexpr auto $PNG_FILTER_TYPE_DEFAULT = PNG_FILTER_TYPE_DEFAULT;

}
