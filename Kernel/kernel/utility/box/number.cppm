module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.number;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box.base;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(NumberBox, BaseBox<TValue>, BaseBox);

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(IntegerBox, NumberBox<TValue>, NumberBox);

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(FloaterBox, NumberBox<TValue>, NumberBox);

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(SizeBox, IntegerBox<TValue>, IntegerBox);

	// ----------------

	template <typename TIt>
	concept IsNumberBox = IsTemplateInstanceOfTt<TIt, NumberBox>;

	template <typename TIt>
	concept IsIntegerBox = IsTemplateInstanceOfTt<TIt, IntegerBox>;

	template <typename TIt>
	concept IsFloaterBox = IsTemplateInstanceOfTt<TIt, FloaterBox>;

	template <typename TIt>
	concept IsSizeBox = IsTemplateInstanceOfTt<TIt, SizeBox>;

	// ----------------

	template <typename TIt>
	concept IsUnsignedIntegerBox =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
		&& (IsBuiltinUnsignedInteger<typename TIt::Value>)
		;

	template <typename TIt>
	concept IsSignedIntegerBox =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
		&& (IsBuiltinSignedInteger<typename TIt::Value>)
		;

	template <typename TIt>
	concept IsSignedFloaterBox =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsFloaterBox<TIt>)
		&& (IsBuiltinSignedFloater<typename TIt::Value>)
		;

	#pragma endregion

	#pragma region operator

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator ==(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value == that.value;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator <(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value < that.value;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator >(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value > that.value;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator <=(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value <= that.value;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator >=(
		TIt const & thix,
		TIt const & that
	) -> bool {
		return thix.value >= that.value;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsSignedIntegerBox<TIt> || IsSignedFloaterBox<TIt>)
	inline constexpr auto operator +(
		TIt const & thix
	) -> TIt {
		return TIt{+thix.value};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsSignedIntegerBox<TIt> || IsSignedFloaterBox<TIt>)
	inline constexpr auto operator -(
		TIt const & thix
	) -> TIt {
		return TIt{-thix.value};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator +(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value + that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator -(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value - that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator *(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value * that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator /(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value / that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator %(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value % that.value)};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator +=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value += that.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator -=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value -= that.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator *=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value *= that.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator /=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value /= that.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator %=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value %= that.value;
		return thix;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ++(
		TIt & thix
	) -> TIt & {
		++thix.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator --(
		TIt & thix
	) -> TIt & {
		--thix.value;
		return thix;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ++(
		TIt & thix,
		int
	) -> TIt {
		return TIt{thix.value++};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator --(
		TIt & thix,
		int
	) -> TIt {
		return TIt{thix.value--};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ~(
		TIt const & thix
	) -> TIt {
		return TIt{static_cast<TIt::Value>(~thix.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator |(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value | that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator &(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value & that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ^(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value ^ that.value)};
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator <<(
		TIt const &   thix,
		TSize const & size
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value << size.value)};
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator >>(
		TIt const &   thix,
		TSize const & size
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value >> size.value)};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator |=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value |= that.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator &=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value &= that.value;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ^=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix.value ^= that.value;
		return thix;
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator <<=(
		TIt &         thix,
		TSize const & size
	) -> TIt & {
		thix.value <<= size.value;
		return thix;
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator >>=(
		TIt &         thix,
		TSize const & size
	) -> TIt & {
		thix.value >>= size.value;
		return thix;
	}

	#pragma endregion

	#pragma region alias

	using IntegerU8 = IntegerBox<ZIntegerU8>;

	using IntegerU16 = IntegerBox<ZIntegerU16>;

	using IntegerU32 = IntegerBox<ZIntegerU32>;

	using IntegerU64 = IntegerBox<ZIntegerU64>;

	using IntegerS8 = IntegerBox<ZIntegerS8>;

	using IntegerS16 = IntegerBox<ZIntegerS16>;

	using IntegerS32 = IntegerBox<ZIntegerS32>;

	using IntegerS64 = IntegerBox<ZIntegerS64>;

	// ----------------

	using FloaterS32 = FloaterBox<ZFloaterS32>;

	using FloaterS64 = FloaterBox<ZFloaterS64>;

	// ----------------

	using SizeU = SizeBox<ZSize>;

	using SizeS = SizeBox<ZSSize>;

	#pragma endregion

	#pragma region regular type

	M_simple_derived_class(Integer, IntegerS64, IntegerS64);

	// ----------------

	M_simple_derived_class(Floater, FloaterS64, FloaterS64);

	// ----------------

	M_simple_derived_class(Size, SizeU, SizeU);

	M_simple_derived_class(SSize, SizeS, SizeS);

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_iu8(
		ZLiteralInteger value
	) -> IntegerU8 {
		return mbox<IntegerU8>(value);
	}

	inline constexpr auto operator ""_iu16(
		ZLiteralInteger value
	) -> IntegerU16 {
		return mbox<IntegerU16>(value);
	}

	inline constexpr auto operator ""_iu32(
		ZLiteralInteger value
	) -> IntegerU32 {
		return mbox<IntegerU32>(value);
	}

	inline constexpr auto operator ""_iu64(
		ZLiteralInteger value
	) -> IntegerU64 {
		return mbox<IntegerU64>(value);
	}

	inline constexpr auto operator ""_is8(
		ZLiteralInteger value
	) -> IntegerS8 {
		return mbox<IntegerS8>(value);
	}

	inline constexpr auto operator ""_is16(
		ZLiteralInteger value
	) -> IntegerS16 {
		return mbox<IntegerS16>(value);
	}

	inline constexpr auto operator ""_is32(
		ZLiteralInteger value
	) -> IntegerS32 {
		return mbox<IntegerS32>(value);
	}

	inline constexpr auto operator ""_is64(
		ZLiteralInteger value
	) -> IntegerS64 {
		return mbox<IntegerS64>(value);
	}

	// ----------------

	inline constexpr auto operator ""_fs32(
		ZLiteralFloater value
	) -> FloaterS32 {
		return mbox<FloaterS32>(value);
	}

	inline constexpr auto operator ""_fs64(
		ZLiteralFloater value
	) -> FloaterS64 {
		return mbox<FloaterS64>(value);
	}

	// ----------------

	inline constexpr auto operator ""_szu(
		ZLiteralInteger value
	) -> SizeU {
		return mbox<SizeU>(value);
	}

	inline constexpr auto operator ""_szs(
		ZLiteralInteger value
	) -> SizeS {
		return mbox<SizeS>(value);
	}

	// ----------------

	inline constexpr auto operator ""_i(
		ZLiteralInteger value
	) -> Integer {
		return mbox<Integer>(value);
	}

	// ----------------

	inline constexpr auto operator ""_f(
		ZLiteralFloater value
	) -> Floater {
		return mbox<Floater>(value);
	}

	// ----------------

	inline constexpr auto operator ""_sz(
		ZLiteralInteger value
	) -> Size {
		return mbox<Size>(value);
	}

	inline constexpr auto operator ""_ix(
		ZLiteralInteger value
	) -> Size {
		return mbox<Size>(value - 1);
	}

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_none_size = Size{0_sz};

	// ----------------

	inline constexpr auto k_begin_index = Size{1_ix};

	inline constexpr auto k_next_index = Size{2_ix};

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
	inline constexpr auto k_type_size = Size{mbox<Size>(sizeof(TIt))};

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
	inline constexpr auto k_type_bit_count = Size{k_type_size<TIt> * 8_sz};

	#pragma endregion

}
