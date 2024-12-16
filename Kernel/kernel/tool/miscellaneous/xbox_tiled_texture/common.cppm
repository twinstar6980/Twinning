module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.xbox_tiled_texture.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Miscellaneous::XboxTiledTexture {

	struct Common {

		inline static constexpr auto k_block_width = Size{32_sz};

		inline static constexpr auto k_chunk_width = Size{32_sz};

	};

}
