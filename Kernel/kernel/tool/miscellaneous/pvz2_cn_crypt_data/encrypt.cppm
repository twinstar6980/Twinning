module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.common;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.tool.data.encryption.rijndael.encrypt;

export namespace Twinning::Kernel::Tool::Miscellaneous::PvZ2CNCryptData {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
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
			}
			else {
				rijndael_plain_container.allocate(rijndael_data_size);
				Range::assign_from(rijndael_plain_container.head(plain_data.size()), plain_data);
				rijndael_plain.set(rijndael_plain_container);
			}
			cipher.write_constant(k_magic_identifier);
			auto rijndael_cipher = OByteStreamView{cipher.forward_view(rijndael_data_size)};
			auto rijndael_key = compute_rijndael_key(key);
			auto rijndael_iv = compute_rijndael_iv(rijndael_key);
			Data::Encryption::Rijndael::Encrypt::process(rijndael_plain, rijndael_cipher, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_crypt_block_size, k_crypt_key_size, rijndael_key, rijndael_iv);
			return;
		}

		// ----------------

		inline static auto estimate_whole (
			Size const & plain_size,
			Size &       cipher_size
		) -> Void {
			cipher_size = k_none_size;
			cipher_size += bs_static_size<MagicIdentifier>();
			cipher_size += compute_padded_size(plain_size, k_crypt_block_size);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & plain_,
			OByteStreamView & cipher_,
			String const &    key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, key);
		}

		inline static auto estimate (
			Size const & plain_size,
			Size &       cipher_size
		) -> Void {
			restruct(cipher_size);
			return estimate_whole(plain_size, cipher_size);
		}

	};

}
