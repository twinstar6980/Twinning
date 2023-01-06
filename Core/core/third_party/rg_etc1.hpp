#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third_party/rg_etc1/rg_etc1.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Core::ThirdParty::rg_etc1 {

	using namespace ::rg_etc1;

}
