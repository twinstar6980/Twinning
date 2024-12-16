module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.pixel;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.image.color;

export namespace Twinning::Kernel::Image {

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

		inline friend constexpr auto operator == (
			BasicPixelRGBA const & thix,
			BasicPixelRGBA const & that
		) -> bool = default;

	};

	#pragma endregion

}
