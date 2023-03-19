#pragma once

#include "core/utility/string/string.hpp"
#include "core/utility/string/encoding.hpp"
#include "core/utility/file_system/path.hpp"
#include "core/third/system/windows.hpp"
#include "core/third/system/posix.hpp"

namespace TwinStar::Core::Process {

	#pragma region detail

	namespace Detail {

		#pragma region windows command line

		// NOTE
		// see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
		// the first string of command line is program path, should not encode as argument string, and must use '\' be path separator

		inline auto encode_windows_command_line_program_string (
			Path const & source,
			String &     destination
		) -> Void {
			destination.append('"'_c);
			destination.append_list(source.to_string(CharacterType::PathSeparator::windows));
			destination.append('"'_c);
			return;
		}

		inline auto encode_windows_command_line_argument_string (
			String const & source,
			String &       destination
		) -> Void {
			auto current_backslash_count = k_none_size;
			destination.append('"'_c);
			for (auto & element : source) {
				if (element == '"'_c) {
					for (auto & index : SizeRange{current_backslash_count}) {
						destination.append('\\'_c);
					}
					destination.append('\\'_c);
				}
				destination.append(element);
				if (element == '\\'_c) {
					++current_backslash_count;
				} else {
					current_backslash_count = k_none_size;
				}
			}
			for (auto & index : SizeRange{current_backslash_count}) {
				destination.append('\\'_c);
			}
			destination.append('"'_c);
			return;
		}

		// ----------------

		inline auto encode_windows_command_line_string (
			Path const &         program,
			List<String> const & argument
		) -> String {
			auto destination_size = k_none_size;
			destination_size += 256_sz + 2_sz;
			for (auto & element : argument) {
				destination_size += element.size() * 2_sz + 2_sz;
			}
			auto destination = String{destination_size};
			encode_windows_command_line_program_string(program, destination);
			for (auto & element : argument) {
				destination.append(' '_c);
				encode_windows_command_line_argument_string(element, destination);
			}
			return destination;
		}

		#pragma endregion

		#pragma region windows environment variable

		// NOTE
		// see https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw

		inline auto encode_windows_environment_variable_string (
			List<String> const & environment
		) -> String {
			auto destination_size = k_none_size;
			for (auto & element : environment) {
				destination_size += element.size() + 1_sz;
			}
			destination_size += 1_sz;
			auto destination = String{destination_size};
			for (auto & element : environment) {
				destination.append_list(element);
				destination.append('\0'_c);
			}
			destination.append('\0'_c);
			return destination;
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region process

	inline auto environment (
	) -> List<String> {
		auto result = List<String>{};
		#if defined M_system_windows
		if (_wenviron == nullptr) {
			_wgetenv(L"");
		}
		for (auto element_pointer_raw = _wenviron; *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = cast_pointer<Character16>(make_pointer(*element_pointer_raw));
			auto element = self_cast<String>(StringEncoding::utf16_to_utf8(CBasicStringView<Character16>{element_pointer, null_terminated_string_size_of(element_pointer)}));
			result.append(as_moveable(element));
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		for (auto element_pointer_raw = environ; *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = cast_pointer<Character>(make_pointer(*element_pointer_raw));
			auto element = String{element_pointer, null_terminated_string_size_of(element_pointer)};
			result.append(as_moveable(element));
		}
		#endif
		return result;
	}

	inline auto exit (
		IntegerU32 const & code
	) -> Void {
		std::exit(static_cast<std::int32_t>(code.value));
		return;
	}

	// NOTE
	// the return value is program's exit code, see the following webpage to understand
	// Windows - https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodeprocess
	// POSIX   - https://pubs.opengroup.org/onlinepubs/9699919799/functions/waitid.html
	// in Windows, the complete 32-bit exit code can always be obtained
	// for POSIX, although the standard states that the waitid function should obtain the full exit code, this is not actually the case in Linux and Macintosh
	// the following are the number of return bits that can be obtained in each system
	// Windows   : all 32 bit
	// Linux     : low 08 bit
	// Macintosh : low 24 bit
	inline auto execute (
		Path const &           program,
		List<String> const &   argument,
		List<String> const &   environment,
		Optional<Path> const & input,
		Optional<Path> const & output,
		Optional<Path> const & error
	) -> IntegerU32 {
		auto result = IntegerU32{};
		#if defined M_system_windows
		auto null_device = Path{"/NUL"_s};
		auto state_b = BOOL{};
		auto state_d = DWORD{};
		auto program_string = BasicString<Character16>{};
		auto argument_string = BasicString<Character16>{};
		auto environment_string = BasicString<Character16>{};
		auto input_string = BasicString<Character16>{};
		auto output_string = BasicString<Character16>{};
		auto error_string = BasicString<Character16>{};
		auto security_attribute = SECURITY_ATTRIBUTES{};
		auto startup_information = STARTUPINFOW{};
		auto process_information = PROCESS_INFORMATION{};
		auto exit_code_d = DWORD{};
		program_string = make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<CBasicStringView<Character8>>(program.to_string(CharacterType::PathSeparator::windows))));
		argument_string = make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<CBasicStringView<Character8>>(Detail::encode_windows_command_line_string(program, argument))));
		environment_string = make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<CBasicStringView<Character8>>(Detail::encode_windows_environment_variable_string(environment))));
		input_string = make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>((!input.has() ? (null_device) : (input.get())).to_string())));
		output_string = make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>((!output.has() ? (null_device) : (output.get())).to_string())));
		error_string = make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>((!error.has() ? (null_device) : (error.get())).to_string())));
		security_attribute.nLength = sizeof(SECURITY_ATTRIBUTES);
		security_attribute.lpSecurityDescriptor = nullptr;
		security_attribute.bInheritHandle = TRUE;
		startup_information.hStdInput = CreateFileW(
			cast_pointer<WCHAR>(input_string.begin()).value,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			&security_attribute,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdInput != INVALID_HANDLE_VALUE);
		startup_information.cb = sizeof(STARTUPINFO);
		startup_information.dwFlags = STARTF_USESTDHANDLES;
		startup_information.hStdOutput = CreateFileW(
			cast_pointer<WCHAR>(output_string.begin()).value,
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			&security_attribute,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdOutput != INVALID_HANDLE_VALUE);
		startup_information.hStdError = CreateFileW(
			cast_pointer<WCHAR>(error_string.begin()).value,
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			&security_attribute,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdError != INVALID_HANDLE_VALUE);
		state_b = CreateProcessW(
			cast_pointer<WCHAR>(program_string.begin()).value,
			cast_pointer<WCHAR>(argument_string.begin()).value,
			nullptr,
			nullptr,
			TRUE,
			CREATE_UNICODE_ENVIRONMENT | DETACHED_PROCESS,
			cast_pointer<WCHAR>(environment_string.begin()).value,
			nullptr,
			&startup_information,
			&process_information
		);
		assert_test(state_b);
		state_d = WaitForSingleObject(process_information.hProcess, INFINITE);
		assert_test(state_d == WAIT_OBJECT_0);
		state_b = CloseHandle(startup_information.hStdInput);
		assert_test(state_b);
		state_b = CloseHandle(startup_information.hStdOutput);
		assert_test(state_b);
		state_b = CloseHandle(startup_information.hStdError);
		assert_test(state_b);
		state_b = GetExitCodeProcess(process_information.hProcess, &exit_code_d);
		assert_test(state_b);
		result = mbw<IntegerU32>(exit_code_d);
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto null_device = Path{"/dev/null"_s};
		auto state_i = int{};
		auto program_string = String{};
		auto argument_string = List<String>{};
		auto argument_string_list = List<Character *>{};
		auto environment_string = List<String>{};
		auto environment_string_list = List<Character *>{};
		auto input_string = String{};
		auto output_string = String{};
		auto error_string = String{};
		auto process_id = pid_t{};
		auto spawn_file_action = posix_spawn_file_actions_t{};
		auto wait_info = siginfo_t{};
		program_string = make_null_terminated_string(program.to_string());
		argument_string.allocate(1_sz + argument.size());
		argument_string.append(program_string);
		for (auto & element : argument) {
			argument_string.append(make_null_terminated_string(element));
		}
		argument_string_list.allocate(argument_string.size() + 1_sz);
		for (auto & element : argument_string) {
			argument_string_list.append(element.begin().value);
		}
		argument_string_list.append(nullptr);
		environment_string.allocate(environment.size());
		for (auto & element : environment) {
			environment_string.append(make_null_terminated_string(element));
		}
		environment_string_list.allocate(environment_string.size() + 1_sz);
		for (auto & element : environment_string) {
			environment_string_list.append(element.begin().value);
		}
		environment_string_list.append(nullptr);
		input_string = make_null_terminated_string((!input.has() ? (null_device) : (input.get())).to_string());
		output_string = make_null_terminated_string((!output.has() ? (null_device) : (output.get())).to_string());
		error_string = make_null_terminated_string((!error.has() ? (null_device) : (error.get())).to_string());
		state_i = posix_spawn_file_actions_init(&spawn_file_action);
		assert_test(state_i == 0);
		state_i = posix_spawn_file_actions_addopen(&spawn_file_action, STDIN_FILENO, cast_pointer<char>(input_string.begin()).value, O_RDONLY, 0);
		assert_test(state_i == 0);
		state_i = posix_spawn_file_actions_addopen(&spawn_file_action, STDOUT_FILENO, cast_pointer<char>(output_string.begin()).value, O_WRONLY, 0);
		assert_test(state_i == 0);
		state_i = posix_spawn_file_actions_addopen(&spawn_file_action, STDERR_FILENO, cast_pointer<char>(error_string.begin()).value, O_WRONLY, 0);
		assert_test(state_i == 0);
		state_i = posix_spawn(&process_id, cast_pointer<char>(program_string.begin()).value, &spawn_file_action, nullptr, cast_pointer<char *>(argument_string_list.begin()).value, cast_pointer<char *>(environment_string_list.begin()).value);
		assert_test(state_i == 0);
		state_i = posix_spawn_file_actions_destroy(&spawn_file_action);
		assert_test(state_i == 0);
		state_i = waitid(P_PID, static_cast<id_t>(process_id), &wait_info, WEXITED | WSTOPPED);
		assert_test(state_i == 0);
		result = mbw<IntegerU32>(wait_info.si_status);
		#endif
		return result;
	}

	// NOTE
	// implement defined
	// on iphone, std::system is not available, this function always return 0x00000000
	inline auto system (
		String const & command
	) -> IntegerU32 {
		auto result = IntegerU32{};
		#if defined M_system_windows
		result = mbw<IntegerU32>(_wsystem(cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(command))).begin()).value));
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android
		result = mbw<IntegerU32>(std::system(cast_pointer<char>(make_null_terminated_string(command).begin()).value));
		#endif
		#if defined M_system_iphone
		result = mbw<IntegerU32>(0x00000000);
		#endif
		return result;
	}

	#pragma endregion

}
