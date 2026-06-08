module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.model.four_character_code;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.memory.bitwise;

export namespace Twinning::Kernel {

	#pragma region type

	struct FourCharacterCode {

		Character8 first{};

		Character8 second{};

		Character8 third{};

		Character8 fourth{};

		// ----------------

		inline friend constexpr auto operator ==(
			FourCharacterCode const & thix,
			FourCharacterCode const & that
		) -> bool = default;

	};

	#pragma endregion

	#pragma region utility

	inline constexpr auto four_character_code_from_string(
		ConstantStringView const & string
	) -> FourCharacterCode {
		assert_test(string.size() == 4_sz);
		auto data = FourCharacterCode{};
		data.first = cast_box<Character8>(string[1_ix]);
		data.second = cast_box<Character8>(string[2_ix]);
		data.third = cast_box<Character8>(string[3_ix]);
		data.fourth = cast_box<Character8>(string[4_ix]);
		return data;
	}

	// ----------------

	inline constexpr auto four_character_code_from_integer(
		IntegerU32 const & integer
	) -> FourCharacterCode {
		auto data = FourCharacterCode{};
		data.fourth = cast_box<Character8>(Bitwise::extract(integer, 1_ix * 8_sz, 8_sz));
		data.third = cast_box<Character8>(Bitwise::extract(integer, 2_ix * 8_sz, 8_sz));
		data.second = cast_box<Character8>(Bitwise::extract(integer, 3_ix * 8_sz, 8_sz));
		data.first = cast_box<Character8>(Bitwise::extract(integer, 4_ix * 8_sz, 8_sz));
		return data;
	}

	inline constexpr auto four_character_code_to_integer(
		FourCharacterCode const & data
	) -> IntegerU32 {
		auto integer = IntegerU32{};
		integer |= Bitwise::infuse(cast_box<IntegerU32>(data.fourth), 1_ix * 8_sz, 8_sz);
		integer |= Bitwise::infuse(cast_box<IntegerU32>(data.third), 2_ix * 8_sz, 8_sz);
		integer |= Bitwise::infuse(cast_box<IntegerU32>(data.second), 3_ix * 8_sz, 8_sz);
		integer |= Bitwise::infuse(cast_box<IntegerU32>(data.first), 4_ix * 8_sz, 8_sz);
		return integer;
	}

	#pragma endregion

}
