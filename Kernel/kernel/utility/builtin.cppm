module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.builtin;

export namespace Twinning::Kernel {

	#pragma region void

	using Void = void;

	#pragma endregion

	#pragma region fundamental

	using ZBoolean8 = bool;

	// ----------------

	using ZIntegerU8 = std::uint8_t;

	using ZIntegerU16 = std::uint16_t;

	using ZIntegerU32 = std::uint32_t;

	using ZIntegerU64 = std::uint64_t;

	using ZIntegerS8 = std::int8_t;

	using ZIntegerS16 = std::int16_t;

	using ZIntegerS32 = std::int32_t;

	using ZIntegerS64 = std::int64_t;

	// ----------------

	using ZFloaterS32 = float;

	using ZFloaterS64 = double;

	// ----------------

	using ZCharacter8 = char8_t;

	using ZCharacter16 = char16_t;

	using ZCharacter32 = char32_t;

	// ----------------

	using ZCharacterN = char;

	using ZCharacterW = wchar_t;

	// ----------------

	using ZByte8 = unsigned char;

	// ----------------

	using ZSizeU = std::size_t;

	using ZSizeS = std::ptrdiff_t;

	#pragma endregion

	#pragma region compound

	template <typename Target>
	using ZPointer = Target (*);

	template <typename Class, typename Target>
	using ZMemberPointer = Target (Class::*);

	// ----------------

	template <typename Element, auto size>
	using ZArray = Element[size];

	#pragma endregion

	#pragma region alias

	using None = Void;

	// ----------------

	using ZBoolean = ZBoolean8;

	using ZInteger = ZIntegerS64;

	using ZFloater = ZFloaterS64;

	using ZCharacter = ZCharacterN;

	using ZByte = ZByte8;

	using ZSize = ZSizeU;

	using ZSSize = ZSizeS;

	// ----------------

	using ZConstantString = char const *;

	// ----------------

	using ZLiteralInteger = unsigned long long const;

	using ZLiteralFloater = long double const;

	using ZLiteralCharacter = char const;

	using ZLiteralSize = std::size_t const;

	using ZLiteralString = char const * const;

	#pragma endregion

	#pragma region miscellaneous

	inline constexpr auto operator ""_szz (
		ZLiteralInteger value
	) -> ZSize {
		return static_cast<ZSize>(value);
	}

	inline constexpr auto operator ""_ixz (
		ZLiteralInteger value
	) -> ZSize {
		return static_cast<ZSize>(value - 1);
	}

	// ----------------

	using StrongOrdering = std::strong_ordering;

	// ----------------

	M_simple_function_alias(swap, std::swap);

	M_simple_function_alias(minimum, std::min);

	M_simple_function_alias(maximum, std::max);

	// ----------------

	template <typename Element>
	inline constexpr auto make_initializer_list (
		std::initializer_list<Element> value
	) -> std::initializer_list<Element> {
		return value;
	}

	#pragma endregion

}
