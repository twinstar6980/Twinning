#pragma once

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace Twinning::Kernel::JSON {

	#pragma region type

	// static auto from ( This       & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This const & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
