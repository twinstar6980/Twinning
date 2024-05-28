#pragma once

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace Twinning::Kernel {

	#pragma region type

	// static auto from ( This       & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This const & thix, That && that, Option && ...option ) -> Void;
	template <typename TElement, typename TThat>
	struct BasicStringAdapter;

	#pragma endregion

}
