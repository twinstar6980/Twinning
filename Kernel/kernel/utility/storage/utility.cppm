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
import twinning.kernel.utility.string.encoding;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.container.list.list;
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

		#pragma region utility for std::path

		#if defined M_system_windows
		inline auto make_regular_path (
			Path & original
		) -> Path & {
			for (auto & element : as_variable(original.relative())) {
				if (!element.empty()) {
					if (element.last() == ' '_c) {
						element.append_list("#"_sv);
					}
					if (element.last() == '.'_c && !((element.size() == 1_sz) || (element.size() == 2_sz && element.first() == '.'_c))) {
						element.append_list("@"_sv);
					}
				}
			}
			return original;
		}
		inline auto make_regular_path (
			Path const & original
		) -> Path {
			auto result = original;
			make_regular_path(result);
			return result;
		}
		#else
		inline auto make_regular_path (
			Path const & original
		) -> Path const& {
			return original;
		}
		#endif

		inline auto make_std_path (
			Path const & value
		) -> std::filesystem::path {
			auto string = make_regular_path(value).to_string();
			if (!string.empty()) [[likely]]
			{
				return std::filesystem::path{self_cast<std::u8string_view>(make_std_string_view(string))};
			}
			else [[unlikely]]
			{
				return std::filesystem::path{std::u8string_view{u8"."}};
			}
		}

		#pragma endregion

		#pragma region RAII wrapper for std::FILE

		class FileHandler {

		protected:

			Pointer<std::FILE> m_value;

		protected:

			#pragma region structor

			explicit FileHandler (
				Pointer<std::FILE> const & value
			) :
				m_value{value} {
			}

			#pragma endregion

		public:

			#pragma region structor

			~FileHandler (
			) {
				if (thiz.m_value != k_null_pointer) {
					auto state = std::fclose(thiz.m_value.value);
					// NOTE : SKIP
					// assert_test(state == 0);
					thiz.m_value = k_null_pointer;
				}
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
				auto file = Third::system::windows::$_wfopen(cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(make_regular_path(target).to_string()))).begin()).value, cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(make_string_view(mode)))).begin()).value);
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
				case std::filesystem::file_type::not_found : {
					result = ObjectType::Constant::none();
					break;
				}
				case std::filesystem::file_type::regular : {
					result = ObjectType::Constant::file();
					break;
				}
				case std::filesystem::file_type::directory : {
					result = ObjectType::Constant::directory();
					break;
				}
				default : {
					result = ObjectType::Constant::other();
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

		#pragma region iterate

		M_enumeration(
			M_wrap(FilterType),
			M_wrap(
				any,
				file,
				directory,
			),
		);

		// ----------------

		template <auto filter> requires
			CategoryConstraint<>
			&& (IsSameV<filter, FilterType>)
		inline auto count (
			Path const &           target,
			Optional<Size> const & depth,
			Size const &           current_depth = k_begin_index
		) -> Size {
			if (current_depth == k_begin_index) {
				assert_test(exist_directory(target));
			}
			auto result = k_none_size;
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & entry : std::filesystem::directory_iterator{make_std_path(target)}) {
					auto name = make_string(self_cast<std::string>(entry.path().filename().generic_u8string()));
					auto type = get_type(entry.status().type());
					if constexpr (filter == FilterType::Constant::any()) {
						if (type == ObjectType::Constant::file() || type == ObjectType::Constant::directory()) {
							++result;
						}
					}
					if constexpr (filter == FilterType::Constant::file()) {
						if (type == ObjectType::Constant::file()) {
							++result;
						}
					}
					if constexpr (filter == FilterType::Constant::directory()) {
						if (type == ObjectType::Constant::directory()) {
							++result;
						}
					}
					if (type == ObjectType::Constant::directory()) {
						result += count<filter>(target / name, depth, current_depth + k_next_index);
					}
				}
			}
			return result;
		}

		template <auto filter> requires
			CategoryConstraint<>
			&& (IsSameV<filter, FilterType>)
		inline auto list (
			Path const &           target,
			Optional<Size> const & depth,
			List<Path> &           result,
			Path const &           current_target = Path{},
			Size const &           current_depth = k_begin_index
		) -> Void {
			if (current_depth == k_begin_index) {
				assert_test(exist_directory(target));
				result.allocate(count<filter>(target, depth));
			}
			if (!depth.has() || current_depth < depth.get()) {
				for (auto & entry : std::filesystem::directory_iterator{make_std_path(target)}) {
					auto name = make_string(self_cast<std::string>(entry.path().filename().generic_u8string()));
					auto type = get_type(entry.status().type());
					if constexpr (filter == FilterType::Constant::any()) {
						if (type == ObjectType::Constant::file() || type == ObjectType::Constant::directory()) {
							result.append(current_target / name);
						}
					}
					if constexpr (filter == FilterType::Constant::file()) {
						if (type == ObjectType::Constant::file()) {
							result.append(current_target / name);
						}
					}
					if constexpr (filter == FilterType::Constant::directory()) {
						if (type == ObjectType::Constant::directory()) {
							result.append(current_target / name);
						}
					}
					if (type == ObjectType::Constant::directory()) {
						list<filter>(target / name, depth, result, current_target / name, current_depth + k_next_index);
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
		std::filesystem::rename(Detail::make_std_path(source), Detail::make_std_path(destination));
		return;
	}

	inline auto remove (
		Path const & source
	) -> Void {
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
		Path const &          target,
		CByteListView const & data
	) -> Void {
		auto size = data.size();
		auto handler = Detail::FileHandler::open_by_write(target);
		auto count = std::fwrite(data.begin().value, size.value, 1, handler.value());
		assert_test(count == 1 || size == k_none_size);
		return;
	}

	// ----------------

	inline auto read_stream_file (
		Path const &      target,
		OByteStreamView & data
	) -> Size {
		auto size = size_file(target);
		assert_test(data.reserve() >= size);
		auto handler = Detail::FileHandler::open_by_read(target);
		auto count = std::fread(data.current_pointer().value, size.value, 1, handler.value());
		assert_test(count == 1 || size == k_none_size);
		data.forward(size);
		return size;
	}

	inline auto write_stream_file (
		Path const &      target,
		IByteStreamView & data
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

	#pragma endregion

	#pragma region iterate

	inline auto count (
		Path const &           target,
		Optional<Size> const & depth = k_null_optional
	) -> Size {
		return Detail::count<Detail::FilterType::Constant::any()>(target, depth);
	}

	inline auto count_file (
		Path const &           target,
		Optional<Size> const & depth = k_null_optional
	) -> Size {
		return Detail::count<Detail::FilterType::Constant::file()>(target, depth);
	}

	inline auto count_directory (
		Path const &           target,
		Optional<Size> const & depth = k_null_optional
	) -> Size {
		return Detail::count<Detail::FilterType::Constant::directory()>(target, depth);
	}

	// ----------------

	inline auto list (
		Path const &           target,
		Optional<Size> const & depth = k_null_optional
	) -> List<Path> {
		auto result = List<Path>{};
		Detail::list<Detail::FilterType::Constant::any()>(target, depth, result);
		return result;
	}

	inline auto list_file (
		Path const &           target,
		Optional<Size> const & depth = k_null_optional
	) -> List<Path> {
		auto result = List<Path>{};
		Detail::list<Detail::FilterType::Constant::file()>(target, depth, result);
		return result;
	}

	inline auto list_directory (
		Path const &           target,
		Optional<Size> const & depth = k_null_optional
	) -> List<Path> {
		auto result = List<Path>{};
		Detail::list<Detail::FilterType::Constant::directory()>(target, depth, result);
		return result;
	}

	#pragma endregion

}
