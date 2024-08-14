#pragma once

#if defined M_compiler_msvc
#pragma warning(push, 0)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "third/PVRTCCompressor/PvrTcDecoder.h"
#include "third/PVRTCCompressor/PvrTcEncoder.h"
#include "third/PVRTCCompressor/RgbBitmap.h"
#include "third/PVRTCCompressor/RgbaBitmap.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Kernel::Third::PVRTCCompressor {

	using namespace ::Javelin;

}
