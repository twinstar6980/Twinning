module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.sexy_texture.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.sexy_texture.version;
import twinning.kernel.tool.popcap.sexy_texture.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.uncompress;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.decode;

export namespace Twinning::Kernel::Tool::Popcap::SexyTexture {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Decode :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::VersionNumber;

		using typename Common::Header;

		using typename Common::FormatFlag;

		using Common::is_valid_format;

		using Common::get_encoding_format;

		// ----------------

		inline static auto process_image(
			InputBinaryStreamView &          data,
			Image::VariableImageView const & image
		) -> Void {
			data.read_constant(k_magic_marker);
			data.read_constant(cast_box<VersionNumber>(t_version.number));
			auto header = Header{};
			data.read(header);
			assert_test(image.size() == Image::ImageSize{cast_box<Size>(header.size_width), cast_box<Size>(header.size_height)});
			auto image_format = cast_box<Integer>(header.format);
			auto format = String{};
			switch (image_format.value) {
				case FormatFlag::lut_8.value: {
					format = "lut_8"_sv;
					break;
				}
				case FormatFlag::argb_8888.value: {
					format = "argb_8888"_sv;
					break;
				}
				case FormatFlag::argb_4444.value: {
					format = "argb_4444"_sv;
					break;
				}
				case FormatFlag::argb_1555.value: {
					format = "argb_1555"_sv;
					break;
				}
				case FormatFlag::rgb_565.value: {
					format = "rgb_565"_sv;
					break;
				}
				case FormatFlag::rgba_8888.value: {
					format = "rgba_8888"_sv;
					break;
				}
				case FormatFlag::rgba_4444.value: {
					format = "rgba_4444"_sv;
					break;
				}
				case FormatFlag::rgba_5551.value: {
					format = "rgba_5551"_sv;
					break;
				}
				case FormatFlag::xrgb_8888.value: {
					format = "argb_8888"_sv;
					break;
				}
				case FormatFlag::la_88.value: {
					format = "la_88"_sv;
					break;
				}
				default: {
					assert_fail(R"(image_format == /* valid */)");
					break;
				}
			}
			auto encoding_format = get_encoding_format(format);
			auto compress_texture_data = cast_box<Boolean>(header.compress_data);
			auto compress_texture_data_size = cast_box<Size>(header.compress_data_size);
			auto texture_data_size = image.size().area() * Texture::Encoding::Common::get_pixel_byte_count(encoding_format);
			auto texture_data_stream = InputBinaryStreamView{};
			auto texture_data_container = Binary{};
			if (!compress_texture_data) {
				texture_data_stream = data.split_forward(texture_data_size);
				assert_test(compress_texture_data_size == 0_sz);
			}
			else {
				texture_data_container.allocate_full(texture_data_size);
				texture_data_stream = like_binary_stream<StreamMode::Constant::input()>(data, texture_data_container);
			}
			if (compress_texture_data) {
				auto raw_texture_data_stream = unsafe_cast<OutputBinaryStreamView>(texture_data_stream.split_all());
				auto ripe_texture_data_stream = data.split_reserve();
				Data::Compression::Deflate::Uncompress::process(raw_texture_data_stream, ripe_texture_data_stream, Data::Compression::Deflate::Container::Constant::zlib(), 15_i);
				assert_test(ripe_texture_data_stream.position() == compress_texture_data_size);
				assert_test(raw_texture_data_stream.full());
				data.forward(ripe_texture_data_stream.position());
			}
			Texture::Encoding::Decode::process(texture_data_stream, image, encoding_format);
			if (!Range::has_if(
				encoding_format.channel,
				[](auto & it) {
					return it.template get<1_ix>() == Texture::Encoding::Channel::Constant::alpha();
				}
			)) {
				for (auto & y : SizeRange{image.size().height}) {
					for (auto & x : SizeRange{image.size().width}) {
						auto & pixel = image[x, y];
						pixel.alpha = Image::k_color_maximum;
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto estimate_image(
			InputBinaryStreamView & data,
			Image::ImageSize &      image_size
		) -> Void {
			data.read_constant(k_magic_marker);
			data.read_constant(cast_box<VersionNumber>(t_version.number));
			auto header = Header{};
			data.read(header);
			image_size = Image::ImageSize{cast_box<Size>(header.size_width), cast_box<Size>(header.size_height)};
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &          data_,
			Image::VariableImageView const & image
		) -> Void {
			M_use_split_stream_of(data);
			return process_image(data, image);
		}

		inline static auto estimate(
			InputBinaryStreamView & data_,
			Image::ImageSize &      image_size
		) -> Void {
			M_use_split_stream_of(data);
			restruct(image_size);
			return estimate_image(data, image_size);
		}

	};

}
