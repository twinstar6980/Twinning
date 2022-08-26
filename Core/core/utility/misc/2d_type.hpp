#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <typename TNumber> requires
		CategoryConstraint<IsPureInstance<TNumber>>
		&& (IsNumberWrapper<TNumber>)
	struct Position2D {

		using Number = TNumber;

		// ----------------

		Number x;
		Number y;

		// ----------------

		friend constexpr auto operator == (
			Position2D const & thix,
			Position2D const & that
		) -> bool = default;

		// ----------------

		friend constexpr auto operator < (
			Position2D const & thix,
			Position2D const & that
		) -> Boolean {
			return thix.y < that.y || (thix.y == that.y && thix.x < that.x);
		}

		friend constexpr auto operator <= (
			Position2D const & thix,
			Position2D const & that
		) -> Boolean {
			return thix.y < that.y || (thix.y == that.y && thix.x <= that.x);
		}

	};

	template <typename TNumber> requires
		CategoryConstraint<IsPureInstance<TNumber>>
		&& (IsNumberWrapper<TNumber>)
	struct Size2D {

		using Number = TNumber;

		// ----------------

		Number width;
		Number height;

		// ----------------

		constexpr auto area (
		) const -> Number {
			return thiz.width * thiz.height;
		}

		// ----------------

		friend constexpr auto operator == (
			Size2D const & thix,
			Size2D const & that
		) -> bool = default;

	};

	#pragma endregion

}
