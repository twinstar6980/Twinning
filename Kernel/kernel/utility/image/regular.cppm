module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.regular;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.image.size;
import twinning.kernel.utility.image.color;
import twinning.kernel.utility.image.pixel;
import twinning.kernel.utility.image.image_view;
import twinning.kernel.utility.image.image;

export namespace Twinning::Kernel::Image {

	#pragma region alias

	using Color = BasicColor<ZIntegerU8>;

	// ----------------

	using Pixel = BasicPixelRGBA<Color>;

	// ----------------

	using VImageView = VBasicImageView<Pixel>;

	using CImageView = CBasicImageView<Pixel>;

	using Image = BasicImage<Pixel>;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_color_minimum = Color{0x00};

	inline constexpr auto k_color_maximum = Color{0xFF};

	#pragma endregion

}
