#pragma once

#include "core/utility/miscellaneous/byte_series/stream.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameV<t_size, Size>)
	struct PaddingBlock {
	};

	#pragma endregion

}
