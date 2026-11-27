module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.basic_pixel;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.image.basic_color;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TColor> requires
		CategoryConstraint<IsPureInstance<TColor>>
	struct BasicPixelRgba {

		using Color = TColor;

		// ----------------

		Color red{};

		Color green{};

		Color blue{};

		Color alpha{};

		// ----------------

		inline friend constexpr auto operator ==(
			BasicPixelRgba const & thix,
			BasicPixelRgba const & that
		) -> bool = default;

	};

	#pragma endregion

}
