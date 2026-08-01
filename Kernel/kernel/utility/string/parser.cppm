module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.parser;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.math.number_variant;
import twinning.kernel.utility.memory.bitwise;
import twinning.kernel.utility.miscellaneous.character_series.stream;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.string.character_type;
import twinning.kernel.utility.support.character_stream;
import twinning.kernel.third.mscharconv;

export namespace Twinning::Kernel::StringParser {

	#pragma region character

	inline auto write_character_repeat(
		OutputCharacterStreamView & stream,
		Character const &           value,
		Size const &                count
	) -> Void {
		for (auto & index : SizeRange{count}) {
			stream.write(value);
		}
		return;
	}

	#pragma endregion

	#pragma region escape value

	inline auto write_escape_character(
		OutputCharacterStreamView & stream,
		Unicode const &             value
	) -> Void {
		if (value >= 0x100_u) {
			if (value >= 0x10000_u) {
				stream.write('U'_c);
				stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 29_ix, 4_sz))));
				stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 25_ix, 4_sz))));
				stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 21_ix, 4_sz))));
				stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 17_ix, 4_sz))));
			}
			else {
				stream.write('u'_c);
			}
			stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 13_ix, 4_sz))));
			stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 9_ix, 4_sz))));
			stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 5_ix, 4_sz))));
			stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 1_ix, 4_sz))));
		}
		else {
			switch (value.value) {
				case '\\': {
					stream.write('\\'_c);
					break;
				}
				case '\'': {
					stream.write('\''_c);
					break;
				}
				case '\"': {
					stream.write('\"'_c);
					break;
				}
				case '\a': {
					stream.write('a'_c);
					break;
				}
				case '\b': {
					stream.write('b'_c);
					break;
				}
				case '\f': {
					stream.write('f'_c);
					break;
				}
				case '\n': {
					stream.write('n'_c);
					break;
				}
				case '\r': {
					stream.write('r'_c);
					break;
				}
				case '\t': {
					stream.write('t'_c);
					break;
				}
				case '\v': {
					stream.write('v'_c);
					break;
				}
				case '\0': {
					stream.write('0'_c);
					break;
				}
				default: {
					stream.write('x'_c);
					stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 5_ix, 4_sz))));
					stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 1_ix, 4_sz))));
					break;
				}
			}
		}
		return;
	}

	inline auto read_escape_character(
		InputCharacterStreamView & stream,
		Unicode &                  value
	) -> Void {
		auto current = stream.read_of();
		switch (current.value) {
			case '\\': {
				value = '\\'_u;
				break;
			}
			case '\'': {
				value = '\''_u;
				break;
			}
			case '\"': {
				value = '\"'_u;
				break;
			}
			case 'a': {
				value = '\a'_u;
				break;
			}
			case 'b': {
				value = '\b'_u;
				break;
			}
			case 'f': {
				value = '\f'_u;
				break;
			}
			case 'n': {
				value = '\n'_u;
				break;
			}
			case 'r': {
				value = '\r'_u;
				break;
			}
			case 't': {
				value = '\t'_u;
				break;
			}
			case 'v': {
				value = '\v'_u;
				break;
			}
			case '0': {
				value = '\0'_u;
				break;
			}
			case 'o': {
				value = '\0'_u;
				for (auto & index : SizeRange{3_sz}) {
					value = value << 3_sz | cast_box<Unicode>(CharacterType::from_number_octal(stream.read_of()));
				}
				break;
			}
			case 'x': {
				value = '\0'_u;
				for (auto & index : SizeRange{2_sz}) {
					value = value << 4_sz | cast_box<Unicode>(CharacterType::from_number_hexadecimal(stream.read_of()));
				}
				break;
			}
			case 'u': {
				value = '\0'_u;
				for (auto & index : SizeRange{4_sz}) {
					value = value << 4_sz | cast_box<Unicode>(CharacterType::from_number_hexadecimal(stream.read_of()));
				}
				break;
			}
			case 'U': {
				value = '\0'_u;
				for (auto & index : SizeRange{8_sz}) {
					value = value << 4_sz | cast_box<Unicode>(CharacterType::from_number_hexadecimal(stream.read_of()));
				}
				break;
			}
			default: {
				assert_fail(R"(current == /* valid */)");
				break;
			}
		}
		return;
	}

	#pragma endregion

	#pragma region e-ascii value

	inline auto write_eascii_character(
		OutputCharacterStreamView & stream,
		Unicode const &             value
	) -> Void {
		assert_test(value < 0x100_u);
		stream.write(unsafe_cast<Character>(cast_box<Character8>(value)));
		return;
	}

	inline auto read_eascii_character(
		InputCharacterStreamView & stream,
		Unicode &                  value
	) -> Void {
		value = cast_box<Unicode>(unsafe_cast<Character8>(stream.read_of()));
		return;
	}

	#pragma endregion

	#pragma region utf-8 value

	inline auto write_utf8_character(
		OutputCharacterStreamView & stream,
		Unicode const &             value
	) -> Void {
		auto extra_size = Size{};
		if (value < 0x80_u) {
			stream.write(unsafe_cast<Character>(cast_box<Character8>(value)));
			extra_size = 0_sz;
		}
		else if (value < 0x800_u) {
			stream.write(unsafe_cast<Character>(0b110'00000_c8 | cast_box<Character8>(Bitwise::extract(value, 6_sz * 1_sz, 6_sz))));
			extra_size = 1_sz;
		}
		else if (value < 0x10000_u) {
			stream.write(unsafe_cast<Character>(0b1110'0000_c8 | cast_box<Character8>(Bitwise::extract(value, 6_sz * 2_sz, 6_sz))));
			extra_size = 2_sz;
		}
		else if (value < 0x110000_u) {
			stream.write(unsafe_cast<Character>(0b11110'000_c8 | cast_box<Character8>(Bitwise::extract(value, 6_sz * 3_sz, 6_sz))));
			extra_size = 3_sz;
		}
		else {
			assert_fail(R"(/* unicode character is valid */)");
		}
		while (extra_size > 0_sz) {
			--extra_size;
			stream.write(unsafe_cast<Character>(0b10'000000_c8 | cast_box<Character8>(Bitwise::extract(value, 6_sz * extra_size, 6_sz))));
		}
		return;
	}

	inline auto read_utf8_character(
		InputCharacterStreamView & stream,
		Unicode &                  value
	) -> Void {
		auto current = unsafe_cast<Character8>(stream.read_of());
		auto extra_size = Size{};
		if (current < 0b1'0000000_c8) {
			value = cast_box<Unicode>(current);
			extra_size = 0_sz;
		}
		else if (current < 0b11'000000_c8) {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		else if (current < 0b111'00000_c8) {
			value = cast_box<Unicode>(current & 0b000'11111_c8);
			extra_size = 1_sz;
		}
		else if (current < 0b1111'0000_c8) {
			value = cast_box<Unicode>(current & 0b0000'1111_c8);
			extra_size = 2_sz;
		}
		else if (current < 0b11111'000_c8) {
			value = cast_box<Unicode>(current & 0b00000'111_c8);
			extra_size = 3_sz;
		}
		else {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		while (extra_size > 0_sz) {
			--extra_size;
			current = unsafe_cast<Character8>(stream.read_of());
			if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
				assert_fail(R"(/* extra utf-8 character is valid */)");
			}
			value = value << 6_sz | cast_box<Unicode>(current & 0b00'111111_c8);
		}
		return;
	}

	inline auto compute_utf8_character_extra_size(
		Character8 const & value
	) -> Size {
		auto extra_size = Size{};
		if (value < 0b1'0000000_c8) {
			extra_size = 0_sz;
		}
		else if (value < 0b11'000000_c8) {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		else if (value < 0b111'00000_c8) {
			extra_size = 1_sz;
		}
		else if (value < 0b1111'0000_c8) {
			extra_size = 2_sz;
		}
		else if (value < 0b11111'000_c8) {
			extra_size = 3_sz;
		}
		else {
			assert_fail(R"(/* first utf-8 character is valid */)");
		}
		return extra_size;
	}

	#pragma endregion

	#pragma region string

	inline auto write_string(
		OutputCharacterStreamView & stream,
		ConstantStringView const &  value,
		Size &                      length
	) -> Void {
		length = value.size();
		stream.write(value);
		return;
	}

	inline auto read_string(
		InputCharacterStreamView & stream,
		ConstantStringView &       value,
		Size const &               length
	) -> Void {
		value = down_cast<ConstantStringView>(stream.forward_view(length));
		return;
	}

	#pragma endregion

	#pragma region e-ascii string

	inline auto write_eascii_string(
		OutputCharacterStreamView & stream,
		String const &              value,
		Size &                      length
	) -> Void {
		auto value_stream = InputCharacterStreamView{value};
		while (!value_stream.full()) {
			auto current = Unicode{};
			read_utf8_character(value_stream, current);
			assert_test(current < 0x100_u);
			stream.write(unsafe_cast<Character>(cast_box<Character8>(current)));
			++length;
		}
		return;
	}

	inline auto read_eascii_string(
		InputCharacterStreamView & stream,
		String &                   value,
		Size const &               length
	) -> Void {
		value.allocate_full(length * 2_sz);
		auto value_stream = InputCharacterStreamView{stream.reserve_view()};
		auto output_stream = OutputCharacterStreamView{value.view()};
		for (auto & index : SizeRange{length}) {
			auto current = unsafe_cast<Character8>(value_stream.read_of());
			write_utf8_character(output_stream, cast_box<Unicode>(current));
		}
		value.set_size(output_stream.position());
		stream.forward(value_stream.position());
		return;
	}

	#pragma endregion

	#pragma region utf-8 string

	inline auto write_utf8_string(
		OutputCharacterStreamView & stream,
		ConstantStringView const &  value,
		Size &                      length
	) -> Void {
		auto value_stream = InputCharacterStreamView{value};
		while (!value_stream.full()) {
			auto current = unsafe_cast<Character8>(value_stream.read_of());
			auto extra_size = compute_utf8_character_extra_size(current);
			stream.write(unsafe_cast<Character>(current));
			while (extra_size > 0_sz) {
				--extra_size;
				current = unsafe_cast<Character8>(value_stream.read_of());
				if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
					assert_fail(R"(/* extra utf-8 character is valid */)");
				}
				stream.write(unsafe_cast<Character>(current));
			}
			++length;
		}
		return;
	}

	inline auto read_utf8_string(
		InputCharacterStreamView & stream,
		ConstantStringView &       value,
		Size const &               length
	) -> Void {
		auto value_stream = InputCharacterStreamView{stream.reserve_view()};
		for (auto & index : SizeRange{length}) {
			auto current = unsafe_cast<Character8>(value_stream.read_of());
			auto extra_size = compute_utf8_character_extra_size(current);
			while (extra_size > 0_sz) {
				--extra_size;
				current = unsafe_cast<Character8>(value_stream.read_of());
				if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
					assert_fail(R"(/* extra utf-8 character is valid */)");
				}
			}
		}
		value = down_cast<ConstantStringView>(value_stream.stream_view());
		stream.forward(value_stream.position());
		return;
	}

	inline auto read_utf8_string_by_size(
		InputCharacterStreamView & stream,
		ConstantStringView &       value,
		Size &                     length,
		Size const &               size
	) -> Void {
		auto value_stream = InputCharacterStreamView{stream.next_view(size)};
		length = 0_sz;
		while (!value_stream.full()) {
			auto current = unsafe_cast<Character8>(value_stream.read_of());
			auto extra_size = compute_utf8_character_extra_size(current);
			while (extra_size > 0_sz) {
				--extra_size;
				current = unsafe_cast<Character8>(value_stream.read_of());
				if ((current & 0b11'000000_c8) != 0b10'000000_c8) {
					assert_fail(R"(/* extra utf-8 character is valid */)");
				}
			}
			++length;
		}
		value = down_cast<ConstantStringView>(value_stream.stream_view());
		stream.forward(value_stream.position());
		return;
	}

	inline auto compute_utf8_string_length(
		ConstantStringView const & value
	) -> Size {
		auto value_stream = InputCharacterStreamView{value};
		auto length = Size{};
		read_utf8_string_by_size(value_stream, as_left(ConstantStringView{}), length, value.size());
		return length;
	}

	#pragma endregion

	#pragma region string until

	inline auto write_string_until(
		OutputCharacterStreamView & stream,
		ConstantStringView const &  value,
		Character const &           end_marker
	) -> Void {
		for (auto & current : value) {
			stream.write(current);
		}
		return;
	}

	inline auto read_string_until(
		InputCharacterStreamView & stream,
		ConstantStringView &       value,
		Character const &          end_marker
	) -> Void {
		auto value_stream = InputCharacterStreamView{stream.reserve_view()};
		while (k_true) {
			auto current = value_stream.read_of();
			if (current == end_marker) {
				value_stream.backward();
				break;
			}
		}
		value = down_cast<ConstantStringView>(value_stream.stream_view());
		stream.forward(value_stream.position());
		return;
	}

	#pragma endregion

	#pragma region escape utf-8 string until

	inline auto write_escape_utf8_string_until(
		OutputCharacterStreamView & stream,
		InputCharacterStreamView &  value,
		Character const &           end_marker
	) -> Void {
		while (!value.full()) {
			auto current = value.read_of();
			if (CharacterType::is_control(current) || current == CharacterType::k_escape_slash || current == end_marker) {
				stream.write(CharacterType::k_escape_slash);
				write_escape_character(stream, cast_box<Unicode>(current));
			}
			else {
				auto extra_size = compute_utf8_character_extra_size(unsafe_cast<Character8>(current));
				stream.write(current);
				while (extra_size > 0_sz) {
					--extra_size;
					auto current_8 = unsafe_cast<Character8>(value.read_of());
					if ((current_8 & 0b11'000000_c8) != 0b10'000000_c8) {
						assert_fail(R"(/* extra utf-8 character is valid */)");
					}
					stream.write(unsafe_cast<Character>(current_8));
				}
			}
		}
		return;
	}

	inline auto read_escape_utf8_string_until(
		InputCharacterStreamView &  stream,
		OutputCharacterStreamView & value,
		Character const &           end_marker
	) -> Void {
		while (k_true) {
			auto current = stream.read_of();
			if (current == end_marker) {
				stream.backward();
				break;
			}
			if (current == CharacterType::k_escape_slash) {
				auto escape_character = Unicode{};
				read_escape_character(stream, escape_character);
				write_utf8_character(value, escape_character);
			}
			else {
				auto extra_size = compute_utf8_character_extra_size(unsafe_cast<Character8>(current));
				value.write(current);
				while (extra_size > 0_sz) {
					--extra_size;
					auto current_8 = unsafe_cast<Character8>(stream.read_of());
					if ((current_8 & 0b11'000000_c8) != 0b10'000000_c8) {
						assert_fail(R"(/* extra utf-8 character is valid */)");
					}
					value.write(unsafe_cast<Character>(current_8));
				}
			}
		}
		return;
	}

	#pragma endregion

	#pragma region comment

	inline auto read_line_comment_after_mark(
		InputCharacterStreamView & stream
	) -> ConstantStringView {
		auto begin = stream.current_pointer();
		auto length = 0_sz;
		while (!stream.full()) {
			if (stream.read_of() == '\n'_c) {
				break;
			}
			++length;
		}
		return ConstantStringView{begin, length};
	}

	inline auto read_block_comment_after_mark(
		InputCharacterStreamView & stream
	) -> ConstantStringView {
		auto begin = stream.current_pointer();
		auto length = 0_sz;
		while (!stream.full()) {
			if (stream.read_of() == '*'_c) {
				if (stream.read_of() == '/'_c) {
					break;
				}
				stream.backward();
			}
			++length;
		}
		return ConstantStringView{begin, length};
	}

	inline auto read_comment_after_first_mark(
		InputCharacterStreamView & stream
	) -> ConstantStringView {
		auto value = ConstantStringView{};
		switch (stream.read_of().value) {
			case '/': {
				value = read_line_comment_after_mark(stream);
				break;
			}
			case '*': {
				value = read_block_comment_after_mark(stream);
				break;
			}
			default: {
				assert_fail(R"(stream.next() == /* valid */)");
				break;
			}
		}
		return value;
	}

	#pragma endregion

	#pragma region null

	inline auto write_null(
		OutputCharacterStreamView & stream,
		Null const &                value
	) -> Void {
		stream.write("null"_sv);
		return;
	}

	inline auto read_null(
		InputCharacterStreamView & stream,
		Null &                     value
	) -> Void {
		assert_test(stream.read_of<Character>() == 'n'_c);
		assert_test(stream.read_of<Character>() == 'u'_c);
		assert_test(stream.read_of<Character>() == 'l'_c);
		assert_test(stream.read_of<Character>() == 'l'_c);
		return;
	}

	#pragma endregion

	#pragma region boolean

	inline auto write_boolean(
		OutputCharacterStreamView & stream,
		Boolean const &             value
	) -> Void {
		stream.write(value ? ("true"_sv) : ("false"_sv));
		return;
	}

	inline auto read_boolean(
		InputCharacterStreamView & stream,
		Boolean &                  value
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

	inline auto write_number(
		OutputCharacterStreamView & stream,
		Integer const &             value,
		Boolean const &             disable_sign_when_positive = k_false
	) -> Void {
		if (value > 0_i && !disable_sign_when_positive) {
			stream.write('+'_c);
		}
		auto valid_begin = stream.reserve_view().begin();
		auto valid_end = stream.reserve_view().end();
		auto convert_result = Third::mscharconv::to_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.value, 10);
		assert_test(convert_result.ec == std::errc{});
		stream.forward(make_box<Size>(convert_result.ptr - unmake_pointer_unsafe<char>(valid_begin)));
		return;
	}

	inline auto read_number(
		InputCharacterStreamView & stream,
		Integer &                  value
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
			assert_test(CharacterType::is_number_decimal(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': {
					continue;
					break;
				}
				default: {
					stream.backward();
					break;
				}
			}
			break;
		}
		auto valid_end = stream.current_pointer();
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.value, 10);
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number(
		OutputCharacterStreamView & stream,
		Floater const &             value,
		Boolean const &             disable_sign_when_positive = k_false
	) -> Void {
		if (value > 0.0_f && !disable_sign_when_positive) {
			stream.write('+'_c);
		}
		auto valid_begin = stream.reserve_view().begin();
		auto valid_end = stream.reserve_view().end();
		auto convert_result = Third::mscharconv::to_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.value, Third::mscharconv::chars_format::fixed);
		assert_test(convert_result.ec == std::errc{});
		stream.forward(make_box<Size>(convert_result.ptr - unmake_pointer_unsafe<char>(valid_begin)));
		if (!Range::has(Range::make_range(valid_begin, stream.current_pointer()), '.'_c)) {
			stream.write('.'_c);
			stream.write('0'_c);
		}
		return;
	}

	inline auto read_number(
		InputCharacterStreamView & stream,
		Floater &                  value
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
			assert_test(CharacterType::is_number_decimal(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': {
					continue;
					break;
				}
				case '.': {
					assert_test(!is_floater);
					is_floater = k_true;
					current = stream.read_of();
					assert_test(CharacterType::is_number_decimal(current));
					continue;
					break;
				}
				case 'e': {
					assert_test(!is_scientific);
					assert_test(is_floater);
					is_scientific = k_true;
					current = stream.read_of();
					assert_test(current == '+'_c || current == '-'_c);
					current = stream.read_of();
					assert_test(CharacterType::is_number_decimal(current));
					continue;
					break;
				}
				default: {
					stream.backward();
					break;
				}
			}
			break;
		}
		assert_test(is_floater);
		auto valid_end = stream.current_pointer();
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.value, !is_scientific ? (Third::mscharconv::chars_format::fixed) : (Third::mscharconv::chars_format::scientific));
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number(
		OutputCharacterStreamView & stream,
		NumberVariant const &       value,
		Boolean const &             disable_sign_when_positive = k_false
	) -> Void {
		switch (value.type().value) {
			case NumberVariantType::Constant::integer().value: {
				write_number(stream, value.get_integer(), disable_sign_when_positive);
				break;
			}
			case NumberVariantType::Constant::floater().value: {
				write_number(stream, value.get_floater(), disable_sign_when_positive);
				break;
			}
			default: throw UnreachableException{};
		}
		return;
	}

	inline auto read_number(
		InputCharacterStreamView & stream,
		NumberVariant &            value
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
			assert_test(CharacterType::is_number_decimal(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': {
					continue;
					break;
				}
				case '.': {
					assert_test(!is_floater);
					is_floater = k_true;
					current = stream.read_of();
					assert_test(CharacterType::is_number_decimal(current));
					continue;
					break;
				}
				case 'e': {
					assert_test(!is_scientific);
					assert_test(is_floater);
					is_scientific = k_true;
					current = stream.read_of();
					assert_test(current == '+'_c || current == '-'_c);
					current = stream.read_of();
					assert_test(CharacterType::is_number_decimal(current));
					continue;
					break;
				}
				default: {
					stream.backward();
					break;
				}
			}
			break;
		}
		auto valid_end = stream.current_pointer();
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars_result{};
		if (!is_floater) {
			convert_result = Third::mscharconv::from_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.set_integer().value, 10);
		}
		else {
			convert_result = Third::mscharconv::from_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.set_floater().value, !is_scientific ? (Third::mscharconv::chars_format::fixed) : (Third::mscharconv::chars_format::scientific));
		}
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	// ----------------

	inline auto write_number_hexadecimal(
		OutputCharacterStreamView & stream,
		Integer const &             value,
		Boolean const &             disable_sign_when_positive = k_false
	) -> Void {
		if (value > 0_i && !disable_sign_when_positive) {
			stream.write('+'_c);
		}
		auto valid_begin = stream.reserve_view().begin();
		auto valid_end = stream.reserve_view().end();
		auto convert_result = Third::mscharconv::to_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.value, 16);
		assert_test(convert_result.ec == std::errc{});
		stream.forward(make_box<Size>(convert_result.ptr - unmake_pointer_unsafe<char>(valid_begin)));
		return;
	}

	inline auto read_number_hexadecimal(
		InputCharacterStreamView & stream,
		Integer &                  value
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
			assert_test(CharacterType::is_number_hexadecimal(current));
		}
		while (!stream.full()) {
			current = stream.read_of();
			switch (current.value) {
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f': {
					continue;
					break;
				}
				default: {
					stream.backward();
					break;
				}
			}
			break;
		}
		auto valid_end = stream.current_pointer();
		assert_test(valid_begin != valid_end);
		auto convert_result = Third::mscharconv::from_chars(unmake_pointer_unsafe<char>(valid_begin), unmake_pointer_unsafe<char>(valid_end), value.value, 16);
		assert_test(convert_result.ec == std::errc{});
		return;
	}

	#pragma endregion

	#pragma region byte

	inline auto write_byte(
		OutputCharacterStreamView & stream,
		Byte const &                value
	) -> Void {
		stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 4_sz, 4_sz))));
		stream.write(CharacterType::to_number_hexadecimal_upper(cast_box<IntegerU8>(Bitwise::extract(value, 0_sz, 4_sz))));
		return;
	}

	inline auto read_byte(
		InputCharacterStreamView & stream,
		Byte &                     value
	) -> Void {
		value = 0x00_b;
		value |= Bitwise::infuse(cast_box<Byte>(CharacterType::from_number_hexadecimal(stream.read_of())), 4_sz, 4_sz);
		value |= Bitwise::infuse(cast_box<Byte>(CharacterType::from_number_hexadecimal(stream.read_of())), 0_sz, 4_sz);
		return;
	}

	#pragma endregion

	#pragma region byte list

	inline auto write_byte_list(
		OutputCharacterStreamView &  stream,
		ConstantByteListView const & value
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

	inline auto read_byte_list(
		InputCharacterStreamView &   stream,
		VariableByteListView const & value
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
