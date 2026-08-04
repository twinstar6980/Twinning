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
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.storage.utility;
import twinning.kernel.dependency.system.win32;
import twinning.kernel.dependency.system.posix;

export namespace Twinning::Kernel::Process {

	#pragma region context

	inline auto get_workspace(
	) -> Path {
		auto target = Path{};
		#if defined M_system_windows
		auto state_d = Dependency::system::win32::$DWORD{};
		state_d = Dependency::system::win32::$GetCurrentDirectoryW(
			0,
			nullptr
		);
		assert_test(state_d != 0);
		auto buffer = BasicString<CharacterW>{make_box<Size>(state_d)};
		state_d = Dependency::system::win32::$GetCurrentDirectoryW(
			unmake_box<Dependency::system::win32::$DWORD>(buffer.capacity()),
			unmake_pointer_unsafe<Dependency::system::win32::$WCHAR>(buffer.begin())
		);
		assert_test(state_d != 0);
		assert_test(state_d == unmake_box<Dependency::system::win32::$DWORD>(buffer.capacity() - 1_sz));
		buffer.set_size(buffer.capacity() - 1_sz);
		target.parse(unsafe_cast<String>(SystemNativeString::wide_to_utf8(buffer)));
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		auto buffer = BasicString<Character>{256_sz};
		while (k_true) {
			auto current_result = Dependency::system::posix::$getcwd(
				unmake_pointer_unsafe<char>(buffer.begin()),
				unmake_box<std::size_t>(buffer.capacity())
			);
			if (current_result != nullptr) {
				break;
			}
			assert_test(Dependency::system::posix::$errno == Dependency::system::posix::$ERANGE);
			buffer.expand(buffer.capacity());
		}
		buffer.set_size(null_terminated_string_size_of(buffer.begin()));
		target.parse(unsafe_cast<String>(buffer));
		#endif
		return target;
	}

	inline auto get_environment(
	) -> Map<String, String> {
		auto result_list = List<String>{};
		#if defined M_system_windows
		auto state_b = Dependency::system::win32::$BOOL{};
		auto string_raw = Dependency::system::win32::$GetEnvironmentStringsW();
		assert_test(string_raw != nullptr);
		auto string_pointer_raw_finalizer = make_finalizer(
			[&] {
				state_b = Dependency::system::win32::$FreeEnvironmentStringsW(string_raw);
				assert_test(state_b != Dependency::system::win32::$FALSE);
			}
		);
		for (auto element_raw = string_raw; *element_raw != L'\0'; ++element_raw) {
			auto element_pointer = make_pointer_unsafe<CharacterW>(element_raw);
			auto element_view = ConstantBasicStringView<CharacterW>{element_pointer, null_terminated_string_size_of(element_pointer)};
			result_list.append(unsafe_cast<String>(SystemNativeString::wide_to_utf8(element_view)));
			element_raw += unmake_box<std::size_t>(element_view.size());
		}
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		for (auto element_raw = Dependency::system::posix::$environ; *element_raw != nullptr; ++element_raw) {
			auto element_pointer = make_pointer_unsafe<Character>(*element_raw);
			auto element_view = ConstantBasicStringView<Character>{element_pointer, null_terminated_string_size_of(element_pointer)};
			result_list.append(element_view);
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
		auto input_handle = Pointer<Void>{};
		auto output_handle = Pointer<Void>{};
		auto error_handle = Pointer<Void>{};
		auto input_handle_finalizer = Storage::Detail::open_file(input_handle, input, k_false, k_true, k_false);
		auto output_handle_finalizer = Storage::Detail::open_file(output_handle, output, k_false, k_false, k_true);
		auto error_handle_finalizer = Storage::Detail::open_file(error_handle, error, k_false, k_false, k_true);
		#if defined M_system_windows
		assert_test(!command.empty());
		auto state_b = Dependency::system::win32::$BOOL{};
		auto state_d = Dependency::system::win32::$DWORD{};
		auto environment_string = String{};
		environment_string.allocate(
			Range::accumulate(
				environment,
				[](auto & element) {
					return element.key.size() + 1_sz + element.value.size() + 1_sz;
				},
				1_sz
			)
		);
		for (auto & element : environment) {
			environment_string.append_list(element.key + "="_sv + element.value);
			environment_string.append('\0'_c);
		}
		auto startup_information = Dependency::system::win32::$STARTUPINFOEXW{};
		startup_information.StartupInfo.cb = sizeof(Dependency::system::win32::$STARTUPINFOEXW);
		startup_information.StartupInfo.dwFlags = Dependency::system::win32::$STARTF_USESTDHANDLES;
		startup_information.StartupInfo.hStdInput = static_cast<Dependency::system::win32::$HANDLE>(input_handle.value);
		startup_information.StartupInfo.hStdOutput = static_cast<Dependency::system::win32::$HANDLE>(output_handle.value);
		startup_information.StartupInfo.hStdError = static_cast<Dependency::system::win32::$HANDLE>(error_handle.value);
		auto startup_attribute_list_size = Dependency::system::win32::$SIZE_T{};
		auto startup_attribute_list_data = ByteArray{};
		state_b = Dependency::system::win32::$InitializeProcThreadAttributeList(nullptr, 1, 0, &startup_attribute_list_size);
		assert_test(state_b == Dependency::system::win32::$FALSE);
		startup_attribute_list_data.allocate(make_box<Size>(startup_attribute_list_size));
		startup_information.lpAttributeList = unmake_pointer_unsafe<AsUnmakeReference<decltype(*Dependency::system::win32::$LPPROC_THREAD_ATTRIBUTE_LIST{})>>(startup_attribute_list_data.begin());
		state_b = Dependency::system::win32::$InitializeProcThreadAttributeList(startup_information.lpAttributeList, 1, 0, &startup_attribute_list_size);
		assert_test(state_b != Dependency::system::win32::$FALSE);
		auto startup_attribute_list_finalizer = make_finalizer(
			[&] {
				Dependency::system::win32::$DeleteProcThreadAttributeList(startup_information.lpAttributeList);
				startup_attribute_list_data.reset();
			}
		);
		auto inherit_handle_list = make_array<Dependency::system::win32::$HANDLE>(
			startup_information.StartupInfo.hStdInput,
			startup_information.StartupInfo.hStdOutput,
			startup_information.StartupInfo.hStdError
		);
		for (auto & inherit_handle : inherit_handle_list) {
			state_b = Dependency::system::win32::$SetHandleInformation(
				inherit_handle,
				Dependency::system::win32::$HANDLE_FLAG_INHERIT,
				Dependency::system::win32::$HANDLE_FLAG_INHERIT
			);
			assert_test(state_b != Dependency::system::win32::$FALSE);
		}
		state_b = Dependency::system::win32::$UpdateProcThreadAttribute(
			startup_information.lpAttributeList,
			0,
			Dependency::system::win32::$PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
			unmake_pointer(inherit_handle_list.begin()),
			unmake_box<std::size_t>(inherit_handle_list.size()) * sizeof(Dependency::system::win32::$HANDLE),
			nullptr,
			nullptr
		);
		assert_test(state_b != Dependency::system::win32::$FALSE);
		auto process_information = Dependency::system::win32::$PROCESS_INFORMATION{};
		state_b = Dependency::system::win32::$CreateProcessW(
			M_use_ntsp_w_safe_of(program.emit_native()),
			M_use_ntsp_w_safe_of(command),
			nullptr,
			nullptr,
			Dependency::system::win32::$TRUE,
			Dependency::system::win32::$CREATE_UNICODE_ENVIRONMENT | Dependency::system::win32::$CREATE_NO_WINDOW | Dependency::system::win32::$EXTENDED_STARTUPINFO_PRESENT,
			M_use_ntsp_w_of(environment_string),
			M_use_ntsp_w_safe_of(workspace.emit_native()),
			reinterpret_cast<Dependency::system::win32::$STARTUPINFOW *>(&startup_information),
			&process_information
		);
		assert_test(state_b != Dependency::system::win32::$FALSE);
		startup_attribute_list_finalizer.dispose();
		input_handle_finalizer.dispose();
		output_handle_finalizer.dispose();
		error_handle_finalizer.dispose();
		auto process_handle_finalizer = make_finalizer(
			[&] {
				state_b = Dependency::system::win32::$CloseHandle(process_information.hProcess);
				assert_test(state_b != Dependency::system::win32::$FALSE);
				state_b = Dependency::system::win32::$CloseHandle(process_information.hThread);
				assert_test(state_b != Dependency::system::win32::$FALSE);
			}
		);
		state_d = Dependency::system::win32::$WaitForSingleObject(
			process_information.hProcess,
			Dependency::system::win32::$INFINITE
		);
		assert_test(state_d == Dependency::system::win32::$WAIT_OBJECT_0);
		auto exit_code = Dependency::system::win32::$DWORD{};
		state_b = Dependency::system::win32::$GetExitCodeProcess(
			process_information.hProcess,
			&exit_code
		);
		assert_test(state_b != Dependency::system::win32::$FALSE);
		process_handle_finalizer.dispose();
		result = make_box<IntegerU32>(exit_code);
		#endif
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
		assert_test(!argument.empty());
		auto state_i = int{};
		auto program_string = M_use_nts_n_safe_of(program.emit_native());
		auto argument_string = List<String>{};
		argument_string.allocate(argument.size());
		for (auto & element : argument) {
			argument_string.append(M_use_nts_n_safe_of(element));
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
			environment_string.append(M_use_nts_n_safe_of(element.key + "="_sv + element.value));
		}
		auto environment_string_list = List<char *>{};
		environment_string_list.allocate(environment_string.size() + 1_sz);
		for (auto & element : environment_string) {
			environment_string_list.append(unmake_pointer_unsafe<char>(element.begin()));
		}
		environment_string_list.append(nullptr);
		auto workspace_string = M_use_nts_n_safe_of(workspace.emit_native());
		auto process_identifier = Dependency::system::posix::$fork();
		assert_test(process_identifier != -1);
		if (process_identifier == 0) {
			try {
				state_i = Dependency::system::posix::$chdir(unmake_pointer_unsafe<char>(workspace_string.begin()));
				assert_test(state_i != -1);
				state_i = Dependency::system::posix::$dup2(static_cast<int>(reinterpret_cast<std::intptr_t>(input_handle.value)), Dependency::system::posix::$STDIN_FILENO);
				assert_test(state_i != -1);
				state_i = Dependency::system::posix::$dup2(static_cast<int>(reinterpret_cast<std::intptr_t>(output_handle.value)), Dependency::system::posix::$STDOUT_FILENO);
				assert_test(state_i != -1);
				state_i = Dependency::system::posix::$dup2(static_cast<int>(reinterpret_cast<std::intptr_t>(error_handle.value)), Dependency::system::posix::$STDERR_FILENO);
				assert_test(state_i != -1);
				input_handle_finalizer.dispose();
				output_handle_finalizer.dispose();
				error_handle_finalizer.dispose();
				Dependency::system::posix::$execve(
					unmake_pointer_unsafe<char>(program_string.begin()),
					unmake_pointer_unsafe<char *>(argument_string_list.begin()),
					unmake_pointer_unsafe<char *>(environment_string_list.begin())
				);
			}
			catch (...) {
			}
			Dependency::system::posix::$_exit(127);
		}
		input_handle_finalizer.dispose();
		output_handle_finalizer.dispose();
		error_handle_finalizer.dispose();
		auto wait_information = Dependency::system::posix::$siginfo_t{};
		state_i = Dependency::system::posix::$waitid(
			Dependency::system::posix::$P_PID,
			static_cast<Dependency::system::posix::$id_t>(process_identifier),
			&wait_information,
			Dependency::system::posix::$WEXITED | Dependency::system::posix::$WSTOPPED
		);
		assert_test(state_i == 0);
		result = make_box<IntegerU32>(wait_information.si_status);
		#endif
		return result;
	}

	#pragma endregion

}
