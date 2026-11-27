module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_string_stream_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel {

	#pragma region type

	// NOTE: INTERFACE
	// - static auto capacity (                     TValue && value, TOption && ...option ) -> Size;
	// - static auto write    ( TThisOutput & thix, TValue && value, TOption && ...option ) -> Void;
	// - static auto read     ( TThisInput  & thix, TValue && value, TOption && ...option ) -> Void;
	template <typename TElement, typename TValue>
	struct BasicStringStreamAdapter;

	// ----------------

	template <typename TElement, typename TValue>
	concept IsHasBasicStringStreamAdapter =
		CategoryConstraint<IsPureInstance<TElement> && IsPureInstance<TValue>>
		&& (IsPureInstance<typename BasicStringStreamAdapter<TElement, TValue>::That>)
		;

	#pragma endregion

}
