module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <cstddef>
#include "dependency/etcpak/ProcessRGB.hpp"
#include "dependency/etcpak/Decode.hpp"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.etcpak;

export namespace Twinning::Kernel::Dependency::etcpak {

	using ::CompressEtc1Rgb;

	using ::CompressEtc2Rgb;

	using ::CompressEtc2Rgba;

	using ::CompressEacR;

	using ::CompressEacRg;

	using ::DecodeRGB;

	using ::DecodeRGBA;

	using ::DecodeR;

	using ::DecodeRG;

}
