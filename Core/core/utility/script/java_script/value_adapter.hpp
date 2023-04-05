#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Core::JavaScript {

	#pragma region type

	// static auto from ( This & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
