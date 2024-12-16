module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.byte;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;
import twinning.kernel.utility.box.number;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(ByteBox, IntegerBox<TValue>, IntegerBox);

	// ----------------

	template <typename It>
	concept IsByteBox = IsTemplateInstanceOfT<It, ByteBox>;

	#pragma endregion

	#pragma region alias

	using Byte8 = ByteBox<ZByte8>;

	#pragma endregion

	#pragma region regular type

	M_simple_derived_class(Byte, Byte8, Byte8);

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_b8 (
		ZLiteralInteger value
	) -> Byte8 {
		return mbox<Byte8>(value);
	}

	inline constexpr auto operator ""_b8 (
		ZLiteralCharacter value
	) -> Byte8 {
		return mbox<Byte8>(value);
	}

	// ----------------

	inline constexpr auto operator ""_b (
		ZLiteralInteger value
	) -> Byte {
		return mbox<Byte>(value);
	}

	inline constexpr auto operator ""_b (
		ZLiteralCharacter value
	) -> Byte {
		return mbox<Byte>(value);
	}

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_null_byte = Byte{0x00_b};

	#pragma endregion

}
