#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/zlib/zlib.h"

#pragma clang diagnostic pop

namespace Twinning::Kernel::Third::zlib {

	using Bytef = ::Bytef;

	using z_stream = ::z_stream;

	inline constexpr auto deflateBound = ::deflateBound;

	inline constexpr auto deflateInit2_ = ::deflateInit2_;

	inline constexpr auto deflate = ::deflate;

	inline constexpr auto deflateEnd = ::deflateEnd;

	inline constexpr auto inflateInit2_ = ::inflateInit2_;

	inline constexpr auto inflate = ::inflate;

	inline constexpr auto inflateEnd = ::inflateEnd;

	inline constexpr auto Z_DEFLATED_ = Z_DEFLATED;

	inline constexpr auto Z_BEST_COMPRESSION_ = Z_BEST_COMPRESSION;

	inline constexpr auto MAX_WBITS_ = MAX_WBITS;

	inline constexpr auto MAX_MEM_LEVEL_ = MAX_MEM_LEVEL;

	inline constexpr auto Z_DEFAULT_STRATEGY_ = Z_DEFAULT_STRATEGY;

	inline constexpr auto Z_FILTERED_ = Z_FILTERED;

	inline constexpr auto Z_HUFFMAN_ONLY_ = Z_HUFFMAN_ONLY;

	inline constexpr auto Z_RLE_ = Z_RLE;

	inline constexpr auto Z_FIXED_ = Z_FIXED;

	inline constexpr auto Z_NO_FLUSH_ = Z_NO_FLUSH;

	inline constexpr auto Z_FINISH_ = Z_FINISH;

	inline constexpr auto Z_OK_ = Z_OK;

	inline constexpr auto Z_STREAM_END_ = Z_STREAM_END;

}
