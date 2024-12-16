module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/bzip2/bzlib.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.bzip2;

export namespace Twinning::Kernel::Third::bzip2 {

	using $bz_stream = ::bz_stream;

	inline constexpr auto $BZ2_bzCompressInit = ::BZ2_bzCompressInit;

	inline constexpr auto $BZ2_bzCompress = ::BZ2_bzCompress;

	inline constexpr auto $BZ2_bzCompressEnd = ::BZ2_bzCompressEnd;

	inline constexpr auto $BZ2_bzDecompressInit = ::BZ2_bzDecompressInit;

	inline constexpr auto $BZ2_bzDecompress = ::BZ2_bzDecompress;

	inline constexpr auto $BZ2_bzDecompressEnd = ::BZ2_bzDecompressEnd;

	inline constexpr auto $BZ_OK = BZ_OK;

	inline constexpr auto $BZ_FINISH = BZ_FINISH;

	inline constexpr auto $BZ_STREAM_END = BZ_STREAM_END;

}
