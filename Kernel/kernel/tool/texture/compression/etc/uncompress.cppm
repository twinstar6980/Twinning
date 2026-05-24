module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.etc.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.third.etcpak;

export namespace Twinning::Kernel::Tool::Texture::Compression::Etc {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			InputByteStreamView &            data,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha_eac,
			Boolean const &                  with_green_eac
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(is_padded_size(image.size().width, block_size.width) && is_padded_size(image.size().height, block_size.height));
			auto block_count = image.size().area() / block_size.area();
			if (generation == Generation::Constant::v1()) {
				auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_format = Encoding::Format::Constant::rgba_8888_o();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Third::etcpak::DecodeRGB(
					rubox<std::uint64_t const *>(data.current_pointer()),
					rubox<std::uint32_t *>(raw_data.begin()),
					ubox<std::int32_t>(image.size().width),
					ubox<std::int32_t>(image.size().height)
				);
				data.forward(ripe_data_size);
				Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
			}
			if (generation == Generation::Constant::v2()) {
				if (!with_alpha_eac) {
					auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::rgba_8888_o();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Third::etcpak::DecodeRGB(
						rubox<std::uint64_t const *>(data.current_pointer()),
						rubox<std::uint32_t *>(raw_data.begin()),
						ubox<std::int32_t>(image.size().width),
						ubox<std::int32_t>(image.size().height)
					);
					data.forward(ripe_data_size);
					Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
				}
				else {
					auto ripe_data_size = block_count * k_block_bit_count * 2_sz / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::rgba_8888_o();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Third::etcpak::DecodeRGBA(
						rubox<std::uint64_t const *>(data.current_pointer()),
						rubox<std::uint32_t *>(raw_data.begin()),
						ubox<std::int32_t>(image.size().width),
						ubox<std::int32_t>(image.size().height)
					);
					data.forward(ripe_data_size);
					Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
				}
			}
			if (generation == Generation::Constant::eac()) {
				if (!with_green_eac) {
					auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::rgba_8888_o();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Third::etcpak::DecodeR(
						rubox<std::uint64_t const *>(data.current_pointer()),
						rubox<std::uint32_t *>(raw_data.begin()),
						ubox<std::int32_t>(image.size().width),
						ubox<std::int32_t>(image.size().height)
					);
					data.forward(ripe_data_size);
					Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
				}
				else {
					auto ripe_data_size = block_count * k_block_bit_count * 2_sz / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::rgba_8888_o();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Third::etcpak::DecodeRG(
						rubox<std::uint64_t const *>(data.current_pointer()),
						rubox<std::uint32_t *>(raw_data.begin()),
						ubox<std::int32_t>(image.size().width),
						ubox<std::int32_t>(image.size().height)
					);
					data.forward(ripe_data_size);
					Encoding::Decode::process(as_left(InputByteStreamView{raw_data.view()}), image, raw_format);
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &            data_,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Boolean const &                  with_alpha_eac,
			Boolean const &                  with_green_eac
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, generation, block_size, with_alpha_eac, with_green_eac);
		}

	};

}
