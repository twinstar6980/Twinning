module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.notation.json.value_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Notation::Json {

	#pragma region type

	// NOTE: INTERFACE
	// - static auto from ( TThis       & thix, TValue && value, TOption && ...option ) -> Void;
	// - static auto to   ( TThis const & thix, TValue && value, TOption && ...option ) -> Void;
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
