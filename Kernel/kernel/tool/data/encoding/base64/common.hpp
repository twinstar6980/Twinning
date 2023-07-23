#pragma once

#include "kernel/utility/utility.hpp"

namespace TwinStar::Kernel::Tool::Data::Encoding::Base64 {

	struct Common {

		inline static constexpr auto k_raw_block_size = Size{3_sz};

		inline static constexpr auto k_ripe_block_size = Size{4_sz};

		inline static constexpr auto k_padding_character = Character{'='_c};

	};

}
