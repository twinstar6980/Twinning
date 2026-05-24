module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.dxtc.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.dxtc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.third.libsquish;

export namespace Twinning::Kernel::Tool::Texture::Compression::Dxtc {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			InputByteStreamView &            data,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(is_padded_size(image.size().width, block_size.width) && is_padded_size(image.size().height, block_size.height));
			auto block_count = image.size().area() / block_size.area();
			if (generation == Generation::Constant::v1()) {
				auto ripe_data_size = block_count * k_block_bit_count_1 / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_format = Encoding::Format::Constant::rgba_8888_o();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Third::libsquish::DecompressImage(
					rubox<Third::libsquish::u8 *>(raw_data.begin()),
					ubox<int>(image.size().width),
					ubox<int>(image.size().height),
					rubox<void const *>(data.current_pointer()),
					Third::libsquish::kDxt1
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
			}
			if (generation == Generation::Constant::v3()) {
				auto ripe_data_size = block_count * k_block_bit_count_3_5 / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_format = Encoding::Format::Constant::rgba_8888_o();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Third::libsquish::DecompressImage(
					rubox<Third::libsquish::u8 *>(raw_data.begin()),
					ubox<int>(image.size().width),
					ubox<int>(image.size().height),
					rubox<void const *>(data.current_pointer()),
					Third::libsquish::kDxt3
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
			}
			if (generation == Generation::Constant::v5()) {
				auto ripe_data_size = block_count * k_block_bit_count_3_5 / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_format = Encoding::Format::Constant::rgba_8888_o();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Third::libsquish::DecompressImage(
					rubox<Third::libsquish::u8 *>(raw_data.begin()),
					ubox<int>(image.size().width),
					ubox<int>(image.size().height),
					rubox<void const *>(data.current_pointer()),
					Third::libsquish::kDxt5
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &            data_,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, generation, block_size, with_alpha);
		}

	};

}
