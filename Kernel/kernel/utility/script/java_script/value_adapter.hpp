#pragma once

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Kernel::JavaScript {

	#pragma region type

	// static auto from ( This & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
