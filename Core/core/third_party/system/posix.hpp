#pragma once

#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "unistd.h"
#include "fcntl.h"
#include "sys/wait.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Core::ThirdParty::posix {
}

#endif
