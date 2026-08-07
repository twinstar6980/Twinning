module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.astc.common;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.encoding.common;

export namespace Twinning::Kernel::Tool::Texture::Compression::Astc {

	M_enumeration(
		M_wrap(Generation),
		M_wrap(
			v0,
		),
	);

	// ----------------

	struct Common {

		inline static constexpr auto k_block_bit_count = Size{128_sz};

		inline static constexpr auto is_valid_block_size(
			Image::ImageSize const & block_size
		) -> Boolean {
			return (block_size.width == 4_sz && block_size.height == 4_sz)
				|| (block_size.width == 5_sz && block_size.height == 4_sz)
				|| (block_size.width == 5_sz && block_size.height == 5_sz)
				|| (block_size.width == 6_sz && block_size.height == 5_sz)
				|| (block_size.width == 6_sz && block_size.height == 6_sz)
				|| (block_size.width == 8_sz && block_size.height == 5_sz)
				|| (block_size.width == 8_sz && block_size.height == 6_sz)
				|| (block_size.width == 8_sz && block_size.height == 8_sz)
				|| (block_size.width == 10_sz && block_size.height == 5_sz)
				|| (block_size.width == 10_sz && block_size.height == 6_sz)
				|| (block_size.width == 10_sz && block_size.height == 8_sz)
				|| (block_size.width == 10_sz && block_size.height == 10_sz)
				|| (block_size.width == 12_sz && block_size.height == 10_sz)
				|| (block_size.width == 12_sz && block_size.height == 12_sz);
		}

	};

}
