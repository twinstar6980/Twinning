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

	inline constexpr auto k_letter_case_offset = Character{'a'_c - 'A'_c};

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

	#pragma region letter

	inline constexpr auto is_letter_lower (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'a'_c, 'z'_c);
	}

	inline constexpr auto is_letter_upper (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'A'_c, 'Z'_c);
	}

	inline constexpr auto is_letter (
		Character const & character
	) -> Boolean {
		return is_letter_lower(character) || is_letter_upper(character);
	}

	// ----------------

	inline constexpr auto is_letter_hexadecimal_lower (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'a'_c, 'f'_c);
	}

	inline constexpr auto is_letter_hexadecimal_upper (
		Character const & character
	) -> Boolean {
		return Math::between(character, 'A'_c, 'F'_c);
	}

	inline constexpr auto is_letter_hexadecimal (
		Character const & character
	) -> Boolean {
		return is_letter_hexadecimal_lower(character) || is_letter_hexadecimal_upper(character);
	}

	#pragma endregion

	#pragma region number

	inline constexpr auto is_number_binary (
		Character const & character
	) -> Boolean {
		return Math::between(character, '0'_c, '1'_c);
	}

	inline constexpr auto is_number_octal (
		Character const & character
	) -> Boolean {
		return Math::between(character, '0'_c, '7'_c);
	}

	inline constexpr auto is_number_decimal (
		Character const & character
	) -> Boolean {
		return Math::between(character, '0'_c, '9'_c);
	}

	inline constexpr auto is_number_hexadecimal_lower (
		Character const & character
	) -> Boolean {
		return is_number_decimal(character) || is_letter_hexadecimal_lower(character);
	}

	inline constexpr auto is_number_hexadecimal_upper (
		Character const & character
	) -> Boolean {
		return is_number_decimal(character) || is_letter_hexadecimal_upper(character);
	}

	inline constexpr auto is_number_hexadecimal (
		Character const & character
	) -> Boolean {
		return is_number_decimal(character) || is_letter_hexadecimal_lower(character) || is_letter_hexadecimal_upper(character);
	}

	#pragma endregion

	#pragma region letter convert

	inline constexpr auto as_letter_lower (
		Character & character
	) -> Void {
		if (is_letter_upper(character)) {
			character += k_letter_case_offset;
		}
		return;
	}

	inline constexpr auto as_letter_upper (
		Character & character
	) -> Void {
		if (is_letter_lower(character)) {
			character -= k_letter_case_offset;
		}
		return;
	}

	// ----------------

	inline constexpr auto to_letter_lower (
		Character const & character
	) -> Character {
		return is_letter_upper(character) ? (character + k_letter_case_offset) : (character);
	}

	inline constexpr auto to_letter_upper (
		Character const & character
	) -> Character {
		return is_letter_upper(character) ? (character - k_letter_case_offset) : (character);
	}

	#pragma endregion

	#pragma region number convert

	inline constexpr auto from_number_octal (
		Character const & character
	) -> IntegerU8 {
		assert_test(is_number_octal(character));
		return cbox<IntegerU8>(character - '0'_c);
	}

	inline constexpr auto to_number_octal (
		IntegerU8 const & number
	) -> Character {
		assert_test(number < 010_iu8);
		return '0'_c + cbox<Character>(number);
	}

	// ----------------

	inline constexpr auto from_number_hexadecimal (
		Character const & character
	) -> IntegerU8 {
		if (is_letter_hexadecimal_lower(character)) {
			return 0xa_iu8 + cbox<IntegerU8>(character - 'a'_c);
		}
		else if (is_letter_hexadecimal_upper(character)) {
			return 0xA_iu8 + cbox<IntegerU8>(character - 'A'_c);
		}
		else if (is_number_decimal(character)) {
			return 0x0_iu8 + cbox<IntegerU8>(character - '0'_c);
		}
		else {
			assert_fail(R"(/* number is valid */)");
		}
	}

	inline constexpr auto to_number_hexadecimal_lower (
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

	inline constexpr auto to_number_hexadecimal_upper (
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
