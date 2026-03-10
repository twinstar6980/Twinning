module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_static_string;

export namespace Twinning::Kernel {

	#pragma region alias

	template <auto t_constant> requires
		AutomaticConstraint
	using StringView = BasicStringView<Character, t_constant>;

	using VariableStringView = VariableBasicStringView<Character>;

	using ConstantStringView = ConstantBasicStringView<Character>;

	using String = BasicString<Character>;

	// ----------------

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameOf<t_size, ZSize>)
	struct StaticString :
		BasicStaticString<Character, mbox<Size>(t_size)> {
		implicit constexpr StaticString(
			ZArray<Character::Value, t_size> const & data
		) :
			BasicStaticString<Character, mbox<Size>(t_size)>{data} {
		}
	};

	#pragma endregion

	#pragma region utility

	inline auto make_string_view(
		ZConstantString const & string,
		ZSize const &           length
	) -> ConstantStringView {
		return ConstantStringView{cast_pointer<Character>(make_pointer(string)), mbox<Size>(length)};
	}

	inline auto make_string(
		ZConstantString const & string,
		ZSize const &           length
	) -> String {
		return String{cast_pointer<Character>(make_pointer(string)), mbox<Size>(length)};
	}

	// ----------------

	inline auto make_string_view(
		std::string_view const & string
	) -> ConstantStringView {
		return make_string_view(string.data(), string.length());
	}

	inline auto make_string(
		std::string_view const & string
	) -> String {
		return make_string(string.data(), string.length());
	}

	// ----------------

	inline auto make_std_string_view(
		ConstantStringView const & string
	) -> std::string_view {
		return std::string_view{cast_pointer<char>(string.begin()).value, string.size().value};
	}

	inline constexpr auto hash_std_string_view(
		std::string_view const & string
	) -> ZIntegerU64 {
		auto offset = ZIntegerU64{14695981039346656037ull};
		auto prime = ZIntegerU64{1099511628211ull};
		auto result = offset;
		for (auto index = 1_ixz; index < string.size(); ++index) {
			result ^= static_cast<ZIntegerU8>(string[index]);
			result *= prime;
		}
		return result;
	}

	// ----------------

	// NOTE: ALIAS: make_std_string
	inline auto mss(
		ConstantStringView const & string
	) -> std::string {
		return std::string{cast_pointer<char>(string.begin()).value, string.size().value};
	}

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

	// ----------------

	template <StaticString t_string> requires
		NoneConstraint
	inline constexpr auto operator ""_shz(
	) -> ZIntegerU64 {
		return operator ""_sh<t_string>().value;
	}

	#pragma endregion

}
