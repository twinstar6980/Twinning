#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"
#include "core/utility/misc/number_variant.hpp"
#include "core/utility/misc/enum.hpp"
#include "core/utility/file_system/path.hpp"
#include "core/utility/container/variant/variant.hpp"
#include "core/utility/container/variant/enumerable_variant.hpp"
#include "core/utility/container/list/list.hpp"
#include "core/utility/container/map/map.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/misc/fourcc.hpp"

namespace TwinKleS::Core::JSON {

	#pragma region type

	// static auto from ( This       & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This const & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
