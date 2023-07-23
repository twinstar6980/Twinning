#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#pragma warning(disable:4365)
#pragma warning(disable:4582)
#pragma warning(disable:4623)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:5026)
#pragma warning(disable:5027)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#pragma clang diagnostic ignored "-Wsigned-enum-bitfield"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wundefined-func-template"
#endif

#include "third/fmt/include/fmt/format.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Kernel::Third::fmt {

	using namespace ::fmt;

}
