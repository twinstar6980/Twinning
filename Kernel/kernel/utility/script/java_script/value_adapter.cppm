module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.script.java_script.value_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::JavaScript {

	#pragma region type

	// NOTE: INTERFACE
	// - static auto from ( TThis & thix, TThat && that, TOption && ...option ) -> Void;
	// - static auto to   ( TThis & thix, TThat && that, TOption && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
