#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TValue, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsSameV<t_constant, ZBoolean>)
	class WrapperView {

	public:

		using Value = TValue;

		inline static constexpr auto constant = ZBoolean{t_constant};

		using QValue = AsConstantIf<Value, constant>;

	protected:

		Pointer<QValue> m_value{};

	public:

		#pragma region structor

		constexpr ~WrapperView (
		) = default;

		// ----------------

		constexpr WrapperView (
		) = default;

		constexpr WrapperView (
			WrapperView const & that
		) = default;

		constexpr WrapperView (
			WrapperView && that
		) = default;

		// ----------------

		explicit constexpr WrapperView (
			QValue & value
		) :
			m_value{&value} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			WrapperView const & that
		) -> WrapperView& = default;

		constexpr auto operator = (
			WrapperView && that
		) -> WrapperView& = default;

		#pragma endregion

		#pragma region value

		constexpr auto set (
			QValue & value
		) -> QValue& {
			thiz.m_value = make_pointer(&value);
			return thiz.m_value.dereference();
		}

		// ----------------

		constexpr auto get (
		) const -> QValue& {
			return thiz.m_value.dereference();
		}

		#pragma endregion

	public:

		#pragma region operator

		friend constexpr auto operator == (
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
	using VWrapperView = WrapperView<Value, false>;

	template <typename Value> requires
		AutoConstraint
	using CWrapperView = WrapperView<Value, true>;

	#pragma endregion

}
