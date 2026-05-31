module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.astc.compress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.astc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.third.astc_encoder;

export namespace Twinning::Kernel::Tool::Texture::Compression::Astc {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			OutputByteStreamView &           data,
			Image::ConstantImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size,
			Integer const &                  quality
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(is_padded_size(image.size().width, block_size.width) && is_padded_size(image.size().height, block_size.height));
			assert_test(Math::between(quality, 0_i, 100_i));
			auto block_count = image.size().area() / block_size.area();
			if (generation == Generation::Constant::v0()) {
				auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_format = Encoding::Format::Constant::rgba_8888_o();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				Encoding::Encode::process(as_left(OutputByteStreamView{raw_data.view()}), image, raw_format);
				auto astc_error = Third::astc_encoder::astcenc_error{};
				auto astc_config = Third::astc_encoder::astcenc_config{};
				astc_error = Third::astc_encoder::astcenc_config_init(
					Third::astc_encoder::astcenc_profile::ASTCENC_PRF_LDR,
					unmake_box<unsigned int>(block_size.width),
					unmake_box<unsigned int>(block_size.height),
					1,
					unmake_box<float>(quality),
					0,
					&astc_config
				);
				assert_test(astc_error == Third::astc_encoder::astcenc_error::ASTCENC_SUCCESS);
				auto astc_context = ZPointer<Third::astc_encoder::astcenc_context>{};
				astc_error = Third::astc_encoder::astcenc_context_alloc(&astc_config, 1, &astc_context, nullptr);
				assert_test(astc_error == Third::astc_encoder::astcenc_error::ASTCENC_SUCCESS);
				auto astc_image_data = unmake_pointer_unsafe<Void>(raw_data.begin());
				auto astc_image = Third::astc_encoder::astcenc_image{
					.dim_x = unmake_box<unsigned int>(image.size().width),
					.dim_y = unmake_box<unsigned int>(image.size().height),
					.dim_z = 1,
					.data_type = Third::astc_encoder::astcenc_type::ASTCENC_TYPE_U8,
					.data = &astc_image_data,
				};
				auto astc_swizzle = Third::astc_encoder::astcenc_swizzle{
					.r = Third::astc_encoder::astcenc_swz::ASTCENC_SWZ_R,
					.g = Third::astc_encoder::astcenc_swz::ASTCENC_SWZ_G,
					.b = Third::astc_encoder::astcenc_swz::ASTCENC_SWZ_B,
					.a = Third::astc_encoder::astcenc_swz::ASTCENC_SWZ_A,
				};
				astc_error = Third::astc_encoder::astcenc_compress_image(
					astc_context,
					&astc_image,
					&astc_swizzle,
					unmake_pointer_unsafe<std::uint8_t>(data.current_pointer()),
					unmake_box<std::size_t>(ripe_data_size),
					0
				);
				Third::astc_encoder::astcenc_context_free(astc_context);
				assert_test(astc_error == Third::astc_encoder::astcenc_error::ASTCENC_SUCCESS);
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
			Integer const &                  quality
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, generation, block_size, quality);
		}

	};

}
