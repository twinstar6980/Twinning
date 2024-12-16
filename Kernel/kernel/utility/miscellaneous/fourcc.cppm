module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.fourcc;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.miscellaneous.byte_series.utility;

export namespace Twinning::Kernel {

	#pragma region type

	struct FourCC {

		Character8 first{};

		Character8 second{};

		Character8 third{};

		Character8 fourth{};

		// ----------------

		inline friend constexpr auto operator == (
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
		fourcc.fourth = cbox<Character8>(clip_bit(integer, 1_ix, 8_sz));
		fourcc.third = cbox<Character8>(clip_bit(integer, 9_ix, 8_sz));
		fourcc.second = cbox<Character8>(clip_bit(integer, 17_ix, 8_sz));
		fourcc.first = cbox<Character8>(clip_bit(integer, 25_ix, 8_sz));
		return fourcc;
	}

	inline constexpr auto fourcc_to_integer (
		FourCC const & fourcc
	) -> IntegerU32 {
		auto integer = IntegerU32{};
		integer |= cbox<IntegerU32>(fourcc.fourth) << 0_sz;
		integer |= cbox<IntegerU32>(fourcc.third) << 8_sz;
		integer |= cbox<IntegerU32>(fourcc.second) << 16_sz;
		integer |= cbox<IntegerU32>(fourcc.first) << 24_sz;
		return integer;
	}

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_fourcc (
		ZLiteralString string,
		ZLiteralSize   length
	) -> FourCC {
		assert_test(length == 4_szz);
		return FourCC{
			.first = mbox<Character8>(string[0]),
			.second = mbox<Character8>(string[1]),
			.third = mbox<Character8>(string[2]),
			.fourth = mbox<Character8>(string[3]),
		};
	}

	#pragma endregion

}
