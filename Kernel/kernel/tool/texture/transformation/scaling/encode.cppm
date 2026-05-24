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
			for (auto & y : SizeRange{raw.size().height}) {
				Range::assign_from(raw_buffer.sub(raw.size().width * y, raw.size().width), raw[y]);
			}
			Third::avir::CImageResizer<>{8}.resizeImage(
				rubox<std::uint8_t *>(raw_buffer.begin()),
				ubox<int>(raw.size().width),
				ubox<int>(raw.size().height),
				0,
				rubox<std::uint8_t *>(ripe_buffer.begin()),
				ubox<int>(ripe.size().width),
				ubox<int>(ripe.size().height),
				4,
				0.0,
				nullptr
			);
			for (auto & y : SizeRange{ripe.size().height}) {
				Range::assign_from(ripe[y], ripe_buffer.sub(ripe.size().width * y, ripe.size().width));
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
