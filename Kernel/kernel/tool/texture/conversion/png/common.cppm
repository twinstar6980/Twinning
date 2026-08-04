module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.conversion.png.common;
import twinning.kernel.utility;
import twinning.kernel.dependency.libpng;

export namespace Twinning::Kernel::Tool::Texture::Conversion::Png {

	struct Common {

		inline static auto png_error(
			Dependency::libpng::$png_structp     png_ptr,
			Dependency::libpng::$png_const_charp error_message
		) -> Void {
			throw UnnamedException{make_std_string("libpng: {}"_sf(error_message))};
		}

		inline static auto png_warning(
			Dependency::libpng::$png_structp     png_ptr,
			Dependency::libpng::$png_const_charp error_message
		) -> Void {
			// NOTE: DEBUG
			return;
		}

		// ----------------

		inline static auto png_read_data(
			Dependency::libpng::$png_structp png_ptr,
			Dependency::libpng::$png_bytep   data,
			Dependency::libpng::$size_t      length
		) -> Void {
			auto & stream = *make_pointer_unsafe<InputByteStreamView>(Dependency::libpng::$png_get_io_ptr(png_ptr));
			if (stream.reserve() < make_box<Size>(length)) {
				Dependency::libpng::$png_error(png_ptr, "Read Error");
			}
			std::memcpy(data, unmake_pointer_unsafe<void>(stream.current_pointer()), length);
			stream.forward(make_box<Size>(length));
			return;
		}

		inline static auto png_write_data(
			Dependency::libpng::$png_structp png_ptr,
			Dependency::libpng::$png_bytep   data,
			Dependency::libpng::$size_t      length
		) -> Void {
			auto & stream = *make_pointer_unsafe<OutputByteStreamView>(Dependency::libpng::$png_get_io_ptr(png_ptr));
			if (stream.reserve() < make_box<Size>(length)) {
				Dependency::libpng::$png_error(png_ptr, "Write Error");
			}
			std::memcpy(unmake_pointer_unsafe<void>(stream.current_pointer()), data, length);
			stream.forward(make_box<Size>(length));
			return;
		}

		inline static auto png_output_flush(
			Dependency::libpng::$png_structp png_ptr
		) -> Void {
			return;
		}

	};

}
