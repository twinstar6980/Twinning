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

		#pragma region structor

		constexpr ~Wrapper (
		) = default;

		// ----------------

		constexpr Wrapper (
		) :
			m_value{} {
		}

		constexpr Wrapper (
			Wrapper const & that
		) = default;

		constexpr Wrapper (
			Wrapper && that
		) = default;

		// ----------------

		template <typename ValueObject> requires
			CategoryConstraint<IsValid<ValueObject>>
			&& (IsSame<AsPure<ValueObject>, Value>)
		explicit constexpr Wrapper (
			ValueObject && value
		) :
			m_value{as_forward<ValueObject>(value)} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			Wrapper const & that
		) -> Wrapper & = default;

		constexpr auto operator = (
			Wrapper && that
		) -> Wrapper & = default;

		#pragma endregion

		#pragma region value

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Value, Argument && ...>)
		constexpr auto set (
			Argument && ... argument
		) -> Value & {
			restruct(thiz.m_value, as_forward<Argument>(argument) ...);
			return thiz.m_value;
		}

		// ----------------

		constexpr auto get (
		) -> Value & {
			return thiz.m_value;
		}

		constexpr auto get (
		) const -> Value const & {
			return thiz.m_value;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			Wrapper const & thix,
			Wrapper const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Value, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Value> && IsValid<Argument ...>>
	inline constexpr auto make_wrapper (
		Argument && ... argument
	) -> Wrapper<Value> {
		auto result = Wrapper<Value>{};
		result.set(as_forward<Argument>(argument) ...);
		return result;
	}

	template <typename Value> requires
		CategoryConstraint<IsValid<Value>>
	inline constexpr auto make_wrapper_of (
		Value && value
	) -> Wrapper<AsPure<Value>> {
		return Wrapper<AsPure<Value>>{as_forward<Value>(value)};
	}

	#pragma endregion

}
