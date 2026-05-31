module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.etc.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.third.etcpak;

export namespace Twinning::Kernel::Tool::Texture::Compression::Etc {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			OutputByteStreamView &           data,
			Image::ConstantImageView const & image,
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
				auto raw_format = Encoding::Format::Constant::argb_8888();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
				Third::etcpak::CompressEtc1Rgb(
					unmake_pointer_unsafe<std::uint32_t>(raw_data.begin()),
					unmake_pointer_unsafe<std::uint64_t>(data.current_pointer()),
					unmake_box<std::uint32_t>(block_count),
					unmake_box<std::size_t>(image.size().width)
				);
				data.forward(ripe_data_size);
			}
			if (generation == Generation::Constant::v2()) {
				if (!with_alpha_eac) {
					auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::argb_8888();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
					Third::etcpak::CompressEtc2Rgb(
						unmake_pointer_unsafe<std::uint32_t>(raw_data.begin()),
						unmake_pointer_unsafe<std::uint64_t>(data.current_pointer()),
						unmake_box<std::uint32_t>(block_count),
						unmake_box<std::size_t>(image.size().width),
						true
					);
					data.forward(ripe_data_size);
				}
				else {
					auto ripe_data_size = block_count * k_block_bit_count * 2_sz / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::argb_8888();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
					Third::etcpak::CompressEtc2Rgba(
						unmake_pointer_unsafe<std::uint32_t>(raw_data.begin()),
						unmake_pointer_unsafe<std::uint64_t>(data.current_pointer()),
						unmake_box<std::uint32_t>(block_count),
						unmake_box<std::size_t>(image.size().width),
						true
					);
					data.forward(ripe_data_size);
				}
			}
			if (generation == Generation::Constant::eac()) {
				if (!with_green_eac) {
					auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::argb_8888();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
					Third::etcpak::CompressEacR(
						unmake_pointer_unsafe<std::uint32_t>(raw_data.begin()),
						unmake_pointer_unsafe<std::uint64_t>(data.current_pointer()),
						unmake_box<std::uint32_t>(block_count),
						unmake_box<std::size_t>(image.size().width)
					);
					data.forward(ripe_data_size);
				}
				else {
					auto ripe_data_size = block_count * k_block_bit_count * 2_sz / k_type_bit_count<Byte>;
					assert_test(ripe_data_size <= data.reserve());
					auto raw_format = Encoding::Format::Constant::argb_8888();
					auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
					Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
					Third::etcpak::CompressEacRg(
						unmake_pointer_unsafe<std::uint32_t>(raw_data.begin()),
						unmake_pointer_unsafe<std::uint64_t>(data.current_pointer()),
						unmake_box<std::uint32_t>(block_count),
						unmake_box<std::size_t>(image.size().width)
					);
					data.forward(ripe_data_size);
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView &           data_,
			Image::ConstantImageView const & image,
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
