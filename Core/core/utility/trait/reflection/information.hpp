#pragma once

#include "core/utility/trait/base.hpp"
#include "core/utility/trait/category.hpp"
#include "core/utility/trait/callable.hpp"

namespace TwinKleS::Core::Trait::Reflection {

	#pragma region type

	template <typename TType>
	struct Information;

	#pragma endregion

	#pragma region utility

	template <typename It>
	concept IsRegisted =
	CategoryConstraint<IsPureInstance<It>>
	&& (IsPureInstance<typename Information<It>::Type>)
	;

	template <typename It>
	concept IsRegistedClass =
	CategoryConstraint<IsPureInstance<It>>
	&& (IsPureInstance<typename Information<It>::Type>)
	&& (IsTypePackage<typename Information<It>::MemberVariable>)
	&& (IsTypePackage<typename Information<It>::MemberFunction>)
	&& (IsTypePackage<typename Information<It>::StaticVariable>)
	&& (IsTypePackage<typename Information<It>::StaticFunction>)
	;

	template <typename It>
	concept IsRegistedEnum =
	CategoryConstraint<IsPureInstance<It>>
	&& (IsPureInstance<typename Information<It>::Type>)
	&& (IsTypePackage<typename Information<It>::EnumValue>)
	;

	#pragma endregion

}
