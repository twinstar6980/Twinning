module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "unistd.h"
#include <dlfcn.h>

#pragma clang diagnostic pop

#endif

export module twinning.shell.dependency.system.posix;

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

export namespace Twinning::Shell::Dependency::system::posix {

	inline constexpr auto $read = read;

	inline constexpr auto $write = write;

	inline constexpr auto $isatty = isatty;

	inline constexpr auto $dlopen = dlopen;

	inline constexpr auto $dlclose = dlclose;

	inline constexpr auto $dlsym = dlsym;

	inline constexpr auto $STDIN_FILENO = STDIN_FILENO;

	inline constexpr auto $STDOUT_FILENO = STDOUT_FILENO;

	inline constexpr auto $RTLD_LAZY = RTLD_LAZY;

	inline constexpr auto $RTLD_LOCAL = RTLD_LOCAL;

}

#endif
