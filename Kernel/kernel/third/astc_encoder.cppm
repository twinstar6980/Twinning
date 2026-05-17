module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/astc_encoder/Source/astcenc.h"

#pragma clang diagnostic pop

export module twinning.kernel.third.astc_encoder;

export namespace Twinning::Kernel::Third::astc_encoder {

	using ::astcenc_error;

	using ::astcenc_profile;

	using ::astcenc_swz;

	using ::astcenc_swizzle;

	using ::astcenc_type;

	using ::astcenc_config;

	using ::astcenc_context;

	using ::astcenc_image;

	using ::astcenc_config_init;

	using ::astcenc_context_alloc;

	using ::astcenc_context_free;

	using ::astcenc_compress_image;

	using ::astcenc_decompress_image;

	inline auto const ASTCENC_PRE_FASTEST = ::ASTCENC_PRE_FASTEST;

	inline auto const ASTCENC_FLG_DECOMPRESS_ONLY = ::ASTCENC_FLG_DECOMPRESS_ONLY;

}
