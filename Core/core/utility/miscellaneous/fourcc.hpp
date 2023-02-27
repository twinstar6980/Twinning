#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"
#include "core/utility/miscellaneous/byte_series/utility.hpp"

namespace TwinStar::Core {

	#pragma region type

	struct FourCC {

		Character8 first;
		Character8 second;
		Character8 third;
		Character8 fourth;

		// ----------------

		friend constexpr auto operator == (
			FourCC const & thix,
			FourCC const & that
		) -> bool = default;

	};

	#pragma endregion

	#pragma region utility

	inline constexpr auto fourcc_from_integer (
		IntegerU32 const & integer
	) -> FourCC {
		auto fourcc = FourCC{};
		fourcc.fourth = cbw<Character8>(clip_bit(integer, 1_ix, 8_sz));
		fourcc.third = cbw<Character8>(clip_bit(integer, 9_ix, 8_sz));
		fourcc.second = cbw<Character8>(clip_bit(integer, 17_ix, 8_sz));
		fourcc.first = cbw<Character8>(clip_bit(integer, 25_ix, 8_sz));
		return fourcc;
	}

	inline constexpr auto fourcc_to_integer (
		FourCC const & fourcc
	) -> IntegerU32 {
		auto integer = IntegerU32{};
		integer |= cbw<IntegerU32>(fourcc.fourth) << 0_sz;
		integer |= cbw<IntegerU32>(fourcc.third) << 8_sz;
		integer |= cbw<IntegerU32>(fourcc.second) << 16_sz;
		integer |= cbw<IntegerU32>(fourcc.first) << 24_sz;
		return integer;
	}

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_fourcc (
		ZLiteralString string,
		ZLiteralLength length
	) -> FourCC {
		assert_test(length == 4_szz);
		return FourCC{
			.first = mbw<Character8>(string[0]),
			.second = mbw<Character8>(string[1]),
			.third = mbw<Character8>(string[2]),
			.fourth = mbw<Character8>(string[3]),
		};
	}

	#pragma endregion

}
