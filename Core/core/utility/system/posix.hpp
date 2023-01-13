#pragma once

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#include "core/utility/string/string.hpp"
#include "core/utility/file_system/file_system.hpp"
#include "core/third_party/system/posix.hpp"
#include <cerrno>

namespace TwinStar::Core::System::POSIX {

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
		int pipe_fd[2] = {0, 0};
		assert_condition(pipe(pipe_fd) == 0);
		{
			auto pipe_flag_0 = fcntl(pipe_fd[0], F_GETFL, 0);
			auto pipe_flag_1 = fcntl(pipe_fd[1], F_GETFL, 0);
			fcntl(pipe_fd[0], F_SETFL, pipe_flag_0 | O_NONBLOCK);
			fcntl(pipe_fd[1], F_SETFL, pipe_flag_1 | O_NONBLOCK);
		}
		auto pid = fork();
		assert_condition(pid >= 0);
		if (pid == 0) {
			close(pipe_fd[0]);
			try {
				auto program_string = make_null_terminated_string(program.to_string());
				auto argument_string = List<String>{};
				argument_string.assign(
					argument,
					[] (auto & element) -> auto {
						return make_null_terminated_string(element);
					}
				);
				auto argument_string_list = List<char *>{1_sz + argument.size() + 1_sz};
				argument_string_list.append(cast_pointer<char>(program_string.begin()).value);
				for (auto & element : argument_string) {
					argument_string_list.append(cast_pointer<char>(element.begin()).value);
				}
				argument_string_list.append(nullptr);
				if (redirect_input.has()) {
					auto redirect_fd = open(cast_pointer<char>(make_null_terminated_string(redirect_input.get().to_string()).begin()).value, O_RDWR, 0777);
					assert_condition(redirect_fd >= 0);
					auto dup_state = dup2(redirect_fd, STDIN_FILENO);
					assert_condition(dup_state != -1);
					close(redirect_fd);
				}
				if (redirect_output.has()) {
					auto redirect_fd = open(cast_pointer<char>(make_null_terminated_string(redirect_output.get().to_string()).begin()).value, O_RDWR, 0777);
					assert_condition(redirect_fd >= 0);
					auto dup_state = dup2(redirect_fd, STDOUT_FILENO);
					assert_condition(dup_state != -1);
					close(redirect_fd);
				}
				if (redirect_error.has()) {
					auto redirect_fd = open(cast_pointer<char>(make_null_terminated_string(redirect_error.get().to_string()).begin()).value, O_RDWR, 0777);
					assert_condition(redirect_fd >= 0);
					auto dup_state = dup2(redirect_fd, STDERR_FILENO);
					assert_condition(dup_state != -1);
					close(redirect_fd);
				}
				execv(cast_pointer<char>(program_string.begin()).value, argument_string_list.begin().value);
			} catch (Exception & exception) {
			}
			write(pipe_fd[1], &errno, sizeof(errno));
			close(pipe_fd[1]);
			std::exit(0);
		} else {
			close(pipe_fd[1]);
			auto info = siginfo_t{};
			auto wait_state = waitid(P_PID, pid, &info, WEXITED | WSTOPPED);
			assert_condition(wait_state == 0);
			auto pipe_read_count = read(pipe_fd[0], &errno, sizeof(errno));
			close(pipe_fd[0]);
			assert_condition(pipe_read_count == 0);
			return mbw<IntegerU32>(info.si_status);
		}
	}

	inline auto system (
		String const & command
	) -> IntegerU32 {
		#if defined M_system_linux || defined M_system_macintosh || defined M_system_android
		return mbw<IntegerU32>(std::system(cast_pointer<char>(make_null_terminated_string(command).begin()).value));
		#endif
		#if defined M_system_iphone
		return mbw<IntegerU32>(0x00000000);
		#endif
	}

	#pragma endregion

}

#endif
