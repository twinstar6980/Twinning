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

	template <typename TIt, typename ... TArgument> requires
		CategoryConstraint<IsInstance<TIt> && IsValid<TArgument ...>>
		&& (IsConstructible<AsPure<TIt>, TArgument && ...>)
	inline constexpr auto allocate_instance(
		TArgument && ... argument
	) -> Pointer<TIt> {
		auto pointer = make_pointer(new TIt{as_forward<TArgument>(argument) ...});
		return pointer;
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto free_instance(
		Pointer<TIt> & pointer
	) -> Void {
		delete pointer.value;
		pointer = k_null_pointer;
		return;
	}

	#pragma endregion

	#pragma region array allocate

	template <typename TIt, typename ... TArgument> requires
		CategoryConstraint<IsInstance<TIt> && IsPureInstance<TArgument ...>>
		&& (IsDefaultConstructible<AsPure<TIt>>)
		&& (IsConstructible<AsPure<TIt>, TArgument const & ...>)
	inline constexpr auto allocate_instance_array(
		Size const &          size,
		TArgument const & ... argument
	) -> Pointer<TIt> {
		auto pointer = make_pointer(new TIt[size.value]{});
		if constexpr (sizeof...(TArgument) != 0_szz) {
			Range::restruct(Range::make_range_n(pointer, size), argument ...);
		}
		return pointer;
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto free_instance_array(
		Pointer<TIt> & pointer
	) -> Void {
		delete[] pointer.value;
		pointer = k_null_pointer;
		return;
	}

	#pragma endregion

}
