#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#endif

#include "third/etcpak/ProcessRGB.hpp"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Kernel::Third::etcpak {

	inline constexpr auto CompressEtc1Rgb = ::CompressEtc1Rgb;

	inline constexpr auto CompressEtc2Rgb = ::CompressEtc2Rgb;

	inline constexpr auto CompressEtc2Rgba = ::CompressEtc2Rgba;

}
