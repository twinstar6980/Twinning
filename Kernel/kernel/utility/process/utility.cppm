module;

#include "kernel/common.hpp"

// NOTE: DEFINE
#if defined M_system_linux
#define si_status _sifields._sigchld.si_status
#endif
#if defined M_system_android
#define si_status _sifields._sigchld._status
#endif

export module twinning.kernel.utility.process.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.storage.path;
import twinning.kernel.utility.storage.utility;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.miscellaneous.system_native_string_extended;
import twinning.kernel.third.system.windows;
import twinning.kernel.third.system.posix;

export namespace Twinning::Kernel::Process {

	#pragma region detail

	namespace Detail {

		#pragma region windows command

		// NOTE: EXPLAIN: see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170

		inline auto encode_windows_command_program_string (
			Path const & source,
			String &     destination
		) -> Void {
			destination.append('"'_c);
			destination.append_list(source.to_string(CharacterType::k_path_separator_windows));
			destination.append('"'_c);
			return;
		}

		inline auto encode_windows_command_argument_string (
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
					current_backslash_count += 1_sz;
				}
				else {
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

		inline auto encode_windows_command_string (
			Path const &         program,
			List<String> const & argument
		) -> String {
			auto destination_size = k_none_size;
			destination_size += 256_sz + 2_sz;
			for (auto & element : argument) {
				destination_size += element.size() * 2_sz + 2_sz;
			}
			auto destination = String{destination_size};
			encode_windows_command_program_string(program, destination);
			for (auto & element : argument) {
				destination.append(' '_c);
				encode_windows_command_argument_string(element, destination);
			}
			return destination;
		}

		#pragma endregion

		#pragma region windows environment

		// NOTE: EXPLAIN: see https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw

		inline auto encode_windows_environment_string (
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

	#pragma region workspace

	inline auto get_workspace (
	) -> Path {
		auto target = std::filesystem::current_path();
		return Path{make_string(self_cast<std::string>(target.generic_u8string()))};
	}

	inline auto set_workspace (
		Path const & target
	) -> Void {
		std::filesystem::current_path(Storage::Detail::make_std_path(target));
		return;
	}

	#pragma endregion

	#pragma region environment

	inline auto get_environment (
		String const & name
	) -> Optional<String> {
		auto value = Optional<String>{};
		#if defined M_system_windows
		auto state_d = Third::system::windows::$DWORD{};
		auto name_w = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<ConstantBasicStringView<CharacterN>>(name)));
		state_d = Third::system::windows::$GetEnvironmentVariableW(cast_pointer<Third::system::windows::$WCHAR>(name_w.begin()).value, nullptr, 0);
		if (state_d == 0) {
			assert_test(Third::system::windows::$GetLastError() == Third::system::windows::$ERROR_ENVVAR_NOT_FOUND);
			value.reset();
		}
		else {
			auto buffer = Array<CharacterW>{mbox<Size>(state_d)};
			state_d = Third::system::windows::$GetEnvironmentVariableW(cast_pointer<Third::system::windows::$WCHAR>(name_w.begin()).value, cast_pointer<Third::system::windows::$WCHAR>(buffer.begin()).value, static_cast<Third::system::windows::$DWORD>(buffer.size().value));
			assert_test(state_d == static_cast<Third::system::windows::$DWORD>((buffer.size() - 1_sz).value));
			value.set(self_cast<String>(SystemNativeString::wide_to_utf8(ConstantBasicStringView<CharacterW>{buffer.begin(), buffer.size() - 1_sz})));
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto value_if = Third::system::posix::$getenv(cast_pointer<char>(make_null_terminated_string(name).begin()).value);
		if (value_if == nullptr) {
			value.reset();
		}
		else {
			value.set(make_string_view(value_if));
		}
		#endif
		return value;
	}

	inline auto set_environment (
		String const &           name,
		Optional<String> const & value
	) -> Void {
		#if defined M_system_windows
		auto state_b = Third::system::windows::$BOOL{};
		auto name_w = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<ConstantBasicStringView<CharacterN>>(name)));
		if (!value.has()) {
			state_b = Third::system::windows::$SetEnvironmentVariableW(cast_pointer<Third::system::windows::$WCHAR>(name_w.begin()).value, nullptr);
		}
		else {
			auto value_w = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<ConstantBasicStringView<CharacterN>>(value.get())));
			state_b = Third::system::windows::$SetEnvironmentVariableW(cast_pointer<Third::system::windows::$WCHAR>(name_w.begin()).value, cast_pointer<Third::system::windows::$WCHAR>(value_w.begin()).value);
		}
		assert_test(state_b != Third::system::windows::$FALSE);
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto state_i = int{};
		if (!value.has()) {
			state_i = Third::system::posix::$unsetenv(cast_pointer<char>(make_null_terminated_string(name).begin()).value);
		}
		else {
			state_i = Third::system::posix::$setenv(cast_pointer<char>(make_null_terminated_string(name).begin()).value, cast_pointer<char>(make_null_terminated_string(value.get()).begin()).value, 1);
		}
		assert_test(state_i == 0);
		#endif
		return;
	}

	// ----------------

	inline auto list_environment (
	) -> List<String> {
		auto result = List<String>{};
		#if defined M_system_windows
		if (Third::system::windows::$_wenviron() == nullptr) {
			Third::system::windows::$_wgetenv(L"");
		}
		for (auto element_pointer_raw = Third::system::windows::$_wenviron(); *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = cast_pointer<CharacterW>(make_pointer(*element_pointer_raw));
			auto element = self_cast<String>(SystemNativeString::wide_to_utf8(ConstantBasicStringView<CharacterW>{element_pointer, null_terminated_string_size_of(element_pointer)}));
			result.append(as_moveable(element));
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		for (auto element_pointer_raw = Third::system::posix::$environ; *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = cast_pointer<Character>(make_pointer(*element_pointer_raw));
			auto element = String{element_pointer, null_terminated_string_size_of(element_pointer)};
			result.append(as_moveable(element));
		}
		#endif
		return result;
	}

	#pragma endregion

	#pragma region process

	// NOTE: EXPLAIN
	// the return value is process's exit code, see the following webpage to understand
	// Windows - https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodeprocess
	// POSIX   - https://pubs.opengroup.org/onlinepubs/9699919799/functions/waitid.html
	// for Windows, the complete 32-bit exit code can always be obtained
	// for POSIX, although the standard states that the waitid function should obtain the full exit code, this is not actually the case in Linux and Macintosh
	// the following are the number of return bits that can be obtained in each system
	// Windows   : all 32 bit
	// Linux     : low 08 bit
	// Macintosh : low 24 bit
	inline auto run_process (
		Path const &           program,
		List<String> const &   argument,
		List<String> const &   environment,
		Optional<Path> const & input,
		Optional<Path> const & output,
		Optional<Path> const & error
	) -> IntegerU32 {
		auto result = IntegerU32{};
		#if defined M_system_windows
		auto state_b = Third::system::windows::$BOOL{};
		auto state_d = Third::system::windows::$DWORD{};
		auto null_device = Path{"/NUL"_s};
		auto program_string = BasicString<CharacterW>{};
		auto argument_string = BasicString<CharacterW>{};
		auto environment_string = BasicString<CharacterW>{};
		auto input_string = BasicString<CharacterW>{};
		auto output_string = BasicString<CharacterW>{};
		auto error_string = BasicString<CharacterW>{};
		auto security_attribute = Third::system::windows::$SECURITY_ATTRIBUTES{};
		auto startup_information = Third::system::windows::$STARTUPINFOW{};
		auto process_information = Third::system::windows::$PROCESS_INFORMATION{};
		auto exit_code_d = Third::system::windows::$DWORD{};
		program_string = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<ConstantBasicStringView<CharacterN>>(program.to_string(CharacterType::k_path_separator_windows))));
		argument_string = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<ConstantBasicStringView<CharacterN>>(Detail::encode_windows_command_string(program, argument))));
		environment_string = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<ConstantBasicStringView<CharacterN>>(Detail::encode_windows_environment_string(environment))));
		input_string = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>((!input.has() ? (null_device) : (input.get())).to_string())));
		output_string = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>((!output.has() ? (null_device) : (output.get())).to_string())));
		error_string = make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>((!error.has() ? (null_device) : (error.get())).to_string())));
		security_attribute.nLength = sizeof(Third::system::windows::$SECURITY_ATTRIBUTES);
		security_attribute.lpSecurityDescriptor = nullptr;
		security_attribute.bInheritHandle = Third::system::windows::$TRUE;
		startup_information.hStdInput = Third::system::windows::$CreateFileW(
			cast_pointer<Third::system::windows::$WCHAR>(input_string.begin()).value,
			Third::system::windows::$GENERIC_READ,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdInput != Third::system::windows::$INVALID_HANDLE_VALUE);
		startup_information.cb = sizeof(Third::system::windows::$STARTUPINFOW);
		startup_information.dwFlags = Third::system::windows::$STARTF_USESTDHANDLES;
		startup_information.hStdOutput = Third::system::windows::$CreateFileW(
			cast_pointer<Third::system::windows::$WCHAR>(output_string.begin()).value,
			Third::system::windows::$GENERIC_WRITE,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdOutput != Third::system::windows::$INVALID_HANDLE_VALUE);
		startup_information.hStdError = Third::system::windows::$CreateFileW(
			cast_pointer<Third::system::windows::$WCHAR>(error_string.begin()).value,
			Third::system::windows::$GENERIC_WRITE,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdError != Third::system::windows::$INVALID_HANDLE_VALUE);
		state_b = Third::system::windows::$CreateProcessW(
			cast_pointer<Third::system::windows::$WCHAR>(program_string.begin()).value,
			cast_pointer<Third::system::windows::$WCHAR>(argument_string.begin()).value,
			nullptr,
			nullptr,
			Third::system::windows::$TRUE,
			Third::system::windows::$CREATE_UNICODE_ENVIRONMENT | Third::system::windows::$DETACHED_PROCESS,
			cast_pointer<Third::system::windows::$WCHAR>(environment_string.begin()).value,
			nullptr,
			&startup_information,
			&process_information
		);
		assert_test(state_b != Third::system::windows::$FALSE);
		state_d = Third::system::windows::$WaitForSingleObject(process_information.hProcess, Third::system::windows::$INFINITE);
		assert_test(state_d == Third::system::windows::$WAIT_OBJECT_0);
		state_b = Third::system::windows::$GetExitCodeProcess(process_information.hProcess, &exit_code_d);
		assert_test(state_b != Third::system::windows::$FALSE);
		result = mbox<IntegerU32>(exit_code_d);
		state_b = Third::system::windows::$CloseHandle(startup_information.hStdInput);
		assert_test(state_b != Third::system::windows::$FALSE);
		state_b = Third::system::windows::$CloseHandle(startup_information.hStdOutput);
		assert_test(state_b != Third::system::windows::$FALSE);
		state_b = Third::system::windows::$CloseHandle(startup_information.hStdError);
		assert_test(state_b != Third::system::windows::$FALSE);
		state_b = Third::system::windows::$CloseHandle(process_information.hProcess);
		assert_test(state_b != Third::system::windows::$FALSE);
		state_b = Third::system::windows::$CloseHandle(process_information.hThread);
		assert_test(state_b != Third::system::windows::$FALSE);
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto state_i = int{};
		auto null_device = Path{"/dev/null"_s};
		auto program_string = String{};
		auto argument_string = List<String>{};
		auto argument_string_list = List<Character *>{};
		auto environment_string = List<String>{};
		auto environment_string_list = List<Character *>{};
		auto input_string = String{};
		auto output_string = String{};
		auto error_string = String{};
		auto process_id = Third::system::posix::$pid_t{};
		auto spawn_file_action = Third::system::posix::$posix_spawn_file_actions_t{};
		auto wait_info = Third::system::posix::$siginfo_t{};
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
		state_i = Third::system::posix::$posix_spawn_file_actions_init(&spawn_file_action);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_addopen(&spawn_file_action, Third::system::posix::$STDIN_FILENO, cast_pointer<char>(input_string.begin()).value, Third::system::posix::$O_RDONLY, 0);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_addopen(&spawn_file_action, Third::system::posix::$STDOUT_FILENO, cast_pointer<char>(output_string.begin()).value, Third::system::posix::$O_WRONLY, 0);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_addopen(&spawn_file_action, Third::system::posix::$STDERR_FILENO, cast_pointer<char>(error_string.begin()).value, Third::system::posix::$O_WRONLY, 0);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn(&process_id, cast_pointer<char>(program_string.begin()).value, &spawn_file_action, nullptr, cast_pointer<char *>(argument_string_list.begin()).value, cast_pointer<char *>(environment_string_list.begin()).value);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_destroy(&spawn_file_action);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$waitid(Third::system::posix::$P_PID, static_cast<Third::system::posix::$id_t>(process_id), &wait_info, Third::system::posix::$WEXITED | Third::system::posix::$WSTOPPED);
		assert_test(state_i == 0);
		result = mbox<IntegerU32>(wait_info.si_status);
		#endif
		return result;
	}

	#pragma endregion

	#pragma region command

	inline auto execute_command (
		String const & command
	) -> IntegerU32 {
		auto result = IntegerU32{};
		#if defined M_system_windows
		result = mbox<IntegerU32>(Third::system::windows::$_wsystem(cast_pointer<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(self_cast<BasicString<CharacterN>>(command))).begin()).value));
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android
		result = mbox<IntegerU32>(std::system(cast_pointer<char>(make_null_terminated_string(command).begin()).value));
		#endif
		#if defined M_system_iphone
		throw UnsupportedException{};
		#endif
		return result;
	}

	#pragma endregion

}
