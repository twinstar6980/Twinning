module;

#include "shell/common.hpp"

export module twinning.shell.utility.interaction;
import twinning.shell.utility.system_native_string;
import twinning.shell.dependency.system.win32;
import twinning.shell.dependency.system.posix;

export namespace Twinning::Shell::Interaction {

	#pragma region internal

	inline auto get_handle(
		bool const & is_input,
		bool const & is_output
	) -> std::intptr_t {
		auto handle = std::intptr_t{-1};
		assert_test(is_input != is_output);
		#if defined M_system_windows
		auto native_handle = Dependency::system::win32::$HANDLE{};
		if (is_input) {
			native_handle = Dependency::system::win32::$GetStdHandle(Dependency::system::win32::$STD_INPUT_HANDLE);
		}
		if (is_output) {
			native_handle = Dependency::system::win32::$GetStdHandle(Dependency::system::win32::$STD_OUTPUT_HANDLE);
		}
		assert_test(native_handle != Dependency::system::win32::$INVALID_HANDLE_VALUE && native_handle != nullptr);
		handle = reinterpret_cast<std::intptr_t>(native_handle);
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto native_handle = int{};
		if (is_input) {
			native_handle = Dependency::system::posix::$STDIN_FILENO;
		}
		if (is_output) {
			native_handle = Dependency::system::posix::$STDOUT_FILENO;
		}
		assert_test(native_handle != -1);
		handle = static_cast<std::intptr_t>(native_handle);
		#endif
		return handle;
	}

	inline auto check_mode(
		std::intptr_t const & handle
	) -> bool {
		auto mode = false;
		#if defined M_system_windows
		auto state_b = Dependency::system::win32::$BOOL{};
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HANDLE>(handle);
		auto native_handle_mode = Dependency::system::win32::$DWORD{};
		state_b = Dependency::system::win32::$GetConsoleMode(native_handle, &native_handle_mode);
		mode = state_b != Dependency::system::win32::$FALSE;
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto state_i = int{};
		auto native_handle = static_cast<int>(handle);
		state_i = Dependency::system::posix::$isatty(native_handle);
		mode = state_i == 1;
		#endif
		return mode;
	}

	// ----------------

	inline auto read_file(
		std::intptr_t const & handle,
		std::string &         text,
		std::size_t const &   limit
	) -> void {
		#if defined M_system_windows
		auto state_b = Dependency::system::win32::$BOOL{};
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HANDLE>(handle);
		auto data = std::vector<char>{};
		data.resize(limit);
		auto current_position = 0zu;
		while (current_position != data.size()) {
			auto current_count = std::min(limit, data.size() - current_position);
			auto current_count_actual = Dependency::system::win32::$DWORD{};
			state_b = Dependency::system::win32::$ReadFile(
				native_handle,
				data.data() + current_position,
				static_cast<Dependency::system::win32::$DWORD>(current_count),
				&current_count_actual,
				nullptr
			);
			assert_test(state_b != Dependency::system::win32::$FALSE);
			current_count = static_cast<std::size_t>(current_count_actual);
			current_position += current_count;
			data.resize(current_position);
		}
		text = std::string{data.data(), current_position};
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto native_handle = static_cast<int>(handle);
		auto data = std::vector<char>{};
		data.resize(limit);
		auto current_position = 0zu;
		while (current_position != data.size()) {
			auto current_count = std::min(limit, data.size() - current_position);
			auto current_count_actual = Dependency::system::posix::$ssize_t{};
			current_count_actual = Dependency::system::posix::$read(
				native_handle,
				data.data() + current_position,
				current_count
			);
			assert_test(current_count_actual != -1z);
			current_count = static_cast<std::size_t>(current_count_actual);
			current_position += current_count;
			data.resize(current_position);
		}
		text = std::string{data.data(), current_position};
		#endif
		return;
	}

	inline auto write_file(
		std::intptr_t const & handle,
		std::string const &   text,
		std::size_t const &   limit
	) -> void {
		#if defined M_system_windows
		auto state_b = Dependency::system::win32::$BOOL{};
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HANDLE>(handle);
		auto data = std::string_view{text.data(), text.size()};
		auto current_position = 0zu;
		while (current_position != data.size()) {
			auto current_count = std::min(limit, data.size() - current_position);
			auto current_count_actual = Dependency::system::win32::$DWORD{};
			state_b = Dependency::system::win32::$WriteFile(
				native_handle,
				data.data() + current_position,
				static_cast<Dependency::system::win32::$DWORD>(current_count),
				&current_count_actual,
				nullptr
			);
			assert_test(state_b != Dependency::system::win32::$FALSE);
			assert_test(static_cast<std::size_t>(current_count_actual) == current_count);
			current_position += current_count;
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto native_handle = static_cast<int>(handle);
		auto data = std::string_view{text.data(), text.size()};
		auto current_position = 0zu;
		while (current_position != data.size()) {
			auto current_count = std::min(limit, data.size() - current_position);
			auto current_count_actual = Dependency::system::posix::$ssize_t{};
			current_count_actual = Dependency::system::posix::$write(
				native_handle,
				text.data() + current_position,
				current_count
			);
			assert_test(current_count_actual != -1z);
			assert_test(static_cast<std::size_t>(current_count_actual) == current_count);
			current_position += current_count;
		}
		#endif
		return;
	}

	// ----------------

	inline auto read_terminal(
		std::intptr_t const & handle,
		std::string &         text,
		std::size_t const &   limit
	) -> void {
		#if defined M_system_windows
		auto state_b = Dependency::system::win32::$BOOL{};
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HANDLE>(handle);
		auto data = std::vector<wchar_t>{};
		data.resize(limit);
		auto current_position = 0zu;
		while (current_position != data.size()) {
			auto current_count = std::min(limit, data.size() - current_position);
			auto current_count_actual = Dependency::system::win32::$DWORD{};
			state_b = Dependency::system::win32::$ReadConsoleW(
				native_handle,
				data.data() + current_position,
				static_cast<Dependency::system::win32::$DWORD>(current_count),
				&current_count_actual,
				nullptr
			);
			assert_test(state_b != Dependency::system::win32::$FALSE);
			current_count = static_cast<std::size_t>(current_count_actual);
			current_position += current_count;
			data.resize(current_position);
		}
		text = SystemNativeString::from_native(std::wstring_view{data.data(), current_position});
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		read_file(handle, text, limit);
		#endif
		return;
	}

	inline auto write_terminal(
		std::intptr_t const & handle,
		std::string const &   text,
		std::size_t const &   limit
	) -> void {
		#if defined M_system_windows
		auto state_b = Dependency::system::win32::$BOOL{};
		auto native_handle = reinterpret_cast<Dependency::system::win32::$HANDLE>(handle);
		auto data = SystemNativeString::to_native(text, false);
		auto current_position = 0zu;
		while (current_position != data.size()) {
			auto current_count = std::min(limit, data.size() - current_position);
			auto current_count_actual = Dependency::system::win32::$DWORD{};
			state_b = Dependency::system::win32::$WriteConsoleW(
				native_handle,
				data.data() + current_position,
				static_cast<Dependency::system::win32::$DWORD>(current_count),
				&current_count_actual,
				nullptr
			);
			assert_test(state_b != Dependency::system::win32::$FALSE);
			assert_test(static_cast<std::size_t>(current_count_actual) == current_count);
			current_position += current_count;
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		write_file(handle, text, limit);
		#endif
		return;
	}

	#pragma endregion

	#pragma region utility

	inline auto configure_locale(
	) -> void {
		#if defined M_system_windows
		Dependency::system::win32::$SetProcessPreferredUILanguages(Dependency::system::win32::$MUI_LANGUAGE_NAME, L"en-US\0\0", nullptr);
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		#endif
		std::locale::global(std::locale::classic());
		return;
	}

	inline auto configure_terminal(
		bool const & is_input,
		bool const & is_output
	) -> void {
		auto handle = get_handle(is_input, is_output);
		if (check_mode(handle)) {
			#if defined M_system_windows
			auto state_b = Dependency::system::win32::$BOOL{};
			auto native_handle = reinterpret_cast<Dependency::system::win32::$HANDLE>(handle);
			auto native_handle_mode = Dependency::system::win32::$DWORD{};
			state_b = Dependency::system::win32::$GetConsoleMode(native_handle, &native_handle_mode);
			assert_test(state_b != Dependency::system::win32::$FALSE);
			if (is_output) {
				native_handle_mode |= Dependency::system::win32::$ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			}
			state_b = Dependency::system::win32::$SetConsoleMode(native_handle, native_handle_mode);
			assert_test(state_b != Dependency::system::win32::$FALSE);
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			#endif
		}
		return;
	}

	// ----------------

	inline auto input_text(
	) -> std::string {
		auto text = std::string{};
		auto handle = get_handle(true, false);
		if (!check_mode(handle)) {
			read_file(handle, text, 1024zu);
		}
		else {
			read_terminal(handle, text, 1024zu);
		}
		return text;
	}

	inline auto output_text(
		std::string const & text
	) -> void {
		auto handle = get_handle(false, true);
		if (!check_mode(handle)) {
			write_file(handle, text, 1024zu);
		}
		else {
			write_terminal(handle, text, 1024zu);
		}
		return;
	}

	#pragma endregion

}
