#pragma once

#if defined M_compiler_msvc
#pragma warning(push, 0)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "third/open_vcdiff/google/vcencoder.h"
#include "third/open_vcdiff/google/vcdecoder.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Kernel::Third::open_vcdiff {

	using namespace ::open_vcdiff;

}
