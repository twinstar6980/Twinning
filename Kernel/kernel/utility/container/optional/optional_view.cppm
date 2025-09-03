module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.optional.optional_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TValue, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsSameOf<t_constant, Boolean>)
	class OptionalView {

	public:

		using Value = TValue;

		inline static constexpr auto constant = Boolean{t_constant};

		using QualifyValue = AsConstantIf<Value, constant.value>;

	protected:

		Boolean m_has;

		Pointer<QualifyValue> m_value;

	public:

		#pragma region structor

		constexpr ~OptionalView (
		) = default;

		// ----------------

		constexpr OptionalView (
		) :
			m_has{},
			m_value{} {
			return;
		}

		constexpr OptionalView (
			OptionalView const & that
		) = default;

		constexpr OptionalView (
			OptionalView && that
		) = default;

		// ----------------

		explicit constexpr OptionalView (
			QualifyValue & value
		) :
			m_has{k_true},
			m_value{&value} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			OptionalView const & that
		) -> OptionalView & = default;

		constexpr auto operator = (
			OptionalView && that
		) -> OptionalView & = default;

		// ----------------

		explicit constexpr operator Boolean () const {
			return thiz.has();
		}

		explicit constexpr operator ZBoolean () const {
			return thiz.has();
		}

		#pragma endregion

		#pragma region value

		constexpr auto has (
		) const -> Boolean {
			return thiz.m_has;
		}

		// ----------------

		constexpr auto reset (
		) -> Void {
			thiz.m_has = k_false;
			thiz.m_value = k_null_pointer;
			return;
		}

		constexpr auto set (
			QualifyValue & value
		) -> QualifyValue & {
			thiz.m_has = k_true;
			thiz.m_value = make_pointer_of(value);
			return thiz.m_value.dereference();
		}

		// ----------------

		constexpr auto get (
		) const -> QualifyValue & {
			assert_test(thiz.has());
			return thiz.m_value.dereference();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			OptionalView const & thix,
			OptionalView const & that
		) -> bool {
			return thix.has() ? (that.has() && thix.get() == that.get()) : (!that.has());
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Value> requires
		AutomaticConstraint
	using VariableOptionalView = OptionalView<Value, k_false>;

	template <typename Value> requires
		AutomaticConstraint
	using ConstantOptionalView = OptionalView<Value, k_true>;

	#pragma endregion

}
