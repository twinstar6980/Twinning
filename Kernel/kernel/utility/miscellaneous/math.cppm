module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.math;
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

}
