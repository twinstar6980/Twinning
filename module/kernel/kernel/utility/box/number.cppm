module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.box.number;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.exception;
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
		auto result = typename TIt::Value{0};
		if constexpr (true) {
			result = static_cast<TIt::Value>(+thix.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsSignedIntegerBox<TIt> || IsSignedFloaterBox<TIt>)
	inline constexpr auto operator -(
		TIt const & thix
	) -> TIt {
		auto result = typename TIt::Value{0};
		if constexpr (IsSignedIntegerBox<TIt>) {
			if (thix.value == std::numeric_limits<typename TIt::Value>::min()) {
				throw UnsupportedException{};
			}
		}
		if constexpr (true) {
			result = static_cast<TIt::Value>(-thix.value);
		}
		return TIt{result};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator +(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if constexpr (IsIntegerBox<TIt>) {
			if (__builtin_add_overflow(thix.value, that.value, &result)) {
				throw UnsupportedException{};
			}
		}
		if constexpr (IsFloaterBox<TIt>) {
			result = static_cast<TIt::Value>(thix.value + that.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator -(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if constexpr (IsIntegerBox<TIt>) {
			if (__builtin_sub_overflow(thix.value, that.value, &result)) {
				throw UnsupportedException{};
			}
		}
		if constexpr (IsFloaterBox<TIt>) {
			result = static_cast<TIt::Value>(thix.value - that.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator *(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if constexpr (IsIntegerBox<TIt>) {
			if (__builtin_mul_overflow(thix.value, that.value, &result)) {
				throw UnsupportedException{};
			}
		}
		if constexpr (IsFloaterBox<TIt>) {
			result = static_cast<TIt::Value>(thix.value * that.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator /(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if constexpr (IsIntegerBox<TIt>) {
			if (that.value == typename TIt::Value{0}) {
				throw UnsupportedException{};
			}
		}
		if constexpr (IsSignedIntegerBox<TIt>) {
			if (thix.value == std::numeric_limits<typename TIt::Value>::min() && that.value == typename TIt::Value{-1}) {
				throw UnsupportedException{};
			}
		}
		if constexpr (true) {
			result = static_cast<TIt::Value>(thix.value / that.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator %(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if constexpr (IsIntegerBox<TIt>) {
			if (that.value == typename TIt::Value{0}) {
				throw UnsupportedException{};
			}
		}
		if constexpr (IsSignedIntegerBox<TIt>) {
			if (thix.value == std::numeric_limits<typename TIt::Value>::min() && that.value == typename TIt::Value{-1}) {
				throw UnsupportedException{};
			}
		}
		if constexpr (true) {
			result = static_cast<TIt::Value>(thix.value % that.value);
		}
		return TIt{result};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator +=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix + that;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator -=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix - that;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator *=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix * that;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto operator /=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix / that;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator %=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix % that;
		return thix;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ++(
		TIt & thix
	) -> TIt & {
		thix = thix + TIt{1};
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator --(
		TIt & thix
	) -> TIt & {
		thix = thix - TIt{1};
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
		auto result = thix;
		thix = thix + TIt{1};
		return result;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator --(
		TIt & thix,
		int
	) -> TIt {
		auto result = thix;
		thix = thix - TIt{1};
		return result;
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ~(
		TIt const & thix
	) -> TIt {
		auto result = typename TIt::Value{0};
		{
			result = static_cast<TIt::Value>(~thix.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator |(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		{
			result = static_cast<TIt::Value>(thix.value | that.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator &(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		{
			result = static_cast<TIt::Value>(thix.value & that.value);
		}
		return TIt{result};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ^(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		{
			result = static_cast<TIt::Value>(thix.value ^ that.value);
		}
		return TIt{result};
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator <<(
		TIt const &   thix,
		TSize const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if (that < make_box<TSize>(sizeof(TIt) * 8zu)) {
			result = static_cast<TIt::Value>(thix.value << that.value);
		}
		return TIt{result};
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator >>(
		TIt const &   thix,
		TSize const & that
	) -> TIt {
		auto result = typename TIt::Value{0};
		if (that < make_box<TSize>(sizeof(TIt) * 8zu)) {
			result = static_cast<TIt::Value>(thix.value >> that.value);
		}
		return TIt{result};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator |=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix | that;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator &=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix & that;
		return thix;
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline constexpr auto operator ^=(
		TIt &       thix,
		TIt const & that
	) -> TIt & {
		thix = thix ^ that;
		return thix;
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator <<=(
		TIt &         thix,
		TSize const & that
	) -> TIt & {
		thix = thix << that;
		return thix;
	}

	template <typename TIt, typename TSize> requires
		CategoryConstraint<IsPureInstance<TIt> && IsPureInstance<TSize>>
		&& (IsIntegerBox<TIt>)
		&& (IsSizeBox<TSize>)
	inline constexpr auto operator >>=(
		TIt &         thix,
		TSize const & that
	) -> TIt & {
		thix = thix >> that;
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

	#pragma region regular

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
		return make_box<IntegerU8>(value);
	}

	inline constexpr auto operator ""_iu16(
		ZLiteralInteger value
	) -> IntegerU16 {
		return make_box<IntegerU16>(value);
	}

	inline constexpr auto operator ""_iu32(
		ZLiteralInteger value
	) -> IntegerU32 {
		return make_box<IntegerU32>(value);
	}

	inline constexpr auto operator ""_iu64(
		ZLiteralInteger value
	) -> IntegerU64 {
		return make_box<IntegerU64>(value);
	}

	inline constexpr auto operator ""_is8(
		ZLiteralInteger value
	) -> IntegerS8 {
		return make_box<IntegerS8>(value);
	}

	inline constexpr auto operator ""_is16(
		ZLiteralInteger value
	) -> IntegerS16 {
		return make_box<IntegerS16>(value);
	}

	inline constexpr auto operator ""_is32(
		ZLiteralInteger value
	) -> IntegerS32 {
		return make_box<IntegerS32>(value);
	}

	inline constexpr auto operator ""_is64(
		ZLiteralInteger value
	) -> IntegerS64 {
		return make_box<IntegerS64>(value);
	}

	// ----------------

	inline constexpr auto operator ""_fs32(
		ZLiteralFloater value
	) -> FloaterS32 {
		return make_box<FloaterS32>(value);
	}

	inline constexpr auto operator ""_fs64(
		ZLiteralFloater value
	) -> FloaterS64 {
		return make_box<FloaterS64>(value);
	}

	// ----------------

	inline constexpr auto operator ""_szu(
		ZLiteralInteger value
	) -> SizeU {
		return make_box<SizeU>(value);
	}

	inline constexpr auto operator ""_szs(
		ZLiteralInteger value
	) -> SizeS {
		return make_box<SizeS>(value);
	}

	// ----------------

	inline constexpr auto operator ""_i(
		ZLiteralInteger value
	) -> Integer {
		return make_box<Integer>(value);
	}

	// ----------------

	inline constexpr auto operator ""_f(
		ZLiteralFloater value
	) -> Floater {
		return make_box<Floater>(value);
	}

	// ----------------

	inline constexpr auto operator ""_sz(
		ZLiteralInteger value
	) -> Size {
		return make_box<Size>(value);
	}

	inline constexpr auto operator ""_ix(
		ZLiteralInteger value
	) -> Size {
		return make_box<Size>(value - 1);
	}

	#pragma endregion

	#pragma region utility

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsUnsignedIntegerBox<TIt>)
	inline constexpr auto unchecked_number_add(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value + that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsUnsignedIntegerBox<TIt>)
	inline constexpr auto unchecked_number_subtract(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value - that.value)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsUnsignedIntegerBox<TIt>)
	inline constexpr auto unchecked_number_multiply(
		TIt const & thix,
		TIt const & that
	) -> TIt {
		return TIt{static_cast<TIt::Value>(thix.value * that.value)};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto minimum_number_of(
	) -> TIt {
		return TIt{std::numeric_limits<typename TIt::Value>::lowest()};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsNumberBox<TIt>)
	inline constexpr auto maximum_number_of(
	) -> TIt {
		return TIt{std::numeric_limits<typename TIt::Value>::max()};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsFloaterBox<TIt>)
	inline constexpr auto infinity_number_of(
	) -> TIt {
		return TIt{std::numeric_limits<typename TIt::Value>::infinity()};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsFloaterBox<TIt>)
	inline constexpr auto invalid_number_of(
	) -> TIt {
		return TIt{std::numeric_limits<typename TIt::Value>::quiet_NaN()};
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
	inline constexpr auto type_byte_size_of(
	) -> Size {
		return Size{sizeof(TIt)};
	}

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
	inline constexpr auto type_bit_size_of(
	) -> Size {
		return Size{sizeof(TIt) * 8};
	}

	#pragma endregion

}
