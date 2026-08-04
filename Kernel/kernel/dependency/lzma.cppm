module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/lzma/LzmaLib.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.lzma;

export namespace Twinning::Kernel::Dependency::lzma {

	using $Byte = Byte;

	inline constexpr auto $LzmaCompress = LzmaCompress;

	inline constexpr auto $LzmaUncompress = LzmaUncompress;

	inline constexpr auto $SZ_OK = SZ_OK;

	inline constexpr auto $LZMA_PROPS_SIZE = LZMA_PROPS_SIZE;

}
