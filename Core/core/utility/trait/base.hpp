#pragma once

#include "core/type.hpp"
#include <type_traits>
#include <concepts>
#include <tuple>

namespace TwinStar::Core::Trait {

	#pragma region constraint comment

	inline constexpr auto NoneConstraint = true;

	inline constexpr auto AutoConstraint = true;

	inline constexpr auto CustomConstraint = true;

	// ----------------

	template <auto condition = true>
	concept CategoryConstraint =
		CustomConstraint
		&& (condition)
		;

	#pragma endregion

	#pragma region utility

	template <typename It> requires
		AutoConstraint
	inline constexpr auto declare (
	) -> It {
		throw "this function can only be used in unevaluated context";
	}

	#pragma endregion

}
