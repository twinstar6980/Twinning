module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.data.json.value_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::JSON {

	#pragma region type

	// static auto from ( This       & thix, That && that, Option && ...option ) -> Void;
	// static auto to   ( This const & thix, That && that, Option && ...option ) -> Void;
	template <typename TThat>
	struct ValueAdapter;

	#pragma endregion

}
