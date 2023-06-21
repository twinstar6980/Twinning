#pragma once

#include "core/utility/image/color.hpp"

namespace TwinStar::Core::Image {

	#pragma region type

	template <typename TColor> requires
		CategoryConstraint<IsPureInstance<TColor>>
	struct BasicPixelRGBA {

		using Color = TColor;

		// ----------------

		Color red{};
		Color green{};
		Color blue{};
		Color alpha{};

		// ----------------

		friend constexpr auto operator == (
			BasicPixelRGBA const & thix,
			BasicPixelRGBA const & that
		) -> bool = default;

	};

	#pragma endregion

}
