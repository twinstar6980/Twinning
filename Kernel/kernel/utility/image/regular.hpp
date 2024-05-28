#pragma once

#include "kernel/utility/image/image.hpp"

namespace Twinning::Kernel::Image {

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
