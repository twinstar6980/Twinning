#pragma once

#include "core/utility/container/list/list.hpp"
#include "core/utility/misc/character_series/type.hpp"
#include "core/utility/string/basic_string_view.hpp"

namespace TwinKleS::Core {

	#pragma region type

	// static auto from ( This       & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This const & thix, That && that, Option && ...option ) -> Void;
	template <typename TElement, typename TThat>
	struct BasicStringAdapter;

	#pragma endregion

}
