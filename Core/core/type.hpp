#pragma once

#include "core/macro.hpp"
#include "core/utility/exception/exception.hpp"
#include "core/utility/exception/simple_exception.hpp"
#include "core/utility/exception/special_exception.hpp"
#include <cstdint>

namespace TwinStar::Core {

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

	using ZFloating32 = float;

	using ZFloating64 = double;

	// ----------------

	using ZCharacterLocale = char;

	using ZCharacterWide = wchar_t;

	using ZCharacter8 = char8_t;

	using ZCharacter16 = char16_t;

	using ZCharacter32 = char32_t;

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

	using ZFloating = ZFloating64;

	using ZCharacter = ZCharacterLocale;

	using ZByte = ZByte8;

	using ZSize = ZSizeU;

	using ZSSize = ZSizeS;

	// ----------------

	using ZConstantString = char const *;

	// ----------------

	using ZLiteralInteger = unsigned long long const;

	using ZLiteralFloating = long double const;

	using ZLiteralCharacter = char const;

	using ZLiteralString = char const * const;

	using ZLiteralLength = std::size_t const;

	#pragma endregion

	#pragma region miscellaneous

	inline constexpr auto operator ""_szz (
		ZLiteralInteger value
	) -> ZSize {
		return ZSize{static_cast<ZSize>(value)};
	}

	inline constexpr auto operator ""_ixz (
		ZLiteralInteger value
	) -> ZSize {
		return ZSize{static_cast<ZSize>(value - 1)};
	}

	#pragma endregion

}
