#pragma once

#include "core/utility/base_wrapper/base.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	class BooleanWrapper :
		public BaseWrapper<TValue> {

	private:

		using BaseWrapper = BaseWrapper<TValue>;

	public:

		using typename BaseWrapper::Value;

	public:

		#pragma region structor

		constexpr ~BooleanWrapper (
		) = default;

		// ----------------

		constexpr BooleanWrapper (
		) = default;

		constexpr BooleanWrapper (
			BooleanWrapper const & that
		) = default;

		constexpr BooleanWrapper (
			BooleanWrapper && that
		) = default;

		// ----------------

		implicit constexpr BooleanWrapper (
			Value const & value
		) :
			BaseWrapper{value} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BooleanWrapper const & that
		) -> BooleanWrapper& = default;

		constexpr auto operator = (
			BooleanWrapper && that
		) -> BooleanWrapper& = default;

		// ----------------

		implicit constexpr operator Value & () {
			return thiz.value;
		}

		implicit constexpr operator Value const & () const {
			return thiz.value;
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsBooleanWrapper = IsTemplateInstanceOfT<It, BooleanWrapper>;

	#pragma endregion

	#pragma region operator

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsBooleanWrapper<It>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
		&& (IsDerivedFrom<It, BooleanWrapper<typename It::Value>>)
	#endif
	inline constexpr auto operator == (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value == that.value;
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsBooleanWrapper<It>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
		&& (IsDerivedFrom<It, BooleanWrapper<typename It::Value>>)
	#endif
	inline constexpr auto operator ! (
		It const & thix
	) -> It {
		return It{!thix.value};
	}

	#pragma endregion

	#pragma region alias

	using Boolean8 = BooleanWrapper<ZBoolean8>;

	#pragma endregion

	#pragma region regular type

	M_define_simple_derived_class(Boolean, Boolean8, Boolean8);

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_true = Boolean{mbw<Boolean>(true)};

	inline constexpr auto k_false = Boolean{mbw<Boolean>(false)};

	#pragma endregion

}
