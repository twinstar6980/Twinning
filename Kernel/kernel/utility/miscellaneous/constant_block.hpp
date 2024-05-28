#pragma once

#include "kernel/utility/miscellaneous/byte_series/stream.hpp"

namespace Twinning::Kernel {

	#pragma region type

	template <auto t_value> requires
		CategoryConstraint<>
		&& (IsBaseWrapper<AsUnmakeConstant<decltype(t_value)>>)
	struct ConstantBlock {

	};

	#pragma endregion

}
