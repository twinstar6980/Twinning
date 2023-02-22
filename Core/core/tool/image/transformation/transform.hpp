#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/image.hpp"
#include "core/third/avir.hpp"

namespace TwinStar::Core::Tool::Image::Transformation {

	struct TransformCommon {

	protected:

	};

	struct Flip :
		TransformCommon {

	protected:

		static auto process_image (
			Image::VImageView const & target,
			Boolean const             flip_horizontal,
			Boolean const             flip_vertical
		) -> Void {
			if (flip_horizontal) {
				for (auto & row : SizeRange{target.size().height}) {
					for (auto & column : SizeRange{target.size().width / 2_sz}) {
						swap(target[row][column], target[row][target.size().width - column - 1_sz]);
					}
				}
			}
			if (flip_vertical) {
				for (auto & column : SizeRange{target.size().width}) {
					for (auto & row : SizeRange{target.size().height / 2_sz}) {
						swap(target[row][column], target[target.size().height - row - 1_sz][column]);
					}
				}
			}
			return;
		}

	public:

		static auto do_process_image (
			Image::VImageView const & target,
			Boolean const             flip_horizontal,
			Boolean const             flip_vertical
		) -> Void {
			return process_image(target, flip_horizontal, flip_vertical);
		}

	};

	struct Scale :
		TransformCommon {

	protected:

		static auto process_image (
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

	public:

		static auto do_process_image (
			Image::CImageView const & source,
			Image::VImageView const & destination
		) -> Void {
			return process_image(source, destination);
		}

	};

}
