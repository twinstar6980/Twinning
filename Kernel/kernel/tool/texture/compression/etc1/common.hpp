#pragma once

#include "kernel/utility/utility.hpp"

namespace TwinStar::Kernel::Tool::Texture::Compression::ETC1 {

	struct Common {

		inline static constexpr auto k_block_width = Size{4_sz};

		inline static constexpr auto k_bpp = Size{4_sz};

	};

}
