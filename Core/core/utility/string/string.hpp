#pragma once

#include "core/utility/string/basic_string_view.hpp"
#include "core/utility/string/basic_string.hpp"
#include "core/utility/string/basic_static_string.hpp"

namespace TwinStar::Core {

	#pragma region alias

	template <auto constant> requires
		AutoConstraint
	using StringView = BasicStringView<Character, constant>;

	using VStringView = VBasicStringView<Character>;

	using CStringView = CBasicStringView<Character>;

	using String = BasicString<Character>;

	// ----------------

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameV<t_size, ZSize>)
	struct StaticString :
		BasicStaticString<Character, mbw<Size>(t_size)> {
		implicit constexpr StaticString (
			ZArray<Character::Value, t_size> const & data
		) :
			BasicStaticString<Character, mbw<Size>(t_size)>{data} {
		}
	};

	#pragma endregion

	#pragma region utility

	inline auto make_string_view (
		ZConstantString const & string,
		ZSize const &           length
	) -> CStringView {
		return CStringView{cast_pointer<Character>(make_pointer(string)), mbw<Size>(length)};
	}

	inline auto make_string (
		ZConstantString const & string,
		ZSize const &           length
	) -> String {
		return String{cast_pointer<Character>(make_pointer(string)), mbw<Size>(length)};
	}

	// ----------------

	inline auto make_string_view (
		std::string_view const & string
	) -> CStringView {
		return make_string_view(string.data(), string.length());
	}

	inline auto make_string (
		std::string_view const & string
	) -> String {
		return make_string(string.data(), string.length());
	}

	// ----------------

	inline auto make_std_string_view (
		CStringView const & string
	) -> std::string_view {
		return std::string_view{cast_pointer<char>(string.begin()).value, string.size().value};
	}

	inline constexpr auto hash_std_string_view (
		std::string_view const & string
	) -> ZIntegerU64 {
		auto offset = ZIntegerU64{14695981039346656037ull};
		auto prime = ZIntegerU64{1099511628211ull};
		auto result = offset;
		for (auto index = 1_ixz; index < string.size(); ++index) {
			result *= prime;
			result ^= static_cast<ZIntegerU8>(string[index]);
		}
		return result;
	}

	#pragma endregion

	#pragma region literal

	template <StaticString string>
	inline constexpr auto operator ""_sv (
	) -> CStringView {
		return string.view();
	}

	template <StaticString string>
	inline constexpr auto operator ""_sl (
	) -> Size {
		return string.view().size();
	}

	template <StaticString string>
	inline constexpr auto operator ""_sh (
	) -> IntegerU64 {
		return string.view().hash();
	}

	template <StaticString string>
	inline auto operator ""_s (
	) -> String {
		return String{string.view()};
	}

	// ----------------

	template <StaticString string>
	inline constexpr auto operator ""_shz (
	) -> ZIntegerU64 {
		return operator ""_sh<string>().value;
	}

	#pragma endregion

}
