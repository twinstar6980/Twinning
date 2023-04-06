#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TValue, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsSameV<t_constant, ZBoolean>)
	class OptionalView {

	public:

		using Value = TValue;

		inline static constexpr auto constant = ZBoolean{t_constant};

		using QValue = AsConstantIf<Value, constant>;

	protected:

		Boolean         m_has{k_false};
		Pointer<QValue> m_value{k_null_pointer};

	public:

		#pragma region structor

		constexpr ~OptionalView (
		) = default;

		// ----------------

		constexpr OptionalView (
		) = default;

		constexpr OptionalView (
			OptionalView const & that
		) = default;

		constexpr OptionalView (
			OptionalView && that
		) = default;

		// ----------------

		explicit constexpr OptionalView (
			QValue & value
		) :
			m_has{k_true},
			m_value{&value} {
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
			QValue & value
		) -> QValue & {
			thiz.m_has = k_true;
			thiz.m_value = make_pointer(&value);
			return thiz.m_value.dereference();
		}

		// ----------------

		constexpr auto get (
		) const -> QValue & {
			assert_test(thiz.has());
			return thiz.m_value.dereference();
		}

		#pragma endregion

	public:

		#pragma region operator

		friend constexpr auto operator == (
			OptionalView const & thix,
			OptionalView const & that
		) -> bool {
			return thix.has() ? (Boolean{that.has() && thix.get() == that.get()}) : (!that.has());
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Value> requires
		AutoConstraint
	using VOptionalView = OptionalView<Value, false>;

	template <typename Value> requires
		AutoConstraint
	using COptionalView = OptionalView<Value, true>;

	#pragma endregion

}
