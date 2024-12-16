module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.allocation;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.null;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.range_wrapper;

export namespace Twinning::Kernel {

	#pragma region single allocate

	template <typename It, typename ... Argument> requires
		CategoryConstraint<IsInstance<It> && IsValid<Argument ...>>
		&& (IsConstructible<AsPure<It>, Argument && ...>)
	inline constexpr auto allocate_instance (
		Argument && ... argument
	) -> Pointer<It> {
		auto pointer = make_pointer(new It{as_forward<Argument>(argument) ...});
		return pointer;
	}

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
	inline constexpr auto free_instance (
		Pointer<It> & pointer
	) -> Void {
		delete pointer.value;
		pointer = k_null_pointer;
		return;
	}

	#pragma endregion

	#pragma region array allocate

	template <typename It, typename ... Argument> requires
		CategoryConstraint<IsInstance<It> && IsPureInstance<Argument ...>>
		&& (IsDefaultConstructible<AsPure<It>>)
		&& (IsConstructible<AsPure<It>, Argument const & ...>)
	inline constexpr auto allocate_instance_array (
		Size const &         size,
		Argument const & ... argument
	) -> Pointer<It> {
		auto pointer = make_pointer(new It[size.value]{});
		if constexpr (sizeof...(Argument) != 0_szz) {
			Range::restruct(Range::make_range_n(pointer, size), argument ...);
		}
		return pointer;
	}

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
	inline constexpr auto free_instance_array (
		Pointer<It> & pointer
	) -> Void {
		delete[] pointer.value;
		pointer = k_null_pointer;
		return;
	}

	#pragma endregion

}
