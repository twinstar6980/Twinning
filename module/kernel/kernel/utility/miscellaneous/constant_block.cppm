module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.constant_block;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <auto t_value> requires
		CategoryConstraint<>
		&& (IsBaseBox<AsUnmakeConstant<decltype(t_value)>>)
	struct ConstantBlock {

	};

	#pragma endregion

}
