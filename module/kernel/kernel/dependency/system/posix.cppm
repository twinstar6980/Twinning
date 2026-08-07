module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/stat.h"
#include "sys/wait.h"

#if defined M_system_macintosh || defined M_system_iphone
extern "C" char * * environ; // NOTE: EXPLAIN: not declared in unistd.h of xcode sdk
#endif

#pragma clang diagnostic pop

#endif

export module twinning.kernel.dependency.system.posix;

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

export namespace Twinning::Kernel::Dependency::system::posix {

	using $off_t = off_t;

	using $stat = struct stat;

	using $id_t = id_t;

	using $pid_t = pid_t;

	using $siginfo_t = siginfo_t;

	inline constexpr auto $open = open;

	inline constexpr auto $close = close;

	inline constexpr auto $lseek = lseek;

	inline constexpr auto $fstat = fstat;

	inline constexpr auto $ftruncate = ftruncate;

	inline constexpr auto $read = read;

	inline constexpr auto $write = write;

	inline constexpr auto $dup2 = dup2;

	inline constexpr auto $chdir = chdir;

	inline constexpr auto $getcwd = getcwd;

	inline constexpr auto $fork = fork;

	inline constexpr auto $execve = execve;

	inline constexpr auto $waitid = waitid;

	inline constexpr auto $_exit = _exit;

	inline constexpr auto $O_RDONLY = O_RDONLY;

	inline constexpr auto $O_WRONLY = O_WRONLY;

	inline constexpr auto $O_RDWR = O_RDWR;

	inline constexpr auto $O_CREAT = O_CREAT;

	inline constexpr auto $O_EXCL = O_EXCL;

	inline constexpr auto $SEEK_SET = SEEK_SET;

	inline constexpr auto $STDIN_FILENO = STDIN_FILENO;

	inline constexpr auto $STDOUT_FILENO = STDOUT_FILENO;

	inline constexpr auto $STDERR_FILENO = STDERR_FILENO;

	inline constexpr auto $WSTOPPED = WSTOPPED;

	inline constexpr auto $WEXITED = WEXITED;

	inline constexpr auto $P_PID = P_PID;

	inline constexpr auto $ERANGE = ERANGE;

	inline auto const & $errno = errno;

	inline auto const & $environ = environ;

}

#endif
