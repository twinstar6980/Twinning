#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/third/libpng.hpp"

namespace TwinStar::Kernel::Tool::Texture::File::PNG {

	struct Common {

		static auto png_error (
			Third::libpng::png_structp     png_ptr,
			Third::libpng::png_const_charp error_message
		) -> Void {
			throw UnnamedException{mss("libpng error : {}"_sf(error_message))};
			return;
		}

		static auto png_warning (
			Third::libpng::png_structp     png_ptr,
			Third::libpng::png_const_charp error_message
		) -> Void {
			M_log(mss("libpng warning : {}"_sf(error_message)));
			return;
		}

		// ----------------

		static auto png_read_data (
			Third::libpng::png_structp png_ptr,
			Third::libpng::png_bytep   data,
			Third::libpng::size_t      length
		) -> Void {
			auto & stream = *static_cast<IByteStreamView *>(Third::libpng::png_get_io_ptr(png_ptr));
			if (stream.reserve() < mbw<Size>(length)) {
				Third::libpng::png_error(png_ptr, "Read Error");
			}
			std::memcpy(data, stream.current_pointer().value, length);
			stream.forward(mbw<Size>(length));
			return;
		}

		static auto png_write_data (
			Third::libpng::png_structp png_ptr,
			Third::libpng::png_bytep   data,
			Third::libpng::size_t      length
		) -> Void {
			auto & stream = *static_cast<OByteStreamView *>(Third::libpng::png_get_io_ptr(png_ptr));
			if (stream.reserve() < mbw<Size>(length)) {
				Third::libpng::png_error(png_ptr, "Write Error");
			}
			std::memcpy(stream.current_pointer().value, data, length);
			stream.forward(mbw<Size>(length));
			return;
		}

		static auto png_output_flush (
			Third::libpng::png_structp png_ptr
		) -> Void {
			return;
		}

	};

}
