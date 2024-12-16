module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.xbox_tiled_texture.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.xbox_tiled_texture.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;

export namespace Twinning::Kernel::Tool::Miscellaneous::XboxTiledTexture {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Texture::Encoding::Format>)
			&& (Texture::Encoding::FormatPackage::has(format))
		inline static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			auto image_size = image.size();
			auto block_height = image_size.width / k_block_width;
			auto chunk_height = image_size.height / k_chunk_width;
			auto data_view = data.forward_view(image_size.area() * Texture::Encoding::bpp_of(format) / k_type_bit_count<Byte>);
			for (auto & block_y : SizeRange{chunk_height}) {
				for (auto & block_x : SizeRange{k_chunk_width}) {
					auto image_row = OStreamView<Image::Pixel>{image[block_y * k_chunk_width + block_x]};
					for (auto & block_row : SizeRange{block_height}) {
						auto position = (block_y * block_height + block_row) * (k_block_width * k_chunk_width) + (block_x * k_block_width);
						auto block_data = IByteStreamView{data_view.sub(position * bpp_of(format) / k_type_bit_count<Byte>, k_block_width * bpp_of(format) / k_type_bit_count<Byte>)};
						for (auto & block_column : SizeRange{k_block_width}) {
							Texture::Encoding::Decode::process_pixel<format>(block_data, image_row.next());
						}
					}
				}
			}
			return;
		}

		inline static auto process_image (
			IByteStreamView &                 data,
			Image::VImageView const &         image,
			Texture::Encoding::Format const & format
		) -> Void {
			Generalization::match<Texture::Encoding::FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					process_image<format>(data, image);
				}
			);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &                 data_,
			Image::VImageView const &         image,
			Texture::Encoding::Format const & format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
