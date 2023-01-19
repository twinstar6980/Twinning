#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third/libpng/png.h"
#include "third/libpng/pngstruct.h"
#include "third/libpng/pnginfo.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Core::ThirdParty::libpng {

	using size_t = ::size_t;

	using png_structp = ::png_structp;

	using png_bytep = ::png_bytep;

	using png_const_bytep = ::png_const_bytep;

	using png_const_charp = ::png_const_charp;

	using png_uint_32 = ::png_uint_32;

	inline constexpr auto png_error = ::png_error;

	inline constexpr auto png_set_error_fn = ::png_set_error_fn;

	inline constexpr auto png_create_info_struct = ::png_create_info_struct;

	inline constexpr auto png_create_read_struct = ::png_create_read_struct;

	inline constexpr auto png_destroy_read_struct = ::png_destroy_read_struct;

	inline constexpr auto png_create_write_struct = ::png_create_write_struct;

	inline constexpr auto png_destroy_write_struct = ::png_destroy_write_struct;

	inline constexpr auto png_read_info = ::png_read_info;

	inline constexpr auto png_read_row = ::png_read_row;

	inline constexpr auto png_read_end = ::png_read_end;

	inline constexpr auto png_write_info = ::png_write_info;

	inline constexpr auto png_write_row = ::png_write_row;

	inline constexpr auto png_write_end = ::png_write_end;

	inline constexpr auto png_set_IHDR = ::png_set_IHDR;

	inline constexpr auto png_set_read_fn = ::png_set_read_fn;

	inline constexpr auto png_set_write_fn = ::png_set_write_fn;

	inline constexpr auto png_init_io = ::png_init_io;

	inline constexpr auto png_get_io_ptr = ::png_get_io_ptr;

	inline constexpr auto PNG_COLOR_TYPE_RGBA_ = PNG_COLOR_TYPE_RGBA;

	inline constexpr auto PNG_INTERLACE_NONE_ = PNG_INTERLACE_NONE;

	inline constexpr auto PNG_COMPRESSION_TYPE_DEFAULT_ = PNG_COMPRESSION_TYPE_DEFAULT;

	inline constexpr auto PNG_FILTER_TYPE_DEFAULT_ = PNG_FILTER_TYPE_DEFAULT;

	inline constexpr auto PNG_LIBPNG_VER_STRING_ = PNG_LIBPNG_VER_STRING;

}
