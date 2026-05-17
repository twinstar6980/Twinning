module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/etcpak/ProcessRGB.hpp"
#include "third/etcpak/Decode.hpp"

#pragma clang diagnostic pop

export module twinning.kernel.third.etcpak;

export namespace Twinning::Kernel::Third::etcpak {

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
