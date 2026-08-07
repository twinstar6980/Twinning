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

		inline static constexpr auto is_valid_format(
			Format const & format
		) -> Boolean {
			auto result = k_true;
			auto bit_count = 0_sz;
			for (auto & channel : format.channel) {
				if (!Math::between(channel.get<2_ix>(), 1_sz, k_type_bit_count<Byte>)) {
					result = k_false;
					break;
				}
				bit_count += channel.get<2_ix>();
			}
			if (!Math::is_padded_size(bit_count, k_type_bit_count<Byte>)) {
				result = k_false;
			}
			if (!Math::between(bit_count, 1_sz * k_type_bit_count<Byte>, 4_sz * k_type_bit_count<Byte>)) {
				result = k_false;
			}
			return result;
		}

		inline static constexpr auto get_pixel_byte_count(
			Format const & format
		) -> Size {
			auto bit_count = Size{};
			for (auto & channel : format.channel) {
				bit_count += channel.get<2_ix>();
			}
			assert_test(Math::is_padded_size(bit_count, k_type_bit_count<Byte>));
			auto byte_count = bit_count / k_type_bit_count<Byte>;
			return byte_count;
		}

	};

}
