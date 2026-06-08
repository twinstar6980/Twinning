module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Texture::Encoding {

	M_enumeration(
		M_wrap(Channel),
		M_wrap(
			minimum,
			maximum,
			red,
			green,
			blue,
			alpha,
			luminance,
		),
	);

	M_record_of_map(
		M_wrap(Format),
		M_wrap(
			(Boolean) endian,
			(List<Tuple<Channel, Size>>) channel,
		),
	);

	// ----------------

	struct Common {

		inline static constexpr auto get_pixel_byte_count(
			Format const & format
		) -> Size {
			auto bit_count = Size{};
			for (auto & channel : format.channel) {
				bit_count += channel.get<2_ix>();
			}
			assert_test(Math::is_padded_size(bit_count, k_type_bit_count<Byte>));
			return bit_count / k_type_bit_count<Byte>;
		}

	};

}
