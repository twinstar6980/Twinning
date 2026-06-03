module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.math.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Math {

	#pragma region check

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsNumberBox<TValue>)
	inline constexpr auto between(
		TValue const & value,
		TValue const & minimum,
		TValue const & maximum
	) -> Boolean {
		return minimum <= value && value <= maximum;
	}

	// ----------------

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsNumberBox<TValue>)
	inline constexpr auto minimum(
		TValue const & value_1,
		TValue const & value_2
	) -> TValue const & {
		return value_1 < value_2 ? (value_1) : (value_2);
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsNumberBox<TValue>)
	inline constexpr auto maximum(
		TValue const & value_1,
		TValue const & value_2
	) -> TValue const & {
		return value_1 > value_2 ? (value_1) : (value_2);
	}

	#pragma endregion

	#pragma region round

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto round(
		TValue const & value
	) -> TValue {
		return make_box<TValue>(std::round(unmake_box<typename TValue::Value>(value)));
	}

	#pragma endregion

	#pragma region padding

	inline constexpr auto is_padded_size(
		Size const & value,
		Size const & unit
	) -> Boolean {
		return value % unit == 0_sz;
	}

	inline constexpr auto compute_padded_size(
		Size const & value,
		Size const & unit
	) -> Size {
		return is_padded_size(value, unit) ? (value) : ((value / unit + 1_sz) * unit);
	}

	inline constexpr auto compute_padding_size(
		Size const & value,
		Size const & unit
	) -> Size {
		return compute_padded_size(value, unit) - value;
	}

	// ----------------

	inline constexpr auto is_padded_size_of_exponent_of_2(
		Size const & value
	) -> Boolean {
		return (value != 0_sz) && ((value & (value - 1_sz)) == 0_sz);
	}

	#pragma endregion

}
