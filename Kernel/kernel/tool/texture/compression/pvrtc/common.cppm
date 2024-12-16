module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Texture::Compression::PVRTC {

	M_enumeration(
		M_wrap(Format),
		M_wrap(
			v1_4bpp_rgb,
			v1_4bpp_rgba,
		),
	);

	using FormatPackage = ValuePackage<
		Format::Constant::v1_4bpp_rgb(),
		Format::Constant::v1_4bpp_rgba()
	>;

	// ----------------

	struct Common {

		inline static constexpr auto k_block_width = Size{4_sz};

		inline static constexpr auto k_bpp_4 = Size{4_sz};

	};

}
