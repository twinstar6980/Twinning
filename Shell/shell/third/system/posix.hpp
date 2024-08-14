#pragma once

#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone

#if defined M_compiler_msvc
#pragma warning(push, 0)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "dlfcn.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Shell::Third::posix {
}

#endif
