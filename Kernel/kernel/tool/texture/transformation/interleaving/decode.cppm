module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.interleaving.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.interleaving.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Interleaving {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe,
			Image::ImageSize const &         tile_size
		) -> Void {
			throw UnimplementedException{};
			return;
		}

		// ----------------

		inline static auto process(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe,
			Image::ImageSize const &         tile_size
		) -> Void {
			return process_image(raw, ripe, tile_size);
		}

	};

}
