module;

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <dlfcn.h>

#pragma clang diagnostic pop

#endif

export module twinning.shell.third.system.posix;

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

export namespace Twinning::Shell::Third::system::posix {

	inline constexpr auto $dlopen = ::dlopen;

	inline constexpr auto $dlclose = ::dlclose;

	inline constexpr auto $dlsym = ::dlsym;

	inline constexpr auto $RTLD_LAZY = RTLD_LAZY;

	inline constexpr auto $RTLD_LOCAL = RTLD_LOCAL;

}

#endif
