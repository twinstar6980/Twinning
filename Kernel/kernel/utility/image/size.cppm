module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.size;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.miscellaneous.dimension;

export namespace Twinning::Kernel::Image {

	#pragma region type

	using ImageSize = Size2<Size>;

	using ImagePosition = Position2<Size>;

	#pragma endregion

	#pragma region constant

	inline constexpr auto k_begin_image_position = ImagePosition{k_begin_index, k_begin_index};

	inline constexpr auto k_none_image_size = ImageSize{k_none_size, k_none_size};

	#pragma endregion

}
