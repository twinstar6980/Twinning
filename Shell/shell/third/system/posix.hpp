#pragma once

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "dlfcn.h"

#if defined M_system_macintosh || defined M_system_iphone
extern char ** environ; // NOTE : not declared in unistd.h of xcode sdk
#endif

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Shell::Third::posix {
}

#endif
