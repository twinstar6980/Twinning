module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Texture::Compression::ETC {

	M_enumeration(
		M_wrap(Format),
		M_wrap(
			v1_rgb,
			v2_rgb,
			v2_rgba,
		),
	);

	using FormatPackage = ValuePackage<
		Format::Constant::v1_rgb(),
		Format::Constant::v2_rgb(),
		Format::Constant::v2_rgba()
	>;

	// ----------------

	struct Common {

		inline static constexpr auto k_block_width = Size{4_sz};

		inline static constexpr auto k_bpp_rgb = Size{4_sz};

		inline static constexpr auto k_bpp_rgba = Size{8_sz};

	};

}
