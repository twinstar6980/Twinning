module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.parser;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.null;
import twinning.kernel.utility.miscellaneous.number_variant;
import twinning.kernel.utility.miscellaneous.character_series.stream;
import twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.byte_series.utility;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.range.range_wrapper;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.support.character_stream.basic;
import twinning.kernel.third.mscharconv;

export namespace Twinning::Kernel::StringParser {

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

	#pragma region escape character

	inline auto write_escape_character (
		OCharacterStreamView & stream,
		Unicode const &        character
	) -> Void {
		if (character >= 0x100_u) {
			if (character >= 0x10000_u) {
				stream.write('U'_c);
				stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 29_ix, 4_sz))));
				stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 25_ix, 4_sz))));
				stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 21_ix, 4_sz))));
				stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 17_ix, 4_sz))));
			}
			else {
				stream.write('u'_c);
			}
			stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 13_ix, 4_sz))));
			stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 9_ix, 4_sz))));
			stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 5_ix, 4_sz))));
			stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 1_ix, 4_sz))));
		}
		else {
			switch (character.value) {
				case '\\' : {
					stream.write('\\'_c);
					break;
				}
				case '\'' : {
					stream.write('\''_c);
					break;
				}
				case '\"' : {
					stream.write('\"'_c);
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
				case '\0' : {
					stream.write('0'_c);
					break;
				}
				default : {
					stream.write('x'_c);
					stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 5_ix, 4_sz))));
					stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(character, 1_ix, 4_sz))));
				}
			}
		}
		return;
	}

	inline auto read_escape_character (
		ICharacterStreamView & stream,
		Unicode &              character
	) -> Void {
		auto current = stream.read_of();
		switch (current.value) {
			case '\\' : {
				character = '\\'_u;
				break;
			}
			case '\'' : {
				character = '\''_u;
				break;
			}
			case '\"' : {
				character = '\"'_u;
				break;
			}
			case 'a' : {
				character = '\a'_u;
				break;
			}
			case 'b' : {
				character = '\b'_u;
				break;
			}
			case 'f' : {
				character = '\f'_u;
				break;
			}
			case 'n' : {
				character = '\n'_u;
				break;
			}
			case 'r' : {
				character = '\r'_u;
				break;
			}
			case 't' : {
				character = '\t'_u;
				break;
			}
			case 'v' : {
				character = '\v'_u;
				break;
			}
			case '0' : {
				character = '\0'_u;
				break;
			}
			case 'o' : {
				character = '\0'_u;
				for (auto & index : SizeRange{3_sz}) {
					character = character << 3_sz | cbox<Unicode>(CharacterType::from_number_oct(stream.read_of()));
				}
				break;
			}
			case 'x' : {
				character = '\0'_u;
				for (auto & index : SizeRange{2_sz}) {
					character = character << 4_sz | cbox<Unicode>(CharacterType::from_number_hex(stream.read_of()));
				}
				break;
			}
			case 'u' : {
				character = '\0'_u;
				for (auto & index : SizeRange{4_sz}) {
					character = character << 4_sz | cbox<Unicode>(CharacterType::from_number_hex(stream.read_of()));
				}
				break;
			}
			case 'U' : {
				character = '\0'_u;
				for (auto & index : SizeRange{8_sz}) {
					character = character << 4_sz | cbox<Unicode>(CharacterType::from_number_hex(stream.read_of()));
				}
				break;
			}
			default : {
				assert_fail(R"(current == /* valid */)");
			}
		}
		return;
	}

	#pragma endregion

	#pragma region e-ascii character

	inline auto write_eascii_character (
		OCharacterStreamView & stream,
		Unicode const &        character
	) -> Void {
		assert_test(character < 0x100_u);
		stream.write(self_cast<Character>(cbox<Character8>(character)));
		return;
	}

	inline auto read_eascii_character (
		ICharacterStreamView & stream,
		Unicode &              character
	) -> Void {
		character = cbox<Unicode>(self_cast<Character8>(stream.read_of()));
		return;
	}

	#pragma endregion

	#pragma region utf-8 character

	inline auto write_utf8_character (
		OCharacterStreamView & stream,
		Unicode const &        character
	) -> Void {
		auto extra_size = Size{};
		if (character < 0x80_u) {
			stream.write(self_cast<Character>(cbox<Character8>(character)));
			extra_size = 0_sz;
		}
		else if (character < 0x800_u) {
			stream.write(self_cast<Character>(0b110'00000_c8 | cbox<Character8>(clip_bit(character, 6_sz * 1_sz, 6_sz))));
			extra_size = 1_sz;
		}
		else if (character < 0x10000_u) {
			stream.write(self_cast<Character>(0b1110'0000_c8 | cbox<Character8>(clip_bit(character, 6_sz * 2_sz, 6_sz))));
			extra_size = 2_sz;
		}
		else if (character < 0x110000_u) {
			stream.write(self_cast<Character>(0b11110'000_c8 | cbox<Character8>(clip_bit(character, 6_sz * 3_sz, 6_sz))));
			extra_size = 3_sz;
		}
		else {
			assert_fail(R"(/* unicode character is valid */)");
		}
		while (extra_size > 0_sz) {
			--extra_size;
			stream.write(self_cast<Character>(0b10'000000_c8 | cbox<Character8>(clip_bit(character, 6_sz * extra_size, 6_sz))));
		}
		return;
	}

	inline auto read_utf8_character (
		ICharacterStreamView & stream,
		Unicode &              character
	) -> Void {
		auto current = self_cast<Character8>(stream.read_of());
		auto extra_size = Size{};
		if (current < 0b1'0000000_c8) {
			character = cbox<Unicode>(current);
			extra_size = 0_sz;
		}
		else if (current < 0b11'000000_c8) {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		else if (current < 0b111'00000_c8) {
			character = cbox<Unicode>(current & 0b000'11111_c8);
			extra_size = 1_sz;
		}
		else if (current < 0b1111'0000_c8) {
			character = cbox<Unicode>(current & 0b0000'1111_c8);
			extra_size = 2_sz;
		}
		else if (current < 0b11111'000_c8) {
			character = cbox<Unicode>(current & 0b00000'111_c8);
			extra_size = 3_sz;
		}
		else {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		while (extra_size > 0_sz) {
			--extra_size;
			current = self_cast<Character8>(stream.read_of());
			if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
				assert_fail(R"(/* extra utf-8 character is valid */)");
			}
			character = character << 6_sz | cbox<Unicode>(current & 0b00'111111_c8);
		}
		return;
	}

	inline auto compute_utf8_character_extra_size (
		Character8 const & character
	) -> Size {
		auto extra_size = Size{};
		if (character < 0b1'0000000_c8) {
			extra_size = 0_sz;
		}
		else if (character < 0b11'000000_c8) {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		else if (character < 0b111'00000_c8) {
			extra_size = 1_sz;
		}
		else if (character < 0b1111'0000_c8) {
			extra_size = 2_sz;
		}
		else if (character < 0b11111'000_c8) {
			extra_size = 3_sz;
		}
		else {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		return extra_size;
	}

	#pragma endregion

	#pragma region string

	inline auto write_string (
		OCharacterStreamView & stream,
		CStringView const &    string,
		Size &                 length
	) -> Void {
		length = string.size();
		stream.write(string);
		return;
	}

	inline auto read_string (
		ICharacterStreamView & stream,
		CStringView &          string,
		Size const &           length
	) -> Void {
		string = down_cast<CStringView>(stream.forward_view(length));
		return;
	}

	#pragma endregion

	#pragma region e-ascii string

	inline auto write_eascii_string (
		OCharacterStreamView & stream,
		String const &         string,
		Size &                 length
	) -> Void {
		auto string_stream = ICharacterStreamView{string};
		while (!string_stream.full()) {
			auto character = Unicode{};
			read_utf8_character(string_stream, character);
			assert_test(character < 0x100_u);
			stream.write(self_cast<Character>(cbox<Character8>(character)));
			++length;
		}
		return;
	}

	inline auto read_eascii_string (
		ICharacterStreamView & stream,
		String &               string,
		Size const &           length
	) -> Void {
		string.allocate_full(length * 2_sz);
		auto string_stream = ICharacterStreamView{stream.reserve_view()};
		auto output_stream = OCharacterStreamView{string.view()};
		for (auto & index : SizeRange{length}) {
			auto current = self_cast<Character8>(string_stream.read_of());
			write_utf8_character(output_stream, cbox<Unicode>(current));
		}
		string.set_size(output_stream.position());
		stream.forward(string_stream.position());
		return;
	}

	#pragma endregion

	#pragma region utf-8 string

	inline auto write_utf8_string (
		OCharacterStreamView & stream,
		CStringView const &    string,
		Size &                 length
	) -> Void {
		auto string_stream = ICharacterStreamView{string};
		while (!string_stream.full()) {
			auto current = self_cast<Character8>(string_stream.read_of());
			auto extra_size = compute_utf8_character_extra_size(current);
			stream.write(self_cast<Character>(current));
			while (extra_size > 0_sz) {
				--extra_size;
				current = self_cast<Character8>(string_stream.read_of());
				if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
					assert_fail(R"(/* extra utf-8 character is valid */)");
				}
				stream.write(self_cast<Character>(current));
			}
			++length;
		}
		return;
	}

	inline auto read_utf8_string (
		ICharacterStreamView & stream,
		CStringView &          string,
		Size const &           length
	) -> Void {
		auto string_stream = ICharacterStreamView{stream.reserve_view()};
		for (auto & index : SizeRange{length}) {
			auto current = self_cast<Character8>(string_stream.read_of());
			auto extra_size = compute_utf8_character_extra_size(current);
			while (extra_size > 0_sz) {
				--extra_size;
				current = self_cast<Character8>(string_stream.read_of());
				if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
					assert_fail(R"(/* extra utf-8 character is valid */)");
				}
			}
		}
		string = down_cast<CStringView>(string_stream.stream_view());
		stream.forward(string_stream.position());
		return;
	}

	inline auto read_utf8_string_by_size (
		ICharacterStreamView & stream,
		CStringView &          string,
		Size &                 length,
		Size const &           size
	) -> Void {
		auto string_stream = ICharacterStreamView{stream.next_view(size)};
		length = k_none_size;
		while (!string_stream.full()) {
			auto current = self_cast<Character8>(string_stream.read_of());
			auto extra_size = compute_utf8_character_extra_size(current);
			while (extra_size > 0_sz) {
				--extra_size;
				current = self_cast<Character8>(string_stream.read_of());
				if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
					assert_fail(R"(/* extra utf-8 character is valid */)");
				}
			}
			++length;
		}
		string = down_cast<CStringView>(string_stream.stream_view());
		stream.forward(string_stream.position());
		return;
	}

	inline auto compute_utf8_string_length (
		CStringView const & string
	) -> Size {
		auto string_stream = ICharacterStreamView{string};
		auto length = Size{};
		read_utf8_string_by_size(string_stream, as_lvalue(CStringView{}), length, string.size());
		return length;
	}

	#pragma endregion

	#pragma region string until

	inline auto write_string_until (
		OCharacterStreamView & stream,
		CStringView const &    string,
		Character const &      end_identifier
	) -> Void {
		for (auto & character : string) {
			stream.write(character);
		}
		return;
	}

	inline auto read_string_until (
		ICharacterStreamView & stream,
		CStringView &          string,
		Character const &      end_identifier
	) -> Void {
		auto string_stream = ICharacterStreamView{stream.reserve_view()};
		while (k_true) {
			auto character = string_stream.read_of();
			if (character == end_identifier) {
				string_stream.backward();
				break;
			}
		}
		string = down_cast<CStringView>(string_stream.stream_view());
		stream.forward(string_stream.position());
		return;
	}

	#pragma endregion

	#pragma region escape utf-8 string until

	inline auto write_escape_utf8_string_until (
		OCharacterStreamView & stream,
		ICharacterStreamView & string,
		Character const &      end_identifier
	) -> Void {
		while (!string.full()) {
			auto current = string.read_of();
			if (CharacterType::is_control(current) || current == CharacterType::k_escape_slash || current == end_identifier) {
				stream.write(CharacterType::k_escape_slash);
				write_escape_character(stream, cbox<Unicode>(current));
			}
			else {
				auto extra_size = compute_utf8_character_extra_size(self_cast<Character8>(current));
				stream.write(current);
				while (extra_size > 0_sz) {
					--extra_size;
					auto current_8 = self_cast<Character8>(string.read_of());
					if ((current_8 & 0b11'000000_c8) != 0b10'000000_c8) {
						assert_fail(R"(/* extra utf-8 character is valid */)");
					}
					stream.write(self_cast<Character>(current_8));
				}
			}
		}
		return;
	}

	inline auto read_escape_utf8_string_until (
		ICharacterStreamView & stream,
		OCharacterStreamView & string,
		Character const &      end_identifier
	) -> Void {
		while (k_true) {
			auto current = stream.read_of();
			if (current == end_identifier) {
				stream.backward();
				break;
			}
			if (current == CharacterType::k_escape_slash) {
				auto escape_character = Unicode{};
				read_escape_character(stream, escape_character);
				write_utf8_character(string, escape_character);
			}
			else {
				auto extra_size = compute_utf8_character_extra_size(self_cast<Character8>(current));
				string.write(current);
				while (extra_size > 0_sz) {
					--extra_size;
					auto current_8 = self_cast<Character8>(stream.read_of());
					if ((current_8 & 0b11'000000_c8) != 0b10'000000_c8) {
						assert_fail(R"(/* extra utf-8 character is valid */)");
					}
					string.write(self_cast<Character>(current_8));
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
				assert_fail(R"(stream.next() == /* valid */)");
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
		assert_test(stream.read_of<Character>() == 'n'_c);
		assert_test(stream.read_of<Character>() == 'u'_c);
		assert_test(stream.read_of<Character>() == 'l'_c);
		assert_test(stream.read_of<Character>() == 'l'_c);
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
		auto first = stream.read_of<Character>();
		if (first == 't'_c) {
			assert_test(stream.read_of<Character>() == 'r'_c);
			assert_test(stream.read_of<Character>() == 'u'_c);
			assert_test(stream.read_of<Character>() == 'e'_c);
			value = k_true;
		}
		else if (first == 'f'_c) {
			assert_test(stream.read_of<Character>() == 'a'_c);
			assert_test(stream.read_of<Character>() == 'l'_c);
			assert_test(stream.read_of<Character>() == 's'_c);
			assert_test(stream.read_of<Character>() == 'e'_c);
			value = k_false;
		}
		else {
			assert_fail(R"(first == /* valid */)");
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
		auto valid_begin = stream.reserve_view().begin();
		auto valid_end = stream.reserve_view().end();
		auto convert_result = Third::mscharconv::to_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.value, 10);
		assert_test(convert_result.ec == std::errc{});
		stream.forward(mbox<Size>(convert_result.ptr - cast_pointer<char>(valid_begin).value));
		return;
	}

	inline auto read_number (
		ICharacterStreamView & stream,
		Integer &              value
	) -> Void {
		auto valid_begin = stream.current_pointer();
		auto current = Character{};
		current = stream.read_of();
		if (current == '+'_c) {
			++valid_begin;
		}
		else if (current == '-'_c) {
		}
		else {
			assert_test(CharacterType::is_number_dec(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
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
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.value, 10);
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number (
		OCharacterStreamView & stream,
		Floater const &        value,
		Boolean const &        disable_sign_when_positive = k_false
	) -> Void {
		if (value > 0.0_f && !disable_sign_when_positive) {
			stream.write('+'_c);
		}
		auto valid_begin = stream.reserve_view().begin();
		auto valid_end = stream.reserve_view().end();
		auto convert_result = Third::mscharconv::to_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.value, Third::mscharconv::chars_format::fixed);
		assert_test(convert_result.ec == std::errc{});
		stream.forward(mbox<Size>(convert_result.ptr - cast_pointer<char>(valid_begin).value));
		if (!Range::has(Range::make_range(valid_begin, stream.current_pointer()), '.'_c)) {
			stream.write('.'_c);
			stream.write('0'_c);
		}
		return;
	}

	inline auto read_number (
		ICharacterStreamView & stream,
		Floater &              value
	) -> Void {
		auto is_floater = k_false;
		auto is_scientific = k_false;
		auto valid_begin = stream.current_pointer();
		auto current = Character{};
		current = stream.read_of();
		if (current == '+'_c) {
			++valid_begin;
		}
		else if (current == '-'_c) {
		}
		else {
			assert_test(CharacterType::is_number_dec(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
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
					assert_test(!is_floater);
					is_floater = k_true;
					current = stream.read_of();
					assert_test(CharacterType::is_number_dec(current));
					continue;
					break;
				}
				case 'e' : {
					assert_test(!is_scientific);
					assert_test(is_floater);
					is_scientific = k_true;
					current = stream.read_of();
					assert_test(current == '+'_c || current == '-'_c);
					current = stream.read_of();
					assert_test(CharacterType::is_number_dec(current));
					continue;
					break;
				}
				default : {
					stream.backward();
				}
			}
			break;
		}
		assert_test(is_floater);
		auto valid_end = stream.current_pointer();
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.value, !is_scientific ? (Third::mscharconv::chars_format::fixed) : (Third::mscharconv::chars_format::scientific));
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number (
		OCharacterStreamView & stream,
		NumberVariant const &  value,
		Boolean const &        disable_sign_when_positive = k_false
	) -> Void {
		switch (value.type().value) {
			case NumberVariantType::Constant::integer().value : {
				write_number(stream, value.get_integer(), disable_sign_when_positive);
				break;
			}
			case NumberVariantType::Constant::floater().value : {
				write_number(stream, value.get_floater(), disable_sign_when_positive);
				break;
			}
		}
		return;
	}

	inline auto read_number (
		ICharacterStreamView & stream,
		NumberVariant &        value
	) -> Void {
		auto is_floater = k_false;
		auto is_scientific = k_false;
		auto valid_begin = stream.current_pointer();
		auto current = Character{};
		current = stream.read_of();
		if (current == '+'_c) {
			++valid_begin;
		}
		else if (current == '-'_c) {
		}
		else {
			assert_test(CharacterType::is_number_dec(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
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
					assert_test(!is_floater);
					is_floater = k_true;
					current = stream.read_of();
					assert_test(CharacterType::is_number_dec(current));
					continue;
					break;
				}
				case 'e' : {
					assert_test(!is_scientific);
					assert_test(is_floater);
					is_scientific = k_true;
					current = stream.read_of();
					assert_test(current == '+'_c || current == '-'_c);
					current = stream.read_of();
					assert_test(CharacterType::is_number_dec(current));
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
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars_result{};
		if (!is_floater) {
			convert_result = Third::mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.set_integer().value, 10);
		}
		else {
			convert_result = Third::mscharconv::from_chars(cast_pointer<char>(valid_begin).value, cast_pointer<char>(valid_end).value, value.set_floater().value, !is_scientific ? (Third::mscharconv::chars_format::fixed) : (Third::mscharconv::chars_format::scientific));
		}
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	#pragma endregion

	#pragma region byte

	inline auto write_byte (
		OCharacterStreamView & stream,
		Byte const &           value
	) -> Void {
		stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(value, 5_ix, 4_sz))));
		stream.write(CharacterType::to_number_hex_upper(cbox<IntegerU8>(clip_bit(value, 1_ix, 4_sz))));
		return;
	}

	inline auto read_byte (
		ICharacterStreamView & stream,
		Byte &                 value
	) -> Void {
		value = 0x00_b;
		value = value << 4_sz | cbox<Byte>(CharacterType::from_number_hex(stream.read_of()));
		value = value << 4_sz | cbox<Byte>(CharacterType::from_number_hex(stream.read_of()));
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
				assert_test(stream.read_of() == ' '_c);
				read_byte(stream, element);
			}
		}
		return;
	}

	#pragma endregion

}
