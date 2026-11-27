module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/zstd/origin/lib/zstd.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.zstd;

export namespace Twinning::Kernel::Dependency::zstd {

	inline constexpr auto $ZSTD_compress = ZSTD_compress;

	inline constexpr auto $ZSTD_decompress = ZSTD_decompress;

	inline constexpr auto $ZSTD_compressBound = ZSTD_compressBound;

	inline constexpr auto $ZSTD_isError = ZSTD_isError;

	inline constexpr auto $ZSTD_getFrameContentSize = ZSTD_getFrameContentSize;

	inline constexpr auto $ZSTD_minCLevel = ZSTD_minCLevel;

	inline constexpr auto $ZSTD_maxCLevel = ZSTD_maxCLevel;

	inline constexpr auto $ZSTD_CONTENTSIZE_UNKNOWN = ZSTD_CONTENTSIZE_UNKNOWN;

	inline constexpr auto $ZSTD_CONTENTSIZE_ERROR = ZSTD_CONTENTSIZE_ERROR;

}
