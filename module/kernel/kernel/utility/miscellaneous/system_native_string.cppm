module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.system_native_string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.dependency.system.win32;
import twinning.kernel.dependency.system.posix;

export namespace Twinning::Kernel::SystemNativeString {

	#pragma region generic

	inline auto to_generic(
		ConstantBasicStringView<Character> const & generic,
		bool const &                               safe_null_terminated
	) -> BasicString<Character> {
		if (safe_null_terminated) {
			assert_test(is_safe_null_terminated_string(generic));
		}
		auto result = BasicString<Character>{};
		result.allocate(generic.size() + 1_sz);
		result.resize(generic.size());
		Range::assign_from(result, generic);
		return result;
	}

	inline auto from_generic(
		ConstantBasicStringView<Character> const & generic
	) -> BasicString<Character> {
		return String{generic};
	}

	#pragma endregion

	#pragma region native

	#if defined M_system_windows

	inline auto to_wide(
		ConstantBasicStringView<Character> const & generic,
		bool const &                               safe_null_terminated
	) -> BasicString<CharacterW> {
		if (safe_null_terminated) {
			assert_test(is_safe_null_terminated_string(generic));
		}
		auto wide = BasicString<CharacterW>{};
		if (!generic.empty()) {
			auto size = Size{};
			assert_test(generic.size() <= cast_box<Size>(maximum_number_of<IntegerBox<int>>()));
			size = make_box<Size>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<char>(generic.data()),
					unmake_box<int>(generic.size()),
					nullptr,
					0
				)
			);
			assert_test(size != 0_sz);
			wide.allocate(size + 1_sz);
			wide.resize(size);
			size = make_box<Size>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<char>(generic.data()),
					unmake_box<int>(generic.size()),
					unmake_pointer_unsafe<wchar_t>(wide.data()),
					unmake_box<int>(wide.size())
				)
			);
			assert_test(size == wide.size());
		}
		return wide;
	}

	inline auto from_wide(
		ConstantBasicStringView<CharacterW> const & wide
	) -> BasicString<Character> {
		auto generic = BasicString<Character>{};
		if (!wide.empty()) {
			auto size = Size{};
			assert_test(wide.size() <= cast_box<Size>(maximum_number_of<IntegerBox<int>>()));
			size = make_box<Size>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<wchar_t>(wide.data()),
					unmake_box<int>(wide.size()),
					nullptr,
					0,
					nullptr,
					nullptr
				)
			);
			assert_test(size != 0_sz);
			generic.allocate(size);
			generic.resize(size);
			size = make_box<Size>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<wchar_t>(wide.data()),
					unmake_box<int>(wide.size()),
					unmake_pointer_unsafe<char>(generic.data()),
					unmake_box<int>(generic.size()),
					nullptr,
					nullptr
				)
			);
			assert_test(size == generic.size());
		}
		return generic;
	}

	// ----------------

	inline auto to_narrow(
		ConstantBasicStringView<Character> const & generic,
		bool const &                               safe_null_terminated
	) -> BasicString<CharacterN> {
		if (safe_null_terminated) {
			assert_test(is_safe_null_terminated_string(generic));
		}
		auto wide = to_wide(generic, false);
		auto narrow = BasicString<CharacterN>{};
		if (!wide.empty()) {
			auto size = Size{};
			assert_test(wide.size() <= cast_box<Size>(maximum_number_of<IntegerBox<int>>()));
			size = make_box<Size>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<wchar_t>(wide.data()),
					unmake_box<int>(wide.size()),
					nullptr,
					0,
					nullptr,
					nullptr
				)
			);
			assert_test(size != 0_sz);
			narrow.allocate(size + 1_sz);
			narrow.resize(size);
			size = make_box<Size>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<wchar_t>(wide.data()),
					unmake_box<int>(wide.size()),
					unmake_pointer_unsafe<char>(narrow.data()),
					unmake_box<int>(narrow.size()),
					nullptr,
					nullptr
				)
			);
			assert_test(size == narrow.size());
		}
		return narrow;
	}

	inline auto from_narrow(
		ConstantBasicStringView<CharacterN> const & narrow
	) -> BasicString<Character> {
		auto wide = BasicString<CharacterW>{};
		if (!narrow.empty()) {
			auto size = Size{};
			assert_test(narrow.size() <= cast_box<Size>(maximum_number_of<IntegerBox<int>>()));
			size = make_box<Size>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<char>(narrow.data()),
					unmake_box<int>(narrow.size()),
					nullptr,
					0
				)
			);
			assert_test(size != 0_sz);
			wide.allocate(size);
			wide.resize(size);
			size = make_box<Size>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					unmake_pointer_unsafe<char>(narrow.data()),
					unmake_box<int>(narrow.size()),
					unmake_pointer_unsafe<wchar_t>(wide.data()),
					unmake_box<int>(wide.size())
				)
			);
			assert_test(size == wide.size());
		}
		return from_wide(wide);
	}

	// ----------------

	inline auto to_native(
		ConstantBasicStringView<Character> const & generic,
		bool const &                               safe_null_terminated
	) -> BasicString<CharacterW> {
		return to_wide(generic, safe_null_terminated);
	}

	inline auto from_native(
		ConstantBasicStringView<CharacterW> const & native
	) -> BasicString<Character> {
		return from_wide(native);
	}

	#endif

	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

	inline auto to_narrow(
		ConstantBasicStringView<Character> const & generic,
		bool const &                               safe_null_terminated
	) -> BasicString<CharacterN> {
		if (safe_null_terminated) {
			assert_test(is_safe_null_terminated_string(generic));
		}
		auto narrow = BasicString<CharacterN>{};
		narrow.allocate(generic.size() + 1_sz);
		narrow.resize(generic.size());
		Range::assign_from(narrow, unsafe_cast<ConstantBasicStringView<CharacterN>>(generic));
		return narrow;
	}

	inline auto from_narrow(
		ConstantBasicStringView<CharacterN> const & narrow
	) -> BasicString<Character> {
		return BasicString<Character>{unsafe_cast<ConstantBasicStringView<Character>>(narrow)};
	}

	// ----------------

	inline auto to_native(
		ConstantBasicStringView<Character> const & generic,
		bool const &                               safe_null_terminated
	) -> BasicString<CharacterN> {
		return to_narrow(generic, safe_null_terminated);
	}

	inline auto from_native(
		ConstantBasicStringView<CharacterN> const & native
	) -> BasicString<Character> {
		return from_narrow(native);
	}

	#endif

	#pragma endregion

}
