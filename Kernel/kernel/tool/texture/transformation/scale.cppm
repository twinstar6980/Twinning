module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.scale;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.common;
import twinning.kernel.third.avir;

export namespace Twinning::Kernel::Tool::Texture::Transformation {

	struct Scale :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image (
			Image::CImageView const & source,
			Image::VImageView const & destination
		) -> Void {
			auto source_buffer = Array<Image::Pixel>{source.size().area()};
			auto destination_buffer = Array<Image::Pixel>{destination.size().area()};
			for (auto & row : SizeRange{source.size().height}) {
				Range::assign_from(source_buffer.sub(source.size().width * row, source.size().width), source[row]);
			}
			Third::avir::CImageResizer<>{8}.resizeImage(
				cast_pointer<std::uint8_t>(source_buffer.begin()).value,
				static_cast<int>(source.size().width.value),
				static_cast<int>(source.size().height.value),
				0,
				cast_pointer<std::uint8_t>(destination_buffer.begin()).value,
				static_cast<int>(destination.size().width.value),
				static_cast<int>(destination.size().height.value),
				4,
				0.0,
				nullptr
			);
			for (auto & row : SizeRange{destination.size().height}) {
				Range::assign_from(destination[row], destination_buffer.sub(destination.size().width * row, destination.size().width));
			}
			return;
		}

		// ----------------

		inline static auto process (
			Image::CImageView const & source,
			Image::VImageView const & destination
		) -> Void {
			return process_image(source, destination);
		}

	};

}
