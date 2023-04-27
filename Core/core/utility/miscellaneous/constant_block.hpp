#pragma once

#include "core/utility/miscellaneous/byte_series/stream.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <auto t_value> requires
		CategoryConstraint<>
		&& (IsBaseWrapper<AsUnmakeConstant<decltype(t_value)>>)
	struct ConstantBlock {
	};

	#pragma endregion

}
