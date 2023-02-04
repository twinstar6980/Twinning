#pragma once

#include "core/utility/utility.hpp"
#include "core/third/Rijndael.hpp"

namespace TwinStar::Core::Tool::Data::Encryption::Rijndael {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			ecb,
			cbc,
			cfb,
		),
	);

	// ----------------

	struct EncryptCommon {

	protected:

		inline static constexpr auto k_valid_block_size = StaticArray<Size, 3_sz>{{
			16_sz, 24_sz, 32_sz
		}};

		inline static constexpr auto k_empty_iv = StaticArray<Character, 32_sz>{{
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
			0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
		}};

	};

	struct Encrypt :
		EncryptCommon {

	protected:

		static auto process_whole (
			IByteStreamView & plain,
			OByteStreamView & cipher,
			Mode const &      mode,
			Size const &      block_size,
			Size const &      key_size,
			String const &    key,
			String const &    iv
		) -> Void {
			assert_condition(is_padded_size(plain.reserve(), block_size));
			assert_condition(Range::has(k_valid_block_size, block_size));
			assert_condition(Range::has(k_valid_block_size, key_size));
			assert_condition(is_padded_size(key.size() * k_type_bit_count<Character>, key_size));
			assert_condition(is_padded_size(iv.size() * k_type_bit_count<Character>, block_size));
			auto iv_view = mode == Mode::Constant::ecb() ? k_empty_iv.view().head(block_size) : iv.view();
			auto plain_size = plain.reserve();
			if (plain_size != k_none_size) {
				auto rijndael = Third::Rijndael::CRijndael{};
				rijndael.MakeKey(cast_pointer<char>(key.begin()).value, cast_pointer<char>(iv_view.begin()).value, static_cast<int>(key_size.value), static_cast<int>(block_size.value));
				rijndael.Encrypt(cast_pointer<char>(plain.current_pointer()).value, cast_pointer<char>(cipher.current_pointer()).value, plain.reserve().value, static_cast<int>(mode.value));
			}
			plain.forward(plain_size);
			cipher.forward(plain_size);
			return;
		}

	public:

		static auto do_process_whole (
			IByteStreamView & plain_,
			OByteStreamView & cipher_,
			Mode const &      mode,
			Size const &      block_size,
			Size const &      key_size,
			String const &    key,
			String const &    iv
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, mode, block_size, key_size, key, iv);
		}

	};

	struct Decrypt :
		EncryptCommon {

	protected:

		static auto process_whole (
			IByteStreamView & cipher,
			OByteStreamView & plain,
			Mode const &      mode,
			Size const &      block_size,
			Size const &      key_size,
			String const &    key,
			String const &    iv
		) -> Void {
			assert_condition(is_padded_size(cipher.reserve(), block_size));
			assert_condition(Range::has(k_valid_block_size, block_size));
			assert_condition(Range::has(k_valid_block_size, key_size));
			assert_condition(is_padded_size(key.size() * k_type_bit_count<Character>, key_size));
			assert_condition(is_padded_size(iv.size() * k_type_bit_count<Character>, block_size));
			auto iv_view = mode == Mode::Constant::ecb() ? k_empty_iv.view().head(block_size) : iv.view();
			auto cipher_size = cipher.reserve();
			if (cipher_size != k_none_size) {
				auto rijndael = Third::Rijndael::CRijndael{};
				rijndael.MakeKey(cast_pointer<char>(key.begin()).value, cast_pointer<char>(iv_view.begin()).value, static_cast<int>(key_size.value), static_cast<int>(block_size.value));
				rijndael.Decrypt(cast_pointer<char>(cipher.current_pointer()).value, cast_pointer<char>(plain.current_pointer()).value, cipher.reserve().value, static_cast<int>(mode.value));
			}
			cipher.forward(cipher_size);
			plain.forward(cipher_size);
			return;
		}

	public:

		static auto do_process_whole (
			IByteStreamView & cipher_,
			OByteStreamView & plain_,
			Mode const &      mode,
			Size const &      block_size,
			Size const &      key_size,
			String const &    key,
			String const &    iv
		) -> Void {
			M_use_zps_of(cipher);
			M_use_zps_of(plain);
			return process_whole(cipher, plain, mode, block_size, key_size, key, iv);
		}

	};

}
