#pragma once

#if defined M_compiler_msvc
#pragma warning(push, 0)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "third/etcpak/ProcessRGB.hpp"

extern "C++" {
	extern void DecodeRGBPart( uint64_t d, uint32_t* dst, uint32_t w );
	extern void DecodeRGBAPart( uint64_t d, uint64_t alpha, uint32_t* dst, uint32_t w );
}

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Kernel::Third::etcpak {

	inline constexpr auto CompressEtc1Rgb = ::CompressEtc1Rgb;

	inline constexpr auto CompressEtc2Rgb = ::CompressEtc2Rgb;

	inline constexpr auto CompressEtc2Rgba = ::CompressEtc2Rgba;

	inline constexpr auto DecodeRGBPart = ::DecodeRGBPart;

	inline constexpr auto DecodeRGBAPart = ::DecodeRGBAPart;

}
