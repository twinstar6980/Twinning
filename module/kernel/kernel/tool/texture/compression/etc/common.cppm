module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Texture::Compression::Etc {

	M_enumeration(
		M_wrap(Generation),
		M_wrap(
			v1,
			v2,
			eac,
		),
	);

	// ----------------

	struct Common {

		inline static constexpr auto k_block_bit_count = Size{64_sz};

		inline static constexpr auto is_valid_block_size(
			Image::ImageSize const & block_size
		) -> Boolean {
			return (block_size.width == 4_sz && block_size.height == 4_sz);
		}

	};

}
