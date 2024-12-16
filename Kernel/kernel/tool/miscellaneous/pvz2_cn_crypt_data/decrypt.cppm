module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.decrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.common;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.tool.data.encryption.rijndael.decrypt;

export namespace Twinning::Kernel::Tool::Miscellaneous::PvZ2CNCryptData {

	struct Decrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView & cipher,
			OByteStreamView & plain,
			String const &    key
		) -> Void {
			auto rijndael_data_size = Size{};
			estimate_whole(cipher.reserve(), rijndael_data_size);
			cipher.read_constant(k_magic_identifier);
			auto rijndael_cipher = IByteStreamView{cipher.forward_view(rijndael_data_size)};
			auto rijndael_plain = OByteStreamView{plain.forward_view(rijndael_data_size)};
			auto rijndael_key = compute_rijndael_key(key);
			auto rijndael_iv = compute_rijndael_iv(rijndael_key);
			Data::Encryption::Rijndael::Decrypt::process(rijndael_cipher, rijndael_plain, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_crypt_block_size, k_crypt_key_size, rijndael_key, rijndael_iv);
			return;
		}

		// ----------------

		inline static auto estimate_whole (
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

		inline static auto process (
			IByteStreamView & cipher_,
			OByteStreamView & plain_,
			String const &    key
		) -> Void {
			M_use_zps_of(cipher);
			M_use_zps_of(plain);
			return process_whole(cipher, plain, key);
		}

		inline static auto estimate (
			Size const & cipher_size,
			Size &       plain_size
		) -> Void {
			restruct(plain_size);
			return estimate_whole(cipher_size, plain_size);
		}

	};

}
