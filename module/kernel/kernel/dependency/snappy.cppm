module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/snappy/origin/snappy-c.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.snappy;

export namespace Twinning::Kernel::Dependency::snappy {

	inline constexpr auto $snappy_compress = snappy_compress;

	inline constexpr auto $snappy_uncompress = snappy_uncompress;

	inline constexpr auto $snappy_max_compressed_length = snappy_max_compressed_length;

	inline constexpr auto $snappy_uncompressed_length = snappy_uncompressed_length;

	inline constexpr auto $SNAPPY_OK = SNAPPY_OK;

}
