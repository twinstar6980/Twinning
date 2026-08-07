module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.premultiplying.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.premultiplying.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Premultiplying {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto compute_value(
			Image::Color const & value,
			Image::Color const & alpha
		) -> Image::Color {
			auto result = cast_box<Floater>(Image::k_color_minimum);
			if (alpha != Image::k_color_minimum) {
				result = Math::round(cast_box<Floater>(value) / (cast_box<Floater>(alpha) / cast_box<Floater>(Image::k_color_maximum)));
				if (result > cast_box<Floater>(Image::k_color_maximum)) {
					result = cast_box<Floater>(Image::k_color_maximum);
				}
			}
			return cast_box<Image::Color>(result);
		}

		// ----------------

		inline static auto process_image(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe
		) -> Void {
			assert_test(raw.size() == ripe.size());
			for (auto & y : SizeRange{raw.size().height}) {
				for (auto & x : SizeRange{raw.size().width}) {
					auto & raw_pixel = raw[y][x];
					auto & ripe_pixel = ripe[y][x];
					raw_pixel.red = compute_value(ripe_pixel.red, ripe_pixel.alpha);
					raw_pixel.green = compute_value(ripe_pixel.green, ripe_pixel.alpha);
					raw_pixel.blue = compute_value(ripe_pixel.blue, ripe_pixel.alpha);
					raw_pixel.alpha = ripe_pixel.alpha;
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
