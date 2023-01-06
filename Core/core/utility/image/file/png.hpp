#pragma once

// TODO : remove file api, RAII wrapper ?

#include "core/utility/image/bitmap.hpp"
#include "core/utility/file_system/file_system.hpp"
#include "core/third_party/libpng.hpp"

namespace TwinStar::Core::Image::File::PNG {

	#pragma region detail

	namespace Detail {

		inline auto png_error (
			ThirdParty::libpng::png_structp     png_ptr,
			ThirdParty::libpng::png_const_charp error_message
		) -> Void {
			throw SimpleException{make_std_string_view("libpng error : {}"_sf(error_message))};
			return;
		}

		inline auto png_read_data_by_memory (
			ThirdParty::libpng::png_structp png_ptr,
			ThirdParty::libpng::png_bytep   data,
			ThirdParty::libpng::size_t      length
		) -> Void {
			auto & stream = *static_cast<IByteStreamView *>(ThirdParty::libpng::png_get_io_ptr(png_ptr));
			if (stream.reserve() < mbw<Size>(length)) {
				ThirdParty::libpng::png_error(png_ptr, "Read Error");
			}
			std::memcpy(data, stream.current_pointer().value, length);
			stream.forward(mbw<Size>(length));
			return;
		}

		inline auto png_write_data_by_memory (
			ThirdParty::libpng::png_structp png_ptr,
			ThirdParty::libpng::png_bytep   data,
			ThirdParty::libpng::size_t      length
		) -> Void {
			auto & stream = *static_cast<OByteStreamView *>(ThirdParty::libpng::png_get_io_ptr(png_ptr));
			if (stream.reserve() < mbw<Size>(length)) {
				ThirdParty::libpng::png_error(png_ptr, "Write Error");
			}
			std::memcpy(stream.current_pointer().value, data, length);
			stream.forward(mbw<Size>(length));
			return;
		}

		inline auto png_flush_data_by_memory (
			ThirdParty::libpng::png_structp png_ptr
		) -> Void {
			return;
		}

	}

	#pragma endregion

	#pragma region basic

	inline auto size (
		CByteListView const & data
	) -> ImageSize {
		auto stream = IByteStreamView{data};
		auto png_struct = ThirdParty::libpng::png_create_read_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		ThirdParty::libpng::png_set_read_fn(png_struct, &stream, &Detail::png_read_data_by_memory);
		ThirdParty::libpng::png_read_info(png_struct, png_info);
		auto size = ImageSize{mbw<Size>((*png_struct).width), mbw<Size>((*png_struct).height)};
		ThirdParty::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
		return size;
	}

	// ----------------

	inline auto read (
		IByteStreamView &   data,
		VBitmapView const & image
	) -> Void {
		auto png_struct = ThirdParty::libpng::png_create_read_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		ThirdParty::libpng::png_set_read_fn(png_struct, &data, &Detail::png_read_data_by_memory);
		ThirdParty::libpng::png_read_info(png_struct, png_info);
		assert_condition((*png_info).color_type == ThirdParty::libpng::PNG_COLOR_TYPE_RGBA_);
		assert_condition(image.size() == ImageSize{mbw<Size>((*png_struct).width), mbw<Size>((*png_struct).height)});
		for (auto & row : image.data()) {
			ThirdParty::libpng::png_read_row(png_struct, reinterpret_cast<ThirdParty::libpng::png_bytep>(row.begin().value), nullptr);
		}
		ThirdParty::libpng::png_read_end(png_struct, png_info);
		ThirdParty::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
		return;
	}

	inline auto write (
		OByteStreamView &   data,
		CBitmapView const & image
	) -> Void {
		auto image_size = image.size();
		auto png_struct = ThirdParty::libpng::png_create_write_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		ThirdParty::libpng::png_set_write_fn(png_struct, &data, &Detail::png_write_data_by_memory, &Detail::png_flush_data_by_memory);
		ThirdParty::libpng::png_set_IHDR(png_struct, png_info, static_cast<ThirdParty::libpng::png_uint_32>(image_size.width.value), static_cast<ThirdParty::libpng::png_uint_32>(image_size.height.value), 8, ThirdParty::libpng::PNG_COLOR_TYPE_RGBA_, ThirdParty::libpng::PNG_INTERLACE_NONE_, ThirdParty::libpng::PNG_COMPRESSION_TYPE_DEFAULT_, ThirdParty::libpng::PNG_FILTER_TYPE_DEFAULT_);
		ThirdParty::libpng::png_write_info(png_struct, png_info);
		for (auto & row : image.data()) {
			ThirdParty::libpng::png_write_row(png_struct, reinterpret_cast<ThirdParty::libpng::png_const_bytep>(row.begin().value));
		}
		ThirdParty::libpng::png_write_end(png_struct, png_info);
		ThirdParty::libpng::png_destroy_write_struct(&png_struct, &png_info);
		return;
	}

	// ----------------

	inline auto read_of (
		IByteStreamView & data
	) -> Bitmap {
		auto image = Bitmap{};
		auto png_struct = ThirdParty::libpng::png_create_read_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		ThirdParty::libpng::png_set_read_fn(png_struct, &data, &Detail::png_read_data_by_memory);
		ThirdParty::libpng::png_read_info(png_struct, png_info);
		assert_condition((*png_info).color_type == ThirdParty::libpng::PNG_COLOR_TYPE_RGBA_);
		image.allocate(ImageSize{mbw<Size>((*png_struct).width), mbw<Size>((*png_struct).height)});
		for (auto & row : image.data()) {
			ThirdParty::libpng::png_read_row(png_struct, reinterpret_cast<ThirdParty::libpng::png_bytep>(row.begin().value), nullptr);
		}
		ThirdParty::libpng::png_read_end(png_struct, png_info);
		ThirdParty::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
		return image;
	}

	#pragma endregion

	#pragma region basic - file style

	inline auto size_file (
		Path const & path
	) -> ImageSize {
		auto png_struct = ThirdParty::libpng::png_create_read_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		auto file = FileSystem::Detail::FileHandler::open_by_read(path);
		ThirdParty::libpng::png_init_io(png_struct, file.value());
		ThirdParty::libpng::png_read_info(png_struct, png_info);
		auto size = ImageSize{mbw<Size>((*png_struct).width), mbw<Size>((*png_struct).height)};
		ThirdParty::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
		return size;
	}

	// ----------------

	inline auto read_file (
		Path const &        path,
		VBitmapView const & image
	) -> Void {
		auto png_struct = ThirdParty::libpng::png_create_read_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		auto file = FileSystem::Detail::FileHandler::open_by_read(path);
		ThirdParty::libpng::png_init_io(png_struct, file.value());
		ThirdParty::libpng::png_read_info(png_struct, png_info);
		assert_condition((*png_info).color_type == ThirdParty::libpng::PNG_COLOR_TYPE_RGBA_);
		assert_condition(image.size() == ImageSize{mbw<Size>((*png_struct).width), mbw<Size>((*png_struct).height)});
		for (auto & row : image.data()) {
			ThirdParty::libpng::png_read_row(png_struct, reinterpret_cast<ThirdParty::libpng::png_bytep>(row.begin().value), nullptr);
		}
		ThirdParty::libpng::png_read_end(png_struct, png_info);
		ThirdParty::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
		return;
	}

	inline auto write_file (
		Path const &        path,
		CBitmapView const & image
	) -> Void {
		auto image_size = image.size();
		auto png_struct = ThirdParty::libpng::png_create_write_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		auto file = FileSystem::Detail::FileHandler::open_by_write(path);
		ThirdParty::libpng::png_init_io(png_struct, file.value());
		ThirdParty::libpng::png_set_IHDR(png_struct, png_info, static_cast<ThirdParty::libpng::png_uint_32>(image_size.width.value), static_cast<ThirdParty::libpng::png_uint_32>(image_size.height.value), 8, ThirdParty::libpng::PNG_COLOR_TYPE_RGBA_, ThirdParty::libpng::PNG_INTERLACE_NONE_, ThirdParty::libpng::PNG_COMPRESSION_TYPE_DEFAULT_, ThirdParty::libpng::PNG_FILTER_TYPE_DEFAULT_);
		ThirdParty::libpng::png_write_info(png_struct, png_info);
		for (auto & row : image.data()) {
			ThirdParty::libpng::png_write_row(png_struct, reinterpret_cast<ThirdParty::libpng::png_const_bytep>(row.begin().value));
		}
		ThirdParty::libpng::png_write_end(png_struct, png_info);
		ThirdParty::libpng::png_destroy_write_struct(&png_struct, &png_info);
		return;
	}

	// ----------------

	inline auto read_file_of (
		Path const & path
	) -> Bitmap {
		auto image = Bitmap{};
		auto png_struct = ThirdParty::libpng::png_create_read_struct(ThirdParty::libpng::PNG_LIBPNG_VER_STRING_, nullptr, nullptr, nullptr);
		ThirdParty::libpng::png_set_error_fn(png_struct, nullptr, Detail::png_error, Detail::png_error);
		auto png_info = ThirdParty::libpng::png_create_info_struct(png_struct);
		auto file = FileSystem::Detail::FileHandler::open_by_read(path);
		ThirdParty::libpng::png_init_io(png_struct, file.value());
		ThirdParty::libpng::png_read_info(png_struct, png_info);
		assert_condition((*png_info).color_type == ThirdParty::libpng::PNG_COLOR_TYPE_RGBA_);
		image.allocate(ImageSize{mbw<Size>((*png_struct).width), mbw<Size>((*png_struct).height)});
		for (auto & row : image.data()) {
			ThirdParty::libpng::png_read_row(png_struct, reinterpret_cast<ThirdParty::libpng::png_bytep>(row.begin().value), nullptr);
		}
		ThirdParty::libpng::png_read_end(png_struct, png_info);
		ThirdParty::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
		return image;
	}

	#pragma endregion

}
