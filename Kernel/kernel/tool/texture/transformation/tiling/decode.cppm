module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.transformation.tiling.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.transformation.tiling.common;

export namespace Twinning::Kernel::Tool::Texture::Transformation::Tiling {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe,
			Image::ImageSize const &         tile_size
		) -> Void {
			assert_test(raw.size() == ripe.size());
			assert_test(is_padded_size(raw.size().width, tile_size.width) && is_padded_size(raw.size().height, tile_size.height));
			auto tile_count_x = raw.size().width / tile_size.width;
			auto tile_count_y = raw.size().height / tile_size.height;
			auto raw_x = 0_sz;
			auto raw_y = 0_sz;
			for (auto & tile_index_y : SizeRange{tile_count_y}) {
				for (auto & tile_index_x : SizeRange{tile_count_x}) {
					for (auto & tile_y : SizeRange{tile_size.height}) {
						for (auto & tile_x : SizeRange{tile_size.width}) {
							auto & raw_pixel = raw[tile_index_y * tile_size.height + tile_y][tile_index_x * tile_size.width + tile_x];
							auto & ripe_pixel = ripe[raw_y][raw_x];
							raw_pixel = ripe_pixel;
							raw_x += 1_sz;
							if (raw_x == ripe.size().width) {
								raw_x = 0_sz;
								raw_y += 1_sz;
							}
						}
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			Image::VariableImageView const & raw,
			Image::ConstantImageView const & ripe,
			Image::ImageSize const &         tile_size
		) -> Void {
			return process_image(raw, ripe, tile_size);
		}

	};

}
