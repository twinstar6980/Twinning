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
import twinning.kernel.utility.string.character_type;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.miscellaneous.system_native_string;
import twinning.kernel.third.system.windows;
import twinning.kernel.third.system.posix;

export namespace Twinning::Kernel::Process {

	#pragma region detail

	namespace Detail {

		#pragma region posix

		inline auto make_posix_string_data_list(
			Optional<String> const & first,
			List<String> const &     source
		) -> List<String> {
			auto destination = List<String>{};
			destination.allocate(1_sz + source.size());
			if (first.has()) {
				destination.append(make_null_terminated_string(first.get()));
			}
			for (auto & element : source) {
				destination.append(make_null_terminated_string(element));
			}
			return destination;
		}

		inline auto make_posix_string_pointer_list(
			List<String> & source
		) -> List<char *> {
			auto destination = List<char *>{};
			destination.allocate(source.size() + 1_sz);
			for (auto & element : source) {
				destination.append(unmake_pointer_unsafe<char>(element.begin()));
			}
			destination.append(nullptr);
			return destination;
		}

		#pragma endregion

		#pragma region windows

		// NOTE: EXPLAIN: see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170

		inline auto encode_windows_command_program_string(
			Path const & source,
			String &     destination
		) -> Void {
			destination.append('"'_c);
			destination.append_list(source.emit_windows(k_true));
			destination.append('"'_c);
			return;
		}

		inline auto encode_windows_command_argument_string(
			String const & source,
			String &       destination
		) -> Void {
			auto current_backslash_count = 0_sz;
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
					current_backslash_count = 0_sz;
				}
			}
			for (auto & index : SizeRange{current_backslash_count}) {
				destination.append('\\'_c);
			}
			destination.append('"'_c);
			return;
		}

		inline auto encode_windows_command_string(
			Path const &         program,
			List<String> const & argument
		) -> String {
			auto destination_size = 0_sz;
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

		// ----------------

		// NOTE: EXPLAIN: see https://learn.microsoft.com/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw

		inline auto encode_windows_environment_string(
			List<String> const & environment
		) -> String {
			auto destination_size = 0_sz;
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

	inline auto get_workspace(
	) -> Path {
		auto target = std::filesystem::current_path();
		return Path{make_string(unsafe_cast<std::string>(target.generic_u8string()))};
	}

	#pragma endregion

	#pragma region environment

	inline auto get_environment(
	) -> List<String> {
		auto result = List<String>{};
		#if defined M_system_windows
		if (Third::system::windows::$_wenviron() == nullptr) {
			Third::system::windows::$_wgetenv(L"");
		}
		for (auto element_pointer_raw = Third::system::windows::$_wenviron(); *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = make_pointer_unsafe<CharacterW>(*element_pointer_raw);
			auto element = unsafe_cast<String>(SystemNativeString::wide_to_utf8(ConstantBasicStringView<CharacterW>{element_pointer, null_terminated_string_size_of(element_pointer)}));
			result.append(as_moveable(element));
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		for (auto element_pointer_raw = Third::system::posix::$environ; *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = make_pointer_unsafe<Character>(*element_pointer_raw);
			auto element = String{element_pointer, null_terminated_string_size_of(element_pointer)};
			result.append(as_moveable(element));
		}
		#endif
		return result;
	}

	inline auto find_environment(
		String const & name
	) -> Optional<String> {
		auto value = Optional<String>{};
		#if defined M_system_windows
		auto state_d = Third::system::windows::$DWORD{};
		auto name_w = make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<ConstantBasicStringView<CharacterN>>(name)));
		state_d = Third::system::windows::$GetEnvironmentVariableW(unmake_pointer_unsafe<Third::system::windows::$WCHAR>(name_w.begin()), nullptr, 0);
		if (state_d == 0) {
			assert_test(Third::system::windows::$GetLastError() == Third::system::windows::$ERROR_ENVVAR_NOT_FOUND);
			value.reset();
		}
		else {
			auto buffer = Array<CharacterW>{make_box<Size>(state_d)};
			state_d = Third::system::windows::$GetEnvironmentVariableW(unmake_pointer_unsafe<Third::system::windows::$WCHAR>(name_w.begin()), unmake_pointer_unsafe<Third::system::windows::$WCHAR>(buffer.begin()), unmake_box<Third::system::windows::$DWORD>(buffer.size()));
			assert_test(state_d == unmake_box<Third::system::windows::$DWORD>(buffer.size() - 1_sz));
			value.set(unsafe_cast<String>(SystemNativeString::wide_to_utf8(ConstantBasicStringView<CharacterW>{buffer.begin(), buffer.size() - 1_sz})));
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto value_if = Third::system::posix::$getenv(unmake_pointer_unsafe<char>(make_null_terminated_string(name).begin()));
		if (value_if == nullptr) {
			value.reset();
		}
		else {
			value.set(make_string_view(value_if));
		}
		#endif
		return value;
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
	// Windows   : all 32-bit
	// Linux     : low 08-bit
	// Macintosh : low 24-bit
	inline auto run_process(
		Path const &           program,
		List<String> const &   argument,
		Path const &           workspace,
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
		auto argument_string = Detail::encode_windows_command_string(program, argument);
		auto environment_string = Detail::encode_windows_environment_string(environment);
		auto security_attribute = Third::system::windows::$SECURITY_ATTRIBUTES{};
		security_attribute.nLength = sizeof(Third::system::windows::$SECURITY_ATTRIBUTES);
		security_attribute.lpSecurityDescriptor = nullptr;
		security_attribute.bInheritHandle = Third::system::windows::$TRUE;
		auto startup_information = Third::system::windows::$STARTUPINFOW{};
		startup_information.cb = sizeof(Third::system::windows::$STARTUPINFOW);
		startup_information.dwFlags = Third::system::windows::$STARTF_USESTDHANDLES;
		startup_information.hStdInput = Third::system::windows::$CreateFileW(
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<BasicString<CharacterN>>((!input.has() ? (null_device) : (input.get())).emit_native()))).begin()),
			Third::system::windows::$GENERIC_READ,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdInput != Third::system::windows::$INVALID_HANDLE_VALUE);
		startup_information.hStdOutput = Third::system::windows::$CreateFileW(
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<BasicString<CharacterN>>((!output.has() ? (null_device) : (output.get())).emit_native()))).begin()),
			Third::system::windows::$GENERIC_WRITE,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdOutput != Third::system::windows::$INVALID_HANDLE_VALUE);
		startup_information.hStdError = Third::system::windows::$CreateFileW(
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<BasicString<CharacterN>>((!error.has() ? (null_device) : (error.get())).emit_native()))).begin()),
			Third::system::windows::$GENERIC_WRITE,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdError != Third::system::windows::$INVALID_HANDLE_VALUE);
		auto process_information = Third::system::windows::$PROCESS_INFORMATION{};
		state_b = Third::system::windows::$CreateProcessW(
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<ConstantBasicStringView<CharacterN>>(program.emit_native()))).begin()),
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<ConstantBasicStringView<CharacterN>>(argument_string))).begin()),
			nullptr,
			nullptr,
			Third::system::windows::$TRUE,
			Third::system::windows::$CREATE_UNICODE_ENVIRONMENT | Third::system::windows::$DETACHED_PROCESS,
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<ConstantBasicStringView<CharacterN>>(environment_string))).begin()),
			unmake_pointer_unsafe<Third::system::windows::$WCHAR>(make_null_terminated_string(SystemNativeString::wide_from_utf8(unsafe_cast<ConstantBasicStringView<CharacterN>>(workspace.emit_native()))).begin()),
			&startup_information,
			&process_information
		);
		assert_test(state_b != Third::system::windows::$FALSE);
		state_d = Third::system::windows::$WaitForSingleObject(
			process_information.hProcess,
			Third::system::windows::$INFINITE
		);
		assert_test(state_d == Third::system::windows::$WAIT_OBJECT_0);
		auto exit_code = Third::system::windows::$DWORD{};
		state_b = Third::system::windows::$GetExitCodeProcess(
			process_information.hProcess,
			&exit_code
		);
		assert_test(state_b != Third::system::windows::$FALSE);
		result = make_box<IntegerU32>(exit_code);
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
		auto argument_string = Detail::make_posix_string_data_list(make_optional_of(program.emit_native()), argument);
		auto argument_string_list = Detail::make_posix_string_pointer_list(argument_string);
		auto environment_string = Detail::make_posix_string_data_list(k_null_optional, environment);
		auto environment_string_list = Detail::make_posix_string_pointer_list(environment_string);
		auto spawn_file_action = Third::system::posix::$posix_spawn_file_actions_t{};
		state_i = Third::system::posix::$posix_spawn_file_actions_init(&spawn_file_action);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_addopen(
			&spawn_file_action,
			Third::system::posix::$STDIN_FILENO,
			unmake_pointer_unsafe<char>(make_null_terminated_string((!input.has() ? (null_device) : (input.get())).emit_native()).begin()),
			Third::system::posix::$O_RDONLY,
			0
		);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_addopen(
			&spawn_file_action,
			Third::system::posix::$STDOUT_FILENO,
			unmake_pointer_unsafe<char>(make_null_terminated_string((!output.has() ? (null_device) : (output.get())).emit_native()).begin()),
			Third::system::posix::$O_WRONLY,
			0
		);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_addopen(
			&spawn_file_action,
			Third::system::posix::$STDERR_FILENO,
			unmake_pointer_unsafe<char>(make_null_terminated_string((!error.has() ? (null_device) : (error.get())).emit_native()).begin()),
			Third::system::posix::$O_WRONLY,
			0
		);
		assert_test(state_i == 0);
		auto process_identifier = Third::system::posix::$pid_t{};
		state_i = Third::system::posix::$posix_spawn(
			&process_identifier,
			unmake_pointer_unsafe<char>(make_null_terminated_string(program.emit_native()).begin()),
			&spawn_file_action,
			nullptr,
			unmake_pointer_unsafe<char *>(argument_string_list.begin()),
			unmake_pointer_unsafe<char *>(environment_string_list.begin())
		);
		assert_test(state_i == 0);
		state_i = Third::system::posix::$posix_spawn_file_actions_destroy(&spawn_file_action);
		assert_test(state_i == 0);
		auto wait_information = Third::system::posix::$siginfo_t{};
		state_i = Third::system::posix::$waitid(
			Third::system::posix::$P_PID,
			static_cast<Third::system::posix::$id_t>(process_identifier),
			&wait_information,
			Third::system::posix::$WEXITED | Third::system::posix::$WSTOPPED
		);
		assert_test(state_i == 0);
		result = make_box<IntegerU32>(wait_information.si_status);
		#endif
		return result;
	}

	#pragma endregion

}
