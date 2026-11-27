module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.math.number_variant;
import twinning.kernel.utility.math.utility;
import twinning.kernel.utility.memory.bitwise;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.binary.binary;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.integer_range;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.support.string_stream;
import twinning.kernel.utility.container.list;
import twinning.kernel.dependency.mscharconv;

export namespace Twinning::Kernel::StringUtility {

	#pragma region basic

	template <typename TResult, typename TSource, typename TSeparator> requires
		CategoryConstraint<IsPureInstance<TResult> && IsPureInstance<TSource> && IsPureInstance<TSeparator>>
		&& (IsTemplateInstanceOfTt<TResult, BasicString>)
		&& (IsConvertible<TSource, ConstantBasicStringView<typename TSource::Element> const &>)
		&& (IsRange<TSeparator>)
	inline auto split(
		TSource const &    source,
		TSeparator const & separator
	) -> List<TResult> {
		auto result = List<TResult>{};
		if (!source.empty()) {
			result.allocate_full(Range::count_many(source, separator) + 1_sz);
			auto sub_count = 0_sz;
			auto sub_begin = 0_sz;
			auto sub_end = 0_sz;
			auto get_next_sub_range = [&](
			) -> Void {
				result[sub_count] = source.sub(sub_begin, sub_end - sub_begin);
				sub_begin = sub_end + 1_sz;
				++sub_count;
				return;
			};
			for (auto & element : source) {
				if (Range::has(separator, element)) {
					get_next_sub_range();
				}
				++sub_end;
			}
			get_next_sub_range();
		}
		return result;
	}

	template <typename TResult, typename TSource> requires
		CategoryConstraint<IsPureInstance<TResult> && IsPureInstance<TSource>>
		&& (IsTemplateInstanceOfTt<TResult, BasicString>)
		&& (IsRange<TSource>)
	inline auto catenate(
		TSource const & source
	) -> TResult {
		auto size = 0_sz;
		for (auto & element : source) {
			size += element.size();
		}
		auto result = TResult{};
		result.allocate(size);
		for (auto & element : source) {
			result.append_range(element);
		}
		return result;
	}

	// ----------------

	inline auto pad(
		ConstantStringView const & view,
		Character const &          character,
		Size const &               size
	) -> String {
		auto result = String{};
		result.allocate(size);
		auto original_size = Math::minimum(view.size(), size);
		auto padding_size = size - original_size;
		for (auto & index : SizeRange{padding_size}) {
			result.append(character);
		}
		result.append_range(view.tail(original_size));
		return result;
	}

	#pragma endregion

	#pragma region character

	inline constexpr auto k_null_character = Character{'\0'_c};

	inline constexpr auto k_escape_character = Character{'\\'_c};

	inline constexpr auto k_path_dot_character = Character{'.'_c};

	inline constexpr auto k_path_directory_separator_posix_character = Character{'/'_c};

	inline constexpr auto k_path_directory_separator_windows_character = Character{'\\'_c};

	inline constexpr auto k_path_volume_separator_windows_character = Character{':'_c};

	#pragma endregion

	#pragma region basic

	inline constexpr auto is_ascii_character(
		Character const & value
	) -> Boolean {
		return (value & 0b10000000_c) == 0b00000000_c;
	}

	inline constexpr auto is_control_character(
		Character const & value
	) -> Boolean {
		return (0x00_c <= value && value <= 0x1F_c) || value == 0x7F_c;
	}

	inline constexpr auto is_number_character(
		Character const & value
	) -> Boolean {
		return Math::between(value, '0'_c, '9'_c);
	}

	inline constexpr auto is_letter_lower_character(
		Character const & value
	) -> Boolean {
		return Math::between(value, 'a'_c, 'z'_c);
	}

	inline constexpr auto is_letter_upper_character(
		Character const & value
	) -> Boolean {
		return Math::between(value, 'A'_c, 'Z'_c);
	}

	inline constexpr auto is_letter_character(
		Character const & value
	) -> Boolean {
		return is_letter_lower_character(value) || is_letter_upper_character(value);
	}

	// ----------------

	inline constexpr auto is_null_character(
		Character const & value
	) -> Boolean {
		return value == k_null_character;
	}

	inline constexpr auto is_escape_character(
		Character const & value
	) -> Boolean {
		return value == k_escape_character;
	}

	// ----------------

	inline constexpr auto is_path_dot_character(
		Character const & value
	) -> Boolean {
		return value == k_path_dot_character;
	}

	inline constexpr auto is_path_directory_separator_character(
		Character const & value
	) -> Boolean {
		return value == k_path_directory_separator_posix_character || value == k_path_directory_separator_windows_character;
	}

	inline constexpr auto is_path_volume_separator_character(
		Character const & value
	) -> Boolean {
		return value == k_path_volume_separator_windows_character;
	}

	#pragma endregion

	#pragma region letter case

	inline constexpr auto to_lower_case_character(
		Character const & value
	) -> Character {
		return !is_letter_upper_character(value) ? value : value + ('a'_c - 'A'_c);
	}

	inline constexpr auto to_upper_case_character(
		Character const & value
	) -> Character {
		return !is_letter_lower_character(value) ? value : value - ('a'_c - 'A'_c);
	}

	// ----------------

	inline constexpr auto as_lower_case(
		VariableStringView const & view
	) -> Void {
		for (auto & character : view) {
			character = to_lower_case_character(character);
		}
		return;
	}

	inline constexpr auto as_upper_case(
		VariableStringView const & view
	) -> Void {
		for (auto & character : view) {
			character = to_upper_case_character(character);
		}
		return;
	}

	#pragma endregion

	#pragma region number radix

	inline constexpr auto is_valid_number_radix(
		Size const & value
	) -> Boolean {
		return 2_sz <= value && value <= 36_sz;
	}

	// ----------------

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto is_number_radix_number_character(
		Character const & value
	) -> Boolean {
		auto first = '0'_c;
		return first <= value && value < first + cast_box<Character>(Math::minimum(10_sz, t_radix));
	}

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto is_number_radix_letter_sensitive_character(
		Character const & value,
		Boolean const &   use_upper
	) -> Boolean {
		auto first = !use_upper ? 'a'_c : 'A'_c;
		return t_radix > 10_sz && first <= value && value < first + cast_box<Character>(t_radix - 10_sz);
	}

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto is_number_radix_letter_character(
		Character const & value
	) -> Boolean {
		return is_number_radix_letter_sensitive_character<t_radix>(value, k_false) || is_number_radix_letter_sensitive_character<t_radix>(value, k_true);
	}

	// ----------------

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto is_number_radix_sensitive_character(
		Character const & value,
		Boolean const &   use_upper
	) -> Boolean {
		return is_number_radix_number_character<t_radix>(value) || is_number_radix_letter_sensitive_character<t_radix>(value, use_upper);
	}

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto is_number_radix_character(
		Character const & value
	) -> Boolean {
		return is_number_radix_number_character<t_radix>(value) || is_number_radix_letter_sensitive_character<t_radix>(value, k_false) || is_number_radix_letter_sensitive_character<t_radix>(value, k_true);
	}

	// ----------------

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto from_number_radix_character(
		Character const & value
	) -> IntegerU8 {
		auto number = IntegerU8{};
		if (is_number_radix_number_character<t_radix>(value)) {
			number = 0_iu8 + cast_box<IntegerU8>(value - '0'_c);
		}
		else if (is_number_radix_letter_sensitive_character<t_radix>(value, k_false)) {
			number = 10_iu8 + cast_box<IntegerU8>(value - 'a'_c);
		}
		else if (is_number_radix_letter_sensitive_character<t_radix>(value, k_true)) {
			number = 10_iu8 + cast_box<IntegerU8>(value - 'A'_c);
		}
		else {
			assert_fail(R"(/* number is valid */)");
		}
		return number;
	}

	template <auto t_radix> requires
		CategoryConstraint<>
		&& (IsSameOf<t_radix, Size>)
		&& (is_valid_number_radix(t_radix).value)
	inline constexpr auto to_number_radix_character(
		IntegerU8 const & number,
		Boolean const &   use_upper
	) -> Character {
		assert_test(number < cast_box<IntegerU8>(t_radix));
		auto value = Character{};
		if (number < 10_iu8) {
			value = '0'_c + cast_box<Character>(number - 0_iu8);
		}
		else {
			auto letter = !use_upper ? 'a'_c : 'A'_c;
			value = letter + cast_box<Character>(number - 10_iu8);
		}
		return value;
	}

	#pragma endregion

	#pragma region escape character

	inline auto write_escape_character(
		OutputStringStreamView & stream,
		Unicode const &          value
	) -> Void {
		if (value >= 0x100_u) {
			if (value >= 0x10000_u) {
				stream.write('U'_c);
				stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 29_ix, 4_sz)), k_true));
				stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 25_ix, 4_sz)), k_true));
				stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 21_ix, 4_sz)), k_true));
				stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 17_ix, 4_sz)), k_true));
			}
			else {
				stream.write('u'_c);
			}
			stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 13_ix, 4_sz)), k_true));
			stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 9_ix, 4_sz)), k_true));
			stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 5_ix, 4_sz)), k_true));
			stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 1_ix, 4_sz)), k_true));
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
					stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 5_ix, 4_sz)), k_true));
					stream.write(to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(value, 1_ix, 4_sz)), k_true));
					break;
				}
			}
		}
		return;
	}

	inline auto read_escape_character(
		InputStringStreamView & stream,
		Unicode &               value
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
					value = value << 3_sz | cast_box<Unicode>(from_number_radix_character<8_sz>(stream.read_of()));
				}
				break;
			}
			case 'x': {
				value = '\0'_u;
				for (auto & index : SizeRange{2_sz}) {
					value = value << 4_sz | cast_box<Unicode>(from_number_radix_character<16_sz>(stream.read_of()));
				}
				break;
			}
			case 'u': {
				value = '\0'_u;
				for (auto & index : SizeRange{4_sz}) {
					value = value << 4_sz | cast_box<Unicode>(from_number_radix_character<16_sz>(stream.read_of()));
				}
				break;
			}
			case 'U': {
				value = '\0'_u;
				for (auto & index : SizeRange{8_sz}) {
					value = value << 4_sz | cast_box<Unicode>(from_number_radix_character<16_sz>(stream.read_of()));
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

	#pragma region e-ascii character

	inline auto write_eascii_character(
		OutputStringStreamView & stream,
		Unicode const &          value
	) -> Void {
		assert_test(value < 0x100_u);
		stream.write(unsafe_cast<Character>(cast_box<Character8>(value)));
		return;
	}

	inline auto read_eascii_character(
		InputStringStreamView & stream,
		Unicode &               value
	) -> Void {
		value = cast_box<Unicode>(unsafe_cast<Character8>(stream.read_of()));
		return;
	}

	#pragma endregion

	#pragma region utf-8 character

	inline auto write_utf8_character(
		OutputStringStreamView & stream,
		Unicode const &          value
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
		InputStringStreamView & stream,
		Unicode &               value
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
		OutputStringStreamView &   stream,
		ConstantStringView const & value,
		Size &                     length
	) -> Void {
		length = value.size();
		for (auto & current : value) {
			stream.write(current);
		}
		return;
	}

	inline auto read_string(
		InputStringStreamView & stream,
		ConstantStringView &    value,
		Size const &            length
	) -> Void {
		value = stream.split_forward(length).view();
		return;
	}

	#pragma endregion

	#pragma region e-ascii string

	inline auto write_eascii_string(
		OutputStringStreamView & stream,
		String const &           value,
		Size &                   length
	) -> Void {
		auto value_stream = InputStringStreamView{value};
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
		InputStringStreamView & stream,
		String &                value,
		Size const &            length
	) -> Void {
		value.allocate_full(length * 2_sz);
		auto value_stream = stream.split_reserve();
		auto output_stream = OutputStringStreamView{value};
		for (auto & index : SizeRange{length}) {
			auto current = unsafe_cast<Character8>(value_stream.read_of());
			write_utf8_character(output_stream, cast_box<Unicode>(current));
		}
		value.resize(output_stream.position());
		stream.forward(value_stream.position());
		return;
	}

	#pragma endregion

	#pragma region utf-8 string

	inline auto write_utf8_string(
		OutputStringStreamView &   stream,
		ConstantStringView const & value,
		Size &                     length
	) -> Void {
		auto value_stream = InputStringStreamView{value};
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
		InputStringStreamView & stream,
		ConstantStringView &    value,
		Size const &            length
	) -> Void {
		auto value_stream = stream.split_reserve();
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
		value = value_stream.split_stream().view();
		stream.forward(value_stream.position());
		return;
	}

	inline auto read_utf8_string_by_size(
		InputStringStreamView & stream,
		ConstantStringView &    value,
		Size &                  length,
		Size const &            size
	) -> Void {
		auto value_stream = stream.split_next(size);
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
		value = value_stream.split_stream().view();
		stream.forward(value_stream.position());
		return;
	}

	inline auto compute_utf8_string_length(
		ConstantStringView const & value
	) -> Size {
		auto value_stream = InputStringStreamView{value};
		auto length = Size{};
		read_utf8_string_by_size(value_stream, as_left(ConstantStringView{}), length, value.size());
		return length;
	}

	#pragma endregion

	#pragma region string until

	inline auto write_string_until(
		OutputStringStreamView &   stream,
		ConstantStringView const & value,
		Character const &          end_marker
	) -> Void {
		for (auto & current : value) {
			stream.write(current);
		}
		return;
	}

	inline auto read_string_until(
		InputStringStreamView & stream,
		ConstantStringView &    value,
		Character const &       end_marker
	) -> Void {
		auto value_stream = stream.split_reserve();
		while (k_true) {
			auto current = value_stream.read_of();
			if (current == end_marker) {
				value_stream.backward();
				break;
			}
		}
		value = value_stream.split_stream().view();
		stream.forward(value_stream.position());
		return;
	}

	#pragma endregion

	#pragma region escape utf-8 string until

	inline auto write_escape_utf8_string_until(
		OutputStringStreamView & stream,
		InputStringStreamView &  value,
		Character const &        end_marker
	) -> Void {
		while (!value.full()) {
			auto current = value.read_of();
			if (is_control_character(current) || is_escape_character(current) || current == end_marker) {
				stream.write(k_escape_character);
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
		InputStringStreamView &  stream,
		OutputStringStreamView & value,
		Character const &        end_marker
	) -> Void {
		while (k_true) {
			auto current = stream.read_of();
			if (current == end_marker) {
				stream.backward();
				break;
			}
			if (is_escape_character(current)) {
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
		InputStringStreamView & stream
	) -> ConstantStringView {
		auto begin = stream.current();
		auto length = 0_sz;
		while (!stream.full()) {
			if (stream.read_of() == '\n'_c) {
				break;
			}
			++length;
		}
		return make_string_view(begin, length);
	}

	inline auto read_block_comment_after_mark(
		InputStringStreamView & stream
	) -> ConstantStringView {
		auto begin = stream.current();
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
		return make_string_view(begin, length);
	}

	inline auto read_comment_after_first_mark(
		InputStringStreamView & stream
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
				assert_fail(R"(stream.read_of() == /* valid */)");
				break;
			}
		}
		return value;
	}

	#pragma endregion

}
