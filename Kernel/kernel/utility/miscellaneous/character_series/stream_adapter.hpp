#pragma once

#include "kernel/utility/container/stream/stream_view.hpp"
#include "kernel/utility/miscellaneous/character_series/container.hpp"
#include "kernel/utility/miscellaneous/character_series/type.hpp"

namespace Twinning::Kernel {

	#pragma region type

	// static [ constexpr ] auto static_size (                             Option && ...option ) -> Size;
	// static [ constexpr ] auto size        (               That && that, Option && ...option ) -> Size;
	// static               auto write       ( ThisO & thix, That && that, Option && ...option ) -> Void;
	// static               auto read        ( ThisI & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct CharacterStreamAdapter;

	#pragma endregion

}
