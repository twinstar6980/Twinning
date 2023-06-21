#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Data::Encryption::Rijndael {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			ecb,
			cbc,
			cfb,
		),
	);

	// ----------------

	struct Common {

		inline static constexpr auto k_empty_iv = StaticArray<Character, 32_sz>{{
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
		}};

		// ----------------

		static constexpr auto is_valid_block_size (
			Size const & value
		) -> Boolean {
			return value == 16_sz || value == 24_sz || value == 32_sz;
		}

	};

}
