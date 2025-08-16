module;

#include "shell/common.hpp"

export module twinning.shell.utility.interaction;
import twinning.shell.utility.system_native_string;
import twinning.shell.third.system.windows;
import twinning.shell.third.system.posix;

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
			auto text_w = std::array<wchar_t, 0x1000>{};
			auto length = Third::system::windows::$DWORD{};
			state_b = Third::system::windows::$ReadConsoleW(handle, text_w.data(), static_cast<Third::system::windows::$DWORD>(text_w.size()), &length, nullptr);
			assert_test(state_b != Twinning::Shell::Third::system::windows::$FALSE);
			text = SystemNativeString::wide_to_utf8(std::wstring_view{text_w.data(), length - 2});
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
			auto text_w = SystemNativeString::wide_from_utf8(text);
			state_b = Third::system::windows::$WriteConsoleW(handle, text_w.data(), static_cast<Third::system::windows::$DWORD>(text_w.size()), nullptr, nullptr);
			assert_test(state_b != Twinning::Shell::Third::system::windows::$FALSE);
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
