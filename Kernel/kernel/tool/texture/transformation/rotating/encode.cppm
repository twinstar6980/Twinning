module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.rotating.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.rotating.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Rotating {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe,
			Boolean const & rotate_90,
			Boolean const & rotate_180
		) -> Void {
			assert_test((!rotate_90) ? (raw.size() == ripe.size()) : (raw.size() == Image::ImageSize{ripe.size().height, ripe.size().width}));
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto ripe_x = x;
					auto ripe_y = y;
					if (rotate_90) {
						ripe_x = raw.size().height - 1_sz - y;
						ripe_y = x;
					}
					if (rotate_180) {
						ripe_x = ripe.size().width - 1_sz - ripe_x;
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
			Boolean const & rotate_90,
			Boolean const & rotate_180
		) -> Void {
			return process_image(raw, ripe, rotate_90, rotate_180);
		}

	};

}
