module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.script.java_script.value_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Script::JavaScript {

	#pragma region type

	// NOTE: INTERFACE
	// - static auto from ( TThis & thix, TValue && value, TOption && ...option ) -> Void;
	// - static auto to   ( TThis & thix, TValue && value, TOption && ...option ) -> Void;
	template <typename TValue>
	struct ValueAdapter;

	// ----------------

	template <typename TValue>
	concept IsHasValueAdapter =
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsPureInstance<typename ValueAdapter<TValue>::That>)
		;

	#pragma endregion

}
