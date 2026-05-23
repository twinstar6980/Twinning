module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.flipping.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.flipping.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Flipping {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe,
			Boolean const &                  flip_horizontal,
			Boolean const &                  flip_vertical
		) -> Void {
			assert_test(raw.size() == ripe.size());
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto ripe_x = x;
					auto ripe_y = y;
					if (flip_horizontal) {
						ripe_x = ripe.size().width - 1_sz - ripe_x;
					}
					if (flip_vertical) {
						ripe_y = ripe.size().height - 1_sz - ripe_y;
					}
					auto & raw_pixel = raw[y][x];
					auto & ripe_pixel = ripe[ripe_y][ripe_x];
					ripe_pixel = raw_pixel;
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe,
			Boolean const &                  flip_horizontal,
			Boolean const &                  flip_vertical
		) -> Void {
			return process_image(raw, ripe, flip_horizontal, flip_vertical);
		}

	};

}
