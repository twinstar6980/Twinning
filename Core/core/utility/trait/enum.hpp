#pragma once

#include "core/utility/trait/base.hpp"
#include "core/utility/trait/category.hpp"

namespace TwinKleS::Core::Trait {

	#pragma region concept

	template <typename It>
	concept IsEnum =
	CategoryConstraint<IsPureInstance<It>>
	&& (std::is_enum_v<It>)
	;

	#pragma endregion

	#pragma region alias

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsEnum<It>)
	using AsEnum = It;

	#pragma endregion

	#pragma region utility

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsEnum<It>)
	using AsEnumUnderlying = std::underlying_type_t<It>;

	#pragma endregion

}
