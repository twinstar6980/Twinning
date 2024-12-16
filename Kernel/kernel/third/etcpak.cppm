module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <stddef.h>
#include "third/etcpak/ProcessRGB.hpp"

// ReSharper disable once CppEnforceFunctionDeclarationStyle
extern "C++" void DecodeRGBPart (uint64_t d, uint32_t * dst, uint32_t w);

// ReSharper disable once CppEnforceFunctionDeclarationStyle
extern "C++" void DecodeRGBAPart (uint64_t d, uint64_t alpha, uint32_t * dst, uint32_t w);

#pragma clang diagnostic pop

export module twinning.kernel.third.etcpak;

export namespace Twinning::Kernel::Third::etcpak {

	using ::CompressEtc1Rgb;

	using ::CompressEtc2Rgb;

	using ::CompressEtc2Rgba;

	using ::DecodeRGBPart;

	using ::DecodeRGBAPart;

}
