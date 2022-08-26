#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#pragma warning(disable:4365)
#pragma warning(disable:5038)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder-ctor"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

#include "third_party/PVRTCCompressor/PvrTcDecoder.h"
#include "third_party/PVRTCCompressor/PvrTcEncoder.h"
#include "third_party/PVRTCCompressor/RgbBitmap.h"
#include "third_party/PVRTCCompressor/RgbaBitmap.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinKleS::Core::ThirdParty::PVRTCCompressor {

	using namespace ::Javelin;

}
