module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.sexy_texture.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.sexy_texture.version;
import twinning.kernel.tool.texture.encoding.common;

export namespace Twinning::Kernel::Tool::Popcap::SexyTexture {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Common {

		using MagicMarker = StaticByteArray<8_sz>;

		inline static constexpr auto k_magic_marker = MagicMarker{{0x53_b, 0x45_b, 0x58_b, 0x59_b, 0x54_b, 0x45_b, 0x58_b, 0x00_b}};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		M_record_of_data(
			M_wrap(Header),
			M_wrap(
				(IntegerU32) size_width,
				(IntegerU32) size_height,
				(IntegerU32) format,
				(IntegerU32) compress_data,
				(ConstantBlock<0x00000001_iu32>) unknown_5,
				(IntegerU32) compress_data_size,
				(ConstantBlock<0x00000000_iu32>) unknown_7,
				(ConstantBlock<0x00000000_iu32>) unknown_8,
				(ConstantBlock<0x00000000_iu32>) unknown_9,
			),
		);

		// ----------------

		struct FormatFlag {

			inline static constexpr auto lut_8 = Integer{1_i};

			inline static constexpr auto argb_8888 = Integer{2_i};

			inline static constexpr auto argb_4444 = Integer{3_i};

			inline static constexpr auto argb_1555 = Integer{4_i};

			inline static constexpr auto rgb_565 = Integer{5_i};

			inline static constexpr auto rgba_8888 = Integer{6_i};

			inline static constexpr auto rgba_4444 = Integer{7_i};

			inline static constexpr auto rgba_5551 = Integer{8_i};

			inline static constexpr auto xrgb_8888 = Integer{9_i};

			inline static constexpr auto la_88 = Integer{10_i};

		};

		// ----------------

		inline static auto is_valid_format(
			String const & name
		) -> Boolean {
			return name == "lut_8"_sv
				|| name == "argb_8888"_sv
				|| name == "argb_4444"_sv
				|| name == "argb_1555"_sv
				|| name == "rgb_565"_sv
				|| name == "rgba_8888"_sv
				|| name == "rgba_4444"_sv
				|| name == "rgba_5551"_sv
				|| name == "xrgb_8888"_sv
				|| name == "la_88"_sv;
		}

		inline static auto get_encoding_format(
			String const & name
		) -> Texture::Encoding::Format {
			auto result = Texture::Encoding::Format{};
			switch (name.hash().value) {
				case "lut_8"_shz: {
					throw UnsupportedException{};
					break;
				}
				case "argb_8888"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 8_sz)
						),
					};
					break;
				}
				case "argb_4444"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 4_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 4_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 4_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 4_sz)
						),
					};
					break;
				}
				case "argb_1555"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 1_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 5_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 5_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 5_sz)
						),
					};
					break;
				}
				case "rgb_565"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 5_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 6_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 5_sz)
						),
					};
					break;
				}
				case "rgba_8888"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_true,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 8_sz)
						),
					};
					break;
				}
				case "rgba_4444"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 4_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 4_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 4_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 4_sz)
						),
					};
					break;
				}
				case "rgba_5551"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 5_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 5_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 5_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 1_sz)
						),
					};
					break;
				}
				case "xrgb_8888"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::maximum(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::red(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::green(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::blue(), 8_sz)
						),
					};
					break;
				}
				case "la_88"_shz: {
					result = Texture::Encoding::Format{
						.endian = k_false,
						.channel = make_list<Tuple<Texture::Encoding::Channel, Size>>(
							make_tuple_of(Texture::Encoding::Channel::Constant::luminance(), 8_sz),
							make_tuple_of(Texture::Encoding::Channel::Constant::alpha(), 8_sz)
						),
					};
					break;
				}
				default: {
					assert_fail(R"(name == /* valid */)");
					break;
				}
			}
			return result;
		}

	};

}
