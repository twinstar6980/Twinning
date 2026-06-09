module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.interleaving.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.interleaving.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Interleaving {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto convert_index(
			Image::ImagePosition const & raw,
			Size const &                 width
		) -> Image::ImagePosition {
			auto index = (Bitwise::expand(raw.x) << 0_sz) | (Bitwise::expand(raw.y) << 1_sz);
			return Image::ImagePosition{index % width, index / width};
		}

		// ----------------

		inline static auto process_image(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe
		) -> Void {
			assert_test(raw.size() == ripe.size());
			assert_test(Math::is_padded_size_of_power_of_two(raw.size().width) && Math::is_padded_size_of_power_of_two(raw.size().height));
			assert_test(raw.size().width == raw.size().height);
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto   ripe_position = convert_index(Image::ImagePosition{x, y}, raw.size().width);
					auto & raw_pixel = raw[y][x];
					auto & ripe_pixel = ripe[ripe_position.y][ripe_position.x];
					ripe_pixel = raw_pixel;
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe
		) -> Void {
			return process_image(raw, ripe);
		}

	};

}
