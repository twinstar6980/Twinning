#pragma once

#include "kernel/utility/trait/base.hpp"
#include "kernel/utility/trait/category.hpp"

namespace TwinStar::Kernel::Trait {

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
