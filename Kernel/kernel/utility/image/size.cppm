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

}
