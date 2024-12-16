module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/open_vcdiff/google/vcencoder.h"
#include "third/open_vcdiff/google/vcdecoder.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.open_vcdiff;

export namespace Twinning::Kernel::Third::open_vcdiff {

	using ::open_vcdiff::HashedDictionary;

	using ::open_vcdiff::VCDiffFormatExtensionFlagValues;

	using ::open_vcdiff::VCDiffStreamingEncoder;

	using ::open_vcdiff::VCDiffStreamingDecoder;

}
