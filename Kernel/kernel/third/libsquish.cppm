module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/libsquish/lib/include/squish/squish.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.libsquish;

export namespace Twinning::Kernel::Third::libsquish {

	using ::squish::u8;

	using ::squish::CompressImage;

	using ::squish::DecompressImage;

	inline auto const kColourClusterFit = ::squish::kColourClusterFit;

	inline auto const kDxt1 = ::squish::kDxt1;

	inline auto const kDxt3 = ::squish::kDxt3;

	inline auto const kDxt5 = ::squish::kDxt5;

}
