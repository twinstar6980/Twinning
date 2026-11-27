module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.math.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Math {

	#pragma region check

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto is_finite(
		TValue const & value
	) -> Boolean {
		return std::isfinite(unmake_box<typename TValue::Value>(value));
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto is_infinity(
		TValue const & value
	) -> Boolean {
		return std::isinf(unmake_box<typename TValue::Value>(value));
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto is_invalid(
		TValue const & value
	) -> Boolean {
		return std::isnan(unmake_box<typename TValue::Value>(value));
	}

	// ----------------

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto extract(
		TValue const & value,
		TValue &       significand,
		Integer &      exponent
	) -> Void {
		auto exponent_value = int{};
		auto significand_value = std::frexp(unmake_box<typename TValue::Value>(value), &exponent_value);
		significand = make_box<TValue>(significand_value);
		exponent = make_box<Integer>(exponent_value);
		return;
	}

	// ----------------

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

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto ceil(
		TValue const & value
	) -> TValue {
		return make_box<TValue>(std::ceil(unmake_box<typename TValue::Value>(value)));
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsFloaterBox<TValue>)
	inline constexpr auto floor(
		TValue const & value
	) -> TValue {
		return make_box<TValue>(std::floor(unmake_box<typename TValue::Value>(value)));
	}

	#pragma endregion

	#pragma region padding

	inline constexpr auto is_padded_size(
		Size const & value,
		Size const & unit
	) -> Boolean {
		assert_test(unit != 0_sz);
		return value % unit == 0_sz;
	}

	inline constexpr auto compute_padded_size(
		Size const & value,
		Size const & unit
	) -> Size {
		assert_test(unit != 0_sz);
		return is_padded_size(value, unit) ? (value) : ((value / unit + 1_sz) * unit);
	}

	inline constexpr auto compute_padding_size(
		Size const & value,
		Size const & unit
	) -> Size {
		assert_test(unit != 0_sz);
		return compute_padded_size(value, unit) - value;
	}

	inline constexpr auto compute_padded_unit_count(
		Size const& value,
		Size const& unit
	) -> Size {
		assert_test(unit != 0_sz);
		return is_padded_size(value, unit) ? (value / unit) : (value / unit + 1_sz);
	}

	// ----------------

	inline constexpr auto is_padded_size_of_power_of_two(
		Size const & value
	) -> Boolean {
		return (value != 0_sz) && ((value & (value - 1_sz)) == 0_sz);
	}

	#pragma endregion

}
