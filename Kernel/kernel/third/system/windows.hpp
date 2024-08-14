#pragma once

#if defined M_system_windows

#if defined M_compiler_msvc
#pragma warning(push, 0)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "Windows.h"
#include "ShlObj.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Kernel::Third::windows {
}

#endif
