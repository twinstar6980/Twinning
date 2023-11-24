#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/utility/exception.hpp"
#include "shell_cli/utility/string.hpp"
#include "shell_cli/third/system/windows.hpp"
#include "shell_cli/third/system/posix.hpp"

namespace TwinStar::ShellCLI::Interaction {

	#pragma region standard

	inline auto input (
	) -> std::string {
		auto text = std::string{};
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto handle = GetStdHandle(STD_INPUT_HANDLE);
		auto handle_mode = DWORD{};
		if (GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = std::array<char16_t, 0x1000>{};
			auto length = DWORD{};
			state_b = ReadConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			assert_test(state_b);
			auto text_8 = utf16_to_utf8(std::u16string_view{text_16.data(), length - 2});
			text = std::move(reinterpret_cast<std::string &>(text_8));
		}
		else {
			std::getline(std::cin, text);
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		std::getline(std::cin, text);
		#endif
		return text;
	}

	inline auto output (
		std::string const & text
	) -> void {
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
		auto handle_mode = DWORD{};
		if (GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
			state_b = WriteConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), nullptr, nullptr);
			assert_test(state_b);
		}
		else {
			std::cout << text << std::flush;
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		std::cout << text << std::flush;
		#endif
		return;
	}

	inline auto error (
		std::string const & text
	) -> void {
		#if defined M_system_windows
		auto state_b = BOOL{};
		auto handle = GetStdHandle(STD_ERROR_HANDLE);
		auto handle_mode = DWORD{};
		if (GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
			state_b = WriteConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), nullptr, nullptr);
			assert_test(state_b);
		}
		else {
			std::cerr << text << std::flush;
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		std::cerr << text << std::flush;
		#endif
		return;
	}

	#pragma endregion

}
