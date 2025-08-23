module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.four_character_code;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.miscellaneous.byte_series.utility;

export namespace Twinning::Kernel {

	#pragma region type

	struct FourCharacterCode {

		Character8 first{};

		Character8 second{};

		Character8 third{};

		Character8 fourth{};

		// ----------------

		inline friend constexpr auto operator == (
			FourCharacterCode const & thix,
			FourCharacterCode const & that
		) -> bool = default;

	};

	#pragma endregion

	#pragma region utility

	inline constexpr auto four_character_code_from_string (
		ConstantStringView const & string
	) -> FourCharacterCode {
		assert_test(string.size() == 4_sz);
		auto data = FourCharacterCode{};
		data.first = cbox<Character8>(string[1_ix]);
		data.second = cbox<Character8>(string[2_ix]);
		data.third = cbox<Character8>(string[3_ix]);
		data.fourth = cbox<Character8>(string[4_ix]);
		return data;
	}

	// ----------------

	inline constexpr auto four_character_code_from_integer (
		IntegerU32 const & integer
	) -> FourCharacterCode {
		auto data = FourCharacterCode{};
		data.fourth = cbox<Character8>(clip_bit(integer, 1_ix, 8_sz));
		data.third = cbox<Character8>(clip_bit(integer, 9_ix, 8_sz));
		data.second = cbox<Character8>(clip_bit(integer, 17_ix, 8_sz));
		data.first = cbox<Character8>(clip_bit(integer, 25_ix, 8_sz));
		return data;
	}

	inline constexpr auto four_character_code_to_integer (
		FourCharacterCode const & data
	) -> IntegerU32 {
		auto integer = IntegerU32{};
		integer |= cbox<IntegerU32>(data.fourth) << 0_sz;
		integer |= cbox<IntegerU32>(data.third) << 8_sz;
		integer |= cbox<IntegerU32>(data.second) << 16_sz;
		integer |= cbox<IntegerU32>(data.first) << 24_sz;
		return integer;
	}

	#pragma endregion

}
