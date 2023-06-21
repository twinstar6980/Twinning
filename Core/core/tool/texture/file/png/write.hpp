#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/texture/file/png/common.hpp"
#include "core/third/libpng.hpp"

namespace TwinStar::Core::Tool::Texture::File::PNG {

	struct Write :
		Common {

		using Common = Common;

		// ----------------

		static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			auto png_struct = Third::libpng::png_create_write_struct(Third::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
			Third::libpng::png_set_error_fn(png_struct, nullptr, &png_error, &png_warning);
			Third::libpng::png_set_write_fn(png_struct, &data, png_write_data, &png_output_flush);
			auto png_info = Third::libpng::png_create_info_struct(png_struct);
			Third::libpng::png_set_IHDR(png_struct, png_info, static_cast<Third::libpng::png_uint_32>(image.size().width.value), static_cast<Third::libpng::png_uint_32>(image.size().height.value), static_cast<int>(k_type_bit_count<Image::Color>.value), Third::libpng::PNG_COLOR_TYPE_RGB_ALPHA_, Third::libpng::PNG_INTERLACE_NONE_, Third::libpng::PNG_COMPRESSION_TYPE_DEFAULT_, Third::libpng::PNG_FILTER_TYPE_DEFAULT_);
			Third::libpng::png_write_info(png_struct, png_info);
			for (auto & row : image.data()) {
				Third::libpng::png_write_row(png_struct, reinterpret_cast<Third::libpng::png_const_bytep>(row.begin().value));
			}
			Third::libpng::png_write_end(png_struct, png_info);
			Third::libpng::png_destroy_write_struct(&png_struct, &png_info);
			return;
		}

		// ----------------

		static auto process (
			OByteStreamView &         data_,
			Image::CImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

}
