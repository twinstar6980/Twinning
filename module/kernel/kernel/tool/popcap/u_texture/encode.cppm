module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.u_texture.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.u_texture.version;
import twinning.kernel.tool.popcap.u_texture.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;

export namespace Twinning::Kernel::Tool::Popcap::UTexture {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Encode :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::Header;

		using typename Common::FormatFlag;

		using Common::is_valid_format;

		using Common::get_encoding_format;

		// ----------------

		inline static auto process_image(
			OutputBinaryStreamView &         data,
			Image::ConstantImageView const & image,
			String const &                   format
		) -> Void {
			assert_test(is_valid_format(format));
			auto encoding_format = get_encoding_format(format);
			data.write_constant(k_magic_marker);
			auto header_stream = data.split_forward(binary_static_size_of<Header>());
			auto image_format = Integer{};
			switch (format.hash().value) {
				case "rgba_8888"_shz: {
					image_format = FormatFlag::rgba_8888;
					break;
				}
				case "rgba_4444"_shz: {
					image_format = FormatFlag::rgba_4444;
					break;
				}
				case "rgba_5551"_shz: {
					image_format = FormatFlag::rgba_5551;
					break;
				}
				case "rgb_565"_shz: {
					image_format = FormatFlag::rgb_565;
					break;
				}
				default: throw UnreachableException{};
			}
			auto texture_data_size = image.size().area() * Texture::Encoding::Common::get_pixel_byte_count(encoding_format);
			auto texture_data_stream = OutputBinaryStreamView{};
			auto texture_data_container = Binary{};
			if constexpr (check_version(t_version, {false})) {
				texture_data_stream = data.split_forward(texture_data_size);
			}
			if constexpr (check_version(t_version, {true})) {
				texture_data_container.allocate_full(texture_data_size);
				texture_data_stream = like_binary_stream<StreamMode::Constant::output()>(data, texture_data_container);
			}
			Texture::Encoding::Encode::process(texture_data_stream, image, encoding_format);
			if constexpr (check_version(t_version, {true})) {
				auto raw_texture_data_stream = unsafe_cast<InputBinaryStreamView>(texture_data_stream.split_all());
				Data::Compression::Deflate::Compress::process(raw_texture_data_stream, data, Data::Compression::Deflate::Container::Constant::zlib(), 9_i, 15_i, 9_i, Data::Compression::Deflate::StrategyMode::Constant::default_mode());
			}
			auto header = Header{};
			header.size_width = cast_box<IntegerU16>(image.size().width);
			header.size_height = cast_box<IntegerU16>(image.size().height);
			header.format = cast_box<IntegerU16>(image_format);
			header_stream.write(header);
			return;
		}

		// ----------------

		inline static auto estimate_image(
			Size &                   data_size_bound,
			Image::ImageSize const & image_size,
			String const &           format
		) -> Void {
			assert_test(is_valid_format(format));
			auto encoding_format = get_encoding_format(format);
			data_size_bound = 0_sz;
			data_size_bound += binary_static_size_of<MagicMarker>();
			data_size_bound += binary_static_size_of<Header>();
			auto texture_data_size = image_size.area() * Texture::Encoding::Common::get_pixel_byte_count(encoding_format);
			auto texture_data_size_bound = Size{};
			if constexpr (check_version(t_version, {false})) {
				texture_data_size_bound = texture_data_size;
			}
			if constexpr (check_version(t_version, {true})) {
				Data::Compression::Deflate::Compress::estimate(texture_data_size, texture_data_size_bound, Data::Compression::Deflate::Container::Constant::zlib(), 15_i, 9_i);
			}
			data_size_bound += texture_data_size_bound;
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView &         data_,
			Image::ConstantImageView const & image,
			String const &                   format
		) -> Void {
			M_use_split_stream_of(data);
			return process_image(data, image, format);
		}

		inline static auto estimate(
			Size &                   data_size_bound,
			Image::ImageSize const & image_size,
			String const &           format
		) -> Void {
			restruct(data_size_bound);
			return estimate_image(data_size_bound, image_size, format);
		}

	};

}
