#pragma once

#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios

#include "core/utility/string/string.hpp"
#include "core/utility/file_system/file_system.hpp"
#include "core/third_party/system/posix.hpp"
#include <cerrno>

namespace TwinStar::Core::System::POSIX {

	#pragma region control

	// NOTE : [[noreturn]]
	inline auto exit (
		IntegerS32 const & code
	) -> Void {
		std::exit(code.value);
		return;
	}

	inline auto sleep (
		Size const & time
	) -> Void {
		// TODO : lose
		usleep(static_cast<useconds_t>(time.value * 1000));
		return;
	}

	#pragma endregion

	#pragma region command

	inline auto system (
		String const & command
	) -> IntegerS32 {
#if defined M_system_linux || defined M_system_macos || defined M_system_android
		return mbw<IntegerS32>(std::system(cast_pointer<char>(make_null_terminated_string(command).begin()).value));
#endif
#if defined M_system_ios
		return mbw<IntegerS32>(0);
#endif
	}

	inline auto process (
		Path const &         path,
		List<String> const & argument
	) -> IntegerS32 {
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
			auto null_terminated_path = make_null_terminated_string(path.to_string());
			auto null_terminated_argument = List<String>{};
			null_terminated_argument.assign(
				argument,
				[] (auto & element) -> auto {
					return make_null_terminated_string(element);
				}
			);
			auto argument_pointer_list = List<char *>{1_sz + argument.size() + 1_sz};
			argument_pointer_list.append(cast_pointer<char>(null_terminated_path.begin()).value);
			for (auto & element : null_terminated_argument) {
				argument_pointer_list.append(cast_pointer<char>(element.begin()).value);
			}
			argument_pointer_list.append(nullptr);
			execv(cast_pointer<char>(null_terminated_path.begin()).value, argument_pointer_list.begin().value);
			write(pipe_fd[1], &errno, sizeof(errno));
			close(pipe_fd[1]);
			std::exit(-1);
		} else {
			close(pipe_fd[1]);
			auto status = int{0};
			waitpid(pid, &status, 0);
			assert_condition(WIFEXITED(status));
			auto pipe_read_count = read(pipe_fd[0], &errno, sizeof(errno));
			close(pipe_fd[0]);
			if (pipe_read_count != 0) {
				assert_failed(R"(/* execv success */)");
			}
			return mbw<IntegerS32>(WEXITSTATUS(status));
		}
	}

	#pragma endregion

}

#endif
