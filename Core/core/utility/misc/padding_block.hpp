#pragma once

#include "core/utility/misc/byte_series/stream.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameV<t_size, ZSize>)
	struct PaddingBlock {
	};

	#pragma endregion

}
