#pragma once

#include "core/utility/base_wrapper/base.hpp"
#include "core/utility/base_wrapper/number.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	M_define_simple_derived_class(EnumeratedWrapper, IntegerWrapper<TValue>, IntegerWrapper);

	// ----------------

	template <typename It>
	concept IsEnumeratedWrapper = IsTemplateInstanceOfT<It, EnumeratedWrapper>;

	#pragma endregion

	#pragma region alias

	using Enumerated8 = EnumeratedWrapper<ZIntegerU8>;

	#pragma endregion

	#pragma region regular type

	M_define_simple_derived_class(Enumerated, Enumerated8, Enumerated8);

	#pragma endregion

	#pragma region literal

	inline constexpr auto operator ""_e8 (
		ZLiteralInteger value
	) -> Enumerated8 {
		return mbw<Enumerated8>(value);
	}

	// ----------------

	inline constexpr auto operator ""_e (
		ZLiteralInteger value
	) -> Enumerated {
		return mbw<Enumerated>(value);
	}

	#pragma endregion

}
