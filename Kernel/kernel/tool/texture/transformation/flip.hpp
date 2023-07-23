#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/texture/transformation/common.hpp"

namespace TwinStar::Kernel::Tool::Texture::Transformation {

	struct Flip :
		Common {

		using Common = Common;

		// ----------------

		static auto process_image (
			Image::VImageView const & target,
			Boolean const &           horizontal,
			Boolean const &           vertical
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

		static auto process (
			Image::VImageView const & target,
			Boolean const &           horizontal,
			Boolean const &           vertical
		) -> Void {
			return process_image(target, horizontal, vertical);
		}

	};

}
