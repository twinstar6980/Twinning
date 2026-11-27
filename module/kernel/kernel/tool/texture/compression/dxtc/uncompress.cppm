module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.dxtc.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.dxtc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.dependency.libsquish;

export namespace Twinning::Kernel::Tool::Texture::Compression::Dxtc {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			InputBinaryStreamView &          data,
			Image::VariableImageView const & image,
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
				auto ripe_data_size = block_count * k_block_bit_count_1 / type_bit_size_of<Byte>();
				assert_test(ripe_data_size <= data.reserve());
				auto raw_data = Binary{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Dependency::libsquish::DecompressImage(
					unmake_pointer_unsafe<Dependency::libsquish::u8>(raw_data.data()),
					unmake_box<int>(image.size().width),
					unmake_box<int>(image.size().height),
					unmake_pointer_unsafe<void>(data.current()),
					Dependency::libsquish::kDxt1
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputBinaryStreamView{raw_data.as_view()}), image, raw_format);
			}
			if (generation == Generation::Constant::v3()) {
				auto ripe_data_size = block_count * k_block_bit_count_3_5 / type_bit_size_of<Byte>();
				assert_test(ripe_data_size <= data.reserve());
				auto raw_data = Binary{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Dependency::libsquish::DecompressImage(
					unmake_pointer_unsafe<Dependency::libsquish::u8>(raw_data.data()),
					unmake_box<int>(image.size().width),
					unmake_box<int>(image.size().height),
					unmake_pointer_unsafe<void>(data.current()),
					Dependency::libsquish::kDxt3
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputBinaryStreamView{raw_data.as_view()}), image, raw_format);
			}
			if (generation == Generation::Constant::v5()) {
				auto ripe_data_size = block_count * k_block_bit_count_3_5 / type_bit_size_of<Byte>();
				assert_test(ripe_data_size <= data.reserve());
				auto raw_data = Binary{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Dependency::libsquish::DecompressImage(
					unmake_pointer_unsafe<Dependency::libsquish::u8>(raw_data.data()),
					unmake_box<int>(image.size().width),
					unmake_box<int>(image.size().height),
					unmake_pointer_unsafe<void>(data.current()),
					Dependency::libsquish::kDxt5
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputBinaryStreamView{raw_data.as_view()}), image, raw_format);
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &          data_,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha
		) -> Void {
			M_use_split_stream_of(data);
			return process_image(data, image, generation, block_size, with_alpha);
		}

	};

}
