module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.math;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Math {

	#pragma region check

	template <typename Value> requires
		CategoryConstraint<IsPureInstance<Value>>
		&& (IsNumberBox<Value>)
	inline constexpr auto between (
		Value const & value,
		Value const & minimum_limit,
		Value const & maximum_limit
	) -> Boolean {
		return minimum_limit <= value && value <= maximum_limit;
	}

	#pragma endregion

	#pragma region rounding

	template <typename Result, typename Value> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Value>>
		&& (IsNumberBox<Result>)
		&& (IsFloaterBox<Value>)
	inline constexpr auto floor (
		Value const & value
	) -> Result {
		return mbox<Result>(std::floor(value.value));
	}

	template <typename Result, typename Value> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Value>>
		&& (IsNumberBox<Result>)
		&& (IsFloaterBox<Value>)
	inline constexpr auto round (
		Value const & value
	) -> Result {
		return mbox<Result>(std::round(value.value));
	}

	template <typename Result, typename Value> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Value>>
		&& (IsNumberBox<Result>)
		&& (IsFloaterBox<Value>)
	inline constexpr auto ceil (
		Value const & value
	) -> Result {
		return mbox<Result>(std::ceil(value.value));
	}

	#pragma endregion

}
