module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	class Optional {

	public:

		using Value = TValue;

	protected:

		std::optional<Value> m_value;

	public:

		#pragma region structor

		constexpr ~Optional (
		) = default;

		// ----------------

		constexpr Optional (
		) :
			m_value{} {
		}

		constexpr Optional (
			Optional const & that
		) = default;

		constexpr Optional (
			Optional && that
		) = default;

		// ----------------

		template <typename ValueObject> requires
			CategoryConstraint<IsValid<ValueObject>>
			&& (IsSame<AsPure<ValueObject>, Value>)
		explicit constexpr Optional (
			ValueObject && value
		) :
			m_value{as_forward<ValueObject>(value)} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			Optional const & that
		) -> Optional & = default;

		constexpr auto operator = (
			Optional && that
		) -> Optional & = default;

		#pragma endregion

		#pragma region value

		constexpr auto has (
		) const -> Boolean {
			return thiz.m_value.has_value();
		}

		// ----------------

		constexpr auto reset (
		) -> Void {
			thiz.m_value.reset();
			return;
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Value, Argument && ...>)
		constexpr auto set (
			Argument && ... argument
		) -> Value & {
			thiz.m_value.emplace(as_forward<Argument>(argument) ...);
			return thiz.m_value.value();
		}

		// ----------------

		constexpr auto get (
		) -> Value & {
			assert_test(thiz.has());
			return thiz.m_value.value();
		}

		constexpr auto get (
		) const -> Value const & {
			assert_test(thiz.has());
			return thiz.m_value.value();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			Optional const & thix,
			Optional const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Value, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Value> && IsValid<Argument ...>>
	inline constexpr auto make_optional (
		Argument && ... argument
	) -> Optional<Value> {
		auto result = Optional<Value>{};
		result.set(as_forward<Argument>(argument) ...);
		return result;
	}

	template <typename Value> requires
		CategoryConstraint<IsValid<Value>>
	inline constexpr auto make_optional_of (
		Value && value
	) -> Optional<AsPure<Value>> {
		return Optional<AsPure<Value>>{as_forward<Value>(value)};
	}

	#pragma endregion

}
