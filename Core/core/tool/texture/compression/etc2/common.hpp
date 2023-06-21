#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Texture::Compression::ETC2 {

	struct Common {

		inline static constexpr auto k_block_width = Size{4_sz};

		inline static constexpr auto k_bpp_a = Size{4_sz};

		inline static constexpr auto k_bpp_rgb = Size{4_sz};

		inline static constexpr auto k_bpp_rgba = Size{8_sz};

	};

}
