#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/data/hash/md5.hpp"
#include "core/tool/data/encryption/rijndael.hpp"

namespace TwinStar::Core::Tool::PopCap::RTON {

	struct EncryptCommon {

	protected:

		using MagicIdentifier = StaticByteArray<2_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x10_b, 0x00_b}};

		// ----------------

		inline static constexpr auto k_crypt_block_size = Size{24_sz};

		inline static constexpr auto k_crypt_key_size = Size{32_sz};

		// ----------------

		static auto hash_to_string (
			CByteListView const & data
		) -> String {
			auto string = String{16_sz * 2_sz};
			auto value_byte = ByteArray{};
			Data::Hash::MD5::Hash::do_process_whole(data, value_byte);
			for (auto & element : value_byte) {
				string.append(CharacterType::to_number_hex_lower(cbw<IntegerU8>(element / 0x10_b)));
				string.append(CharacterType::to_number_hex_lower(cbw<IntegerU8>(element % 0x10_b)));
			}
			return string;
		}

	};

	struct Encrypt :
		EncryptCommon {

	protected:

		static auto compute_size (
			Size const & plain_size,
			Size &       cipher_size
		) -> Void {
			cipher_size = k_none_size;
			cipher_size += bs_static_size<MagicIdentifier>();
			cipher_size += compute_padded_size(plain_size, k_crypt_block_size);
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & plain,
			OByteStreamView & cipher,
			String const &    key
		) -> Void {
			auto rijndael_plain_container = ByteArray{};
			auto rijndael_plain = IByteStreamView{};
			auto plain_data = plain.forward_view(plain.reserve());
			auto rijndael_data_size = compute_padded_size(plain_data.size(), k_crypt_block_size);
			if (plain_data.size() == rijndael_data_size) {
				rijndael_plain.set(plain_data);
			} else {
				rijndael_plain_container.allocate(rijndael_data_size);
				Range::assign_from(rijndael_plain_container.head(plain_data.size()), plain_data);
				rijndael_plain.set(rijndael_plain_container);
			}
			cipher.write_constant(k_magic_identifier);
			auto rijndael_cipher = OByteStreamView{cipher.forward_view(rijndael_data_size)};
			auto rijndael_key = hash_to_string(to_byte_view(key.view()));
			auto rijndael_iv = String{rijndael_key.sub(5_ix, 24_sz)};
			Data::Encryption::Rijndael::Encrypt::do_process_whole(rijndael_plain, rijndael_cipher, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_crypt_block_size, k_crypt_key_size, rijndael_key, rijndael_iv);
			return;
		}

	public:

		static auto do_compute_size (
			Size const & plain_size,
			Size &       cipher_size
		) -> Void {
			restruct(cipher_size);
			return compute_size(plain_size, cipher_size);
		}

		static auto do_process_whole (
			IByteStreamView & plain_,
			OByteStreamView & cipher_,
			String const &    key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, key);
		}

	};

	struct Decrypt :
		EncryptCommon {

	protected:

		static auto compute_size (
			Size const & cipher_size,
			Size &       plain_size
		) -> Void {
			plain_size = k_none_size;
			assert_test(cipher_size >= bs_static_size<MagicIdentifier>());
			auto rijndael_data_size = cipher_size - bs_static_size<MagicIdentifier>();
			assert_test(is_padded_size(rijndael_data_size, k_crypt_block_size));
			plain_size += rijndael_data_size;
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView & cipher,
			OByteStreamView & plain,
			String const &    key
		) -> Void {
			auto rijndael_data_size = Size{};
			compute_size(cipher.reserve(), rijndael_data_size);
			cipher.read_constant(k_magic_identifier);
			auto rijndael_cipher = IByteStreamView{cipher.forward_view(rijndael_data_size)};
			auto rijndael_plain = OByteStreamView{plain.forward_view(rijndael_data_size)};
			auto rijndael_key = hash_to_string(to_byte_view(key.view()));
			auto rijndael_iv = String{rijndael_key.sub(5_ix, 24_sz)};
			Data::Encryption::Rijndael::Decrypt::do_process_whole(rijndael_cipher, rijndael_plain, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_crypt_block_size, k_crypt_key_size, rijndael_key, rijndael_iv);
			return;
		}

	public:

		static auto do_compute_size (
			Size const & cipher_size,
			Size &       plain_size
		) -> Void {
			restruct(plain_size);
			return compute_size(cipher_size, plain_size);
		}

		static auto do_process_whole (
			IByteStreamView & cipher_,
			OByteStreamView & plain_,
			String const &    key
		) -> Void {
			M_use_zps_of(cipher);
			M_use_zps_of(plain);
			return process_whole(cipher, plain, key);
		}

	};

}
