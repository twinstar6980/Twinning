#pragma once

#include "core/utility/image/basic_bitmap.hpp"

namespace TwinStar::Core::Image {

	#pragma region alias

	using VBitmapView = BasicBitmapView<Pixel, false>;

	using CBitmapView = BasicBitmapView<Pixel, true>;

	using Bitmap = BasicBitmap<Pixel>;

	#pragma endregion

}
