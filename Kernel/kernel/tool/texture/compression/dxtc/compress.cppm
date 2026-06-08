module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.dxtc.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.dxtc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.third.libsquish;

export namespace Twinning::Kernel::Tool::Texture::Compression::Dxtc {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			OutputByteStreamView &           data,
			Image::ConstantImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(Math::is_padded_size(image.size().width, block_size.width) && Math::is_padded_size(image.size().height, block_size.height));
			auto block_count = image.size().area() / block_size.area();
			auto raw_format = Encoding::Format{
				.endian = k_true,
				.channel = make_list<Tuple<Encoding::Channel, Size>>(
					make_tuple_of(Encoding::Channel::Constant::red(), 8_sz),
					make_tuple_of(Encoding::Channel::Constant::green(), 8_sz),
					make_tuple_of(Encoding::Channel::Constant::blue(), 8_sz),
					make_tuple_of(!with_alpha ? Encoding::Channel::Constant::maximum() : Encoding::Channel::Constant::alpha(), 8_sz)
				),
			};
			if (generation == Generation::Constant::v1()) {
				auto ripe_data_size = block_count * k_block_bit_count_1 / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
				Third::libsquish::CompressImage(
					unmake_pointer_unsafe<Third::libsquish::u8>(raw_data.begin()),
					unmake_box<int>(image.size().width),
					unmake_box<int>(image.size().height),
					unmake_pointer_unsafe<void>(data.current_pointer()),
					Third::libsquish::kColourClusterFit | Third::libsquish::kDxt1
				);
				data.forward(ripe_data_size);
			}
			if (generation == Generation::Constant::v3()) {
				auto ripe_data_size = block_count * k_block_bit_count_3_5 / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
				Third::libsquish::CompressImage(
					unmake_pointer_unsafe<Third::libsquish::u8>(raw_data.begin()),
					unmake_box<int>(image.size().width),
					unmake_box<int>(image.size().height),
					unmake_pointer_unsafe<void>(data.current_pointer()),
					Third::libsquish::kColourClusterFit | Third::libsquish::kDxt3
				);
				data.forward(ripe_data_size);
			}
			if (generation == Generation::Constant::v5()) {
				auto ripe_data_size = block_count * k_block_bit_count_3_5 / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
				Third::libsquish::CompressImage(
					unmake_pointer_unsafe<Third::libsquish::u8>(raw_data.begin()),
					unmake_box<int>(image.size().width),
					unmake_box<int>(image.size().height),
					unmake_pointer_unsafe<void>(data.current_pointer()),
					Third::libsquish::kColourClusterFit | Third::libsquish::kDxt5
				);
				data.forward(ripe_data_size);
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView &           data_,
			Image::ConstantImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, generation, block_size, with_alpha);
		}

	};

}
