#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Data::Encoding::Base64 {

	struct EncodeCommon {

	protected:

		inline static constexpr auto k_raw_block_size = Size{3_sz};

		inline static constexpr auto k_ripe_block_size = Size{4_sz};

		inline static constexpr auto k_padding_character = Character{'='_c};

	};

	struct Encode :
		EncodeCommon {

	protected:

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

		static auto compute_size (
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			ripe_size = compute_padded_size(raw_size, k_raw_block_size) / k_raw_block_size * k_ripe_block_size;
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView &      raw,
			OCharacterStreamView & ripe
		) -> Void {
			auto raw_size = raw.reserve();
			auto buffer = Array<Byte>{k_raw_block_size};
			while (!raw.full()) {
				buffer[1_ix] = raw.read_of();
				buffer[2_ix] = !raw.full() ? (raw.read_of()) : (k_null_byte);
				buffer[3_ix] = !raw.full() ? (raw.read_of()) : (k_null_byte);
				ripe.write(k_table[cbw<Size>(clip_bit(buffer[1_ix], 3_ix, 6_sz) << 0_sz)]);
				ripe.write(k_table[cbw<Size>(clip_bit(buffer[1_ix], 1_ix, 2_sz) << 4_sz | clip_bit(buffer[2_ix], 5_ix, 4_sz) << 0_sz)]);
				ripe.write(k_table[cbw<Size>(clip_bit(buffer[2_ix], 1_ix, 4_sz) << 2_sz | clip_bit(buffer[3_ix], 7_ix, 2_sz) << 0_sz)]);
				ripe.write(k_table[cbw<Size>(clip_bit(buffer[3_ix], 1_ix, 6_sz) << 0_sz)]);
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
					throw NeverException{};
				}
			}
			return;
		}

	public:

		static auto do_compute_size (
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			restruct(ripe_size);
			return compute_size(raw_size, ripe_size);
		}

		// ----------------

		static auto do_process_whole (
			IByteStreamView &      raw_,
			OCharacterStreamView & ripe_
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe);
		}

	};

	struct Decode :
		EncodeCommon {

	protected:

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

		static auto compute_size (
			CCharacterListView const & ripe,
			Size &                     raw_size
		) -> Void {
			assert_condition(is_padded_size(ripe.size(), k_ripe_block_size));
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

		static auto process_whole (
			ICharacterStreamView & ripe,
			OByteStreamView &      raw
		) -> Void {
			auto raw_size = Size{};
			compute_size(ripe.reserve_view(), raw_size);
			auto raw_end = raw.position() + raw_size;
			auto buffer = Array<Character>{k_ripe_block_size};
			while (!ripe.full()) {
				buffer[1_ix] = ripe.read_of();
				buffer[2_ix] = ripe.read_of();
				buffer[3_ix] = ripe.read_of();
				buffer[4_ix] = ripe.read_of();
				raw.write(k_table[cbw<Size>(buffer[1_ix])] << 2_sz | k_table[cbw<Size>(buffer[2_ix])] >> 4_sz);
				if (raw.position() == raw_end) {
					break;
				}
				raw.write(k_table[cbw<Size>(buffer[2_ix])] << 4_sz | k_table[cbw<Size>(buffer[3_ix])] >> 2_sz);
				if (raw.position() == raw_end) {
					break;
				}
				raw.write(k_table[cbw<Size>(buffer[3_ix])] << 6_sz | k_table[cbw<Size>(buffer[4_ix])] >> 0_sz);
			}
			return;
		}

	public:

		static auto do_compute_size (
			CCharacterListView const & ripe,
			Size &                     raw_size
		) -> Void {
			restruct(raw_size);
			return compute_size(ripe, raw_size);
		}

		// ----------------

		static auto do_process_whole (
			ICharacterStreamView & ripe_,
			OByteStreamView &      raw_
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw);
		}

	};

}
