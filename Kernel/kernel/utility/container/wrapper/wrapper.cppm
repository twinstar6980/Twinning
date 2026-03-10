module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.wrapper.wrapper;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	class Wrapper {

	public:

		using Value = TValue;

	protected:

		Value m_value;

	public:

		#pragma region constructor

		constexpr ~Wrapper(
		) = default;

		// ----------------

		constexpr Wrapper(
		) :
			m_value{} {
			return;
		}

		constexpr Wrapper(
			Wrapper const & that
		) = default;

		constexpr Wrapper(
			Wrapper && that
		) = default;

		// ----------------

		template <typename TValueObject> requires
			CategoryConstraint<IsValid<TValueObject>>
			&& (IsSame<AsPure<TValueObject>, Value>)
		explicit constexpr Wrapper(
			TValueObject && value
		) :
			m_value{as_forward<TValueObject>(value)} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			Wrapper const & that
		) -> Wrapper & = default;

		constexpr auto operator =(
			Wrapper && that
		) -> Wrapper & = default;

		#pragma endregion

		#pragma region value

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Value, TArgument && ...>)
		constexpr auto set(
			TArgument && ... argument
		) -> Value & {
			restruct(thiz.m_value, as_forward<TArgument>(argument) ...);
			return thiz.m_value;
		}

		// ----------------

		constexpr auto get(
		) -> Value & {
			return thiz.m_value;
		}

		constexpr auto get(
		) const -> Value const & {
			return thiz.m_value;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			Wrapper const & thix,
			Wrapper const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TValue, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TArgument ...>>
	inline constexpr auto make_wrapper(
		TArgument && ... argument
	) -> Wrapper<TValue> {
		auto result = Wrapper<TValue>{};
		result.set(as_forward<TArgument>(argument) ...);
		return result;
	}

	template <typename TValue> requires
		CategoryConstraint<IsValid<TValue>>
	inline constexpr auto make_wrapper_of(
		TValue && value
	) -> Wrapper<AsPure<TValue>> {
		return Wrapper<AsPure<TValue>>{as_forward<TValue>(value)};
	}

	#pragma endregion

}
