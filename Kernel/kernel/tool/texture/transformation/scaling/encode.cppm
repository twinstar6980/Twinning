module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.scaling.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.scaling.common;
import twinning.kernel.third.avir;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Scaling {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			Image::ConstantImageView const & raw,
			Image::VariableImageView const & ripe
		) -> Void {
			auto raw_buffer = Array<Image::Pixel>{raw.size().area()};
			auto ripe_buffer = Array<Image::Pixel>{ripe.size().area()};
			for (auto & row : SizeRange{raw.size().height}) {
				Range::assign_from(raw_buffer.sub(raw.size().width * row, raw.size().width), raw[row]);
			}
			Third::avir::CImageResizer<>{8}.resizeImage(
				cast_pointer<std::uint8_t>(raw_buffer.begin()).value,
				static_cast<int>(raw.size().width.value),
				static_cast<int>(raw.size().height.value),
				0,
				cast_pointer<std::uint8_t>(ripe_buffer.begin()).value,
				static_cast<int>(ripe.size().width.value),
				static_cast<int>(ripe.size().height.value),
				4,
				0.0,
				nullptr
			);
			for (auto & row : SizeRange{ripe.size().height}) {
				Range::assign_from(ripe[row], ripe_buffer.sub(ripe.size().width * row, ripe.size().width));
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
