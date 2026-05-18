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

	inline constexpr auto kColourClusterFit = ::squish::kColourClusterFit;

	inline constexpr auto kDxt1 = ::squish::kDxt1;

	inline constexpr auto kDxt3 = ::squish::kDxt3;

	inline constexpr auto kDxt5 = ::squish::kDxt5;

}
