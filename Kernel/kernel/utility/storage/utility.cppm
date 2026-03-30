module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.storage.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.storage.path;
import twinning.kernel.utility.miscellaneous.finalizer;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.byte_series.stream;
import twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.miscellaneous.system_native_string_extended;
import twinning.kernel.third.system.windows;

export namespace Twinning::Kernel::Storage {

	#pragma region declaration

	inline auto create_link(
		Path const &    target,
		Path const &    referent,
		Boolean const & is_directory
	) -> Void;

	inline auto resolve_link(
		Path const & target
	) -> Path;

	inline auto exist_directory(
		Path const & target
	) -> Boolean;

	inline auto create_directory(
		Path const & target
	) -> Void;

	#pragma endregion

	#pragma region detail

	namespace Detail {

		#pragma region path

		inline auto make_std_path(
			Path const & value
		) -> std::filesystem::path {
			auto string = value.emit_native();
			return std::filesystem::path{self_cast<std::u8string_view>(make_std_string_view(string))};
		}

		#pragma endregion

		#pragma region type

		M_enumeration(
			M_wrap(FileType),
			M_wrap(
				none,
				link,
				file,
				directory,
				other,
			),
		);

		// ----------------

		inline auto get_type(
			std::filesystem::file_type const & type
		) -> FileType {
			auto result = FileType{};
			switch (type) {
				case std::filesystem::file_type::not_found: {
					result = FileType::Constant::none();
					break;
				}
				case std::filesystem::file_type::symlink: {
					result = FileType::Constant::link();
					break;
				}
				case std::filesystem::file_type::regular: {
					result = FileType::Constant::file();
					break;
				}
				case std::filesystem::file_type::directory: {
					result = FileType::Constant::directory();
					break;
				}
				default: {
					result = FileType::Constant::other();
					break;
				}
			}
			return result;
		}

		inline auto get_type(
			Path const &    target,
			Boolean const & follow_link
		) -> FileType {
			auto status = !follow_link ? std::filesystem::symlink_status(make_std_path(target)) : std::filesystem::status(make_std_path(target));
			return get_type(status.type());
		}

		#pragma endregion

		#pragma region file

		inline auto open_file(
			Path const &            target,
			ZConstantString const & mode,
			Pointer<std::FILE> &    result
		) -> auto {
			#if defined M_system_windows
			auto file = Third::system::windows::$_wfopen(cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>(target.emit_native()))).begin()).value, cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>(make_string_view(mode)))).begin()).value);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto file = std::fopen(cast_pointer<char>(make_null_terminated_string(target.emit_native()).begin()).value, mode);
			#endif
			assert_test(file != nullptr);
			result = make_pointer(file);
			return make_finalizer(
				[&result]() {
					std::fclose(result.value); // NOTE: EXPLAIN: skip result check
					return;
				}
			);
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region basic

	inline auto exist(
		Path const & target
	) -> Boolean {
		if (target.type() == PathType::Constant::nothing()) {
			return k_false;
		}
		auto type = Detail::get_type(target, false);
		return type != Detail::FileType::Constant::none();
	}

	inline auto copy(
		Path const &    target,
		Path const &    placement,
		Boolean const & follow_link
	) -> Void {
		assert_test(exist(target));
		assert_test(!exist(placement));
		auto placement_parent = placement.parent();
		if (placement_parent.has() && !exist_directory(placement_parent.get())) {
			create_directory(placement_parent.get());
		}
		auto type = Detail::get_type(target, false);
		if (follow_link && type == Detail::FileType::Constant::link()) {
			auto referent_type = Detail::get_type(target, true);
			if (referent_type != Detail::FileType::Constant::none()) {
				type = referent_type;
			}
		}
		if (type == Detail::FileType::Constant::link()) {
			auto referent = resolve_link(target);
			auto is_directory = Boolean{};
			#if defined M_system_windows
			auto attribute = Third::system::windows::$GetFileAttributesW(cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>(target.emit_native()))).begin()).value);
			is_directory = attribute != Third::system::windows::$INVALID_FILE_ATTRIBUTES && (attribute & Third::system::windows::$FILE_ATTRIBUTE_DIRECTORY) != 0;
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			is_directory = k_false;
			#endif
			create_link(placement, referent, is_directory);
		}
		else if (type == Detail::FileType::Constant::file()) {
			std::filesystem::copy_file(Detail::make_std_path(target), Detail::make_std_path(placement));
		}
		else if (type == Detail::FileType::Constant::directory()) {
			create_directory(placement);
			for (auto & item : std::filesystem::directory_iterator{Detail::make_std_path(target), std::filesystem::directory_options::none}) {
				auto item_name = make_string(self_cast<std::string>(item.path().filename().generic_u8string()));
				copy(target.join(item_name), placement.join(item_name), follow_link);
			}
		}
		else {
			throw UnsupportedException();
		}
		return;
	}

	inline auto rename(
		Path const & target,
		Path const & placement
	) -> Void {
		assert_test(exist(target));
		assert_test(!exist(placement));
		auto placement_parent = placement.parent();
		if (placement_parent.has() && !exist_directory(placement_parent.get())) {
			create_directory(placement_parent.get());
		}
		auto type = Detail::get_type(target, false);
		if (type == Detail::FileType::Constant::link()) {
			std::filesystem::rename(Detail::make_std_path(target), Detail::make_std_path(placement));
		}
		else if (type == Detail::FileType::Constant::file()) {
			std::filesystem::rename(Detail::make_std_path(target), Detail::make_std_path(placement));
		}
		else if (type == Detail::FileType::Constant::directory()) {
			std::filesystem::rename(Detail::make_std_path(target), Detail::make_std_path(placement));
		}
		else {
			throw UnsupportedException();
		}
		return;
	}

	inline auto remove(
		Path const & target
	) -> Void {
		assert_test(exist(target));
		auto type = Detail::get_type(target, false);
		if (type == Detail::FileType::Constant::link()) {
			std::filesystem::remove_all(Detail::make_std_path(target));
		}
		else if (type == Detail::FileType::Constant::file()) {
			std::filesystem::remove_all(Detail::make_std_path(target));
		}
		else if (type == Detail::FileType::Constant::directory()) {
			std::filesystem::remove_all(Detail::make_std_path(target));
		}
		else {
			throw UnsupportedException();
		}
		return;
	}

	#pragma endregion

	#pragma region link

	inline auto exist_link(
		Path const & target
	) -> Boolean {
		if (target.type() == PathType::Constant::nothing()) {
			return k_false;
		}
		auto type = Detail::get_type(target, false);
		return type == Detail::FileType::Constant::link();
	}

	inline auto create_link(
		Path const &    target,
		Path const &    referent,
		Boolean const & is_directory
	) -> Void {
		assert_test(!exist(target));
		auto target_parent = target.parent();
		if (target_parent.has() && !exist_directory(target_parent.get())) {
			create_directory(target_parent.get());
		}
		if (!is_directory) {
			std::filesystem::create_symlink(Detail::make_std_path(referent), Detail::make_std_path(target));
		}
		else {
			std::filesystem::create_directory_symlink(Detail::make_std_path(referent), Detail::make_std_path(target));
		}
		return;
	}

	inline auto resolve_link(
		Path const & target
	) -> Path {
		assert_test(exist_link(target));
		auto referent = std::filesystem::read_symlink(Detail::make_std_path(target));
		return Path{make_string(self_cast<std::string>(referent.generic_u8string()))};
	}

	#pragma endregion

	#pragma region file

	inline auto exist_file(
		Path const & target
	) -> Boolean {
		if (target.type() == PathType::Constant::nothing()) {
			return k_false;
		}
		auto type = Detail::get_type(target, true);
		return type == Detail::FileType::Constant::file();
	}

	inline auto create_file(
		Path const & target
	) -> Void {
		assert_test(!exist(target));
		auto target_parent = target.parent();
		if (target_parent.has() && !exist_directory(target_parent.get())) {
			create_directory(target_parent.get());
		}
		auto handler = Pointer<std::FILE>{};
		auto finalizer = Detail::open_file(target, "wb", handler);
		return;
	}

	// ----------------

	inline auto size_file(
		Path const & target
	) -> Size {
		assert_test(exist_file(target));
		auto size = std::filesystem::file_size(Detail::make_std_path(target));
		return mbox<Size>(size);
	}

	// ----------------

	inline auto read_file(
		Path const & target
	) -> ByteArray {
		assert_test(exist_file(target));
		auto size = size_file(target);
		auto data = ByteArray{size};
		auto handler = Pointer<std::FILE>{};
		auto finalizer = Detail::open_file(target, "rb", handler);
		auto count = std::fread(data.begin().value, size.value, 1, handler.value);
		assert_test(count == 1 || size == k_none_size);
		return data;
	}

	inline auto write_file(
		Path const &                 target,
		ConstantByteListView const & data
	) -> Void {
		assert_test(exist_file(target));
		auto size = data.size();
		auto handler = Pointer<std::FILE>{};
		auto finalizer = Detail::open_file(target, "wb", handler);
		auto count = std::fwrite(data.begin().value, size.value, 1, handler.value);
		assert_test(count == 1 || size == k_none_size);
		return;
	}

	// ----------------

	inline auto read_file_stream(
		Path const &           target,
		OutputByteStreamView & data
	) -> Size {
		assert_test(exist_file(target));
		auto size = size_file(target);
		assert_test(data.reserve() >= size);
		auto handler = Pointer<std::FILE>{};
		auto finalizer = Detail::open_file(target, "rb", handler);
		auto count = std::fread(data.current_pointer().value, size.value, 1, handler.value);
		assert_test(count == 1 || size == k_none_size);
		data.forward(size);
		return size;
	}

	inline auto write_file_stream(
		Path const &          target,
		InputByteStreamView & data
	) -> Size {
		assert_test(exist_file(target));
		auto size = data.reserve();
		auto handler = Pointer<std::FILE>{};
		auto finalizer = Detail::open_file(target, "wb", handler);
		auto count = std::fwrite(data.current_pointer().value, size.value, 1, handler.value);
		assert_test(count == 1 || size == k_none_size);
		data.forward(size);
		return size;
	}

	#pragma endregion

	#pragma region directory

	inline auto exist_directory(
		Path const & target
	) -> Boolean {
		if (target.type() == PathType::Constant::nothing()) {
			return k_false;
		}
		auto type = Detail::get_type(target, true);
		return type == Detail::FileType::Constant::directory();
	}

	inline auto create_directory(
		Path const & target
	) -> Void {
		assert_test(!exist(target));
		std::filesystem::create_directories(Detail::make_std_path(target));
		return;
	}

	// ----------------

	inline auto list_directory(
		Path const &           target,
		Optional<Size> const & depth,
		Boolean const &        follow_link,
		Boolean const &        allow_link,
		Boolean const &        allow_file,
		Boolean const &        allow_directory
	) -> List<Path> {
		assert_test(exist_directory(target));
		auto result_count = k_none_size;
		auto result = List<Path>{};
		auto iterate_count = [&]<typename TSelf>(
			TSelf const & self,
			Path const &  current_target,
			Size const &  current_depth
		) -> Void {
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & item : std::filesystem::directory_iterator{Detail::make_std_path(current_target), std::filesystem::directory_options::none}) {
					auto item_type = Detail::get_type(item.symlink_status().type());
					auto item_name = make_string(self_cast<std::string>(item.path().filename().generic_u8string()));
					if (follow_link && item_type == Detail::FileType::Constant::link()) {
						auto referent_type = Detail::get_type(item.status().type());
						if (referent_type != Detail::FileType::Constant::none()) {
							item_type = referent_type;
						}
					}
					if (allow_link && item_type == Detail::FileType::Constant::link()) {
						++result_count;
					}
					if (allow_file && item_type == Detail::FileType::Constant::file()) {
						++result_count;
					}
					if (allow_directory && item_type == Detail::FileType::Constant::directory()) {
						++result_count;
					}
					if (item_type == Detail::FileType::Constant::directory()) {
						self(self, current_target.join(item_name), current_depth + k_next_index);
					}
				}
			}
			return;
		};
		auto iterate = [&]<typename TSelf>(
			TSelf const & self,
			Path const &  current_target,
			Path const &  current_item,
			Size const &  current_depth
		) -> Void {
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & item : std::filesystem::directory_iterator{Detail::make_std_path(current_target), std::filesystem::directory_options::none}) {
					auto item_type = Detail::get_type(item.symlink_status().type());
					auto item_name = make_string(self_cast<std::string>(item.path().filename().generic_u8string()));
					auto item_path = current_item.join(item_name);
					if (follow_link && item_type == Detail::FileType::Constant::link()) {
						auto referent_type = Detail::get_type(item.status().type());
						if (referent_type != Detail::FileType::Constant::none()) {
							item_type = referent_type;
						}
					}
					if (allow_link && item_type == Detail::FileType::Constant::link()) {
						result.append(item_path);
					}
					if (allow_file && item_type == Detail::FileType::Constant::file()) {
						result.append(item_path);
					}
					if (allow_directory && item_type == Detail::FileType::Constant::directory()) {
						result.append(item_path);
					}
					if (item_type == Detail::FileType::Constant::directory()) {
						self(self, current_target.join(item_name), item_path, current_depth + k_next_index);
					}
				}
			}
			return;
		};
		iterate_count(iterate_count, target, k_begin_index);
		result.allocate(result_count);
		iterate(iterate, target, Path{PathType::Constant::relative()}, k_begin_index);
		return result;
	}

	#pragma endregion

}
