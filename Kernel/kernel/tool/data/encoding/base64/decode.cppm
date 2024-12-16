module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encoding.base64.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encoding.base64.common;

export namespace Twinning::Kernel::Tool::Data::Encoding::Base64 {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static constexpr auto k_table = StaticArray<Byte, 128_sz>{{
			// none
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
			// '+'
			0x3E_b,
			// none
			0xFF_b, 0xFF_b, 0xFF_b,
			// '/'
			0x3F_b,
			// '0' - '9'
			0x34_b, 0x35_b, 0x36_b, 0x37_b, 0x38_b, 0x39_b, 0x3A_b, 0x3B_b, 0x3C_b, 0x3D_b,
			// none
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
			// 'A' - 'Z'
			0x00_b, 0x01_b, 0x02_b, 0x03_b, 0x04_b, 0x05_b, 0x06_b, 0x07_b, 0x08_b, 0x09_b, 0x0A_b, 0x0B_b, 0x0C_b,
			0x0D_b, 0x0E_b, 0x0F_b, 0x10_b, 0x11_b, 0x12_b, 0x13_b, 0x14_b, 0x15_b, 0x16_b, 0x17_b, 0x18_b, 0x19_b,
			// none
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
			// 'a' - 'z'
			0x1A_b, 0x1B_b, 0x1C_b, 0x1D_b, 0x1E_b, 0x1F_b, 0x20_b, 0x21_b, 0x22_b, 0x23_b, 0x24_b, 0x25_b, 0x26_b,
			0x27_b, 0x28_b, 0x29_b, 0x2A_b, 0x2B_b, 0x2C_b, 0x2D_b, 0x2E_b, 0x2F_b, 0x30_b, 0x31_b, 0x32_b, 0x33_b,
			// none
			0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b, 0xFF_b,
		}};

		// ----------------

		inline static auto process_whole (
			ICharacterStreamView & ripe,
			OByteStreamView &      raw
		) -> Void {
			auto raw_size = Size{};
			estimate_whole(ripe.reserve_view(), raw_size);
			auto raw_end = raw.position() + raw_size;
			auto buffer = Array<Character>{k_ripe_block_size};
			while (!ripe.full()) {
				buffer[1_ix] = ripe.read_of();
				buffer[2_ix] = ripe.read_of();
				buffer[3_ix] = ripe.read_of();
				buffer[4_ix] = ripe.read_of();
				raw.write(k_table[cbox<Size>(buffer[1_ix])] << 2_sz | k_table[cbox<Size>(buffer[2_ix])] >> 4_sz);
				if (raw.position() == raw_end) {
					break;
				}
				raw.write(k_table[cbox<Size>(buffer[2_ix])] << 4_sz | k_table[cbox<Size>(buffer[3_ix])] >> 2_sz);
				if (raw.position() == raw_end) {
					break;
				}
				raw.write(k_table[cbox<Size>(buffer[3_ix])] << 6_sz | k_table[cbox<Size>(buffer[4_ix])] >> 0_sz);
			}
			return;
		}

		// ----------------

		inline static auto estimate_whole (
			CCharacterListView const & ripe,
			Size &                     raw_size
		) -> Void {
			assert_test(is_padded_size(ripe.size(), k_ripe_block_size));
			auto padding_size = k_none_size;
			if (!ripe.empty()) {
				if (ripe[ripe.last_index() - 1_ix] == k_padding_character) {
					++padding_size;
				}
				if (ripe[ripe.last_index() - 2_ix] == k_padding_character) {
					++padding_size;
				}
			}
			raw_size = ripe.size() / k_ripe_block_size * k_raw_block_size - padding_size;
			return;
		}

		// ----------------

		inline static auto process (
			ICharacterStreamView & ripe_,
			OByteStreamView &      raw_
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw);
		}

		inline static auto estimate (
			CCharacterListView const & ripe,
			Size &                     raw_size
		) -> Void {
			restruct(raw_size);
			return estimate_whole(ripe, raw_size);
		}

	};

}
