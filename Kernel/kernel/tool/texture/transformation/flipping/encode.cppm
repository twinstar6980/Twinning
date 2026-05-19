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
			ripe.draw(raw);
			if (flip_horizontal) {
				for (auto & y : SizeRange{ripe.size().height}) {
					for (auto & x : SizeRange{ripe.size().width / 2_sz}) {
						swap(ripe[y][x], ripe[y][ripe.size().width - x - 1_sz]);
					}
				}
			}
			if (flip_vertical) {
				for (auto & x : SizeRange{ripe.size().width}) {
					for (auto & y : SizeRange{ripe.size().height / 2_sz}) {
						swap(ripe[y][x], ripe[ripe.size().height - y - 1_sz][x]);
					}
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
