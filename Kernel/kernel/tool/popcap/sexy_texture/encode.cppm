module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.sexy_texture.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.sexy_texture.version;
import twinning.kernel.tool.popcap.sexy_texture.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;

export namespace Twinning::Kernel::Tool::PopCap::SexyTexture {

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::Header;

		using typename Common::FormatFlag;

		// ----------------

		inline static auto process_image (
			OByteStreamView &                 data,
			Image::CImageView const &         image,
			Texture::Encoding::Format const & format,
			Boolean const &                   compress_texture_data
		) -> Void {
			data.write_constant(k_magic_identifier);
			data.write_constant(cbox<VersionNumber>(version.number));
			auto header_stream = OByteStreamView{data.forward_view(bs_static_size<Header>())};
			auto image_format = Integer{};
			switch (format.value) {
				case Texture::Encoding::Format::Constant::argb_8888().value : {
					image_format = FormatFlag::argb_8888;
					break;
				}
				case Texture::Encoding::Format::Constant::argb_4444().value : {
					image_format = FormatFlag::argb_4444;
					break;
				}
				case Texture::Encoding::Format::Constant::argb_1555().value : {
					image_format = FormatFlag::argb_1555;
					break;
				}
				case Texture::Encoding::Format::Constant::rgb_565().value : {
					image_format = FormatFlag::rgb_565;
					break;
				}
				case Texture::Encoding::Format::Constant::rgba_8888_o().value : {
					image_format = FormatFlag::rgba_8888_o;
					break;
				}
				case Texture::Encoding::Format::Constant::rgba_4444().value : {
					image_format = FormatFlag::rgba_4444;
					break;
				}
				case Texture::Encoding::Format::Constant::rgba_5551().value : {
					image_format = FormatFlag::rgba_5551;
					break;
				}
				case Texture::Encoding::Format::Constant::la_88().value : {
					image_format = FormatFlag::la_88;
					break;
				}
				default : {
					assert_fail(R"(format == /* valid */)");
				}
			}
			auto compress_texture_data_size = Size{};
			auto texture_data_size = image.size().area() * Texture::Encoding::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_view = VByteListView{};
			auto texture_data_container = ByteArray{};
			if (!compress_texture_data) {
				texture_data_view = data.forward_view(texture_data_size);
				compress_texture_data_size = k_none_size;
			}
			else {
				texture_data_container.allocate(texture_data_size);
				texture_data_view = texture_data_container.as_view();
			}
			Texture::Encoding::Encode::process(as_lvalue(OByteStreamView{texture_data_view}), image, format);
			if (compress_texture_data) {
				auto texture_data_stream = IByteStreamView{texture_data_container};
				auto ripe_texture_data_stream = OByteStreamView{data.reserve_view()};
				Data::Compression::Deflate::Compress::process(texture_data_stream, ripe_texture_data_stream, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
				compress_texture_data_size = ripe_texture_data_stream.position();
				data.forward(ripe_texture_data_stream.position());
			}
			auto header = Header{};
			header.size_width = cbox<IntegerU32>(image.size().width);
			header.size_height = cbox<IntegerU32>(image.size().height);
			header.format = cbox<IntegerU32>(image_format);
			header.compress_data = cbox<IntegerU32>(compress_texture_data);
			header.compress_data_size = cbox<IntegerU32>(compress_texture_data_size);
			header_stream.write(header);
			return;
		}

		// ----------------

		inline static auto estimate_image (
			Size &                            data_size_bound,
			Image::ImageSize const &          image_size,
			Texture::Encoding::Format const & format,
			Boolean const &                   compress_texture_data
		) -> Void {
			data_size_bound = k_none_size;
			data_size_bound += bs_static_size<MagicIdentifier>();
			data_size_bound += bs_static_size<VersionNumber>();
			data_size_bound += bs_static_size<Header>();
			auto texture_data_size = image_size.area() * Texture::Encoding::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_size_bound = Size{};
			if (!compress_texture_data) {
				texture_data_size_bound = texture_data_size;
			}
			else {
				Data::Compression::Deflate::Compress::estimate(texture_data_size, texture_data_size_bound, 15_sz, 9_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
			}
			data_size_bound += texture_data_size_bound;
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &                 data_,
			Image::CImageView const &         image,
			Texture::Encoding::Format const & format,
			Boolean const &                   compress_texture_data
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format, compress_texture_data);
		}

		inline static auto estimate (
			Size &                            data_size_bound,
			Image::ImageSize const &          image_size,
			Texture::Encoding::Format const & format,
			Boolean const &                   compress_texture_data
		) -> Void {
			restruct(data_size_bound);
			return estimate_image(data_size_bound, image_size, format, compress_texture_data);
		}

	};

}
