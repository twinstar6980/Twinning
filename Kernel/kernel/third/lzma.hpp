#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/lzma/LzmaLib.h"

#pragma clang diagnostic pop

namespace Twinning::Kernel::Third::lzma {

	using Byte = ::Byte;

	inline constexpr auto SZ_OK_ = SZ_OK;

	inline constexpr auto LzmaCompress = ::LzmaCompress;

	inline constexpr auto LzmaUncompress = ::LzmaUncompress;

	inline constexpr auto LZMA_PROPS_SIZE_ = LZMA_PROPS_SIZE;

}
