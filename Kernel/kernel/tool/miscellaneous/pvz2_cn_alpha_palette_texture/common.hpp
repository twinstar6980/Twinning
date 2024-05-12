#pragma once

#include "kernel/utility/utility.hpp"

namespace TwinStar::Kernel::Tool::Miscellaneous::PvZ2CNAlphaPaletteTexture {

	struct Common {

		inline static constexpr auto k_maximum_bit_count = Size{4_sz};

		// ----------------

		static auto test_palette (
			List<Image::Color> const & palette
		) -> Size {
			assert_test(0b1_sz < palette.size() && palette.size() <= 0b1_sz << k_maximum_bit_count);
			auto bit_count = k_none_size;
			for (auto & index : SizeRange{k_maximum_bit_count}) {
				++bit_count;
				if (palette.size() <= 0b1_sz << bit_count) {
					break;
				}
			}
			auto maximum_value = cbw<Image::Color>(0b1_iu8 << k_maximum_bit_count);
			assert_test(Range::all_of(palette, [&] (auto & element) -> auto { return element <= maximum_value; }));
			return bit_count;
		}

	};

}
