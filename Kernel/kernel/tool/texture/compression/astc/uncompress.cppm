module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.compression.astc.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.texture.compression.astc.common;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.third.astc_encoder;

export namespace Twinning::Kernel::Tool::Texture::Compression::Astc {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_image(
			InputByteStreamView &            data,
			Image::VariableImageView const & image,
			Generation const &               generation,
			Image::ImageSize const &         block_size
		) -> Void {
			assert_test(is_valid_block_size(block_size));
			assert_test(is_padded_size(image.size().width, block_size.width) && is_padded_size(image.size().height, block_size.height));
			auto block_count = image.size().area() / block_size.area();
			if (generation == Generation::Constant::v0()) {
				auto ripe_data_size = block_count * k_block_bit_count / k_type_bit_count<Byte>;
				assert_test(ripe_data_size <= data.reserve());
				auto raw_format = Encoding::Format::Constant::rgba_8888_o();
				auto raw_data = ByteArray{image.size().area() * Encoding::Common::get_pixel_byte_count(raw_format)};
				auto astc_error = Third::astc_encoder::astcenc_error{};
				auto astc_config = Third::astc_encoder::astcenc_config{};
				astc_error = Third::astc_encoder::astcenc_config_init(
					Third::astc_encoder::astcenc_profile::ASTCENC_PRF_LDR,
					static_cast<unsigned int>(block_size.width.value),
					static_cast<unsigned int>(block_size.height.value),
					1,
					Third::astc_encoder::ASTCENC_PRE_FASTEST,
					Third::astc_encoder::ASTCENC_FLG_DECOMPRESS_ONLY,
					&astc_config
				);
				assert_test(astc_error == Third::astc_encoder::astcenc_error::ASTCENC_SUCCESS);
				auto astc_context = ZPointer<Third::astc_encoder::astcenc_context>{};
				astc_error = Third::astc_encoder::astcenc_context_alloc(&astc_config, 1, &astc_context, nullptr);
				assert_test(astc_error == Third::astc_encoder::astcenc_error::ASTCENC_SUCCESS);
				auto astc_image_data = cast_pointer<Void>(raw_data.begin()).value;
				auto astc_image = Third::astc_encoder::astcenc_image{
					.dim_x = static_cast<unsigned int>(image.size().width.value),
					.dim_y = static_cast<unsigned int>(image.size().height.value),
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
				astc_error = Third::astc_encoder::astcenc_decompress_image(
					astc_context,
					cast_pointer<std::uint8_t>(data.current_pointer()).value,
					ripe_data_size.value,
					&astc_image,
					&astc_swizzle,
					0
				);
				Third::astc_encoder::astcenc_context_free(astc_context);
				assert_test(astc_error == Third::astc_encoder::astcenc_error::ASTCENC_SUCCESS);
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
			Image::ImageSize const &         block_size
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, generation, block_size);
		}

	};

}
