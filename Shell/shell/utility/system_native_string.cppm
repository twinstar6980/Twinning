module;

#include "shell/common.hpp"

export module twinning.shell.utility.system_native_string;
import twinning.shell.third.system.windows;
import twinning.shell.third.system.posix;

export namespace Twinning::Shell::SystemNativeString {

	#pragma region windows

	#if defined M_system_windows

	inline auto wide_to_utf8 (
		std::wstring_view const & source
	) -> std::string {
		auto destination = std::string{};
		if (!source.empty()) {
			auto destination_size = std::size_t{};
			destination_size = static_cast<std::size_t>(
				Third::system::windows::$WideCharToMultiByte(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$WC_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					nullptr,
					0,
					nullptr,
					nullptr
				)
			);
			assert_test(destination_size != 0);
			destination.reserve(destination_size + 1);
			destination.resize(destination_size);
			destination_size = static_cast<std::size_t>(
				Third::system::windows::$WideCharToMultiByte(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$WC_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					destination.data(),
					static_cast<int>(destination.size()),
					nullptr,
					nullptr
				)
			);
			assert_test(destination_size == destination.size());
		}
		return destination;
	}

	// ----------------

	inline auto wide_from_utf8 (
		std::string_view const & source
	) -> std::wstring {
		auto destination = std::wstring{};
		if (!source.empty()) {
			auto destination_size = std::size_t{};
			destination_size = static_cast<std::size_t>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					nullptr,
					0
				)
			);
			assert_test(destination_size != 0);
			destination.reserve(destination_size + 1);
			destination.resize(destination_size);
			destination_size = static_cast<std::size_t>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_UTF8,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					destination.data(),
					static_cast<int>(destination.size())
				)
			);
			assert_test(destination_size == destination.size());
		}
		return destination;
	}

	inline auto wide_from_ansi (
		std::string_view const & source
	) -> std::wstring {
		auto destination = std::wstring{};
		if (!source.empty()) {
			auto destination_size = std::size_t{};
			destination_size = static_cast<std::size_t>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_ACP,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					nullptr,
					0
				)
			);
			assert_test(destination_size != 0);
			destination.reserve(destination_size + 1);
			destination.resize(destination_size);
			destination_size = static_cast<std::size_t>(
				Third::system::windows::$MultiByteToWideChar(
					Third::system::windows::$CP_ACP,
					Third::system::windows::$MB_ERR_INVALID_CHARS,
					source.data(),
					static_cast<int>(source.size()),
					destination.data(),
					static_cast<int>(destination.size())
				)
			);
			assert_test(destination_size == destination.size());
		}
		return destination;
	}

	#endif

	#pragma endregion

	#pragma region posix

	#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

	#endif

	#pragma endregion

	#pragma region universal

	inline auto utf8_from_native (
		std::string_view const & source
	) -> std::string {
		#if defined M_system_windows
		return wide_to_utf8(wide_from_ansi(source));
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		return std::string{source};
		#endif
	}

	#pragma endregion

}
