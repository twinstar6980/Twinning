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

		inline static constexpr auto k_table = StaticArray<Character, 64_sz>{{
			// 'A' - 'Z'
			'A'_c, 'B'_c, 'C'_c, 'D'_c, 'E'_c, 'F'_c, 'G'_c, 'H'_c, 'I'_c, 'J'_c, 'K'_c, 'L'_c, 'M'_c,
			'N'_c, 'O'_c, 'P'_c, 'Q'_c, 'R'_c, 'S'_c, 'T'_c, 'U'_c, 'V'_c, 'W'_c, 'X'_c, 'Y'_c, 'Z'_c,
			// 'a' - 'z'
			'a'_c, 'b'_c, 'c'_c, 'd'_c, 'e'_c, 'f'_c, 'g'_c, 'h'_c, 'i'_c, 'j'_c, 'k'_c, 'l'_c, 'm'_c,
			'n'_c, 'o'_c, 'p'_c, 'q'_c, 'r'_c, 's'_c, 't'_c, 'u'_c, 'v'_c, 'w'_c, 'x'_c, 'y'_c, 'z'_c,
			// '0' - '9'
			'0'_c, '1'_c, '2'_c, '3'_c, '4'_c, '5'_c, '6'_c, '7'_c, '8'_c, '9'_c,
			// '+' '/'
			'+'_c, '/'_c,
		}};

		// ----------------

		inline static auto process_whole (
			IByteStreamView &      raw,
			OCharacterStreamView & ripe
		) -> Void {
			auto raw_size = raw.reserve();
			auto buffer = Array<Byte>{k_raw_block_size};
			while (!raw.full()) {
				buffer[1_ix] = raw.read_of();
				buffer[2_ix] = !raw.full() ? (raw.read_of()) : (k_null_byte);
				buffer[3_ix] = !raw.full() ? (raw.read_of()) : (k_null_byte);
				ripe.write(k_table[cbox<Size>(clip_bit(buffer[1_ix], 3_ix, 6_sz) << 0_sz)]);
				ripe.write(k_table[cbox<Size>(clip_bit(buffer[1_ix], 1_ix, 2_sz) << 4_sz | clip_bit(buffer[2_ix], 5_ix, 4_sz) << 0_sz)]);
				ripe.write(k_table[cbox<Size>(clip_bit(buffer[2_ix], 1_ix, 4_sz) << 2_sz | clip_bit(buffer[3_ix], 7_ix, 2_sz) << 0_sz)]);
				ripe.write(k_table[cbox<Size>(clip_bit(buffer[3_ix], 1_ix, 6_sz) << 0_sz)]);
			}
			switch ((raw_size % k_raw_block_size).value) {
				case 0_sz .value : {
					break;
				}
				case 1_sz .value : {
					ripe.backward(2_sz);
					ripe.write(k_padding_character);
					ripe.write(k_padding_character);
					break;
				}
				case 2_sz .value : {
					ripe.backward(1_sz);
					ripe.write(k_padding_character);
					break;
				}
				default : {
					throw ImpossibleException{};
				}
			}
			return;
		}

		// ----------------

		inline static auto estimate_whole (
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			ripe_size = compute_padded_size(raw_size, k_raw_block_size) / k_raw_block_size * k_ripe_block_size;
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &      raw_,
			OCharacterStreamView & ripe_
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe);
		}

		inline static auto estimate (
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			restruct(ripe_size);
			return estimate_whole(raw_size, ripe_size);
		}

	};

}
