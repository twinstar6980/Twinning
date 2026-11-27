module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.any;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.box;

export namespace Twinning::Kernel {

	#pragma region type

	class Any {

	protected:

		std::any m_container;

	public:

		#pragma region constructor

		~Any(
		) = default;

		// ----------------

		Any(
		) :
			m_container{} {
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
			return make_box<Boolean>(thiz.m_container.has_value());
		}

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
		auto is(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_container.type() == typeid(TValueObject));
		}

		// ----------------

		auto reset(
		) -> Void {
			thiz.m_container.reset();
			return;
		}

		template <typename TValueObject, typename ... TArgument> requires
			CategoryConstraint<IsPureInstance<TValueObject> && IsValid<TArgument ...>>
			&& (IsConstructible<TValueObject, TArgument && ...>)
		auto set(
			TArgument && ... argument
		) -> TValueObject & {
			thiz.m_container.emplace<TValueObject>(as_forward<TArgument>(argument) ...);
			return *std::any_cast<TValueObject>(&thiz.m_container);
		}

		// ----------------

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
		auto get(
		) -> TValueObject & {
			assert_test(thiz.is<TValueObject>());
			return *std::any_cast<TValueObject>(&thiz.m_container);
		}

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
		auto get(
		) const -> TValueObject const & {
			assert_test(thiz.is<TValueObject>());
			return *std::any_cast<TValueObject>(&thiz.m_container);
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
