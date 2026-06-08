module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.u_texture.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.u_texture.version;
import twinning.kernel.tool.texture.encoding.common;

export namespace Twinning::Kernel::Tool::Popcap::UTexture {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Common {

		using MagicMarker = StaticByteArray<2_sz>;

		inline static constexpr auto k_magic_marker = MagicMarker{{0x75_b, 0x0A_b}};

		// ----------------

		M_record_of_data(
			M_wrap(Header),
			M_wrap(
				(IntegerU16) size_width,
				(IntegerU16) size_height,
				(IntegerU16) format,
			),
		);

		// ----------------

		struct FormatFlag {

			inline static constexpr auto rgba_8888 = Integer{1_i};

			inline static constexpr auto rgba_4444 = Integer{2_i};

			inline static constexpr auto rgba_5551 = Integer{3_i};

			inline static constexpr auto rgb_565 = Integer{4_i};

		};

		// ----------------

		inline static auto is_valid_format(
			String const & name
		) -> Boolean {
			return name == "rgba_8888"_sv
				|| name == "rgba_4444"_sv
				|| name == "rgba_5551"_sv
				|| name == "rgb_565"_sv;
		}

		inline static auto get_encoding_format(
			String const & name
		) -> Texture::Encoding::Format {
			auto result = Texture::Encoding::Format{};
			switch (name.hash().value) {
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
				default: {
					assert_fail(R"(name == /* valid */)");
					break;
				}
			}
			return result;
		}

	};

}
