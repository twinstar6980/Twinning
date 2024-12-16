module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.u_texture.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.u_texture.version;
import twinning.kernel.tool.popcap.u_texture.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.uncompress;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;

export namespace Twinning::Kernel::Tool::PopCap::UTexture {

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::Header;

		using typename Common::FormatFlag;

		// ----------------

		inline static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			data.read_constant(k_magic_identifier);
			auto header = Header{};
			data.read(header);
			assert_test(image.size() == Image::ImageSize{cbox<Size>(header.size_width), cbox<Size>(header.size_height)});
			auto image_format = cbox<Integer>(header.format);
			auto format = Texture::Encoding::Format{};
			auto opacity = Boolean{};
			switch (image_format.value) {
				case FormatFlag::rgba_8888_o.value : {
					format = Texture::Encoding::Format::Constant::rgba_8888_o();
					opacity = k_false;
					break;
				}
				case FormatFlag::rgba_4444.value : {
					format = Texture::Encoding::Format::Constant::rgba_4444();
					opacity = k_false;
					break;
				}
				case FormatFlag::rgba_5551.value : {
					format = Texture::Encoding::Format::Constant::rgba_5551();
					opacity = k_false;
					break;
				}
				case FormatFlag::rgb_565.value : {
					format = Texture::Encoding::Format::Constant::rgb_565();
					opacity = k_true;
					break;
				}
				default : {
					assert_fail(R"(image_format == /* valid */)");
				}
			}
			auto texture_data_size = image.size().area() * Texture::Encoding::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_view = CByteListView{};
			auto texture_data_container = ByteArray{};
			if constexpr (check_version(version, {false})) {
				texture_data_view = data.forward_view(texture_data_size);
			}
			if constexpr (check_version(version, {true})) {
				texture_data_container.allocate(texture_data_size);
				texture_data_view = texture_data_container.as_view();
			}
			if constexpr (check_version(version, {true})) {
				auto texture_data_stream = OByteStreamView{texture_data_container};
				Data::Compression::Deflate::Uncompress::process(data, texture_data_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				assert_test(texture_data_stream.full());
			}
			Texture::Encoding::Decode::process(as_lvalue(IByteStreamView{texture_data_view}), image, format);
			if (opacity) {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						pixel.alpha = Image::k_color_maximum;
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto estimate_image (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			auto data_stream = IByteStreamView{data};
			data_stream.read_constant(k_magic_identifier);
			auto header = Header{};
			data_stream.read(header);
			image_size = Image::ImageSize{cbox<Size>(header.size_width), cbox<Size>(header.size_height)};
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &         data_,
			Image::VImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

		inline static auto estimate (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			restruct(image_size);
			return estimate_image(data, image_size);
		}

	};

}
