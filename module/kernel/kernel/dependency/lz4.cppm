module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/lz4/origin/lib/lz4.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.lz4;

export namespace Twinning::Kernel::Dependency::lz4 {

	inline constexpr auto $LZ4_MAX_INPUT_SIZE = LZ4_MAX_INPUT_SIZE;

	inline constexpr auto $LZ4_compressBound = LZ4_compressBound;

	inline constexpr auto $LZ4_compress_default = LZ4_compress_default;

	inline constexpr auto $LZ4_decompress_safe = LZ4_decompress_safe;

}
