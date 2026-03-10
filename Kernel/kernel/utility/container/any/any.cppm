module;

#include <typeinfo>
#include "kernel/common.hpp"

export module twinning.kernel.utility.container.any.any;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	class Any {

	protected:

		std::any m_value;

	public:

		#pragma region constructor

		~Any(
		) = default;

		// ----------------

		Any(
		) :
			m_value{} {
			return;
		}

		Any(
			Any const & that
		) = default;

		Any(
			Any && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator =(
			Any const & that
		) -> Any & = default;

		auto operator =(
			Any && that
		) -> Any & = default;

		#pragma endregion

		#pragma region value

		auto has(
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_value.has_value());
		}

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
		auto is(
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_value.type() == typeid(TValueObject));
		}

		// ----------------

		auto reset(
		) -> Void {
			thiz.m_value.reset();
			return;
		}

		template <typename TValueObject, typename ... TArgument> requires
			CategoryConstraint<IsPureInstance<TValueObject> && IsValid<TArgument ...>>
			&& (IsConstructible<TValueObject, TArgument && ...>)
		auto set(
			TArgument && ... argument
		) -> TValueObject & {
			thiz.m_value.emplace<TValueObject>(as_forward<TArgument>(argument) ...);
			return *std::any_cast<TValueObject>(&thiz.m_value);
		}

		// ----------------

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
		auto get(
		) -> TValueObject & {
			assert_test(thiz.is<TValueObject>());
			return *std::any_cast<TValueObject>(&thiz.m_value);
		}

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
		auto get(
		) const -> TValueObject const & {
			assert_test(thiz.is<TValueObject>());
			return *std::any_cast<TValueObject>(&thiz.m_value);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TValue, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TValue> && IsValid<TArgument ...>>
	inline auto make_any(
		TArgument && ... argument
	) -> Any {
		auto result = Any{};
		result.set<TValue>(as_forward<TArgument>(argument) ...);
		return result;
	}

	template <typename TValue> requires
		CategoryConstraint<IsValid<TValue>>
	inline auto make_any_of(
		TValue && value
	) -> Any {
		return make_any<AsPure<TValue>>(as_forward<TValue>(value));
	}

	#pragma endregion

}
