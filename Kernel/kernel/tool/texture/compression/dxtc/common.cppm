module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.dxtc.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Texture::Compression::Dxtc {

	M_enumeration(
		M_wrap(Generation),
		M_wrap(
			v1,
			v3,
			v5,
		),
	);

	// ----------------

	struct Common {

		inline static constexpr auto k_block_bit_count_1 = Size{64_sz};

		inline static constexpr auto k_block_bit_count_3_5 = Size{128_sz};

		inline static constexpr auto is_valid_block_size(
			Image::ImageSize const & block_size
		) -> Boolean {
			return (block_size.width == 4_sz && block_size.height == 4_sz);
		}

	};

}
