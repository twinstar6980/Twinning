module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.range;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename TIt>
	concept IsIterator =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsValid<decltype(*declare<TIt>())>)
		;

	template <typename TIt>
	concept IsRange =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIterator<decltype(declare<TIt>().begin())>)
		&& (IsIterator<decltype(declare<TIt>().end())>)
		;

	#pragma endregion

}
