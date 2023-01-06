#pragma once

#include "core/utility/miscellaneous/character_series/stream.hpp"
#include "core/utility/miscellaneous/character_series/type.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/base_wrapper/wrapper.hpp"
#include "core/utility/null.hpp"
#include "core/utility/miscellaneous/number_variant.hpp"
#include "core/utility/range/algorithm.hpp"

#if defined M_system_windows
#include <charconv>
namespace mscharconv = std;
#endif
#if defined M_system_linux || defined M_system_macos || defined M_system_android || defined M_system_ios
#include "core/third_party/mscharconv.hpp"
namespace mscharconv = TwinStar::Core::ThirdParty::mscharconv;
#endif

namespace TwinStar::Core::StringParser {

	#pragma region character

	inline auto write_character_repeat (
		OCharacterStreamView & stream,
		Character const &      character,
		Size const &           count
	) -> Void {
		for (auto & index : SizeRange{count}) {
			stream.write(character);
		}
		return;
	}

	#pragma endregion

	#pragma region utf-8 character

	inline auto write_utf8_character (
		OCharacterStreamView & stream,
		Character32 const &    character
	) -> Void {
		auto extra_size = Size{};
		if (character < 0x80_c32) {
			stream.write(cbw<Character>(character));
			extra_size = 0_sz;
		} else if (character < 0x800_c32) {
			stream.write(0b110'00000_c | cbw<Character>(clip_bit(character, 6_sz * 1_sz, 6_sz)));
			extra_size = 1_sz;
		} else if (character < 0x10000_c32) {
			stream.write(0b1110'0000_c | cbw<Character>(clip_bit(character, 6_sz * 2_sz, 6_sz)));
			extra_size = 2_sz;
		} else if (character < 0x110000_c32) {
			stream.write(0b11110'000_c | cbw<Character>(clip_bit(character, 6_sz * 3_sz, 6_sz)));
			extra_size = 3_sz;
		} else {
			assert_failed(R"(/* unicode character is valid */)");
		}
		while (extra_size != 0_sz) {
			--extra_size;
			stream.write(0b10'000000_c | cbw<Character>(clip_bit(character, 6_sz * extra_size, 6_sz)));
		}
		return;
	}

	// TODO : maybe bug if character is signed ?
	inline auto read_utf8_character (
		ICharacterStreamView & stream,
		Character32 &          character
	) -> Void {
		auto current = stream.read_of();
		auto extra_size = Size{};
		if (current < 0b10000000_c) {
			character = cbw<Character32>(current);
			extra_size = 0_sz;
		} else if (current < 0b11'000000_c) {
			assert_failed(R"(/* first utf-8 character is valid */)");
		} else if (current < 0b111'00000_c) {
			character = cbw<Character32>(current & 0b000'11111_c);
			extra_size = 1_sz;
		} else if (current < 0b1111'0000_c) {
			character = cbw<Character32>(current & 0b0000'1111_c);
			extra_size = 2_sz;
		} else if (current < 0b11111'000_c) {
			character = cbw<Character32>(current & 0b00000'111_c);
			extra_size = 3_sz;
		} else {
			assert_failed(R"(/* first utf-8 character is valid */)");
		}
		while (extra_size != 0_sz) {
			--extra_size;
			current = stream.read_of();
			if ((current & 0x10'000000_c) != 0x10'000000_c) {
				assert_failed(R"(/* extra utf-8 character is valid */)");
			}
			character = character << 6_sz | cbw<Character32>(current & 0b00'111111_c);
		}
		return;
	}

	inline auto compute_utf8_character_size_by_first_character (
		Character8 const & character
	) -> Size {
		auto size = Size{};
		if (character < 0b10000000_c8) {
			size = 1_sz;
		} else if (character < 0b11'000000_c8) {
			assert_failed(R"(/* first utf-8 character is valid */)");
		} else if (character < 0b111'00000_c8) {
			size = 2_sz;
		} else if (character < 0b1111'0000_c8) {
			size = 3_sz;
		} else if (character < 0b11111'000_c8) {
			size = 4_sz;
		} else {
			assert_failed(R"(/* first utf-8 character is valid */)");
		}
		return size;
	}

	#pragma endregion

	#pragma region escape character

	inline auto write_escape_character (
		OCharacterStreamView & stream,
		Character32 const &    character
	) -> Void {
		if (character >= 0x80_c32) {
			if (character >= 0x10000_c32) {
				stream.write('U'_c);
				stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 29_ix, 4_sz))));
				stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 25_ix, 4_sz))));
				stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 21_ix, 4_sz))));
				stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 17_ix, 4_sz))));
			} else {
				stream.write('u'_c);
			}
			stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 13_ix, 4_sz))));
			stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 9_ix, 4_sz))));
			stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 5_ix, 4_sz))));
			stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 1_ix, 4_sz))));
		} else {
			switch (character.value) {
				case '\'' :
				case '\"' :
				case '\?' :
				case '\\' : {
					stream.write(cbw<Character>(character));
					break;
				}
				case '\a' : {
					stream.write('a'_c);
					break;
				}
				case '\b' : {
					stream.write('b'_c);
					break;
				}
				case '\f' : {
					stream.write('f'_c);
					break;
				}
				case '\n' : {
					stream.write('n'_c);
					break;
				}
				case '\r' : {
					stream.write('r'_c);
					break;
				}
				case '\t' : {
					stream.write('t'_c);
					break;
				}
				case '\v' : {
					stream.write('v'_c);
					break;
				}
				default : {
					stream.write('x'_c);
					stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 5_ix, 4_sz))));
					stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(character, 1_ix, 4_sz))));
				}
			}
		}
		return;
	}

	inline auto read_escape_character (
		ICharacterStreamView & stream,
		Character32 &          character
	) -> Void {
		auto current = stream.read_of();
		switch (current.value) {
			case '\'' : {
				character = '\''_c32;
				break;
			}
			case '\"' : {
				character = '\"'_c32;
				break;
			}
			case '\?' : {
				character = '\?'_c32;
				break;
			}
			case '\\' : {
				character = '\\'_c32;
				break;
			}
			case 'a' : {
				character = '\a'_c32;
				break;
			}
			case 'b' : {
				character = '\b'_c32;
				break;
			}
			case 'f' : {
				character = '\f'_c32;
				break;
			}
			case 'n' : {
				character = '\n'_c32;
				break;
			}
			case 'r' : {
				character = '\r'_c32;
				break;
			}
			case 't' : {
				character = '\t'_c32;
				break;
			}
			case 'v' : {
				character = '\v'_c32;
				break;
			}
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' : {
				character = cbw<Character32>(CharacterType::from_number_oct(current));
				for (auto & index : SizeRange{2_sz}) {
					character = character << 3_sz | cbw<Character32>(CharacterType::from_number_oct(stream.read_of()));
				}
				break;
			}
			case 'x' : {
				character = '\0'_c32;
				for (auto & index : SizeRange{2_sz}) {
					character = character << 4_sz | cbw<Character32>(CharacterType::from_number_hex(stream.read_of()));
				}
				break;
			}
			case 'u' : {
				character = '\0'_c32;
				for (auto & index : SizeRange{4_sz}) {
					character = character << 4_sz | cbw<Character32>(CharacterType::from_number_hex(stream.read_of()));
				}
				break;
			}
			case 'U' : {
				character = '\0'_c32;
				for (auto & index : SizeRange{8_sz}) {
					character = character << 4_sz | cbw<Character32>(CharacterType::from_number_hex(stream.read_of()));
				}
				break;
			}
			default : {
				assert_failed(R"(current == /* valid */)");
			}
		}
		return;
	}

	#pragma endregion

	#pragma region string

	// TODO : end identifier behavior ? ignore or not ?
	inline auto write_string (
		OCharacterStreamView & stream,
		CStringView const &    string,
		Character const &      end_identifier = CharacterType::k_null
	) -> Void {
		for (auto & element : string) {
			stream.write(element);
		}
		return;
	}

	// TODO : end identifier behavior ? backward or not ?
	inline auto read_string (
		ICharacterStreamView & stream,
		CStringView &          string,
		Character const &      end_identifier = CharacterType::k_null
	) -> Void {
		auto begin = stream.current_pointer();
		auto size = k_none_size;
		while (!stream.full()) {
			if (stream.read_of() == end_identifier) {
				stream.backward();
				break;
			}
			++size;
		}
		string.set(begin, size);
		return;
	}

	#pragma endregion

	#pragma region escape string

	// TODO : end identifier behavior ? ignore or not ?
	inline auto write_escape_string (
		OCharacterStreamView & stream,
		ICharacterStreamView & string,
		Character const &      end_identifier
	) -> Void {
		while (!string.full()) {
			auto character = string.read_of();
			if (CharacterType::is_control(character) || character == CharacterType::k_escape_slash || character == end_identifier) {
				stream.write(CharacterType::k_escape_slash);
				write_escape_character(stream, cbw<Character32>(character));
			} else {
				auto character_size = compute_utf8_character_size_by_first_character(self_cast<Character8>(character));
				stream.write(character);
				if (character_size >= 2_sz) {
					stream.write(string.read_of());
				}
				if (character_size >= 3_sz) {
					stream.write(string.read_of());
				}
				if (character_size >= 4_sz) {
					stream.write(string.read_of());
				}
			}
		}
		return;
	}

	// TODO : end identifier behavior ? backward or not ?
	inline auto read_escape_string (
		ICharacterStreamView & stream,
		OCharacterStreamView & string,
		Character const &      end_identifier
	) -> Void {
		while (!stream.full()) {
			auto character = stream.read_of();
			if (character == end_identifier) {
				stream.backward();
				break;
			}
			if (character == CharacterType::k_escape_slash) {
				auto escape_character = Character32{};
				read_escape_character(stream, escape_character);
				write_utf8_character(string, escape_character);
			} else {
				auto character_size = compute_utf8_character_size_by_first_character(self_cast<Character8>(character));
				string.write(character);
				if (character_size >= 2_sz) {
					string.write(stream.read_of());
				}
				if (character_size >= 3_sz) {
					string.write(stream.read_of());
				}
				if (character_size >= 4_sz) {
					string.write(stream.read_of());
				}
			}
		}
		return;
	}

	#pragma endregion

	#pragma region comment

	inline auto read_line_comment_after_mark (
		ICharacterStreamView & stream
	) -> CStringView {
		auto begin = stream.current_pointer();
		auto length = k_none_size;
		while (!stream.full()) {
			if (stream.read_of() == '\n'_c) {
				break;
			}
			++length;
		}
		return CStringView{begin, length};
	}

	inline auto read_block_comment_after_mark (
		ICharacterStreamView & stream
	) -> CStringView {
		auto begin = stream.current_pointer();
		auto length = k_none_size;
		while (!stream.full()) {
			if (stream.read_of() == '*'_c) {
				if (stream.read_of() == '/'_c) {
					break;
				}
				stream.backward();
			}
			++length;
		}
		return CStringView{begin, length};
	}

	inline auto read_comment_after_first_mark (
		ICharacterStreamView & stream
	) -> CStringView {
		auto result = CStringView{};
		switch (stream.read_of().value) {
			case '/' : {
				result = read_line_comment_after_mark(stream);
				break;
			}
			case '*' : {
				result = read_block_comment_after_mark(stream);
				break;
			}
			default : {
				assert_failed(R"(stream.next() == /* valid */)");
			}
		}
		return result;
	}

	#pragma endregion

	#pragma region null

	inline auto write_null (
		OCharacterStreamView & stream,
		Null const &           value
	) -> Void {
		stream.write("null"_sv);
		return;
	}

	inline auto read_null (
		ICharacterStreamView & stream,
		Null &                 value
	) -> Void {
		assert_condition(stream.read_of<Character>() == 'n'_c);
		assert_condition(stream.read_of<Character>() == 'u'_c);
		assert_condition(stream.read_of<Character>() == 'l'_c);
		assert_condition(stream.read_of<Character>() == 'l'_c);
		return;
	}

	#pragma endregion

	#pragma region boolean

	inline auto write_boolean (
		OCharacterStreamView & stream,
		Boolean const &        value
	) -> Void {
		stream.write(value ? ("true"_sv) : ("false"_sv));
		return;
	}

	inline auto read_boolean (
		ICharacterStreamView & stream,
		Boolean &              value
	) -> Void {
		auto first_character = stream.read_of<Character>();
		if (first_character == 't'_c) {
			assert_condition(stream.read_of<Character>() == 'r'_c);
			assert_condition(stream.read_of<Character>() == 'u'_c);
			assert_condition(stream.read_of<Character>() == 'e'_c);
			value = k_true;
		} else if (first_character == 'f'_c) {
			assert_condition(stream.read_of<Character>() == 'a'_c);
			assert_condition(stream.read_of<Character>() == 'l'_c);
			assert_condition(stream.read_of<Character>() == 's'_c);
			assert_condition(stream.read_of<Character>() == 'e'_c);
			value = k_false;
		} else {
			assert_failed(R"(first_character == /* valid */)");
		}
		return;
	}

	#pragma endregion

	#pragma region number

	inline auto write_number (
		OCharacterStreamView & stream,
		Integer const &        value,
		Boolean const &        disable_sign_when_positive = k_false
	) -> Void {
		if (value > 0_i && !disable_sign_when_positive) {
			stream.write('+'_c);
		}
		auto format_result = ThirdParty::fmt::format_to_n(cast_pointer<char>(stream.current_pointer()).value, stream.reserve().value, "{:d}", value.value);
		stream.forward(mbw<Size>(format_result.size));
		return;
	}

	inline auto read_number (
		ICharacterStreamView & stream,
		Integer &              value
	) -> Void {
		auto valid_begin = stream.current_pointer();
		{
			auto first_character = stream.read_of();
			if (first_character == '+'_c) {
				++valid_begin;
			} else if (first_character == '-'_c) {
			} else {
				assert_condition(CharacterType::is_number_dec(first_character));
			}
		}
		while (!stream.full()) {
			auto current_character = stream.read_of();
			switch (current_character.value) {
				case '0' :
				case '1' :
				case '2' :
				case '3' :
				case '4' :
				case '5' :
				case '6' :
				case '7' :
				case '8' :
				case '9' : {
					continue;
					break;
				}
				default : {
					stream.backward();
				}
			}
			break;
		}
		auto valid_end = stream.current_pointer();
		assert_condition(valid_begin != valid_end);
		auto parse_result = mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.value, 10);
		assert_condition(parse_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number (
		OCharacterStreamView & stream,
		Floating const &       value,
		Boolean const &        disable_sign_when_positive = k_false
	) -> Void {
		if (value > 0.0_f && !disable_sign_when_positive) {
			stream.write('+'_c);
		}
		auto format_result = ThirdParty::fmt::format_to_n(cast_pointer<char>(stream.current_pointer()).value, stream.reserve().value, "{:#}", value == 0.0_f ? 0.0_f .value : value.value);
		stream.forward(mbw<Size>(format_result.size));
		return;
	}

	inline auto read_number (
		ICharacterStreamView & stream,
		Floating &             value
	) -> Void {
		auto is_floating = k_false;
		auto is_scientific = k_false;
		auto valid_begin = stream.current_pointer();
		{
			auto first_character = stream.read_of();
			if (first_character == '+'_c) {
				++valid_begin;
			} else if (first_character == '-'_c) {
			} else {
				assert_condition(CharacterType::is_number_dec(first_character));
			}
		}
		while (!stream.full()) {
			auto current_character = stream.read_of();
			switch (current_character.value) {
				case '0' :
				case '1' :
				case '2' :
				case '3' :
				case '4' :
				case '5' :
				case '6' :
				case '7' :
				case '8' :
				case '9' : {
					continue;
					break;
				}
				case '.' : {
					assert_condition(!is_floating);
					is_floating = k_true;
					auto next_character = stream.read_of();
					assert_condition(CharacterType::is_number_dec(next_character));
					continue;
					break;
				}
				case 'e' : {
					assert_condition(!is_scientific);
					is_scientific = k_true;
					auto next_character = stream.read_of();
					assert_condition(next_character == '+'_c || next_character == '-'_c);
					continue;
					break;
				}
				default : {
					stream.backward();
				}
			}
			break;
		}
		assert_condition(is_floating);
		auto valid_end = stream.current_pointer();
		assert_condition(valid_begin != valid_end);
		auto parse_result = mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.value, is_scientific ? mscharconv::chars_format::scientific : mscharconv::chars_format::fixed);
		assert_condition(parse_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number (
		OCharacterStreamView & stream,
		NumberVariant const &  value,
		Boolean const &        disable_sign_when_positive = k_false
	) -> Void {
		switch (value.type().value) {
			case NumberVariantType::Constant::floating().value : {
				write_number(stream, value.get_floating(), disable_sign_when_positive);
				break;
			}
			case NumberVariantType::Constant::integer().value : {
				write_number(stream, value.get_integer(), disable_sign_when_positive);
				break;
			}
		}
		return;
	}

	inline auto read_number (
		ICharacterStreamView & stream,
		NumberVariant &        value
	) -> Void {
		auto is_floating = k_false;
		auto is_scientific = k_false;
		auto valid_begin = stream.current_pointer();
		{
			auto first_character = stream.read_of();
			if (first_character == '+'_c) {
				++valid_begin;
			} else if (first_character == '-'_c) {
			} else {
				assert_condition(CharacterType::is_number_dec(first_character));
			}
		}
		while (!stream.full()) {
			auto current_character = stream.read_of();
			switch (current_character.value) {
				case '0' :
				case '1' :
				case '2' :
				case '3' :
				case '4' :
				case '5' :
				case '6' :
				case '7' :
				case '8' :
				case '9' : {
					continue;
					break;
				}
				case '.' : {
					assert_condition(!is_floating);
					is_floating = k_true;
					auto next_character = stream.read_of();
					assert_condition(CharacterType::is_number_dec(next_character));
					continue;
					break;
				}
				case 'e' : {
					assert_condition(!is_scientific);
					is_scientific = k_true;
					auto next_character = stream.read_of();
					assert_condition(next_character == '+'_c || next_character == '-'_c);
					continue;
					break;
				}
				default : {
					stream.backward();
				}
			}
			break;
		}
		auto valid_end = stream.current_pointer();
		assert_condition(valid_begin != valid_end);
		auto parse_result = mscharconv::from_chars_result{};
		if (!is_floating) {
			parse_result = mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.set_integer().value, 10);
		} else {
			parse_result = mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.set_floating().value, is_scientific ? mscharconv::chars_format::scientific : mscharconv::chars_format::fixed);
		}
		assert_condition(parse_result.ec == std::errc{});
		return;
	}

	#pragma endregion

	#pragma region byte

	inline auto write_byte (
		OCharacterStreamView & stream,
		Byte const &           value
	) -> Void {
		stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(value, 5_ix, 4_sz))));
		stream.write(CharacterType::to_number_hex_upper(cbw<IntegerU8>(clip_bit(value, 1_ix, 4_sz))));
		return;
	}

	inline auto read_byte (
		ICharacterStreamView & stream,
		Byte &                 value
	) -> Void {
		value = 0x00_b;
		value = value << 4_sz | cbw<Byte>(CharacterType::from_number_hex(stream.read_of()));
		value = value << 4_sz | cbw<Byte>(CharacterType::from_number_hex(stream.read_of()));
		return;
	}

	#pragma endregion

	#pragma region byte list

	inline auto write_byte_list (
		OCharacterStreamView & stream,
		CByteListView const &  value
	) -> Void {
		if (!value.empty()) {
			write_byte(stream, value.first());
			for (auto & element : value.tail(value.size() - 1_sz)) {
				stream.write(' '_c);
				write_byte(stream, element);
			}
		}
		return;
	}

	inline auto read_byte_list (
		ICharacterStreamView & stream,
		VByteListView const &  value
	) -> Void {
		if (!value.empty()) {
			read_byte(stream, value.first());
			for (auto & element : value.tail(value.size() - 1_sz)) {
				assert_condition(stream.read_of() == ' '_c);
				read_byte(stream, element);
			}
		}
		return;
	}

	#pragma endregion

}
