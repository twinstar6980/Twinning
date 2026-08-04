module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.conversion.png.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.conversion.png.common;
import twinning.kernel.dependency.libpng;

export namespace Twinning::Kernel::Tool::Texture::Conversion::Png {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			OutputByteStreamView &           data,
			Image::ConstantImageView const & image
		) -> Void {
			auto png_struct = Dependency::libpng::$png_create_write_struct(Dependency::libpng::$PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			Dependency::libpng::$png_set_error_fn(png_struct, nullptr, &png_error, &png_warning);
			Dependency::libpng::$png_set_write_fn(png_struct, &data, png_write_data, &png_output_flush);
			auto png_info = Dependency::libpng::$png_create_info_struct(png_struct);
			Dependency::libpng::$png_set_IHDR(png_struct, png_info, unmake_box<Dependency::libpng::$png_uint_32>(image.size().width), unmake_box<Dependency::libpng::$png_uint_32>(image.size().height), unmake_box<int>(k_type_bit_count<Image::Color>), Dependency::libpng::$PNG_COLOR_TYPE_RGB_ALPHA, Dependency::libpng::$PNG_INTERLACE_NONE, Dependency::libpng::$PNG_COMPRESSION_TYPE_DEFAULT, Dependency::libpng::$PNG_FILTER_TYPE_DEFAULT);
			Dependency::libpng::$png_write_info(png_struct, png_info);
			for (auto & row : image.data()) {
				Dependency::libpng::$png_write_row(png_struct, unmake_pointer_unsafe<Dependency::libpng::$png_byte>(row.begin()));
			}
			Dependency::libpng::$png_write_end(png_struct, png_info);
			Dependency::libpng::$png_destroy_write_struct(&png_struct, &png_info);
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView &           data_,
			Image::ConstantImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

}
