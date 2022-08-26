#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"
#include "core/utility/string/string.hpp"

namespace TwinKleS::Core {

	#pragma region type

	// TODO : use character-array ?
	M_simple_derived_class(FourCC, IntegerWrapper<ZIntegerU32>, IntegerWrapper);

	#pragma endregion

	#pragma region utility

	inline constexpr auto make_fourcc (
		Character const & a,
		Character const & b,
		Character const & c,
		Character const & d
	) -> FourCC {
		if constexpr (std::endian::native == std::endian::little) {
			return cbw<FourCC>(a) << 0_sz |
				cbw<FourCC>(b) << 8_sz |
				cbw<FourCC>(c) << 16_sz |
				cbw<FourCC>(d) << 24_sz;
		} else {
			return cbw<FourCC>(a) << 24_sz |
				cbw<FourCC>(b) << 16_sz |
				cbw<FourCC>(c) << 8_sz |
				cbw<FourCC>(d) << 0_sz;
		}
	}

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_fourcc (
		ZLiteralString string,
		ZLiteralLength length
	) -> FourCC {
		assert_condition(mbw<Size>(length) == 4_sz);
		return make_fourcc(
			mbw<Character>(string[0]),
			mbw<Character>(string[1]),
			mbw<Character>(string[2]),
			mbw<Character>(string[3])
		);
	}

	#pragma endregion

}
