module;

#include "shell/common.hpp"

export module twinning.shell.utility.system_native_string;
import twinning.shell.dependency.system.win32;
import twinning.shell.dependency.system.posix;

export namespace Twinning::Shell::SystemNativeString {

	#pragma region generic

	inline auto to_generic(
		std::string_view const & generic,
		bool const &             safe_null_terminated
	) -> std::string {
		if (safe_null_terminated) {
			assert_test(!generic.contains('\0'));
		}
		return std::string{generic};
	}

	inline auto from_generic(
		std::string_view const & generic
	) -> std::string {
		return std::string{generic};
	}

	#pragma endregion

	#pragma region native

	#if defined M_system_windows

	inline auto to_wide(
		std::string_view const & generic,
		bool const &             safe_null_terminated
	) -> std::wstring {
		if (safe_null_terminated) {
			assert_test(!generic.contains('\0'));
		}
		auto wide = std::wstring{};
		if (!generic.empty()) {
			auto size = std::size_t{};
			assert_test(generic.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
			size = static_cast<std::size_t>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					generic.data(),
					static_cast<int>(generic.size()),
					nullptr,
					0
				)
			);
			assert_test(size != 0zu);
			wide.reserve(size + 1zu);
			wide.resize(size);
			size = static_cast<std::size_t>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					generic.data(),
					static_cast<int>(generic.size()),
					wide.data(),
					static_cast<int>(wide.size())
				)
			);
			assert_test(size == wide.size());
		}
		return wide;
	}

	inline auto from_wide(
		std::wstring_view const & wide
	) -> std::string {
		auto generic = std::string{};
		if (!wide.empty()) {
			auto size = std::size_t{};
			assert_test(wide.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
			size = static_cast<std::size_t>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					wide.data(),
					static_cast<int>(wide.size()),
					nullptr,
					0,
					nullptr,
					nullptr
				)
			);
			assert_test(size != 0zu);
			generic.reserve(size + 1zu);
			generic.resize(size);
			size = static_cast<std::size_t>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_UTF8,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					wide.data(),
					static_cast<int>(wide.size()),
					generic.data(),
					static_cast<int>(generic.size()),
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
		std::string_view const & generic,
		bool const &             safe_null_terminated
	) -> std::string {
		if (safe_null_terminated) {
			assert_test(!generic.contains('\0'));
		}
		auto wide = to_wide(generic, false);
		auto narrow = std::string{};
		if (!wide.empty()) {
			auto size = std::size_t{};
			assert_test(wide.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
			size = static_cast<std::size_t>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					wide.data(),
					static_cast<int>(wide.size()),
					nullptr,
					0,
					nullptr,
					nullptr
				)
			);
			assert_test(size != 0zu);
			narrow.reserve(size + 1zu);
			narrow.resize(size);
			size = static_cast<std::size_t>(
				Dependency::system::win32::$WideCharToMultiByte(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$WC_ERR_INVALID_CHARS,
					wide.data(),
					static_cast<int>(wide.size()),
					narrow.data(),
					static_cast<int>(narrow.size()),
					nullptr,
					nullptr
				)
			);
			assert_test(size == narrow.size());
		}
		return narrow;
	}

	inline auto from_narrow(
		std::string_view const & narrow
	) -> std::string {
		auto wide = std::wstring{};
		if (!narrow.empty()) {
			auto size = std::size_t{};
			assert_test(narrow.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
			size = static_cast<std::size_t>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					narrow.data(),
					static_cast<int>(narrow.size()),
					nullptr,
					0
				)
			);
			assert_test(size != 0zu);
			wide.reserve(size + 1zu);
			wide.resize(size);
			size = static_cast<std::size_t>(
				Dependency::system::win32::$MultiByteToWideChar(
					Dependency::system::win32::$CP_ACP,
					Dependency::system::win32::$MB_ERR_INVALID_CHARS,
					narrow.data(),
					static_cast<int>(narrow.size()),
					wide.data(),
					static_cast<int>(wide.size())
				)
			);
			assert_test(size == wide.size());
		}
		return from_wide(wide);
	}

	// ----------------

	inline auto to_native(
		std::string_view const & generic,
		bool const &             safe_null_terminated
	) -> std::wstring {
		return to_wide(generic, safe_null_terminated);
	}

	inline auto from_native(
		std::wstring_view const & native
	) -> std::string {
		return from_wide(native);
	}

	#endif

	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

	inline auto to_narrow(
		std::string_view const & generic,
		bool const &             safe_null_terminated
	) -> std::string {
		if (safe_null_terminated) {
			assert_test(!generic.contains('\0'));
		}
		return std::string{generic};
	}

	inline auto from_narrow(
		std::string_view const & narrow
	) -> std::string {
		return std::string{narrow};
	}

	// ----------------

	inline auto to_native(
		std::string_view const & generic,
		bool const &             safe_null_terminated
	) -> std::string {
		return to_narrow(generic, safe_null_terminated);
	}

	inline auto from_native(
		std::string_view const & native
	) -> std::string {
		return from_narrow(native);
	}

	#endif

	#pragma endregion

}
