#pragma once

#if defined M_compiler_msvc
#pragma warning(push)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif

#include "third/bzip2/bzlib.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace TwinStar::Kernel::Third::bzip2 {

	using bz_stream = ::bz_stream;

	inline constexpr auto BZ2_bzCompressInit = ::BZ2_bzCompressInit;

	inline constexpr auto BZ2_bzCompress = ::BZ2_bzCompress;

	inline constexpr auto BZ2_bzCompressEnd = ::BZ2_bzCompressEnd;

	inline constexpr auto BZ2_bzDecompressInit = ::BZ2_bzDecompressInit;

	inline constexpr auto BZ2_bzDecompress = ::BZ2_bzDecompress;

	inline constexpr auto BZ2_bzDecompressEnd = ::BZ2_bzDecompressEnd;

	inline constexpr auto BZ_OK_ = BZ_OK;

	inline constexpr auto BZ_FINISH_ = BZ_FINISH;

	inline constexpr auto BZ_STREAM_END_ = BZ_STREAM_END;

}
