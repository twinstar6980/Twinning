module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.binary.basic_binary_stream_adapter;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel {

	#pragma region type

	// NOTE: INTERFACE
	// - static [ constexpr ] auto static_size (                                      TOption && ...option ) -> Size;
	// - static               auto size        (                     TValue && value, TOption && ...option ) -> Size;
	// - static               auto write       ( TThisOutput & thix, TValue && value, TOption && ...option ) -> Void;
	// - static               auto read        ( TThisInput  & thix, TValue && value, TOption && ...option ) -> Void;
	template <typename TElement, typename TValue>
	struct BasicBinaryStreamAdapter;

	// ----------------

	template <typename TElement, typename TValue>
	concept IsHasBasicBinaryStreamAdapter =
		CategoryConstraint<IsPureInstance<TElement> && IsPureInstance<TValue>>
		&& (IsPureInstance<typename BasicBinaryStreamAdapter<TElement, TValue>::That>)
		;

	#pragma endregion

}
