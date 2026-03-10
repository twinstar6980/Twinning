module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.base;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.exception.exception;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Trait {

	#pragma region constraint comment

	// ReSharper disable CppInconsistentNaming

	inline constexpr auto NoneConstraint = true;

	inline constexpr auto CustomConstraint = true;

	inline constexpr auto AutomaticConstraint = true;

	// ----------------

	template <auto t_condition = true>
	concept CategoryConstraint =
		CustomConstraint
		&& (t_condition)
		;

	// ReSharper restore CppInconsistentNaming

	#pragma endregion

	#pragma region utility

	template <typename> requires
		NoneConstraint
	inline constexpr auto k_static_assert_fail = bool{false};

	// ----------------

	template <typename TIt> requires
		NoneConstraint
	inline constexpr auto declare(
	) -> TIt {
		throw UnreachableException{};
	}

	#pragma endregion

}
