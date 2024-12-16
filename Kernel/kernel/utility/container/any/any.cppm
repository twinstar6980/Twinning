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

		#pragma region structor

		~Any (
		) = default;

		// ----------------

		Any (
		) :
			m_value{} {
		}

		Any (
			Any const & that
		) = default;

		Any (
			Any && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Any const & that
		) -> Any & = default;

		auto operator = (
			Any && that
		) -> Any & = default;

		#pragma endregion

		#pragma region value

		auto has (
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_value.has_value());
		}

		template <typename ValueObject> requires
			CategoryConstraint<IsPureInstance<ValueObject>>
		auto is (
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_value.type() == typeid(ValueObject));
		}

		// ----------------

		auto reset (
		) -> Void {
			thiz.m_value.reset();
			return;
		}

		template <typename ValueObject, typename ... Argument> requires
			CategoryConstraint<IsPureInstance<ValueObject> && IsValid<Argument ...>>
			&& (IsConstructible<ValueObject, Argument && ...>)
		auto set (
			Argument && ... argument
		) -> ValueObject & {
			thiz.m_value.emplace<ValueObject>(as_forward<Argument>(argument) ...);
			return *std::any_cast<ValueObject>(&thiz.m_value);
		}

		// ----------------

		template <typename ValueObject> requires
			CategoryConstraint<IsPureInstance<ValueObject>>
		auto get (
		) -> ValueObject & {
			assert_test(thiz.is<ValueObject>());
			return *std::any_cast<ValueObject>(&thiz.m_value);
		}

		template <typename ValueObject> requires
			CategoryConstraint<IsPureInstance<ValueObject>>
		auto get (
		) const -> ValueObject const & {
			assert_test(thiz.is<ValueObject>());
			return *std::any_cast<ValueObject>(&thiz.m_value);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Value, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Value> && IsValid<Argument ...>>
	inline auto make_any (
		Argument && ... argument
	) -> Any {
		auto result = Any{};
		result.set<Value>(as_forward<Argument>(argument) ...);
		return result;
	}

	template <typename Value> requires
		CategoryConstraint<IsValid<Value>>
	inline auto make_any_of (
		Value && value
	) -> Any {
		return make_any<AsPure<Value>>(as_forward<Value>(value));
	}

	#pragma endregion

}
