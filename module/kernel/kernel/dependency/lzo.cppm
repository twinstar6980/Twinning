module;

// ReSharper disable IdentifierTypo CppInconsistentNaming

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "dependency/lzo/origin/include/lzo/lzo1x.h"

#pragma clang diagnostic pop

export module twinning.kernel.dependency.lzo;

export namespace Twinning::Kernel::Dependency::lzo {

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wold-style-cast"

	using $lzo_byte = lzo_byte;

	using $lzo_uint = lzo_uint;

	inline constexpr auto $lzo1x_1_compress = lzo1x_1_compress;

	inline constexpr auto $lzo1x_decompress_safe = lzo1x_decompress_safe;

	inline constexpr auto $LZO1X_1_MEM_COMPRESS = LZO1X_1_MEM_COMPRESS;

	inline constexpr auto $LZO_E_OK = LZO_E_OK;

	inline auto $lzo_init(
	) -> int {
		return lzo_init();
	}

	#pragma clang diagnostic pop

}
