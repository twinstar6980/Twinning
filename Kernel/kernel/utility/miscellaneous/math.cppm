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
		TValue const & minimum_limit,
		TValue const & maximum_limit
	) -> Boolean {
		return minimum_limit <= value && value <= maximum_limit;
	}

	#pragma endregion

	#pragma region rounding

	template <typename TResult, typename TValue> requires
		CategoryConstraint<IsPureInstance<TResult> && IsPureInstance<TValue>>
		&& (IsNumberBox<TResult>)
		&& (IsFloaterBox<TValue>)
	inline constexpr auto floor(
		TValue const & value
	) -> TResult {
		return mbox<TResult>(std::floor(value.value));
	}

	template <typename TResult, typename TValue> requires
		CategoryConstraint<IsPureInstance<TResult> && IsPureInstance<TValue>>
		&& (IsNumberBox<TResult>)
		&& (IsFloaterBox<TValue>)
	inline constexpr auto round(
		TValue const & value
	) -> TResult {
		return mbox<TResult>(std::round(value.value));
	}

	template <typename TResult, typename TValue> requires
		CategoryConstraint<IsPureInstance<TResult> && IsPureInstance<TValue>>
		&& (IsNumberBox<TResult>)
		&& (IsFloaterBox<TValue>)
	inline constexpr auto ceil(
		TValue const & value
	) -> TResult {
		return mbox<TResult>(std::ceil(value.value));
	}

	#pragma endregion

}
