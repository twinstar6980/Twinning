#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third/open_vcdiff/google/vcencoder.h"
#include "third/open_vcdiff/google/vcdecoder.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Kernel::Third::open_vcdiff {

	using namespace ::open_vcdiff;

}
