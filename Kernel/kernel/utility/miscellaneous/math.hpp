#pragma once

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace Twinning::Kernel::Math {

	#pragma region check

	template <typename Value> requires
		CategoryConstraint<IsPureInstance<Value>>
		&& (IsNumberWrapper<Value>)
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
		&& (IsNumberWrapper<Result>)
		&& (IsFloaterWrapper<Value>)
	inline constexpr auto floor (
		Value const & value
	) -> Result {
		return mbw<Result>(std::floor(value.value));
	}

	template <typename Result, typename Value> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Value>>
		&& (IsNumberWrapper<Result>)
		&& (IsFloaterWrapper<Value>)
	inline constexpr auto round (
		Value const & value
	) -> Result {
		return mbw<Result>(std::round(value.value));
	}

	template <typename Result, typename Value> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Value>>
		&& (IsNumberWrapper<Result>)
		&& (IsFloaterWrapper<Value>)
	inline constexpr auto ceil (
		Value const & value
	) -> Result {
		return mbw<Result>(std::ceil(value.value));
	}

	#pragma endregion

}
