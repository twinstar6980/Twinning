#pragma once

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Kernel {

	#pragma region type

	template <typename TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue>>
	struct Position1 {

		using XValue = TXValue;

		// ----------------

		XValue x{};

		// ----------------

		friend constexpr auto operator == (
			Position1 const & thix,
			Position1 const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue>>
	struct Position2 {

		using XValue = TXValue;

		using YValue = TYValue;

		// ----------------

		XValue x{};
		YValue y{};

		// ----------------

		friend constexpr auto operator == (
			Position2 const & thix,
			Position2 const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue, typename TZValue = TYValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue> && IsPureInstance<TZValue>>
	struct Position3 {

		using XValue = TXValue;

		using YValue = TYValue;

		using ZValue = TZValue;

		// ----------------

		XValue x{};
		YValue y{};
		ZValue z{};

		// ----------------

		friend constexpr auto operator == (
			Position3 const & thix,
			Position3 const & that
		) -> bool = default;

	};

	// ----------------

	template <typename TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue>>
	struct Size1 {

		using XValue = TXValue;

		// ----------------

		XValue width{};

		// ----------------

		constexpr auto area (
		) const -> XValue requires
			(IsSame<XValue, XValue>) &&
			(IsNumberWrapper<XValue>) {
			return thiz.width;
		}

		// ----------------

		friend constexpr auto operator == (
			Size1 const & thix,
			Size1 const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue>>
	struct Size2 {

		using XValue = TXValue;

		using YValue = TYValue;

		// ----------------

		XValue width{};
		YValue height{};

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
			Size2 const & thix,
			Size2 const & that
		) -> bool = default;

	};

	template <typename TXValue, typename TYValue = TXValue, typename TZValue = TYValue> requires
		CategoryConstraint<IsPureInstance<TXValue> && IsPureInstance<TYValue> && IsPureInstance<TZValue>>
	struct Size3 {

		using XValue = TXValue;

		using YValue = TYValue;

		using ZValue = TZValue;

		// ----------------

		XValue width{};
		YValue height{};
		ZValue depth{};

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
			Size3 const & thix,
			Size3 const & that
		) -> bool = default;

	};

	#pragma endregion

}
