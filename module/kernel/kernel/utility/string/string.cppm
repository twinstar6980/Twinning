module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_stream_flag;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.string.basic_string_stream_adapter;
import twinning.kernel.utility.string.basic_string_formatter;
import twinning.kernel.utility.string.basic_static_string;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region alias

	template <auto t_constant> requires
		AutomaticConstraint
	using StringView = BasicStringView<Character, t_constant>;

	using VariableStringView = VariableBasicStringView<Character>;

	using ConstantStringView = ConstantBasicStringView<Character>;

	using String = BasicString<Character>;

	using StringStreamFlag = BasicStringStreamFlag;

	template <auto t_mode> requires
		AutomaticConstraint
	using StringStreamView = BasicStringStreamView<Character, t_mode>;

	using AccessStringStreamView = AccessBasicStringStreamView<Character>;

	using InputStringStreamView = InputBasicStringStreamView<Character>;

	using OutputStringStreamView = OutputBasicStringStreamView<Character>;

	using StringFormatter = BasicStringFormatter<Character>;

	template <auto t_size> requires
		AutomaticConstraint
	using StaticString = BasicStaticString<Character, t_size>;

	#pragma endregion

	#pragma region literal

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_sv(
	) -> ConstantStringView {
		return t_string.view();
	}

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_sl(
	) -> Size {
		return t_string.view().size();
	}

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_sh(
	) -> IntegerU64 {
		return t_string.view().hash();
	}

	template <StaticString t_string> requires
		NoneConstraint
	inline auto operator ""_s(
	) -> String {
		return String{t_string.view()};
	}

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_sf(
	) -> StringFormatter {
		return StringFormatter{t_string.view()};
	}

	// ----------------

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_shz(
	) -> ZIntegerU64 {
		return operator ""_sh<t_string>().value;
	}

	#pragma endregion

	#pragma region utility

	template <typename TElement> requires
		CategoryConstraint<IsInstance<TElement>>
		&& (IsCharacterBox<AsPure<TElement>>)
	inline constexpr auto compute_null_terminated_string_length(
		Pointer<TElement> const & string,
		Size const &              limit
	) -> Size {
		auto null_character = make_box<AsPure<TElement>>('\0');
		auto length = 0_sz;
		for (length = 0_sz; length < limit; ++length) {
			if (string[length] == null_character) {
				break;
			}
		}
		return length;
	}

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
	inline constexpr auto is_safe_null_terminated_string(
		BasicStringView<TElement, t_constant> const & string
	) -> Boolean {
		return compute_null_terminated_string_length(string.data(), string.size()) == string.size();
	}

	// ----------------

	template <typename TElement> requires
		CategoryConstraint<IsInstance<TElement>>
		&& (IsCharacterBox<AsPure<TElement>>)
	inline constexpr auto make_string_view(
		Pointer<TElement> const & string,
		Size const &              length
	) -> BasicStringView<AsPure<TElement>, make_box<Boolean>(IsConstant<TElement>)> {
		return BasicStringView<AsPure<TElement>, make_box<Boolean>(IsConstant<TElement>)>{string, length};
	}

	template <typename TElement> requires
		CategoryConstraint<IsInstance<TElement>>
		&& (IsCharacterBox<AsPure<TElement>>)
	inline constexpr auto make_string_view_from_indeterminate(
		Pointer<TElement> const & string,
		Size const &              limit
	) -> BasicStringView<AsPure<TElement>, make_box<Boolean>(IsConstant<TElement>)> {
		return make_string_view(string, compute_null_terminated_string_length(string, limit));
	}

	template <auto t_size> requires
		CategoryConstraint<>
		&& IsSameOf<t_size, ZSize>
	inline auto make_string_view_from_reflection(
		Reflection::String<t_size> const & string
	) -> ConstantStringView {
		return make_string_view(make_pointer_unsafe<Character>(string.data()), make_box<Size>(string.length()));
	}

	// ----------------

	inline auto from_std_string_view(
		std::string_view const & string
	) -> ConstantStringView {
		return make_string_view(make_pointer_unsafe<Character>(string.data()), make_box<Size>(string.length()));
	}

	inline auto from_std_string(
		std::string_view const & string
	) -> String {
		return String{from_std_string_view(string)};
	}

	// ----------------

	inline auto to_std_string_view(
		ConstantStringView const & string
	) -> std::string_view {
		return std::string_view{unmake_pointer_unsafe<char>(string.data()), unmake_box<std::size_t>(string.size())};
	}

	inline auto to_std_string(
		ConstantStringView const & string
	) -> std::string {
		return std::string{to_std_string_view(string)};
	}

	#pragma endregion

	#pragma region convert

	template <typename TValue, typename ... TOption> requires
		CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
		&& (IsHasBasicStringStreamAdapter<Character, AsPure<TValue>>)
	inline auto string_from(
		String &       string,
		TValue &&      value,
		TOption && ... option
	) -> String & {
		string.allocate_full(BasicStringStreamAdapter<Character, AsPure<TValue>>::capacity(as_forward<TValue>(value), as_forward<TOption>(option) ...));
		auto stream = OutputStringStreamView{string};
		stream.write(as_forward<TValue>(value), as_forward<TOption>(option) ...);
		string.resize(stream.position());
		return string;
	}

	template <typename TValue, typename ... TOption> requires
		CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
		&& (IsHasBasicStringStreamAdapter<Character, AsPure<TValue>>)
	inline auto string_to(
		String const & string,
		TValue &&      value,
		TOption && ... option
	) -> TValue && {
		auto stream = InputStringStreamView{string};
		stream.read(as_forward<TValue>(value), as_forward<TOption>(option) ...);
		assert_test(stream.full());
		return as_forward<TValue>(value);
	}

	// ----------------

	template <typename TValue, typename ... TOption> requires
		CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
		&& (IsHasBasicStringStreamAdapter<Character, AsPure<TValue>>)
	inline auto string_from_of(
		TValue &&      value,
		TOption && ... option
	) -> String {
		return string_from(as_left(String{}), as_forward<TValue>(value), as_forward<TOption>(option) ...);
	}

	template <typename TValue, typename ... TOption> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TOption ...>>
		&& (IsHasBasicStringStreamAdapter<Character, AsPure<TValue>>)
	inline auto string_to_of(
		String const & string,
		TOption && ... option
	) -> TValue {
		return string_to(string, as_left(TValue{}), as_forward<TOption>(option) ...);
	}

	#pragma endregion

}
