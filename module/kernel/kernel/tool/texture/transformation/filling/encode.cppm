module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.filling.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.filling.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Filling {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe,
			Optional<Image::Color> const &   fill_red,
			Optional<Image::Color> const &   fill_green,
			Optional<Image::Color> const &   fill_blue,
			Optional<Image::Color> const &   fill_alpha
		) -> Void {
			assert_test(raw.size() == ripe.size());
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto & raw_pixel = raw[y][x];
					auto & ripe_pixel = ripe[y][x];
					ripe_pixel.red = fill_red.has() ? fill_red.get() : raw_pixel.red;
					ripe_pixel.green = fill_green.has() ? fill_green.get() : raw_pixel.green;
					ripe_pixel.blue = fill_blue.has() ? fill_blue.get() : raw_pixel.blue;
					ripe_pixel.alpha = fill_alpha.has() ? fill_alpha.get() : raw_pixel.alpha;
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe,
			Optional<Image::Color> const &   fill_red,
			Optional<Image::Color> const &   fill_green,
			Optional<Image::Color> const &   fill_blue,
			Optional<Image::Color> const &   fill_alpha
		) -> Void {
			return process_image(raw, ripe, fill_red, fill_green, fill_blue, fill_alpha);
		}

	};

}
