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

		#pragma region constructor

		constexpr ~Optional(
		) = default;

		// ----------------

		constexpr Optional(
		) :
			m_value{} {
			return;
		}

		constexpr Optional(
			Optional const & that
		) = default;

		constexpr Optional(
			Optional && that
		) = default;

		// ----------------

		template <typename TValueObject> requires
			CategoryConstraint<IsValid<TValueObject>>
			&& (IsSame<AsPure<TValueObject>, Value>)
		explicit constexpr Optional(
			TValueObject && value
		) :
			m_value{as_forward<TValueObject>(value)} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			Optional const & that
		) -> Optional & = default;

		constexpr auto operator =(
			Optional && that
		) -> Optional & = default;

		#pragma endregion

		#pragma region value

		constexpr auto has(
		) const -> Boolean {
			return thiz.m_value.has_value();
		}

		// ----------------

		constexpr auto reset(
		) -> Void {
			thiz.m_value.reset();
			return;
		}

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Value, TArgument && ...>)
		constexpr auto set(
			TArgument && ... argument
		) -> Value & {
			thiz.m_value.emplace(as_forward<TArgument>(argument) ...);
			return thiz.m_value.value();
		}

		// ----------------

		constexpr auto get(
		) -> Value & {
			assert_test(thiz.has());
			return thiz.m_value.value();
		}

		constexpr auto get(
		) const -> Value const & {
			assert_test(thiz.has());
			return thiz.m_value.value();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			Optional const & thix,
			Optional const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TValue, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TArgument ...>>
	inline constexpr auto make_optional(
		TArgument && ... argument
	) -> Optional<TValue> {
		auto result = Optional<TValue>{};
		result.set(as_forward<TArgument>(argument) ...);
		return result;
	}

	template <typename TValue> requires
		CategoryConstraint<IsValid<TValue>>
	inline constexpr auto make_optional_of(
		TValue && value
	) -> Optional<AsPure<TValue>> {
		return Optional<AsPure<TValue>>{as_forward<TValue>(value)};
	}

	#pragma endregion

}
