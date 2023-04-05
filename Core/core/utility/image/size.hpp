#pragma once

#include "core/utility/data/json/value.hpp"
#include "core/utility/miscellaneous/dimension.hpp"

namespace TwinStar::Core::Image {

	#pragma region type

	using ImageSize = Size2<Size>;

	using ImagePosition = Position2<Size>;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_begin_image_position = ImagePosition{k_begin_index, k_begin_index};

	inline constexpr auto k_none_image_size = ImageSize{k_none_size, k_none_size};

	#pragma endregion

}
