#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue>>
	struct Position1D {

		using XValue = TXValue;

		// ----------------

		XValue x;

		// ----------------

		friend constexpr auto operator == (
			Position1D const & thix,
			Position1D const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue>>
	struct Position2D {

		using XValue = TXValue;

		using YValue = TYValue;

		// ----------------

		XValue x;
		YValue y;

		// ----------------

		friend constexpr auto operator == (
			Position2D const & thix,
			Position2D const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue, typename TZValue = TYValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue> && IsPureInstance<TZValue>>
	struct Position3D {

		using XValue = TXValue;

		using YValue = TYValue;

		using ZValue = TZValue;

		// ----------------

		XValue x;
		YValue y;
		ZValue z;

		// ----------------

		friend constexpr auto operator == (
			Position3D const & thix,
			Position3D const & that
		) -> bool = default;

	};

	// ----------------

	template <typename TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue>>
	struct Size1D {

		using XValue = TXValue;

		// ----------------

		XValue width;

		// ----------------

		constexpr auto area (
		) const -> XValue requires
			(IsSame<XValue, XValue>) &&
			(IsNumberWrapper<XValue>) {
			return thiz.width;
		}

		// ----------------

		friend constexpr auto operator == (
			Size1D const & thix,
			Size1D const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue>>
	struct Size2D {

		using XValue = TXValue;

		using YValue = TYValue;

		// ----------------

		XValue width;
		YValue height;

		// ----------------

		constexpr auto area (
		) const -> XValue requires
			(IsSame<XValue, XValue>) &&
			(IsSame<XValue, YValue>) &&
			(IsNumberWrapper<XValue>) {
			return thiz.width * thiz.height;
		}

		// ----------------

		friend constexpr auto operator == (
			Size2D const & thix,
			Size2D const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue, typename TZValue = TYValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue> && IsPureInstance<TZValue>>
	struct Size3D {

		using XValue = TXValue;

		using YValue = TYValue;

		using ZValue = TZValue;

		// ----------------

		XValue width;
		YValue height;
		ZValue depth;

		// ----------------

		constexpr auto area (
		) const -> XValue requires
			(IsSame<XValue, XValue>) &&
			(IsSame<XValue, YValue>) &&
			(IsSame<XValue, ZValue>) &&
			(IsNumberWrapper<XValue>) {
			return thiz.width * thiz.height * thiz.depth;
		}

		// ----------------

		friend constexpr auto operator == (
			Size3D const & thix,
			Size3D const & that
		) -> bool = default;

	};

	#pragma endregion

}
