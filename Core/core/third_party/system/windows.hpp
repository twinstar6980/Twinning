#pragma once

#if defined M_system_windows

#if defined M_compiler_msvc
#pragma warning(push)
#pragma warning(disable:5039)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "Windows.h"
#include "ShlObj.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

#undef max
#undef min

namespace TwinKleS::Core::ThirdParty::windows {
}

#endif
