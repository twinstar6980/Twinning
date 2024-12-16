module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.character_series.stream_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	// static [ constexpr ] auto static_size (                             Option && ...option ) -> Size;
	// static [ constexpr ] auto size        (               That && that, Option && ...option ) -> Size;
	// static               auto write       ( ThisO & thix, That && that, Option && ...option ) -> Void;
	// static               auto read        ( ThisI & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct CharacterStreamAdapter;

	#pragma endregion

}
