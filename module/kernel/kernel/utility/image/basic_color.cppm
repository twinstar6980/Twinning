module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.basic_color;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<>
		&& (IsBaseBoxValue<TValue>)
	M_simple_derived_class(BasicColor, IntegerBox<TValue>, IntegerBox);

	#pragma endregion

}
