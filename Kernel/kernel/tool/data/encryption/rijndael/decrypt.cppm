module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encryption.rijndael.decrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.third.Rijndael;

export namespace Twinning::Kernel::Tool::Data::Encryption::Rijndael {

	struct Decrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView & cipher,
			OByteStreamView & plain,
			Mode const &      mode,
			Size const &      block_size,
			Size const &      key_size,
			String const &    key,
			String const &    iv
		) -> Void {
			assert_test(is_padded_size(cipher.reserve(), block_size));
			assert_test(is_valid_block_size(block_size));
			assert_test(is_valid_block_size(key_size));
			assert_test(is_padded_size(bs_size(key) * k_type_bit_count<Character>, key_size));
			assert_test(is_padded_size(bs_size(iv) * k_type_bit_count<Character>, block_size));
			auto iv_view = mode == Mode::Constant::ecb() ? (k_empty_iv.view().head(block_size)) : (iv.view());
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

		// ----------------

		inline static auto process (
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
