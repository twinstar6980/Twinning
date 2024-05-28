#pragma once

#include "kernel/utility/image/color.hpp"

namespace Twinning::Kernel::Image {

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
