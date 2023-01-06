#pragma once

#include "core/utility/trait/trait.hpp"

namespace TwinStar::Core {

	#pragma region value

	template <typename It>
	concept IsBaseWrapperValue =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsBuiltinFundamental<It> || IsBuiltinPointer<It> || IsBuiltinEnumeration<It>)
		;

	#pragma endregion

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	class BaseWrapper {

	public:

		using Value = TValue;

	public:

		Value value{};

	public:

		#pragma region structor

		constexpr ~BaseWrapper (
		) = default;

		// ----------------

		constexpr BaseWrapper (
		) = default;

		constexpr BaseWrapper (
			BaseWrapper const & that
		) = default;

		constexpr BaseWrapper (
			BaseWrapper && that
		) = default;

		// ----------------

		explicit constexpr BaseWrapper (
			Value const & value
		) :
			value{value} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BaseWrapper const & that
		) -> BaseWrapper& = default;

		constexpr auto operator = (
			BaseWrapper && that
		) -> BaseWrapper& = default;

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsBaseWrapper = IsTemplateInstanceOfT<It, BaseWrapper>;

	#pragma endregion

	#pragma region utility

	// NOTE : short-name of make_base_wrapper
	template <typename Wrapper, typename Value> requires
		CategoryConstraint<IsPureInstance<Wrapper> && IsPureInstance<Value>>
		&& (IsBaseWrapper<Wrapper>)
		&& (IsBaseWrapperValue<Value>)
	inline constexpr auto mbw (
		Value const & value
	) -> Wrapper {
		return Wrapper{static_cast<typename Wrapper::Value>(value)};
	}

	// ----------------

	// NOTE : short-name of cast_base_wrapper
	template <typename Destination, typename Source> requires
		CategoryConstraint<IsPureInstance<Destination> && IsPureInstance<Source>>
		&& (IsBaseWrapper<Destination>)
		&& (IsBaseWrapper<Source>)
	inline constexpr auto cbw (
		Source const & source
	) -> Destination {
		return Destination{static_cast<typename Destination::Value>(source.value)};
	}

	#pragma endregion

}
