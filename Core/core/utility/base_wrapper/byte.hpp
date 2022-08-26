#pragma once

#include "core/utility/base_wrapper/number.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	M_simple_derived_class(ByteWrapper, IntegerWrapper<TValue>, IntegerWrapper);

	// ----------------

	template <typename It>
	concept IsByteWrapper = IsTemplateInstanceOfT<It, ByteWrapper>;

	#pragma endregion

	#pragma region alias

	using Byte8 = ByteWrapper<ZByte8>;

	// ----------------

	M_simple_derived_class(Byte, Byte8, Byte8);

	#pragma region

	#pragma endregion literal

	inline constexpr auto operator ""_b8 (
		ZLiteralInteger value
	) -> Byte8 {
		return mbw<Byte8>(value);
	}

	inline constexpr auto operator ""_b8 (
		ZLiteralCharacter value
	) -> Byte8 {
		return mbw<Byte8>(value);
	}

	// ----------------

	inline constexpr auto operator ""_b (
		ZLiteralInteger value
	) -> Byte {
		return mbw<Byte>(value);
	}

	inline constexpr auto operator ""_b (
		ZLiteralCharacter value
	) -> Byte {
		return mbw<Byte>(value);
	}

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null_byte = Byte{0x00_b};

	#pragma endregion

}
