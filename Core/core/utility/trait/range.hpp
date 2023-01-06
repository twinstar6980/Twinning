#pragma once

#include "core/utility/trait/base.hpp"
#include "core/utility/trait/category.hpp"

namespace TwinStar::Core::Trait {

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
