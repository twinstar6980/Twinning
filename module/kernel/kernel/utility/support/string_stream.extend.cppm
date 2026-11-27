module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.string_stream.extend;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.string.basic_string_stream_adapter;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.string.utility;
import twinning.kernel.utility.storage.path;
import twinning.kernel.utility.math.number_variant;
import twinning.kernel.utility.math.utility;
import twinning.kernel.utility.binary.binary;
import twinning.kernel.utility.memory.bitwise;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.integer_range;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.model.four_character_code;
import twinning.kernel.dependency.mscharconv;

export namespace Twinning::Kernel {

	#pragma region string

	template <auto t_constant> requires
		AutomaticConstraint
	struct BasicStringStreamAdapter<Character, StringView<t_constant>> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = StringView<t_constant>;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return that.size();
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

	};

	template <>
	struct BasicStringStreamAdapter<Character, String> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = String;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return that.size();
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			that.allocate_full(thix.reserve());
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

	};

	#pragma endregion

	#pragma region unicode

	template <>
	struct BasicStringStreamAdapter<Character, Unicode> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Unicode;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return 6_sz;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			StringUtility::write_utf8_character(thix, that);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			StringUtility::read_utf8_character(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region null

	template <>
	struct BasicStringStreamAdapter<Character, Null> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Null;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return "null"_sl;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write("null"_sv);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			assert_test(thix.read_of<Character>() == 'n'_c);
			assert_test(thix.read_of<Character>() == 'u'_c);
			assert_test(thix.read_of<Character>() == 'l'_c);
			assert_test(thix.read_of<Character>() == 'l'_c);
			return;
		}

	};

	#pragma endregion

	#pragma region boolean

	template <>
	struct BasicStringStreamAdapter<Character, Boolean> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Boolean;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return !that ? "false"_sl : "true"_sl;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(!that ? "false"_sv : "true"_sv);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			auto current = thix.read_of<Character>();
			if (current == 'f'_c) {
				assert_test(thix.read_of<Character>() == 'a'_c);
				assert_test(thix.read_of<Character>() == 'l'_c);
				assert_test(thix.read_of<Character>() == 's'_c);
				assert_test(thix.read_of<Character>() == 'e'_c);
				that = k_false;
			}
			else if (current == 't'_c) {
				assert_test(thix.read_of<Character>() == 'r'_c);
				assert_test(thix.read_of<Character>() == 'u'_c);
				assert_test(thix.read_of<Character>() == 'e'_c);
				that = k_true;
			}
			else {
				assert_fail(R"(current == /* valid */)");
			}
			return;
		}

	};

	#pragma endregion

	#pragma region number

	template <>
	struct BasicStringStreamAdapter<Character, Integer> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Integer;

		// ----------------

		inline static auto capacity(
			That const &    that,
			Size const &    radix,
			Boolean const & use_upper,
			Boolean const & disable_sign_when_positive
		) -> Size {
			auto value = that;
			auto size = 0_sz;
			while (value != 0_i) {
				value /= cast_box<Integer>(radix);
				++size;
			}
			return 1_sz + Math::maximum(size, 1_sz);
		}

		inline static auto write(
			ThisOutput &    thix,
			That const &    that,
			Size const &    radix,
			Boolean const & use_upper,
			Boolean const & disable_sign_when_positive
		) -> Void {
			if (that > 0_i && !disable_sign_when_positive) {
				thix.write('+'_c);
			}
			auto valid_begin = thix.split_reserve().view().begin();
			auto valid_end = thix.split_reserve().view().end();
			auto convert_result = Dependency::mscharconv::to_chars(
				unmake_pointer_unsafe<char>(valid_begin),
				unmake_pointer_unsafe<char>(valid_end),
				that.value,
				unmake_box<int>(radix)
			);
			assert_test(convert_result.ec == std::errc{});
			auto view = thix.split_forward(make_box<Size>(convert_result.ptr - unmake_pointer_unsafe<char>(valid_begin))).view();
			if (use_upper) {
				StringUtility::as_upper_case(view);
			}
			return;
		}

		inline static auto read(
			ThisInput &  thix,
			That &       that,
			Size const & radix
		) -> Void {
			do {
				auto current = Character{};
				auto valid_begin = thix.current();
				auto is_negative = k_false;
				current = thix.read_of();
				if (current == '+'_c) {
					++valid_begin;
				}
				else if (current == '-'_c) {
					is_negative = k_true;
				}
				else {
					thix.backward();
				}
				assert_test(StringUtility::is_number_radix_character<36_sz>(thix.read_of()));
				while (!thix.full()) {
					current = thix.read_of();
					// TODO: use actual radix
					if (StringUtility::is_number_radix_character<36_sz>(current)) {
						continue;
					}
					thix.backward();
					break;
				}
				auto valid_end = thix.current();
				auto convert_result = Dependency::mscharconv::from_chars(
					unmake_pointer_unsafe<char>(valid_begin),
					unmake_pointer_unsafe<char>(valid_end),
					that.value,
					unmake_box<int>(radix)
				);
				assert_test(convert_result.ec == std::errc{});
			}
			while (k_false);
			return;
		}

		// ----------------

		inline static auto capacity(
			That const &    that,
			Boolean const & disable_sign_when_positive
		) -> Size {
			return BasicStringStreamAdapter<Character, Integer>::capacity(that, 10_sz, k_false, disable_sign_when_positive);
		}

		inline static auto write(
			ThisOutput &    thix,
			That const &    that,
			Boolean const & disable_sign_when_positive
		) -> Void {
			thix.write(that, 10_sz, k_false, disable_sign_when_positive);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			thix.read(that, 10_sz);
			return;
		}

	};

	template <>
	struct BasicStringStreamAdapter<Character, Floater> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Floater;

		// ----------------

		inline static auto capacity(
			That const &    that,
			Boolean const & disable_sign_when_positive
		) -> Size {
			auto result = 0_sz;
			if (Math::is_invalid(that)) {
				result += 3_sz;
			}
			else if (Math::is_infinity(that)) {
				result += 4_sz;
			}
			else {
				auto significand = That{};
				auto exponent = Integer{};
				Math::extract(that, significand, exponent);
				result += cast_box<Size>(exponent < 0_i ? -exponent : exponent) * 30103_sz / 100000_sz + 2_sz;
				result += 19_sz;
			}
			return result;
		}

		inline static auto write(
			ThisOutput &    thix,
			That const &    that,
			Boolean const & disable_sign_when_positive
		) -> Void {
			do {
				if (Math::is_invalid(that)) {
					thix.write('N'_c);
					thix.write('a'_c);
					thix.write('N'_c);
					break;
				}
				if (Math::is_infinity(that)) {
					if (that < 0.0_f) {
						thix.write('-'_c);
					}
					if (that > 0.0_f && !disable_sign_when_positive) {
						thix.write('+'_c);
					}
					thix.write('I'_c);
					thix.write('n'_c);
					thix.write('f'_c);
					break;
				}
				if (that > 0.0_f && !disable_sign_when_positive) {
					thix.write('+'_c);
				}
				auto valid_begin = thix.split_reserve().view().begin();
				auto valid_end = thix.split_reserve().view().end();
				auto convert_result = Dependency::mscharconv::to_chars(
					unmake_pointer_unsafe<char>(valid_begin),
					unmake_pointer_unsafe<char>(valid_end),
					that.value,
					Dependency::mscharconv::chars_format::fixed
				);
				assert_test(convert_result.ec == std::errc{});
				thix.forward(make_box<Size>(convert_result.ptr - unmake_pointer_unsafe<char>(valid_begin)));
				if (!Range::has(Range::make_range(valid_begin, thix.current()), '.'_c)) {
					thix.write('.'_c);
					thix.write('0'_c);
				}
			}
			while (k_false);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			do {
				auto current = Character{};
				current = thix.read_of();
				if (current == 'N'_c) {
					assert_test(thix.read_of() == 'a'_c);
					assert_test(thix.read_of() == 'N'_c);
					that = invalid_number_of<That>();
					break;
				}
				thix.backward();
				auto valid_begin = thix.current();
				auto is_negative = k_false;
				current = thix.read_of();
				if (current == '+'_c) {
					++valid_begin;
				}
				else if (current == '-'_c) {
					is_negative = k_true;
				}
				else {
					thix.backward();
				}
				current = thix.read_of();
				if (current == 'I'_c) {
					assert_test(thix.read_of() == 'n'_c);
					assert_test(thix.read_of() == 'f'_c);
					that = is_negative ? -infinity_number_of<That>() : +infinity_number_of<That>();
					break;
				}
				thix.backward();
				auto is_floater = k_false;
				auto is_scientific = k_false;
				assert_test(StringUtility::is_number_character(thix.read_of()));
				while (!thix.full()) {
					current = thix.read_of();
					if (StringUtility::is_number_character(current)) {
						continue;
					}
					if (current == '.'_c) {
						assert_test(!is_floater);
						is_floater = k_true;
						current = thix.read_of();
						assert_test(StringUtility::is_number_character(current));
						continue;
					}
					if (current == 'e'_c) {
						assert_test(!is_scientific);
						assert_test(is_floater);
						is_scientific = k_true;
						current = thix.read_of();
						assert_test(current == '+'_c || current == '-'_c);
						current = thix.read_of();
						assert_test(StringUtility::is_number_character(current));
						continue;
					}
					thix.backward();
					break;
				}
				auto valid_end = thix.current();
				assert_test(is_floater);
				auto convert_result = Dependency::mscharconv::from_chars(
					unmake_pointer_unsafe<char>(valid_begin),
					unmake_pointer_unsafe<char>(valid_end),
					that.value,
					!is_scientific ? (Dependency::mscharconv::chars_format::fixed) : (Dependency::mscharconv::chars_format::scientific)
				);
				assert_test(convert_result.ec == std::errc{});
			}
			while (k_false);
			return;
		}

	};

	template <>
	struct BasicStringStreamAdapter<Character, NumberVariant> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = NumberVariant;

		// ----------------

		inline static auto capacity(
			That const &    that,
			Boolean const & disable_sign_when_positive
		) -> Size {
			auto result = 0_sz;
			switch (that.type().value) {
				case NumberVariantType::Constant::integer().value: {
					result = BasicStringStreamAdapter<Character, Integer>::capacity(that.get_integer(), disable_sign_when_positive);
					break;
				}
				case NumberVariantType::Constant::floater().value: {
					result = BasicStringStreamAdapter<Character, Floater>::capacity(that.get_floater(), disable_sign_when_positive);
					break;
				}
				default: throw UnreachableException{};
			}
			return result;
		}

		inline static auto write(
			ThisOutput &    thix,
			That const &    that,
			Boolean const & disable_sign_when_positive
		) -> Void {
			switch (that.type().value) {
				case NumberVariantType::Constant::integer().value: {
					thix.write(that.get_integer(), disable_sign_when_positive);
					break;
				}
				case NumberVariantType::Constant::floater().value: {
					thix.write(that.get_floater(), disable_sign_when_positive);
					break;
				}
				default: throw UnreachableException{};
			}
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			do {
				auto current = Character{};
				current = thix.read_of();
				if (current == 'N'_c) {
					assert_test(thix.read_of() == 'a'_c);
					assert_test(thix.read_of() == 'N'_c);
					that.set_floater() = invalid_number_of<Floater>();
					break;
				}
				thix.backward();
				auto valid_begin = thix.current();
				auto is_negative = k_false;
				current = thix.read_of();
				if (current == '+'_c) {
					++valid_begin;
				}
				else if (current == '-'_c) {
					is_negative = k_true;
				}
				else {
					thix.backward();
				}
				current = thix.read_of();
				if (current == 'I'_c) {
					assert_test(thix.read_of() == 'n'_c);
					assert_test(thix.read_of() == 'f'_c);
					that.set_floater() = is_negative ? -infinity_number_of<Floater>() : +infinity_number_of<Floater>();
					break;
				}
				thix.backward();
				auto is_floater = k_false;
				auto is_scientific = k_false;
				assert_test(StringUtility::is_number_character(thix.read_of()));
				while (!thix.full()) {
					current = thix.read_of();
					if (StringUtility::is_number_character(current)) {
						continue;
					}
					if (current == '.'_c) {
						assert_test(!is_floater);
						is_floater = k_true;
						current = thix.read_of();
						assert_test(StringUtility::is_number_character(current));
						continue;
					}
					if (current == 'e'_c) {
						assert_test(!is_scientific);
						assert_test(is_floater);
						is_scientific = k_true;
						current = thix.read_of();
						assert_test(current == '+'_c || current == '-'_c);
						current = thix.read_of();
						assert_test(StringUtility::is_number_character(current));
						continue;
					}
					thix.backward();
					break;
				}
				auto valid_end = thix.current();
				auto convert_result = Dependency::mscharconv::from_chars_result{};
				if (!is_floater) {
					convert_result = Dependency::mscharconv::from_chars(
						unmake_pointer_unsafe<char>(valid_begin),
						unmake_pointer_unsafe<char>(valid_end),
						that.set_integer().value,
						10
					);
				}
				else {
					convert_result = Dependency::mscharconv::from_chars(
						unmake_pointer_unsafe<char>(valid_begin),
						unmake_pointer_unsafe<char>(valid_end),
						that.set_floater().value,
						!is_scientific ? (Dependency::mscharconv::chars_format::fixed) : (Dependency::mscharconv::chars_format::scientific)
					);
				}
				assert_test(convert_result.ec == std::errc{});
			}
			while (k_false);
			return;
		}

	};

	#pragma endregion

	#pragma region byte

	template <>
	struct BasicStringStreamAdapter<Character, Byte> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Byte;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return 2_sz;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(StringUtility::to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(that, 4_sz, 4_sz)), k_true));
			thix.write(StringUtility::to_number_radix_character<16_sz>(cast_box<IntegerU8>(Bitwise::extract(that, 0_sz, 4_sz)), k_true));
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			that = 0x00_b;
			that |= Bitwise::infuse(cast_box<Byte>(StringUtility::from_number_radix_character<16_sz>(thix.read_of())), 4_sz, 4_sz);
			that |= Bitwise::infuse(cast_box<Byte>(StringUtility::from_number_radix_character<16_sz>(thix.read_of())), 0_sz, 4_sz);
			return;
		}

	};

	template <>
	struct BasicStringStreamAdapter<Character, Binary> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Binary;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return that.size() * 2_sz + Math::maximum(that.size(), 1_sz) - 1_sz;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			if (!that.empty()) {
				thix.write(that.first());
				for (auto & element : that.tail(that.size() - 1_sz)) {
					thix.write_constant(' '_c);
					thix.write(element);
				}
			}
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			assert_test(thix.size() == 0_sz || thix.size() % 3_sz == 2_sz);
			that.allocate_full((thix.size() + 1_sz) / 3_sz);
			if (!that.empty()) {
				thix.read(that.first());
				for (auto & element : that.tail(that.size() - 1_sz)) {
					thix.read_constant(' '_c);
					thix.read(element);
				}
			}
			return;
		}

	};

	#pragma endregion

	#pragma region enumeration

	template <typename TType> requires
		AutomaticConstraint
		&& (IsEnumerationBox<TType>)
		&& (IsDerivedFrom<TType, Enumeration<typename TType::Value>>)
		&& (!IsSame<TType, Enumeration<typename TType::Value>>)
	struct BasicStringStreamAdapter<Character, TType> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = TType;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			auto result = Size{};
			Generalization::match_if<typename That::Reflection::EnumerationValue>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					return TField::value == that.value;
				},
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					result = make_string_view_from_reflection(TField::name).size();
				}
			);
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			Generalization::match_if<typename That::Reflection::EnumerationValue>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					return TField::value == that.value;
				},
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					thix.write(make_string_view_from_reflection(TField::name));
				}
			);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			auto thix_hash = thix.split_forward(thix.reserve()).view().hash();
			Generalization::match_if<typename That::Reflection::EnumerationValue>(
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					return make_string_view_from_reflection(TField::name).hash() == thix_hash;
				},
				[&]<auto t_index, typename TField>(ValuePackage<t_index>, TypePackage<TField>) {
					that.value = TField::value;
				}
			);
			return;
		}

	};

	#pragma endregion

	#pragma region miscellaneous

	template <>
	struct BasicStringStreamAdapter<Character, FourCharacterCode> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = FourCharacterCode;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return 4_sz;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(cast_box<Character>(that.first));
			thix.write(cast_box<Character>(that.second));
			thix.write(cast_box<Character>(that.third));
			thix.write(cast_box<Character>(that.fourth));
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			that.first = cast_box<Character8>(thix.read_of<Character>());
			that.second = cast_box<Character8>(thix.read_of<Character>());
			that.third = cast_box<Character8>(thix.read_of<Character>());
			that.fourth = cast_box<Character8>(thix.read_of<Character>());
			return;
		}

	};

	template <>
	struct BasicStringStreamAdapter<Character, Path> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Path;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			auto result = 0_sz;
			result += 10_sz;
			for (auto & segment : that.segment()) {
				result += segment.size() + 1_sz;
			}
			return result;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			thix.write(that.emit());
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			auto thix_view = thix.split_forward(thix.reserve()).view();
			that.parse(String{thix_view});
			return;
		}

	};

	#pragma endregion

}
