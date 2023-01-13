#pragma once

#if defined M_system_windows

#include "core/utility/string/string.hpp"
#include "core/utility/string/encoding.hpp"
#include "core/utility/file_system/path.hpp"
#include "core/third_party/system/windows.hpp"

namespace TwinStar::Core::System::Windows {

	#pragma region detail

	namespace Detail {

		#pragma region command line

		// NOTE
		// see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
		// the first string of command line is program path, should not encode as argument string, and must use '\' be path separator

		inline auto encode_command_line_program_string (
			Path const & source,
			String &     destination
		) -> Void {
			destination.append('"'_c);
			destination.append_list(source.to_string(CharacterType::PathSeparator::windows));
			destination.append('"'_c);
			return;
		}

		inline auto encode_command_line_argument_string (
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

		inline auto encode_command_line_string (
			Path const &         program,
			List<String> const & argument
		) -> String {
			auto destination_size = k_none_size;
			destination_size += 256_sz + 2_sz;
			for (auto & element : argument) {
				destination_size += element.size() * 2_sz + 2_sz;
			}
			auto destination = String{destination_size};
			encode_command_line_program_string(program, destination);
			for (auto & element : argument) {
				destination.append(' '_c);
				encode_command_line_argument_string(element, destination);
			}
			return destination;
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region process

	inline auto exit (
		IntegerU32 const & code
	) -> Void {
		std::exit(static_cast<std::int32_t>(code.value));
		return;
	}

	inline auto execute (
		Path const &           program,
		List<String> const &   argument,
		Optional<Path> const & redirect_input,
		Optional<Path> const & redirect_output,
		Optional<Path> const & redirect_error
	) -> IntegerU32 {
		auto state_b = BOOL{};
		auto state_d = DWORD{};
		auto program_string = program.to_string(CharacterType::PathSeparator::windows);
		auto argument_string = Detail::encode_command_line_string(program, argument);
		auto startup_information = STARTUPINFOW{
			.cb = sizeof(STARTUPINFO),
			.dwFlags = STARTF_USESTDHANDLES,
			.hStdInput = GetStdHandle(STD_INPUT_HANDLE),
			.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE),
			.hStdError = GetStdHandle(STD_ERROR_HANDLE),
		};
		auto process_information = PROCESS_INFORMATION{
		};
		auto security_attribute = SECURITY_ATTRIBUTES{
			.nLength = sizeof(SECURITY_ATTRIBUTES),
			.lpSecurityDescriptor = nullptr,
			.bInheritHandle = TRUE,
		};
		if (redirect_input.has()) {
			startup_information.hStdInput = CreateFileW(
				cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(redirect_input.get().to_string()))).begin()).value,
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				&security_attribute,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr
			);
			assert_condition(startup_information.hStdInput != INVALID_HANDLE_VALUE);
		}
		if (redirect_output.has()) {
			startup_information.hStdOutput = CreateFileW(
				cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(redirect_output.get().to_string()))).begin()).value,
				GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				&security_attribute,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr
			);
			assert_condition(startup_information.hStdOutput != INVALID_HANDLE_VALUE);
		}
		if (redirect_error.has()) {
			startup_information.hStdError = CreateFileW(
				cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(redirect_error.get().to_string()))).begin()).value,
				GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				&security_attribute,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr
			);
			assert_condition(startup_information.hStdError != INVALID_HANDLE_VALUE);
		}
		state_b = CreateProcessW(
			cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(program_string))).begin()).value,
			cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<CBasicStringView<Character8>>(argument_string))).begin()).value,
			nullptr,
			nullptr,
			TRUE,
			CREATE_UNICODE_ENVIRONMENT,
			nullptr,
			nullptr,
			&startup_information,
			&process_information
		);
		assert_condition(state_b);
		state_d = WaitForSingleObject(process_information.hProcess, INFINITE);
		assert_condition(state_d == WAIT_OBJECT_0);
		if (redirect_input.has()) {
			state_b = CloseHandle(startup_information.hStdInput);
			assert_condition(state_b);
		}
		if (redirect_output.has()) {
			state_b = CloseHandle(startup_information.hStdOutput);
			assert_condition(state_b);
		}
		if (redirect_error.has()) {
			state_b = CloseHandle(startup_information.hStdError);
			assert_condition(state_b);
		}
		auto result = DWORD{};
		state_b = GetExitCodeProcess(process_information.hProcess, &result);
		assert_condition(state_b);
		return mbw<IntegerU32>(result);
	}

	inline auto system (
		String const & command
	) -> IntegerU32 {
		return mbw<IntegerU32>(_wsystem(cast_pointer<WCHAR>(make_null_terminated_string(StringEncoding::utf8_to_utf16(self_cast<BasicString<Character8>>(command))).begin()).value));
	}

	#pragma endregion

}

#endif
