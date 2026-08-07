module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encoding.base64.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encoding.base64.common;

export namespace Twinning::Kernel::Tool::Data::Encoding::Base64 {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static constexpr auto k_table = StaticByteArray<64_sz>{{
			// 'A' - 'Z'
			'A'_b, 'B'_b, 'C'_b, 'D'_b, 'E'_b, 'F'_b, 'G'_b, 'H'_b, 'I'_b, 'J'_b, 'K'_b, 'L'_b, 'M'_b,
			'N'_b, 'O'_b, 'P'_b, 'Q'_b, 'R'_b, 'S'_b, 'T'_b, 'U'_b, 'V'_b, 'W'_b, 'X'_b, 'Y'_b, 'Z'_b,
			// 'a' - 'z'
			'a'_b, 'b'_b, 'c'_b, 'd'_b, 'e'_b, 'f'_b, 'g'_b, 'h'_b, 'i'_b, 'j'_b, 'k'_b, 'l'_b, 'm'_b,
			'n'_b, 'o'_b, 'p'_b, 'q'_b, 'r'_b, 's'_b, 't'_b, 'u'_b, 'v'_b, 'w'_b, 'x'_b, 'y'_b, 'z'_b,
			// '0' - '9'
			'0'_b, '1'_b, '2'_b, '3'_b, '4'_b, '5'_b, '6'_b, '7'_b, '8'_b, '9'_b,
			// '+' '/'
			'+'_b, '/'_b,
		}};

		inline static auto query_table(
			Size const & index
		) -> Byte const & {
			auto & value = k_table[index];
			return value;
		}

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  raw,
			OutputByteStreamView & ripe
		) -> Void {
			auto raw_size = raw.reserve();
			auto buffer = ByteArray{k_raw_block_size};
			while (!raw.full()) {
				buffer[1_ix] = raw.read_of();
				buffer[2_ix] = !raw.full() ? (raw.read_of()) : (k_null_byte);
				buffer[3_ix] = !raw.full() ? (raw.read_of()) : (k_null_byte);
				ripe.write(query_table(cast_box<Size>(Bitwise::infuse(Bitwise::extract(buffer[1_ix], 2_sz, 6_sz), 0_sz, 6_sz))));
				ripe.write(query_table(cast_box<Size>(Bitwise::infuse(Bitwise::extract(buffer[1_ix], 0_sz, 2_sz), 4_sz, 2_sz) | Bitwise::infuse(Bitwise::extract(buffer[2_ix], 4_sz, 4_sz), 0_sz, 4_sz))));
				ripe.write(query_table(cast_box<Size>(Bitwise::infuse(Bitwise::extract(buffer[2_ix], 0_sz, 4_sz), 2_sz, 4_sz) | Bitwise::infuse(Bitwise::extract(buffer[3_ix], 6_sz, 2_sz), 0_sz, 2_sz))));
				ripe.write(query_table(cast_box<Size>(Bitwise::infuse(Bitwise::extract(buffer[3_ix], 0_sz, 6_sz), 0_sz, 6_sz))));
			}
			switch ((raw_size % k_raw_block_size).value) {
				case 0_sz .value: {
					break;
				}
				case 1_sz .value: {
					ripe.backward(2_sz);
					ripe.write(k_padding_byte);
					ripe.write(k_padding_byte);
					break;
				}
				case 2_sz .value: {
					ripe.backward(1_sz);
					ripe.write(k_padding_byte);
					break;
				}
				default: throw UnreachableException{};
			}
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			ripe_size = Math::compute_padded_size(raw_size, k_raw_block_size) / k_raw_block_size * k_ripe_block_size;
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  raw_,
			OutputByteStreamView & ripe_
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe);
		}

		inline static auto estimate(
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			restruct(ripe_size);
			return estimate_whole(raw_size, ripe_size);
		}

	};

}
