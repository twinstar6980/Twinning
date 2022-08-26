#pragma once

#include "core/base_type.hpp"
#include <type_traits>
#include <concepts>
#include <tuple>

namespace TwinKleS::Core::Trait {

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

	// ReSharper disable once CppFunctionIsNotImplemented
	template <typename It> requires
		AutoConstraint
	inline constexpr auto declare (
	) -> It;

	#pragma endregion

}
