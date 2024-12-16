module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.range;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename It>
	concept IsIterator =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsValid<decltype(*declare<It>())>)
		;

	template <typename It>
	concept IsRange =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIterator<decltype(declare<It>().begin())>)
		&& (IsIterator<decltype(declare<It>().end())>)
		;

	#pragma endregion

}
