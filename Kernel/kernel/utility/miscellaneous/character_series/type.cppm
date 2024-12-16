module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.static_array.static_array;
import twinning.kernel.utility.miscellaneous.math;

export namespace Twinning::Kernel::CharacterType {

	#pragma region constant

	inline constexpr auto k_null = Character{'\0'_c};

	inline constexpr auto k_escape_slash = Character{'\\'_c};

	inline constexpr auto k_path_separator_generic = Character{'/'_c};

	inline constexpr auto k_path_separator_windows = Character{'\\'_c};

	inline constexpr auto k_path_separator_set = StaticArray<Character, 2_sz>{{k_path_separator_generic, k_path_separator_windows}};

	inline constexpr auto k_case_diff = Character{'a'_c - 'A'_c};

	#pragma endregion

	#pragma region basic

	inline constexpr auto is_ascii (
		Character const & character
	) -> Boolean {
		return (character & 0b10000000_c) == 0xb00000000_c;
	}

	inline constexpr auto is_control (
		Character const & character
	) -> Boolean {
		return (0x00_c <= character && character <= 0x1F_c) || character == 0x7F_c;
	}

	#pragma endregion

	#pragma region special

	inline constexpr auto is_null (
		Character const & character
	) -> Boolean {
		return character == k_null;
	}

	inline constexpr auto is_escape_slash (
		Character const & character
	) -> Boolean {
		return character == k_escape_slash;
	}

	inline constexpr auto is_path_separator (
		Character const & character
	) -> Boolean {
		return character == k_path_separator_generic || character == k_path_separator_windows;
	}

	#pragma endregion

	#pragma region alpha

	inline constexpr auto is_alpha_lower (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'a'_c, 'z'_c);
	}

	inline constexpr auto is_alpha_upper (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'A'_c, 'Z'_c);
	}

	inline constexpr auto is_alpha (
		Character const & character
	) -> Boolean {
		return is_alpha_lower(character) || is_alpha_upper(character);
	}

	#pragma endregion

	#pragma region hex alpha

	inline constexpr auto is_alpha_hex_lower (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'a'_c, 'f'_c);
	}

	inline constexpr auto is_alpha_hex_upper (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'A'_c, 'F'_c);
	}

	inline constexpr auto is_alpha_hex (
		Character const & character
	) -> Boolean {
		return is_alpha_hex_lower(character) || is_alpha_hex_upper(character);
	}

	#pragma endregion

	#pragma region number

	inline constexpr auto is_number_bin (
		Character const & character
	) -> Boolean {
		return Math::between(character, '0'_c, '1'_c);
	}

	inline constexpr auto is_number_oct (
		Character const & character
	) -> Boolean {
		return Math::between(character, '0'_c, '7'_c);
	}

	inline constexpr auto is_number_dec (
		Character const & character
	) -> Boolean {
		return Math::between(character, '0'_c, '9'_c);
	}

	inline constexpr auto is_number_hex_lower (
		Character const & character
	) -> Boolean {
		return is_number_dec(character) || is_alpha_hex_lower(character);
	}

	inline constexpr auto is_number_hex_upper (
		Character const & character
	) -> Boolean {
		return is_number_dec(character) || is_alpha_hex_upper(character);
	}

	inline constexpr auto is_number_hex (
		Character const & character
	) -> Boolean {
		return is_number_dec(character) || is_alpha_hex_lower(character) || is_alpha_hex_upper(character);
	}

	#pragma endregion

	#pragma region alpha cast

	inline constexpr auto as_alpha_lower (
		Character & character
	) -> Void {
		if (is_alpha_upper(character)) {
			character += k_case_diff;
		}
		return;
	}

	inline constexpr auto as_alpha_upper (
		Character & character
	) -> Void {
		if (is_alpha_lower(character)) {
			character -= k_case_diff;
		}
		return;
	}

	// ----------------

	inline constexpr auto to_alpha_lower (
		Character const & character
	) -> Character {
		return is_alpha_upper(character) ? (character + k_case_diff) : (character);
	}

	inline constexpr auto to_alpha_upper (
		Character const & character
	) -> Character {
		return is_alpha_upper(character) ? (character - k_case_diff) : (character);
	}

	#pragma endregion

	#pragma region alpha comparison

	inline constexpr auto equal_icase (
		Character const & thix,
		Character const & that
	) -> Boolean {
		return thix == that || to_alpha_lower(thix) == to_alpha_lower(that);
	}

	#pragma endregion

	#pragma region oct number convert

	inline constexpr auto from_number_oct (
		Character const & character
	) -> IntegerU8 {
		assert_test(is_number_oct(character));
		return cbox<IntegerU8>(character - '0'_c);
	}

	inline constexpr auto to_number_oct (
		IntegerU8 const & number
	) -> Character {
		assert_test(number < 010_iu8);
		return '0'_c + cbox<Character>(number);
	}

	#pragma endregion

	#pragma region hex number convert

	inline constexpr auto from_number_hex (
		Character const & character
	) -> IntegerU8 {
		if (is_alpha_hex_lower(character)) {
			return 0xa_iu8 + cbox<IntegerU8>(character - 'a'_c);
		}
		else if (is_alpha_hex_upper(character)) {
			return 0xA_iu8 + cbox<IntegerU8>(character - 'A'_c);
		}
		else if (is_number_dec(character)) {
			return 0x0_iu8 + cbox<IntegerU8>(character - '0'_c);
		}
		else {
			assert_fail(R"(/* number is valid */)");
		}
	}

	inline constexpr auto to_number_hex_lower (
		IntegerU8 const & number
	) -> Character {
		assert_test(number < 0x10_iu8);
		if (number >= 0xa_iu8) {
			return 'a'_c + cbox<Character>(number - 0xa_iu8);
		}
		else {
			return '0'_c + cbox<Character>(number);
		}
	}

	inline constexpr auto to_number_hex_upper (
		IntegerU8 const & number
	) -> Character {
		assert_test(number < 0x10_iu8);
		if (number >= 0xA_iu8) {
			return 'A'_c + cbox<Character>(number - 0xA_iu8);
		}
		else {
			return '0'_c + cbox<Character>(number);
		}
	}

	#pragma endregion

}
