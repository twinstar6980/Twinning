#pragma once

#include "kernel/utility/utility.hpp"

namespace Twinning::Kernel::Tool::Texture::Compression::PVRTC4 {

	struct Common {

		inline static constexpr auto k_block_width = Size{4_sz};

		inline static constexpr auto k_bpp = Size{4_sz};

	};

}
