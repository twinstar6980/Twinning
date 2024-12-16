module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encryption.rijndael.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.third.Rijndael;

export namespace Twinning::Kernel::Tool::Data::Encryption::Rijndael {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView & plain,
			OByteStreamView & cipher,
			Mode const &      mode,
			Size const &      block_size,
			Size const &      key_size,
			String const &    key,
			String const &    iv
		) -> Void {
			assert_test(is_padded_size(plain.reserve(), block_size));
			assert_test(is_valid_block_size(block_size));
			assert_test(is_valid_block_size(key_size));
			assert_test(is_padded_size(bs_size(key) * k_type_bit_count<Character>, key_size));
			assert_test(is_padded_size(bs_size(iv) * k_type_bit_count<Character>, block_size));
			auto iv_view = mode == Mode::Constant::ecb() ? (k_empty_iv.view().head(block_size)) : (iv.view());
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

		// ----------------

		inline static auto process (
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

}
