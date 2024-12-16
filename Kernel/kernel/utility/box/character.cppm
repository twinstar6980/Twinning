module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.character;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;
import twinning.kernel.utility.box.number;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(CharacterBox, IntegerBox<TValue>, IntegerBox);

	// ----------------

	template <typename It>
	concept IsCharacterBox = IsTemplateInstanceOfT<It, CharacterBox>;

	#pragma endregion

	#pragma region alias

	using Character8 = CharacterBox<ZCharacter8>;

	using Character16 = CharacterBox<ZCharacter16>;

	using Character32 = CharacterBox<ZCharacter32>;

	// ----------------

	using CharacterN = CharacterBox<ZCharacterN>;

	using CharacterW = CharacterBox<ZCharacterW>;

	#pragma endregion

	#pragma region regular type

	M_simple_derived_class(Character, CharacterN, CharacterN);

	M_simple_derived_class(Unicode, Character32, Character32);

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_c8 (
		ZLiteralInteger value
	) -> Character8 {
		return mbox<Character8>(value);
	}

	inline constexpr auto operator ""_c8 (
		ZLiteralCharacter value
	) -> Character8 {
		return mbox<Character8>(value);
	}

	inline constexpr auto operator ""_c16 (
		ZLiteralInteger value
	) -> Character16 {
		return mbox<Character16>(value);
	}

	inline constexpr auto operator ""_c16 (
		ZLiteralCharacter value
	) -> Character16 {
		return mbox<Character16>(value);
	}

	inline constexpr auto operator ""_c32 (
		ZLiteralInteger value
	) -> Character32 {
		return mbox<Character32>(value);
	}

	inline constexpr auto operator ""_c32 (
		ZLiteralCharacter value
	) -> Character32 {
		return mbox<Character32>(value);
	}

	// ----------------

	inline constexpr auto operator ""_cn (
		ZLiteralInteger value
	) -> CharacterN {
		return mbox<CharacterN>(value);
	}

	inline constexpr auto operator ""_cn (
		ZLiteralCharacter value
	) -> CharacterN {
		return mbox<CharacterN>(value);
	}

	inline constexpr auto operator ""_cw (
		ZLiteralInteger value
	) -> CharacterW {
		return mbox<CharacterW>(value);
	}

	inline constexpr auto operator ""_cw (
		ZLiteralCharacter value
	) -> CharacterW {
		return mbox<CharacterW>(value);
	}

	// ----------------

	inline constexpr auto operator ""_c (
		ZLiteralInteger value
	) -> Character {
		return mbox<Character>(value);
	}

	inline constexpr auto operator ""_c (
		ZLiteralCharacter value
	) -> Character {
		return mbox<Character>(value);
	}

	inline constexpr auto operator ""_u (
		ZLiteralInteger value
	) -> Unicode {
		return mbox<Unicode>(value);
	}

	inline constexpr auto operator ""_u (
		ZLiteralCharacter value
	) -> Unicode {
		return mbox<Unicode>(value);
	}

	#pragma endregion

}
