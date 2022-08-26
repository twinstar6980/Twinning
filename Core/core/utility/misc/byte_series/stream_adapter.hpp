#pragma once

#include "core/utility/container/stream/stream_view.hpp"
#include "core/utility/misc/byte_series/container.hpp"
#include "core/utility/misc/byte_series/utility.hpp"

namespace TwinKleS::Core {

	#pragma region type

	// static [ constexpr ] auto static_size (                             Option && ...option ) -> Size;
	// static [ constexpr ] auto size        (               That && that, Option && ...option ) -> Size;
	// static               auto write       ( ThisO & thix, That && that, Option && ...option ) -> Void;
	// static               auto read        ( ThisI & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ByteStreamAdapter;

	#pragma endregion

}
