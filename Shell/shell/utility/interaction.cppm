module;

#include "shell/common.hpp"

export module twinning.shell.utility.interaction;
import twinning.shell.utility.string;
import twinning.shell.third.system.posix;
import twinning.shell.third.system.windows;

export namespace Twinning::Shell::Interaction {

	#pragma region console

	inline auto input_text (
	) -> std::string {
		auto text = std::string{};
		#if defined M_system_windows
		auto state_b = Third::system::windows::$BOOL{};
		auto handle = Third::system::windows::$GetStdHandle(Third::system::windows::$STD_INPUT_HANDLE);
		auto handle_mode = Third::system::windows::$DWORD{};
		if (Third::system::windows::$GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = std::array<char16_t, 0x1000>{};
			auto length = Third::system::windows::$DWORD{};
			state_b = Third::system::windows::$ReadConsoleW(handle, text_16.data(), static_cast<Third::system::windows::$DWORD>(text_16.size()), &length, nullptr);
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

	inline auto output_text (
		std::string const & text
	) -> void {
		#if defined M_system_windows
		auto state_b = Third::system::windows::$BOOL{};
		auto handle = Third::system::windows::$GetStdHandle(Third::system::windows::$STD_OUTPUT_HANDLE);
		auto handle_mode = Third::system::windows::$DWORD{};
		if (Third::system::windows::$GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = utf8_to_utf16(reinterpret_cast<std::u8string const &>(text));
			state_b = Third::system::windows::$WriteConsoleW(handle, text_16.data(), static_cast<Third::system::windows::$DWORD>(text_16.size()), nullptr, nullptr);
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

	#pragma endregion

}
