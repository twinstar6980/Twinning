module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.byte_series.stream_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	// NOTE: INTERFACE
	// - static [ constexpr ] auto static_size (                                    TOption && ...option ) -> Size;
	// - static [ constexpr ] auto size        (                     TThat && that, TOption && ...option ) -> Size;
	// - static               auto write       ( TThisOutput & thix, TThat && that, TOption && ...option ) -> Void;
	// - static               auto read        ( TThisInput  & thix, TThat && that, TOption && ...option ) -> Void;
	template <typename TThat>
	struct ByteStreamAdapter;

	#pragma endregion

}
