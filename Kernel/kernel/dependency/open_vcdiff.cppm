module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/open_vcdiff/google/vcencoder.h"
#include "dependency/open_vcdiff/google/vcdecoder.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.open_vcdiff;

export namespace Twinning::Kernel::Dependency::open_vcdiff {

	using ::open_vcdiff::HashedDictionary;

	using ::open_vcdiff::VCDiffFormatExtensionFlagValues;

	using ::open_vcdiff::VCDiffStreamingEncoder;

	using ::open_vcdiff::VCDiffStreamingDecoder;

}
