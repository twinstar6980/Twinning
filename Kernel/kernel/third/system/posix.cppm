module;

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "spawn.h"
#include "sys/wait.h"

#if defined M_system_macintosh || defined M_system_iphone
extern "C" char * * environ; // NOTE : EXPLAIN - not declared in unistd.h of xcode sdk
#endif

#pragma clang diagnostic pop

#endif

export module twinning.kernel.third.system.posix;

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

export namespace Twinning::Kernel::Third::system::posix {

	using $id_t = ::id_t;

	using $pid_t = ::pid_t;

	using $siginfo_t = ::siginfo_t;

	using $posix_spawn_file_actions_t = ::posix_spawn_file_actions_t;

	inline constexpr auto $getenv = ::getenv;

	inline constexpr auto $setenv = ::setenv;

	inline constexpr auto $unsetenv = ::unsetenv;

	inline constexpr auto $posix_spawn = ::posix_spawn;

	inline constexpr auto $posix_spawn_file_actions_init = ::posix_spawn_file_actions_init;

	inline constexpr auto $posix_spawn_file_actions_addopen = ::posix_spawn_file_actions_addopen;

	inline constexpr auto $posix_spawn_file_actions_destroy = ::posix_spawn_file_actions_destroy;

	inline constexpr auto $waitid = ::waitid;

	inline constexpr auto $O_RDONLY = O_RDONLY;

	inline constexpr auto $O_WRONLY = O_WRONLY;

	inline constexpr auto $STDIN_FILENO = STDIN_FILENO;

	inline constexpr auto $STDOUT_FILENO = STDOUT_FILENO;

	inline constexpr auto $STDERR_FILENO = STDERR_FILENO;

	inline constexpr auto $WSTOPPED = WSTOPPED;

	inline constexpr auto $WEXITED = WEXITED;

	inline constexpr auto $P_PID = P_PID;

	inline auto const & $environ = ::environ;

}

#endif
