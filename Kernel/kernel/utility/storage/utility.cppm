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
import twinning.kernel.utility.string.character_type;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.math.utility;
import twinning.kernel.utility.miscellaneous.system_native_string;
import twinning.kernel.third.system.windows;
import twinning.kernel.third.system.posix;

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

		inline auto emit_path(
			Path const & value
		) -> std::filesystem::path {
			auto value_string = value.emit_native();
			M_use_nts_safe(value_string);
			return std::filesystem::path{unsafe_cast<std::u8string_view>(make_std_string_view(value_string))};
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

		inline auto resolve_type(
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

		inline auto resolve_type(
			Path const &    target,
			Boolean const & follow_link
		) -> FileType {
			auto status = !follow_link ? std::filesystem::symlink_status(emit_path(target)) : std::filesystem::status(emit_path(target));
			return resolve_type(status.type());
		}

		#pragma endregion

		#pragma region file

		inline auto open_file(
			Pointer<Void> & handle,
			Path const &    path,
			Boolean const & mode_create,
			Boolean const & mode_read,
			Boolean const & mode_write
		) -> auto {
			assert_test(mode_read || mode_write);
			#if defined M_system_windows
			auto native_handle = Third::system::windows::$CreateFileW(
				M_use_ntsp_w_safe_of(path.emit_native()),
				(!mode_read ? 0 : Third::system::windows::$GENERIC_READ) | (!mode_write ? 0 : Third::system::windows::$GENERIC_WRITE),
				Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
				nullptr,
				!mode_create ? Third::system::windows::$OPEN_EXISTING : Third::system::windows::$CREATE_NEW,
				Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
				nullptr
			);
			assert_test(native_handle != Third::system::windows::$INVALID_HANDLE_VALUE);
			handle = make_pointer_unsafe<Void>(native_handle);
			return make_finalizer(
				[native_handle] {
					auto state_b = Third::system::windows::$BOOL{};
					state_b = Third::system::windows::$CloseHandle(native_handle);
					assert_test(state_b != Third::system::windows::$FALSE);
				}
			);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto flag = int{};
			if (mode_read && !mode_write) {
				flag |= Third::system::posix::$O_RDONLY;
			}
			if (!mode_read && mode_write) {
				flag |= Third::system::posix::$O_WRONLY;
			}
			if (mode_read && mode_write) {
				flag |= Third::system::posix::$O_RDWR;
			}
			if (mode_create) {
				flag |= Third::system::posix::$O_CREAT | Third::system::posix::$O_EXCL;
			}
			auto native_handle = Third::system::posix::$open(
				M_use_ntsp_n_safe_of(path.emit_native()),
				flag,
				0644
			);
			assert_test(native_handle != -1);
			handle = make_pointer_unsafe<Void>(reinterpret_cast<void *>(native_handle));
			return make_finalizer(
				[native_handle] {
					auto state_i = int{};
					state_i = Third::system::posix::$close(native_handle);
					assert_test(state_i != -1);
				}
			);
			#endif
		}

		inline auto seek_file(
			Pointer<Void> const & handle,
			Size const &          position
		) -> Void {
			#if defined M_system_windows
			auto state_b = Third::system::windows::$BOOL{};
			state_b = Third::system::windows::$SetFilePointerEx(
				static_cast<Third::system::windows::$HANDLE>(handle.value),
				Third::system::windows::$LARGE_INTEGER{
					.QuadPart = unmake_box<Third::system::windows::$LONGLONG>(position),
				},
				nullptr,
				Third::system::windows::$FILE_BEGIN
			);
			assert_test(state_b != Third::system::windows::$FALSE);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto state_o = Third::system::posix::$off_t{};
			state_o = Third::system::posix::$lseek(
				static_cast<int>(reinterpret_cast<std::intptr_t>(handle.value)),
				unmake_box<Third::system::posix::$off_t>(position),
				Third::system::posix::$SEEK_SET
			);
			assert_test(state_o != static_cast<Third::system::posix::$off_t>(-1));
			#endif
			return;
		}

		// ----------------

		inline auto size_file(
			Pointer<Void> const & handle
		) -> Size {
			auto result = Size{};
			#if defined M_system_windows
			auto state_b = Third::system::windows::$BOOL{};
			auto native_position = Third::system::windows::$LARGE_INTEGER{};
			state_b = Third::system::windows::$GetFileSizeEx(
				static_cast<Third::system::windows::$HANDLE>(handle.value),
				&native_position
			);
			assert_test(state_b != Third::system::windows::$FALSE);
			result = make_box<Size>(native_position.QuadPart);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto state_i = int{};
			auto stat = Third::system::posix::$stat{};
			state_i = Third::system::posix::$fstat(
				static_cast<int>(reinterpret_cast<std::intptr_t>(handle.value)),
				&stat
			);
			assert_test(state_i != -1);
			result = make_box<Size>(stat.st_size);
			#endif
			return result;
		}

		inline auto resize_file(
			Pointer<Void> const & handle,
			Size const &          size
		) -> Void {
			#if defined M_system_windows
			auto state_b = Third::system::windows::$BOOL{};
			seek_file(handle, size);
			state_b = Third::system::windows::$SetEndOfFile(
				static_cast<Third::system::windows::$HANDLE>(handle.value)
			);
			assert_test(state_b != Third::system::windows::$FALSE);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto state_i = int{};
			state_i = Third::system::posix::$ftruncate(
				static_cast<int>(reinterpret_cast<std::intptr_t>(handle.value)),
				unmake_box<Third::system::posix::$off_t>(size)
			);
			assert_test(state_i != -1);
			#endif
			return;
		}

		// ----------------

		inline auto read_file(
			Pointer<Void> const &        handle,
			VariableByteListView const & data
		) -> Void {
			#if defined M_system_windows
			auto state_b = Third::system::windows::$BOOL{};
			auto current_position = 0_sz;
			while (current_position != data.size()) {
				auto current_count = Math::minimum(0x80000000_sz, data.size() - current_position);
				auto current_count_actual = Third::system::windows::$DWORD{};
				state_b = Third::system::windows::$ReadFile(
					static_cast<Third::system::windows::$HANDLE>(handle.value),
					unmake_pointer_unsafe<void>(data.begin() + current_position),
					unmake_box<Third::system::windows::$DWORD>(current_count),
					&current_count_actual,
					nullptr
				);
				assert_test(state_b != Third::system::windows::$FALSE);
				assert_test(make_box<Size>(current_count_actual) == current_count);
				current_position += current_count;
			}
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto current_position = 0_sz;
			while (current_position != data.size()) {
				auto current_count = Math::minimum(0x80000000_sz, data.size() - current_position);
				auto current_count_actual = Third::system::posix::$read(
					static_cast<int>(reinterpret_cast<std::intptr_t>(handle.value)),
					unmake_pointer_unsafe<void>(data.begin() + current_position),
					unmake_box<std::size_t>(current_count)
				);
				assert_test(current_count_actual != -1);
				assert_test(make_box<Size>(current_count_actual) == current_count);
				current_position += current_count;
			}
			#endif
			return;
		}

		inline auto write_file(
			Pointer<Void> const &        handle,
			ConstantByteListView const & data
		) -> Void {
			#if defined M_system_windows
			auto state_b = Third::system::windows::$BOOL{};
			auto current_position = 0_sz;
			while (current_position != data.size()) {
				auto current_count = Math::minimum(0x80000000_sz, data.size() - current_position);
				auto current_count_actual = Third::system::windows::$DWORD{};
				state_b = Third::system::windows::$WriteFile(
					static_cast<Third::system::windows::$HANDLE>(handle.value),
					unmake_pointer_unsafe<void>(data.begin() + current_position),
					unmake_box<Third::system::windows::$DWORD>(current_count),
					&current_count_actual,
					nullptr
				);
				assert_test(state_b != Third::system::windows::$FALSE);
				assert_test(make_box<Size>(current_count_actual) == current_count);
				current_position += current_count;
			}
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			auto current_position = 0_sz;
			while (current_position != data.size()) {
				auto current_count = Math::minimum(0x80000000_sz, data.size() - current_position);
				auto current_count_actual = Third::system::posix::$write(
					static_cast<int>(reinterpret_cast<std::intptr_t>(handle.value)),
					unmake_pointer_unsafe<void>(data.begin() + current_position),
					unmake_box<std::size_t>(current_count)
				);
				assert_test(current_count_actual != -1);
				assert_test(make_box<Size>(current_count_actual) == current_count);
				current_position += current_count;
			}
			#endif
			return;
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region basic

	inline auto exist(
		Path const & target
	) -> Boolean {
		if (target.type() != PathType::Constant::absolute()) {
			return k_false;
		}
		auto type = Detail::resolve_type(target, false);
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
		auto type = Detail::resolve_type(target, false);
		if (follow_link && type == Detail::FileType::Constant::link()) {
			auto referent_type = Detail::resolve_type(target, true);
			if (referent_type != Detail::FileType::Constant::none()) {
				type = referent_type;
			}
		}
		if (type == Detail::FileType::Constant::link()) {
			auto referent = resolve_link(target);
			auto is_directory = Boolean{};
			#if defined M_system_windows
			auto attribute = Third::system::windows::$GetFileAttributesW(M_use_ntsp_w_safe_of(target.emit_native()));
			is_directory = attribute != Third::system::windows::$INVALID_FILE_ATTRIBUTES && (attribute & Third::system::windows::$FILE_ATTRIBUTE_DIRECTORY) != 0;
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			is_directory = k_false;
			#endif
			create_link(placement, referent, is_directory);
		}
		else if (type == Detail::FileType::Constant::file()) {
			std::filesystem::copy_file(Detail::emit_path(target), Detail::emit_path(placement));
		}
		else if (type == Detail::FileType::Constant::directory()) {
			create_directory(placement);
			for (auto & item : std::filesystem::directory_iterator{Detail::emit_path(target), std::filesystem::directory_options::none}) {
				auto item_name = make_string(unsafe_cast<std::string>(item.path().filename().generic_u8string()));
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
		auto type = Detail::resolve_type(target, false);
		if (type == Detail::FileType::Constant::link()) {
			std::filesystem::rename(Detail::emit_path(target), Detail::emit_path(placement));
		}
		else if (type == Detail::FileType::Constant::file()) {
			std::filesystem::rename(Detail::emit_path(target), Detail::emit_path(placement));
		}
		else if (type == Detail::FileType::Constant::directory()) {
			std::filesystem::rename(Detail::emit_path(target), Detail::emit_path(placement));
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
		auto type = Detail::resolve_type(target, false);
		if (type == Detail::FileType::Constant::link()) {
			std::filesystem::remove_all(Detail::emit_path(target));
		}
		else if (type == Detail::FileType::Constant::file()) {
			std::filesystem::remove_all(Detail::emit_path(target));
		}
		else if (type == Detail::FileType::Constant::directory()) {
			std::filesystem::remove_all(Detail::emit_path(target));
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
		if (target.type() != PathType::Constant::absolute()) {
			return k_false;
		}
		auto type = Detail::resolve_type(target, false);
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
			std::filesystem::create_symlink(Detail::emit_path(referent), Detail::emit_path(target));
		}
		else {
			std::filesystem::create_directory_symlink(Detail::emit_path(referent), Detail::emit_path(target));
		}
		return;
	}

	inline auto resolve_link(
		Path const & target
	) -> Path {
		assert_test(exist_link(target));
		auto referent = std::filesystem::read_symlink(Detail::emit_path(target));
		return Path{make_string(unsafe_cast<std::string>(referent.generic_u8string()))};
	}

	#pragma endregion

	#pragma region file

	inline auto exist_file(
		Path const & target
	) -> Boolean {
		if (target.type() != PathType::Constant::absolute()) {
			return k_false;
		}
		auto type = Detail::resolve_type(target, true);
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
		auto handle = Pointer<Void>{};
		auto handle_finalizer = Detail::open_file(handle, target, k_true, k_true, k_true);
		return;
	}

	// ----------------

	inline auto size_file(
		Path const & target
	) -> Size {
		assert_test(exist_file(target));
		auto handle = Pointer<Void>{};
		auto handle_finalizer = Detail::open_file(handle, target, k_false, k_true, k_false);
		return Detail::size_file(handle);
	}

	inline auto resize_file(
		Path const & target,
		Size const & size
	) -> Void {
		assert_test(exist_file(target));
		auto handle = Pointer<Void>{};
		auto handle_finalizer = Detail::open_file(handle, target, k_false, k_false, k_true);
		Detail::resize_file(handle, size);
		return;
	}

	// ----------------

	inline auto read_file(
		Path const &                 target,
		Size const &                 offset,
		VariableByteListView const & data
	) -> Void {
		assert_test(exist_file(target));
		auto handle = Pointer<Void>{};
		auto handle_finalizer = Detail::open_file(handle, target, k_false, k_true, k_false);
		auto capacity = Detail::size_file(handle);
		assert_test(capacity >= offset + data.size());
		Detail::seek_file(handle, offset);
		Detail::read_file(handle, data);
		return;
	}

	inline auto write_file(
		Path const &                 target,
		Size const &                 offset,
		ConstantByteListView const & data
	) -> Void {
		assert_test(exist_file(target));
		auto handle = Pointer<Void>{};
		auto handle_finalizer = Detail::open_file(handle, target, k_false, k_false, k_true);
		auto capacity = Detail::size_file(handle);
		assert_test(capacity >= offset + data.size());
		Detail::seek_file(handle, offset);
		Detail::write_file(handle, data);
		return;
	}

	#pragma endregion

	#pragma region directory

	inline auto exist_directory(
		Path const & target
	) -> Boolean {
		if (target.type() != PathType::Constant::absolute()) {
			return k_false;
		}
		auto type = Detail::resolve_type(target, true);
		return type == Detail::FileType::Constant::directory();
	}

	inline auto create_directory(
		Path const & target
	) -> Void {
		assert_test(!exist(target));
		std::filesystem::create_directories(Detail::emit_path(target));
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
		auto result_count = 0_sz;
		auto result = List<Path>{};
		auto iterate_count = [&]<typename TSelf>(
			TSelf const & self,
			Path const &  current_target,
			Size const &  current_depth
		) -> Void {
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & item : std::filesystem::directory_iterator{Detail::emit_path(current_target), std::filesystem::directory_options::none}) {
					auto item_type = Detail::resolve_type(item.symlink_status().type());
					auto item_name = make_string(unsafe_cast<std::string>(item.path().filename().generic_u8string()));
					if (follow_link && item_type == Detail::FileType::Constant::link()) {
						auto referent_type = Detail::resolve_type(item.status().type());
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
						self(self, current_target.join(item_name), current_depth + 1_sz);
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
				for (auto & item : std::filesystem::directory_iterator{Detail::emit_path(current_target), std::filesystem::directory_options::none}) {
					auto item_type = Detail::resolve_type(item.symlink_status().type());
					auto item_name = make_string(unsafe_cast<std::string>(item.path().filename().generic_u8string()));
					auto item_path = current_item.join(item_name);
					if (follow_link && item_type == Detail::FileType::Constant::link()) {
						auto referent_type = Detail::resolve_type(item.status().type());
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
						self(self, current_target.join(item_name), item_path, current_depth + 1_sz);
					}
				}
			}
			return;
		};
		iterate_count(iterate_count, target, 0_sz);
		result.allocate(result_count);
		iterate(iterate, target, Path{PathType::Constant::detached()}, 0_sz);
		return result;
	}

	#pragma endregion

}
