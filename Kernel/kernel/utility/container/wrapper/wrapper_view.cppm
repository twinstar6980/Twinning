module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.wrapper.wrapper_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsSameOf<t_constant, Boolean>)
	class WrapperView {

	public:

		using Value = TValue;

		inline static constexpr auto constant = Boolean{t_constant};

		using QualifyValue = AsConstantIf<Value, constant.value>;

	protected:

		Pointer<QualifyValue> m_value;

	public:

		#pragma region structor

		constexpr ~WrapperView (
		) = default;

		// ----------------

		constexpr WrapperView (
		) :
			m_value{} {
		}

		constexpr WrapperView (
			WrapperView const & that
		) = default;

		constexpr WrapperView (
			WrapperView && that
		) = default;

		// ----------------

		explicit constexpr WrapperView (
			QualifyValue & value
		) :
			m_value{&value} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			WrapperView const & that
		) -> WrapperView & = default;

		constexpr auto operator = (
			WrapperView && that
		) -> WrapperView & = default;

		#pragma endregion

		#pragma region value

		constexpr auto set (
			QualifyValue & value
		) -> QualifyValue & {
			thiz.m_value = make_pointer_of(value);
			return thiz.m_value.dereference();
		}

		// ----------------

		constexpr auto get (
		) const -> QualifyValue & {
			return thiz.m_value.dereference();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			WrapperView const & thix,
			WrapperView const & that
		) -> bool {
			return thix.get() == that.get();
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Value> requires
		AutoConstraint
	using VariableWrapperView = WrapperView<Value, k_false>;

	template <typename Value> requires
		AutoConstraint
	using ConstantWrapperView = WrapperView<Value, k_true>;

	#pragma endregion

}
