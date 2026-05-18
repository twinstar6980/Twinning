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
			Image::VariableImageView const & target,
			Boolean const &                  horizontal,
			Boolean const &                  vertical
		) -> Void {
			if (horizontal) {
				for (auto & row : SizeRange{target.size().height}) {
					for (auto & column : SizeRange{target.size().width / 2_sz}) {
						swap(target[row][column], target[row][target.size().width - column - 1_sz]);
					}
				}
			}
			if (vertical) {
				for (auto & column : SizeRange{target.size().width}) {
					for (auto & row : SizeRange{target.size().height / 2_sz}) {
						swap(target[row][column], target[target.size().height - row - 1_sz][column]);
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			Image::VariableImageView const & target,
			Boolean const &                  horizontal,
			Boolean const &                  vertical
		) -> Void {
			return process_image(target, horizontal, vertical);
		}

	};

}
