#pragma once

#include "core/utility/container/wrapper/wrapper.hpp"
#include "core/utility/null.hpp"
#include "core/utility/misc/byte_series/container.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/data/json/value.hpp"

namespace TwinKleS::Core::JS {

	#pragma region type

	// static auto from ( This & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
