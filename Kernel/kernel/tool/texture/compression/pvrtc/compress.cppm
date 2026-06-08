module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.pvrtc.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.third.PVRTCCompressor;

export namespace Twinning::Kernel::Tool::Texture::Compression::Pvrtc {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			OutputByteStreamView &           data,
			Image::ConstantImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  use_bpp4,
			Boolean const &                  with_alpha
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(Math::is_padded_size(image.size().width, block_size.width) && Math::is_padded_size(image.size().height, block_size.height));
			auto block_count = image.size().area() / block_size.area();
			if (generation == Generation::Constant::v1()) {
				if (!use_bpp4) {
					throw UnsupportedException{};
				}
				else {
					if (!with_alpha) {
						auto ripe_data_size = block_count * k_block_bit_count_4 / k_type_bit_count<Byte>;
						assert_test(ripe_data_size <= data.reserve());
						auto pvrtc_image = Third::PVRTCCompressor::RgbBitmap{unmake_box<int>(image.size().width), unmake_box<int>(image.size().height)};
						auto raw_format = Encoding::Format{
							.endian = k_false,
							.channel = make_list<Tuple<Encoding::Channel, Size>>(
								make_tuple_of(Encoding::Channel::Constant::red(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::green(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::blue(), 8_sz)
							),
						};
						auto raw_data = to_byte_view(VariableListView<Third::PVRTCCompressor::ColorRgb<unsigned char>>{make_pointer(pvrtc_image.GetData()), image.size().area()});
						Encoding::Encode::process(as_left(OutputByteStreamView{raw_data}), image, raw_format);
						Third::PVRTCCompressor::PvrTcEncoder::EncodeRgb4Bpp(
							unmake_pointer_unsafe<void>(data.current_pointer()),
							pvrtc_image
						);
						data.forward(ripe_data_size);
					}
					else {
						auto ripe_data_size = block_count * k_block_bit_count_4 / k_type_bit_count<Byte>;
						assert_test(ripe_data_size <= data.reserve());
						auto pvrtc_image = Third::PVRTCCompressor::RgbaBitmap{unmake_box<int>(image.size().width), unmake_box<int>(image.size().height)};
						auto raw_format = Encoding::Format{
							.endian = k_false,
							.channel = make_list<Tuple<Encoding::Channel, Size>>(
								make_tuple_of(Encoding::Channel::Constant::alpha(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::red(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::green(), 8_sz),
								make_tuple_of(Encoding::Channel::Constant::blue(), 8_sz)
							),
						};
						auto raw_data = to_byte_view(VariableListView<Third::PVRTCCompressor::ColorRgba<unsigned char>>{make_pointer(pvrtc_image.GetData()), image.size().area()});
						Encoding::Encode::process(as_left(OutputByteStreamView{raw_data}), image, raw_format);
						Third::PVRTCCompressor::PvrTcEncoder::EncodeRgba4Bpp(
							unmake_pointer_unsafe<void>(data.current_pointer()),
							pvrtc_image
						);
						data.forward(ripe_data_size);
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
			OutputByteStreamView &           data_,
			Image::ConstantImageView const & image,
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
