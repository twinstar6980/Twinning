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
import twinning.kernel.utility.container.map.map;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.miscellaneous.finalizer;
import twinning.kernel.utility.miscellaneous.system_native_string;
import twinning.kernel.third.system.windows;
import twinning.kernel.third.system.posix;

export namespace Twinning::Kernel::Process {

	#pragma region context

	inline auto get_workspace(
	) -> Path {
		auto target = std::filesystem::current_path();
		return Path{make_string(unsafe_cast<std::string>(target.generic_u8string()))};
	}

	inline auto get_environment(
	) -> Map<String, String> {
		auto result_list = List<String>{};
		#if defined M_system_windows
		if (Third::system::windows::$_wenviron() == nullptr) {
			Third::system::windows::$_wgetenv(L"");
		}
		for (auto element_pointer_raw = Third::system::windows::$_wenviron(); *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = make_pointer_unsafe<CharacterW>(*element_pointer_raw);
			auto element = unsafe_cast<String>(SystemNativeString::wide_to_utf8(ConstantBasicStringView<CharacterW>{element_pointer, null_terminated_string_size_of(element_pointer)}));
			result_list.append(as_moveable(element));
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		for (auto element_pointer_raw = Third::system::posix::$environ; *element_pointer_raw != nullptr; ++element_pointer_raw) {
			auto element_pointer = make_pointer_unsafe<Character>(*element_pointer_raw);
			auto element = String{element_pointer, null_terminated_string_size_of(element_pointer)};
			result_list.append(as_moveable(element));
		}
		#endif
		auto result = Map<String, String>{};
		for (auto & element : result_list) {
			auto splitter_position = Range::find_index(element, '='_c).get();
			auto element_key = element.head(splitter_position);
			auto element_value = element.tail(element.size() - (splitter_position + "="_sl));
			result.append(element_key, element_value);
		}
		return result;
	}

	#pragma endregion

	#pragma region child

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
	inline auto run_child(
		Path const &                program,
		List<String> const &        argument,
		String const &              command,
		Path const &                workspace,
		Map<String, String> const & environment,
		Path const &                input,
		Path const &                output,
		Path const &                error
	) -> IntegerU32 {
		auto result = IntegerU32{};
		assert_test(Storage::exist_file(program));
		assert_test(Storage::exist_directory(workspace));
		assert_test(Storage::exist_file(input));
		assert_test(Storage::exist_file(output));
		assert_test(Storage::exist_file(error));
		#if defined M_system_windows
		assert_test(!command.empty());
		auto state_b = Third::system::windows::$BOOL{};
		auto state_d = Third::system::windows::$DWORD{};
		auto environment_string_size = 0_sz;
		for (auto & element : environment) {
			environment_string_size += element.key.size() + 1_sz + element.value.size() + 1_sz;
		}
		environment_string_size += 1_sz;
		auto environment_string = String{environment_string_size};
		for (auto & element : environment) {
			environment_string.append_list(element.key + "="_sv + element.value);
			environment_string.append('\0'_c);
		}
		environment_string.append('\0'_c);
		auto security_attribute = Third::system::windows::$SECURITY_ATTRIBUTES{};
		security_attribute.nLength = sizeof(Third::system::windows::$SECURITY_ATTRIBUTES);
		security_attribute.lpSecurityDescriptor = nullptr;
		security_attribute.bInheritHandle = Third::system::windows::$TRUE;
		auto startup_information = Third::system::windows::$STARTUPINFOW{};
		startup_information.cb = sizeof(Third::system::windows::$STARTUPINFOW);
		startup_information.dwFlags = Third::system::windows::$STARTF_USESTDHANDLES;
		startup_information.hStdInput = Third::system::windows::$INVALID_HANDLE_VALUE;
		startup_information.hStdOutput = Third::system::windows::$INVALID_HANDLE_VALUE;
		startup_information.hStdError = Third::system::windows::$INVALID_HANDLE_VALUE;
		auto standard_handle_finalizer = make_finalizer(
			[&] {
				if (startup_information.hStdInput != Third::system::windows::$INVALID_HANDLE_VALUE) {
					state_b = Third::system::windows::$CloseHandle(startup_information.hStdInput);
					assert_test(state_b != Third::system::windows::$FALSE);
				}
				if (startup_information.hStdOutput != Third::system::windows::$INVALID_HANDLE_VALUE) {
					state_b = Third::system::windows::$CloseHandle(startup_information.hStdOutput);
					assert_test(state_b != Third::system::windows::$FALSE);
				}
				if (startup_information.hStdError != Third::system::windows::$INVALID_HANDLE_VALUE) {
					state_b = Third::system::windows::$CloseHandle(startup_information.hStdError);
					assert_test(state_b != Third::system::windows::$FALSE);
				}
			}
		);
		startup_information.hStdInput = Third::system::windows::$CreateFileW(
			M_use_ntsp_w_of(input.emit_native()),
			Third::system::windows::$GENERIC_READ,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdInput != Third::system::windows::$INVALID_HANDLE_VALUE);
		startup_information.hStdOutput = Third::system::windows::$CreateFileW(
			M_use_ntsp_w_of(output.emit_native()),
			Third::system::windows::$GENERIC_WRITE,
			Third::system::windows::$FILE_SHARE_READ | Third::system::windows::$FILE_SHARE_WRITE | Third::system::windows::$FILE_SHARE_DELETE,
			&security_attribute,
			Third::system::windows::$OPEN_EXISTING,
			Third::system::windows::$FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		assert_test(startup_information.hStdOutput != Third::system::windows::$INVALID_HANDLE_VALUE);
		startup_information.hStdError = Third::system::windows::$CreateFileW(
			M_use_ntsp_w_of(error.emit_native()),
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
			M_use_ntsp_w_of(program.emit_native()),
			M_use_ntsp_w_of(command),
			nullptr,
			nullptr,
			Third::system::windows::$TRUE,
			Third::system::windows::$CREATE_UNICODE_ENVIRONMENT | Third::system::windows::$CREATE_NO_WINDOW,
			M_use_ntsp_w_of(environment_string),
			M_use_ntsp_w_of(workspace.emit_native()),
			&startup_information,
			&process_information
		);
		assert_test(state_b != Third::system::windows::$FALSE);
		auto process_handle_finalizer = make_finalizer(
			[&] {
				state_b = Third::system::windows::$CloseHandle(process_information.hProcess);
				assert_test(state_b != Third::system::windows::$FALSE);
				state_b = Third::system::windows::$CloseHandle(process_information.hThread);
				assert_test(state_b != Third::system::windows::$FALSE);
			}
		);
		standard_handle_finalizer.dispose();
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
		process_handle_finalizer.dispose();
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		assert_test(!argument.empty());
		auto state_i = int{};
		auto program_string = M_use_nts_n_of(program.emit_native());
		auto argument_string = List<String>{};
		argument_string.allocate(argument.size());
		for (auto & element : argument) {
			argument_string.append(M_use_nts_n_of(element));
		}
		auto argument_string_list = List<char *>{};
		argument_string_list.allocate(argument_string.size() + 1_sz);
		for (auto & element : argument_string) {
			argument_string_list.append(unmake_pointer_unsafe<char>(element.begin()));
		}
		argument_string_list.append(nullptr);
		auto environment_string = List<String>{};
		environment_string.allocate(environment.size());
		for (auto & element : environment) {
			environment_string.append(M_use_nts_n_of(element.key + "="_sv + element.value));
		}
		auto environment_string_list = List<char *>{};
		environment_string_list.allocate(environment_string.size() + 1_sz);
		for (auto & element : environment_string) {
			environment_string_list.append(unmake_pointer_unsafe<char>(element.begin()));
		}
		environment_string_list.append(nullptr);
		auto workspace_string = M_use_nts_n_of(workspace.emit_native());
		auto input_handle = int{-1};
		auto output_handle = int{-1};
		auto error_handle = int{-1};
		auto standard_handle_finalizer = make_finalizer(
			[&] {
				if (input_handle > Third::system::posix::$STDERR_FILENO) {
					state_i = Third::system::posix::$close(input_handle);
					assert_test(state_i != -1);
				}
				if (output_handle > Third::system::posix::$STDERR_FILENO) {
					state_i = Third::system::posix::$close(output_handle);
					assert_test(state_i != -1);
				}
				if (error_handle > Third::system::posix::$STDERR_FILENO) {
					state_i = Third::system::posix::$close(error_handle);
					assert_test(state_i != -1);
				}
			}
		);
		input_handle = Third::system::posix::$open(
			M_use_ntsp_n_of(input.emit_native()),
			Third::system::posix::$O_RDONLY,
			0
		);
		assert_test(input_handle != -1);
		output_handle = Third::system::posix::$open(
			M_use_ntsp_n_of(output.emit_native()),
			Third::system::posix::$O_WRONLY,
			0
		);
		assert_test(output_handle != -1);
		error_handle = Third::system::posix::$open(
			M_use_ntsp_n_of(error.emit_native()),
			Third::system::posix::$O_WRONLY,
			0
		);
		assert_test(error_handle != -1);
		auto process_identifier = Third::system::posix::$fork();
		if (process_identifier != 0) {
			standard_handle_finalizer.dispose();
		}
		assert_test(process_identifier != -1);
		if (process_identifier == 0) {
			try {
				state_i = Third::system::posix::$chdir(unmake_pointer_unsafe<char>(workspace_string.begin()));
				assert_test(state_i != -1);
				state_i = Third::system::posix::$dup2(input_handle, Third::system::posix::$STDIN_FILENO);
				assert_test(state_i != -1);
				state_i = Third::system::posix::$dup2(output_handle, Third::system::posix::$STDOUT_FILENO);
				assert_test(state_i != -1);
				state_i = Third::system::posix::$dup2(error_handle, Third::system::posix::$STDERR_FILENO);
				assert_test(state_i != -1);
				standard_handle_finalizer.dispose();
				Third::system::posix::$execve(
					unmake_pointer_unsafe<char>(program_string.begin()),
					unmake_pointer_unsafe<char *>(argument_string_list.begin()),
					unmake_pointer_unsafe<char *>(environment_string_list.begin())
				);
			}
			catch (...) {
			}
			Third::system::posix::$_exit(127);
		}
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
