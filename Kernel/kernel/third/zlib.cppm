module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/zlib/zlib.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.zlib;

export namespace Twinning::Kernel::Third::zlib {

	using $Bytef = ::Bytef;

	using $z_stream = ::z_stream;

	inline constexpr auto $deflateBound = ::deflateBound;

	inline constexpr auto $deflateInit2_ = ::deflateInit2_;

	inline constexpr auto $deflate = ::deflate;

	inline constexpr auto $deflateEnd = ::deflateEnd;

	inline constexpr auto $inflateInit2_ = ::inflateInit2_;

	inline constexpr auto $inflate = ::inflate;

	inline constexpr auto $inflateEnd = ::inflateEnd;

	inline constexpr auto $ZLIB_VERSION = ZLIB_VERSION;

	inline constexpr auto $MAX_MEM_LEVEL = MAX_MEM_LEVEL;

	inline constexpr auto $MAX_WBITS = MAX_WBITS;

	inline constexpr auto $Z_NO_FLUSH = Z_NO_FLUSH;

	inline constexpr auto $Z_FINISH = Z_FINISH;

	inline constexpr auto $Z_OK = Z_OK;

	inline constexpr auto $Z_STREAM_END = Z_STREAM_END;

	inline constexpr auto $Z_BEST_COMPRESSION = Z_BEST_COMPRESSION;

	inline constexpr auto $Z_FILTERED = Z_FILTERED;

	inline constexpr auto $Z_HUFFMAN_ONLY = Z_HUFFMAN_ONLY;

	inline constexpr auto $Z_RLE = Z_RLE;

	inline constexpr auto $Z_FIXED = Z_FIXED;

	inline constexpr auto $Z_DEFAULT_STRATEGY = Z_DEFAULT_STRATEGY;

	inline constexpr auto $Z_DEFLATED = Z_DEFLATED;

}
