module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.storage.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.storage.path;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.byte_series.stream;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.miscellaneous.system_native_string_extended;
import twinning.kernel.third.system.windows;

export namespace Twinning::Kernel::Storage {

	#pragma region declaration

	inline auto exist_directory (
		Path const & target
	) -> Boolean;

	inline auto create_directory (
		Path const & target
	) -> Void;

	#pragma endregion

	#pragma region detail

	namespace Detail {

		#pragma region path

		#if defined M_system_windows

		inline auto make_regular_path (
			Path const & original
		) -> Path {
			auto result = original;
			for (auto & element : result.relative()) {
				while (!element.empty()) {
					if (element.last() == ' '_c) {
						element.remove_tail();
						continue;
					}
					if (element.last() == '.'_c && !((element.size() == 1_sz) || (element.size() == 2_sz && element.first() == '.'_c))) {
						element.remove_tail();
						continue;
					}
					break;
				}
			}
			return result;
		}

		#endif

		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

		inline auto make_regular_path (
			Path const & original
		) -> Path const & {
			return original;
		}

		#endif

		// ----------------

		inline auto make_std_path (
			Path const & value
		) -> std::filesystem::path {
			auto string = make_regular_path(value).to_string();
			if (!string.empty()) [[likely]] {
				return std::filesystem::path{self_cast<std::u8string_view>(make_std_string_view(string))};
			}
			else [[unlikely]] {
				return std::filesystem::path{std::u8string_view{u8"."}};
			}
		}

		#pragma endregion

		#pragma region type

		M_enumeration(
			M_wrap(ObjectType),
			M_wrap(
				none,
				file,
				directory,
				other,
			),
		);

		// ----------------

		inline auto get_type (
			std::filesystem::file_type const & type
		) -> ObjectType {
			auto result = ObjectType{};
			switch (type) {
				case std::filesystem::file_type::not_found: {
					result = ObjectType::Constant::none();
					break;
				}
				case std::filesystem::file_type::regular: {
					result = ObjectType::Constant::file();
					break;
				}
				case std::filesystem::file_type::directory: {
					result = ObjectType::Constant::directory();
					break;
				}
				default: {
					result = ObjectType::Constant::other();
					break;
				}
			}
			return result;
		}

		inline auto get_type (
			Path const & target
		) -> ObjectType {
			auto status = std::filesystem::status(make_std_path(target));
			return get_type(status.type());
		}

		#pragma endregion

		#pragma region file

		class FileHandler {

		protected:

			Pointer<std::FILE> m_value;

		protected:

			#pragma region structor

			explicit FileHandler (
				Pointer<std::FILE> const & value
			) :
				m_value{value} {
				return;
			}

			#pragma endregion

		public:

			#pragma region structor

			~FileHandler (
			) {
				if (thiz.m_value != k_null_pointer) {
					std::fclose(thiz.m_value.value); // NOTE: EXPLAIN: skip result check
					thiz.m_value = k_null_pointer;
				}
				return;
			}

			// ----------------

			FileHandler (
			) = delete;

			FileHandler (
				FileHandler const & that
			) = delete;

			FileHandler (
				FileHandler && that
			) = delete;

			#pragma endregion

			#pragma region operator

			auto operator = (
				FileHandler const & that
			) -> FileHandler & = delete;

			auto operator = (
				FileHandler && that
			) -> FileHandler & = delete;

			#pragma endregion

			#pragma region value

			auto value (
			) -> ZPointer<std::FILE> {
				return thiz.m_value.value;
			}

			#pragma endregion

		public:

			#pragma region open

			inline static auto open (
				Path const &            target,
				ZConstantString const & mode
			) -> FileHandler {
				#if defined M_system_windows
				auto file = Third::system::windows::$_wfopen(cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>(make_regular_path(target).to_string()))).begin()).value, cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>(make_string_view(mode)))).begin()).value);
				#endif
				#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
				auto file = std::fopen(cast_pointer<char>(make_null_terminated_string(make_regular_path(target).to_string()).begin()).value, mode);
				#endif
				assert_test(file != nullptr);
				return FileHandler{make_pointer(file)};
			}

			// ----------------

			inline static auto open_by_read (
				Path const & target
			) -> FileHandler {
				if (!exist_directory(target.parent())) {
					assert_fail(R"(/* parent directory not exist */)");
				}
				return open(target, "rb");
			}

			inline static auto open_by_write (
				Path const & target
			) -> FileHandler {
				if (!exist_directory(target.parent())) {
					create_directory(target.parent());
				}
				return open(target, "wb");
			}

			inline static auto open_by_append (
				Path const & target
			) -> FileHandler {
				if (!exist_directory(target.parent())) {
					create_directory(target.parent());
				}
				return open(target, "ab");
			}

			#pragma endregion

		};

		#pragma endregion

		#pragma region directory

		template <auto allow_file, auto allow_directory> requires
			CategoryConstraint<>
			&& (IsSameOf<allow_file, Boolean>)
			&& (IsSameOf<allow_directory, Boolean>)
		inline auto count_directory (
			Path const &           target,
			Optional<Size> const & depth,
			Size const &           current_depth
		) -> Size {
			if (current_depth == k_begin_index) {
				assert_test(exist_directory(target));
			}
			auto result = k_none_size;
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & item : std::filesystem::directory_iterator{make_std_path(target)}) {
					auto item_type = get_type(item.status().type());
					auto item_name = make_string(self_cast<std::string>(item.path().filename().generic_u8string()));
					if constexpr (allow_file) {
						if (item_type == ObjectType::Constant::file()) {
							++result;
						}
					}
					if constexpr (allow_directory) {
						if (item_type == ObjectType::Constant::directory()) {
							++result;
						}
					}
					if (item_type == ObjectType::Constant::directory()) {
						result += count_directory<allow_file, allow_directory>(target / item_name, depth, current_depth + k_next_index);
					}
				}
			}
			return result;
		}

		template <auto allow_file, auto allow_directory> requires
			CategoryConstraint<>
			&& (IsSameOf<allow_file, Boolean>)
			&& (IsSameOf<allow_directory, Boolean>)
		inline auto list_directory (
			Path const &           target,
			Optional<Size> const & depth,
			List<Path> &           result,
			Path const &           current_target,
			Size const &           current_depth
		) -> Void {
			if (current_depth == k_begin_index) {
				assert_test(exist_directory(target));
				result.allocate(count_directory<allow_file, allow_directory>(target, depth, k_begin_index));
			}
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & item : std::filesystem::directory_iterator{make_std_path(target)}) {
					auto item_type = get_type(item.status().type());
					auto item_name = make_string(self_cast<std::string>(item.path().filename().generic_u8string()));
					auto item_path = current_target / item_name;
					if constexpr (allow_file) {
						if (item_type == ObjectType::Constant::file()) {
							result.append(item_path);
						}
					}
					if constexpr (allow_directory) {
						if (item_type == ObjectType::Constant::directory()) {
							result.append(item_path);
						}
					}
					if (item_type == ObjectType::Constant::directory()) {
						list_directory<allow_file, allow_directory>(target / item_name, depth, result, item_path, current_depth + k_next_index);
					}
				}
			}
			return;
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region exist

	inline auto exist (
		Path const & target
	) -> Boolean {
		auto type = Detail::get_type(target);
		return type == Detail::ObjectType::Constant::file() || type == Detail::ObjectType::Constant::directory();
	}

	inline auto exist_file (
		Path const & target
	) -> Boolean {
		auto type = Detail::get_type(target);
		return type == Detail::ObjectType::Constant::file();
	}

	inline auto exist_directory (
		Path const & target
	) -> Boolean {
		auto type = Detail::get_type(target);
		return type == Detail::ObjectType::Constant::directory();
	}

	#pragma endregion

	#pragma region basic

	inline auto copy (
		Path const & source,
		Path const & destination
	) -> Void {
		assert_test(exist(source));
		if (!destination.relative().empty() && !exist_directory(destination.parent())) {
			create_directory(destination.parent());
		}
		std::filesystem::copy(Detail::make_std_path(source), Detail::make_std_path(destination), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
		return;
	}

	inline auto rename (
		Path const & source,
		Path const & destination
	) -> Void {
		assert_test(exist(source));
		std::filesystem::rename(Detail::make_std_path(source), Detail::make_std_path(destination));
		return;
	}

	inline auto remove (
		Path const & source
	) -> Void {
		assert_test(exist(source));
		std::filesystem::remove_all(Detail::make_std_path(source));
		return;
	}

	#pragma endregion

	#pragma region link

	inline auto create_link (
		Path const &    target,
		Path const &    object,
		Boolean const & is_directory
	) -> Void {
		if (!target.relative().empty() && !exist_directory(target.parent())) {
			create_directory(target.parent());
		}
		if (!is_directory) {
			std::filesystem::create_symlink(Detail::make_std_path(object), Detail::make_std_path(target));
		}
		else {
			std::filesystem::create_directory_symlink(Detail::make_std_path(object), Detail::make_std_path(target));
		}
		return;
	}

	inline auto parse_link (
		Path const & target
	) -> Path {
		auto object = std::filesystem::read_symlink(Detail::make_std_path(target));
		return Path{make_string(self_cast<std::string>(object.generic_u8string()))};
	}

	#pragma endregion

	#pragma region hard link

	inline auto create_hard_link (
		Path const & target,
		Path const & object
	) -> Void {
		if (!target.relative().empty() && !exist_directory(target.parent())) {
			create_directory(target.parent());
		}
		std::filesystem::create_hard_link(Detail::make_std_path(object), Detail::make_std_path(target));
		return;
	}

	#pragma endregion

	#pragma region file

	inline auto create_file (
		Path const & target
	) -> Void {
		auto handler = Detail::FileHandler::open_by_append(target);
		return;
	}

	// ----------------

	inline auto size_file (
		Path const & target
	) -> Size {
		auto size = std::filesystem::file_size(Detail::make_std_path(target));
		return mbox<Size>(size);
	}

	inline auto resize_file (
		Path const & target,
		Size const & size
	) -> Void {
		std::filesystem::resize_file(Detail::make_std_path(target), size.value);
		return;
	}

	// ----------------

	inline auto read_file (
		Path const & target
	) -> ByteArray {
		auto size = size_file(target);
		auto data = ByteArray{size};
		auto handler = Detail::FileHandler::open_by_read(target);
		auto count = std::fread(data.begin().value, size.value, 1, handler.value());
		assert_test(count == 1 || size == k_none_size);
		return data;
	}

	inline auto write_file (
		Path const &                 target,
		ConstantByteListView const & data
	) -> Void {
		auto size = data.size();
		auto handler = Detail::FileHandler::open_by_write(target);
		auto count = std::fwrite(data.begin().value, size.value, 1, handler.value());
		assert_test(count == 1 || size == k_none_size);
		return;
	}

	// ----------------

	inline auto read_file_stream (
		Path const &           target,
		OutputByteStreamView & data
	) -> Size {
		auto size = size_file(target);
		assert_test(data.reserve() >= size);
		auto handler = Detail::FileHandler::open_by_read(target);
		auto count = std::fread(data.current_pointer().value, size.value, 1, handler.value());
		assert_test(count == 1 || size == k_none_size);
		data.forward(size);
		return size;
	}

	inline auto write_file_stream (
		Path const &          target,
		InputByteStreamView & data
	) -> Size {
		auto size = data.reserve();
		auto handler = Detail::FileHandler::open_by_write(target);
		auto count = std::fwrite(data.current_pointer().value, size.value, 1, handler.value());
		assert_test(count == 1 || size == k_none_size);
		data.forward(size);
		return size;
	}

	#pragma endregion

	#pragma region directory

	inline auto create_directory (
		Path const & target
	) -> Void {
		std::filesystem::create_directories(Detail::make_std_path(target));
		return;
	}

	// ----------------

	inline auto list_directory (
		Path const &           target,
		Optional<Size> const & depth,
		Boolean const &        allow_file,
		Boolean const &        allow_directory
	) -> List<Path> {
		auto result = List<Path>{};
		if (allow_file == k_false && allow_directory == k_false) {
			Detail::list_directory<k_false, k_false>(target, depth, result, Path{}, k_begin_index);
		}
		if (allow_file == k_false && allow_directory == k_true) {
			Detail::list_directory<k_false, k_true>(target, depth, result, Path{}, k_begin_index);
		}
		if (allow_file == k_true && allow_directory == k_false) {
			Detail::list_directory<k_true, k_false>(target, depth, result, Path{}, k_begin_index);
		}
		if (allow_file == k_true && allow_directory == k_true) {
			Detail::list_directory<k_true, k_true>(target, depth, result, Path{}, k_begin_index);
		}
		return result;
	}

	#pragma endregion

}
