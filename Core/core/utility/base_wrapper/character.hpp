#pragma once

#include "core/utility/base_wrapper/base.hpp"
#include "core/utility/base_wrapper/number.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(CharacterWrapper, IntegerWrapper<TValue>, IntegerWrapper);

	// ----------------

	template <typename It>
	concept IsCharacterWrapper = IsTemplateInstanceOfT<It, CharacterWrapper>;

	#pragma endregion

	#pragma region alias

	using CharacterLocale = CharacterWrapper<ZCharacterLocale>;

	using CharacterWide = CharacterWrapper<ZCharacterWide>;

	using Character8 = CharacterWrapper<ZCharacter8>;

	using Character16 = CharacterWrapper<ZCharacter16>;

	using Character32 = CharacterWrapper<ZCharacter32>;

	#pragma endregion

	#pragma region regular type

	M_define_simple_derived_class(Character, CharacterLocale, CharacterLocale);

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_cl (
		ZLiteralInteger value
	) -> CharacterLocale {
		return mbw<CharacterLocale>(value);
	}

	inline constexpr auto operator ""_cl (
		ZLiteralCharacter value
	) -> CharacterLocale {
		return mbw<CharacterLocale>(value);
	}

	// ----------------

	inline constexpr auto operator ""_cw (
		ZLiteralInteger value
	) -> CharacterWide {
		return mbw<CharacterWide>(value);
	}

	inline constexpr auto operator ""_cw (
		ZLiteralCharacter value
	) -> CharacterWide {
		return mbw<CharacterWide>(value);
	}

	// ----------------

	inline constexpr auto operator ""_c8 (
		ZLiteralInteger value
	) -> Character8 {
		return mbw<Character8>(value);
	}

	inline constexpr auto operator ""_c8 (
		ZLiteralCharacter value
	) -> Character8 {
		return mbw<Character8>(value);
	}

	// ----------------

	inline constexpr auto operator ""_c16 (
		ZLiteralInteger value
	) -> Character16 {
		return mbw<Character16>(value);
	}

	inline constexpr auto operator ""_c16 (
		ZLiteralCharacter value
	) -> Character16 {
		return mbw<Character16>(value);
	}

	// ----------------

	inline constexpr auto operator ""_c32 (
		ZLiteralInteger value
	) -> Character32 {
		return mbw<Character32>(value);
	}

	inline constexpr auto operator ""_c32 (
		ZLiteralCharacter value
	) -> Character32 {
		return mbw<Character32>(value);
	}

	// ----------------

	inline constexpr auto operator ""_c (
		ZLiteralInteger value
	) -> Character {
		return mbw<Character>(value);
	}

	inline constexpr auto operator ""_c (
		ZLiteralCharacter value
	) -> Character {
		return mbw<Character>(value);
	}

	#pragma endregion

}
