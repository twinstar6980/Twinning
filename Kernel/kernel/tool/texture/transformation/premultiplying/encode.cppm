module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.premultiplying.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.premultiplying.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Premultiplying {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto compute_value(
			Image::Color const & value,
			Image::Color const & alpha
		) -> Image::Color {
			auto result = cast_box<Floater>(Image::k_color_minimum);
			if (alpha != Image::k_color_minimum) {
				result = Math::round(cast_box<Floater>(value) * (cast_box<Floater>(alpha) / cast_box<Floater>(Image::k_color_maximum)));
			}
			return cast_box<Image::Color>(result);
		}

		// ----------------

		inline static auto process_image(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe
		) -> Void {
			assert_test(raw.size() == ripe.size());
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto & raw_pixel = raw[y][x];
					auto & ripe_pixel = ripe[y][x];
					ripe_pixel.red = compute_value(raw_pixel.red, raw_pixel.alpha);
					ripe_pixel.green = compute_value(raw_pixel.green, raw_pixel.alpha);
					ripe_pixel.blue = compute_value(raw_pixel.blue, raw_pixel.alpha);
					ripe_pixel.alpha = raw_pixel.alpha;
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
