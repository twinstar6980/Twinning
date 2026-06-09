module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.pvrtc.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.third.PVRTCCompressor;
import twinning.kernel.tool.texture.encoding.decode;

export namespace Twinning::Kernel::Tool::Texture::Compression::Pvrtc {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			InputByteStreamView &            data,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  use_bpp4,
			Boolean const &                  with_alpha
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(Math::is_padded_size(image.size().width, block_size.width) && Math::is_padded_size(image.size().height, block_size.height));
			auto block_count = image.size().area() / block_size.area();
			if (generation == Generation::Constant::v1()) {
				assert_test(Math::is_padded_size_of_power_of_two(image.size().width) && Math::is_padded_size_of_power_of_two(image.size().height));
				assert_test(image.size().width == image.size().height);
				if (!use_bpp4) {
					throw UnsupportedException{};
				}
				else {
					if (!with_alpha) {
						auto ripe_data_size = block_count * k_block_bit_count_4 / k_type_bit_count<Byte>;
						assert_test(ripe_data_size <= data.reserve());
						auto raw_format = Encoding::Format{
							.endian = k_false,
							.channel = make_list<Tuple<Encoding::Channel, Size>>(
								make_tuple_of(Encoding::Channel::Constant::red(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::green(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::blue(), 8_sz)
							),
						};
						auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
						Third::PVRTCCompressor::PvrTcDecoder::DecodeRgb4Bpp(
							unmake_pointer_unsafe<Third::PVRTCCompressor::ColorRgb<unsigned char>>(raw_data.begin()),
							Third::PVRTCCompressor::Point2<int>{unmake_box<int>(image.size().width), unmake_box<int>(image.size().height)},
							unmake_pointer_unsafe<void>(data.current_pointer())
						);
						data.forward(ripe_data_size);
						Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
					}
					else {
						auto ripe_data_size = block_count * k_block_bit_count_4 / k_type_bit_count<Byte>;
						assert_test(ripe_data_size <= data.reserve());
						auto raw_format = Encoding::Format{
							.endian = k_false,
							.channel = make_list<Tuple<Encoding::Channel, Size>>(
								make_tuple_of(Encoding::Channel::Constant::alpha(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::red(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::green(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::blue(), 8_sz)
							),
						};
						auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
						Third::PVRTCCompressor::PvrTcDecoder::DecodeRgba4Bpp(
							unmake_pointer_unsafe<Third::PVRTCCompressor::ColorRgba<unsigned char>>(raw_data.begin()),
							Third::PVRTCCompressor::Point2<int>{unmake_box<int>(image.size().width), unmake_box<int>(image.size().height)},
							unmake_pointer_unsafe<void>(data.current_pointer())
						);
						data.forward(ripe_data_size);
						Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
					}
				}
			}
			if (generation == Generation::Constant::v2()) {
				throw UnsupportedException{};
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &            data_,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  use_bpp4,
			Boolean const &                  with_alpha
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, generation, block_size, use_bpp4, with_alpha);
		}

	};

}
