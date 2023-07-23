#pragma once

#include "kernel/utility/base_wrapper/base.hpp"

namespace TwinStar::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(NumberWrapper, BaseWrapper<TValue>, BaseWrapper);

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(IntegerWrapper, NumberWrapper<TValue>, NumberWrapper);

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(FloaterWrapper, NumberWrapper<TValue>, NumberWrapper);

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(SizeWrapper, IntegerWrapper<TValue>, IntegerWrapper);

	// ----------------

	template <typename It>
	concept IsNumberWrapper = IsTemplateInstanceOfT<It, NumberWrapper>;

	template <typename It>
	concept IsIntegerWrapper = IsTemplateInstanceOfT<It, IntegerWrapper>;

	template <typename It>
	concept IsFloaterWrapper = IsTemplateInstanceOfT<It, FloaterWrapper>;

	template <typename It>
	concept IsSizeWrapper = IsTemplateInstanceOfT<It, SizeWrapper>;

	// ----------------

	template <typename It>
	concept IsUnsignedIntegerWrapper =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
		&& (IsBuiltinUnsignedInteger<typename It::Value>)
		;

	template <typename It>
	concept IsSignedIntegerWrapper =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
		&& (IsBuiltinSignedInteger<typename It::Value>)
		;

	template <typename It>
	concept IsSignedFloaterWrapper =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsFloaterWrapper<It>)
		&& (IsBuiltinSignedFloater<typename It::Value>)
		;

	#pragma endregion

	#pragma region operator

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator == (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value == that.value;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator < (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value < that.value;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator > (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value > that.value;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator <= (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value <= that.value;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator >= (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value >= that.value;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsSignedIntegerWrapper<It> || IsSignedFloaterWrapper<It>)
	inline constexpr auto operator + (
		It const & thix
	) -> It {
		return It{+thix.value};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsSignedIntegerWrapper<It> || IsSignedFloaterWrapper<It>)
	inline constexpr auto operator - (
		It const & thix
	) -> It {
		return It{-thix.value};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator + (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value + that.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator - (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value - that.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator * (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value * that.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator / (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value / that.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator % (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value % that.value)};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator += (
		It &       thix,
		It const & that
	) -> It & {
		thix.value += that.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator -= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value -= that.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator *= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value *= that.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsNumberWrapper<It>)
	inline constexpr auto operator /= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value /= that.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator %= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value %= that.value;
		return thix;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator ++ (
		It & thix
	) -> It & {
		++thix.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator -- (
		It & thix
	) -> It & {
		--thix.value;
		return thix;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator ++ (
		It & thix,
		int
	) -> It {
		return It{thix.value++};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator -- (
		It & thix,
		int
	) -> It {
		return It{thix.value--};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator ~ (
		It const & thix
	) -> It {
		return It{static_cast<typename It::Value>(~thix.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator | (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value | that.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator & (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value & that.value)};
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator ^ (
		It const & thix,
		It const & that
	) -> It {
		return It{static_cast<typename It::Value>(thix.value ^ that.value)};
	}

	template <typename It, typename Size> requires
		CategoryConstraint<IsPureInstance<It> && IsPureInstance<Size>>
		&& (IsIntegerWrapper<It>)
		&& (IsSizeWrapper<Size>)
	inline constexpr auto operator << (
		It const &   thix,
		Size const & size
	) -> It {
		return It{static_cast<typename It::Value>(thix.value << size.value)};
	}

	template <typename It, typename Size> requires
		CategoryConstraint<IsPureInstance<It> && IsPureInstance<Size>>
		&& (IsIntegerWrapper<It>)
		&& (IsSizeWrapper<Size>)
	inline constexpr auto operator >> (
		It const &   thix,
		Size const & size
	) -> It {
		return It{static_cast<typename It::Value>(thix.value >> size.value)};
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator |= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value |= that.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator &= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value &= that.value;
		return thix;
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline constexpr auto operator ^= (
		It &       thix,
		It const & that
	) -> It & {
		thix.value ^= that.value;
		return thix;
	}

	template <typename It, typename Size> requires
		CategoryConstraint<IsPureInstance<It> && IsPureInstance<Size>>
		&& (IsIntegerWrapper<It>)
		&& (IsSizeWrapper<Size>)
	inline constexpr auto operator <<= (
		It &         thix,
		Size const & size
	) -> It & {
		thix.value <<= size.value;
		return thix;
	}

	template <typename It, typename Size> requires
		CategoryConstraint<IsPureInstance<It> && IsPureInstance<Size>>
		&& (IsIntegerWrapper<It>)
		&& (IsSizeWrapper<Size>)
	inline constexpr auto operator >>= (
		It &         thix,
		Size const & size
	) -> It & {
		thix.value >>= size.value;
		return thix;
	}

	#pragma endregion

	#pragma region alias

	using IntegerU8 = IntegerWrapper<ZIntegerU8>;

	using IntegerU16 = IntegerWrapper<ZIntegerU16>;

	using IntegerU32 = IntegerWrapper<ZIntegerU32>;

	using IntegerU64 = IntegerWrapper<ZIntegerU64>;

	using IntegerS8 = IntegerWrapper<ZIntegerS8>;

	using IntegerS16 = IntegerWrapper<ZIntegerS16>;

	using IntegerS32 = IntegerWrapper<ZIntegerS32>;

	using IntegerS64 = IntegerWrapper<ZIntegerS64>;

	// ----------------

	using FloaterS32 = FloaterWrapper<ZFloaterS32>;

	using FloaterS64 = FloaterWrapper<ZFloaterS64>;

	// ----------------

	using SizeU = SizeWrapper<ZSize>;

	using SizeS = SizeWrapper<ZSSize>;

	#pragma endregion

	#pragma region regular type

	M_define_simple_derived_class(Integer, IntegerS64, IntegerS64);

	// ----------------

	M_define_simple_derived_class(Floater, FloaterS64, FloaterS64);

	// ----------------

	M_define_simple_derived_class(Size, SizeU, SizeU);

	M_define_simple_derived_class(SSize, SizeS, SizeS);

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_iu8 (
		ZLiteralInteger value
	) -> IntegerU8 {
		return mbw<IntegerU8>(value);
	}

	inline constexpr auto operator ""_iu16 (
		ZLiteralInteger value
	) -> IntegerU16 {
		return mbw<IntegerU16>(value);
	}

	inline constexpr auto operator ""_iu32 (
		ZLiteralInteger value
	) -> IntegerU32 {
		return mbw<IntegerU32>(value);
	}

	inline constexpr auto operator ""_iu64 (
		ZLiteralInteger value
	) -> IntegerU64 {
		return mbw<IntegerU64>(value);
	}

	inline constexpr auto operator ""_is8 (
		ZLiteralInteger value
	) -> IntegerS8 {
		return mbw<IntegerS8>(value);
	}

	inline constexpr auto operator ""_is16 (
		ZLiteralInteger value
	) -> IntegerS16 {
		return mbw<IntegerS16>(value);
	}

	inline constexpr auto operator ""_is32 (
		ZLiteralInteger value
	) -> IntegerS32 {
		return mbw<IntegerS32>(value);
	}

	inline constexpr auto operator ""_is64 (
		ZLiteralInteger value
	) -> IntegerS64 {
		return mbw<IntegerS64>(value);
	}

	// ----------------

	inline constexpr auto operator ""_fs32 (
		ZLiteralFloater value
	) -> FloaterS32 {
		return mbw<FloaterS32>(value);
	}

	inline constexpr auto operator ""_fs64 (
		ZLiteralFloater value
	) -> FloaterS64 {
		return mbw<FloaterS64>(value);
	}

	// ----------------

	inline constexpr auto operator ""_szu (
		ZLiteralInteger value
	) -> SizeU {
		return mbw<SizeU>(value);
	}

	inline constexpr auto operator ""_szs (
		ZLiteralInteger value
	) -> SizeS {
		return mbw<SizeS>(value);
	}

	// ----------------

	inline constexpr auto operator ""_i (
		ZLiteralInteger value
	) -> Integer {
		return mbw<Integer>(value);
	}

	// ----------------

	inline constexpr auto operator ""_f (
		ZLiteralFloater value
	) -> Floater {
		return mbw<Floater>(value);
	}

	// ----------------

	inline constexpr auto operator ""_sz (
		ZLiteralInteger value
	) -> Size {
		return mbw<Size>(value);
	}

	inline constexpr auto operator ""_ix (
		ZLiteralInteger value
	) -> Size {
		return mbw<Size>(value - 1);
	}

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_none_size = Size{0_sz};

	// ----------------

	inline constexpr auto k_begin_index = Size{1_ix};

	inline constexpr auto k_next_index = Size{2_ix};

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
	inline constexpr auto k_type_size = Size{mbw<Size>(sizeof(It))};

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
	inline constexpr auto k_type_bit_count = Size{k_type_size<It> * 8_sz};

	#pragma endregion

}
