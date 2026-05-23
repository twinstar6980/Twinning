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

		inline static auto convert_index(
			Image::ImagePosition const & ripe,
			Size const &                 width
		) -> Image::ImagePosition {
			auto index = ripe.y * width + ripe.x;
			return Image::ImagePosition{shrink_bit(index >> 0_sz), shrink_bit(index >> 1_sz)};
		}

		// ----------------

		inline static auto process_image(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe
		) -> Void {
			assert_test(raw.size() == ripe.size());
			assert_test(is_padded_size_of_exponent_of_2(raw.size().width) && is_padded_size_of_exponent_of_2(raw.size().height));
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto   raw_position = convert_index(Image::ImagePosition{x, y}, raw.size().width);
					auto & raw_pixel = raw[raw_position.y][raw_position.x];
					auto & ripe_pixel = ripe[y][x];
					raw_pixel = ripe_pixel;
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe
		) -> Void {
			return process_image(raw, ripe);
		}

	};

}
