module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/brotli/origin/c/include/brotli/encode.h"
#include "dependency/brotli/origin/c/include/brotli/decode.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.brotli;

export namespace Twinning::Kernel::Dependency::brotli {

	using $byte = uint8_t;

	using $BrotliEncoderMode = BrotliEncoderMode;

	using $BrotliDecoderResult = BrotliDecoderResult;

	inline constexpr auto $BrotliEncoderMaxCompressedSize = BrotliEncoderMaxCompressedSize;

	inline constexpr auto $BrotliEncoderCompress = BrotliEncoderCompress;

	inline constexpr auto $BrotliDecoderDecompress = BrotliDecoderDecompress;

	inline constexpr auto $BROTLI_TRUE = BROTLI_TRUE;

	inline constexpr auto $BROTLI_DECODER_RESULT_SUCCESS = BROTLI_DECODER_RESULT_SUCCESS;

	inline constexpr auto $BROTLI_MIN_QUALITY = BROTLI_MIN_QUALITY;

	inline constexpr auto $BROTLI_MAX_QUALITY = BROTLI_MAX_QUALITY;

	inline constexpr auto $BROTLI_MIN_WINDOW_BITS = BROTLI_MIN_WINDOW_BITS;

	inline constexpr auto $BROTLI_MAX_WINDOW_BITS = BROTLI_MAX_WINDOW_BITS;

	inline constexpr auto $BROTLI_MODE_GENERIC = BROTLI_MODE_GENERIC;

	inline constexpr auto $BROTLI_MODE_TEXT = BROTLI_MODE_TEXT;

	inline constexpr auto $BROTLI_MODE_FONT = BROTLI_MODE_FONT;

}
