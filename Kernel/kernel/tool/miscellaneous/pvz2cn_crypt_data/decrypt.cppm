module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.decrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.common;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.tool.data.encryption.rijndael.decrypt;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnCryptData {

	struct Decrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView & plain,
			InputByteStreamView &  cipher,
			String const &         key
		) -> Void {
			auto rijndael_data_size = Size{};
			estimate_whole(rijndael_data_size, cipher.reserve());
			cipher.read_constant(k_magic_marker);
			auto rijndael_cipher = InputByteStreamView{cipher.forward_view(rijndael_data_size)};
			auto rijndael_plain = OutputByteStreamView{plain.forward_view(rijndael_data_size)};
			auto rijndael_key = compute_rijndael_key(key);
			auto rijndael_initialization_vector = compute_rijndael_initialization_vector(rijndael_key);
			Data::Encryption::Rijndael::Decrypt::process(rijndael_plain, rijndael_cipher, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_rijndael_block_size, rijndael_key, rijndael_initialization_vector);
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size &       plain_size,
			Size const & cipher_size
		) -> Void {
			plain_size = 0_sz;
			assert_test(cipher_size >= bs_static_size<MagicMarker>());
			auto rijndael_data_size = cipher_size - bs_static_size<MagicMarker>();
			assert_test(Math::is_padded_size(rijndael_data_size, cast_box<Size>(k_rijndael_block_size)));
			plain_size += rijndael_data_size;
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView & plain_,
			InputByteStreamView &  cipher_,
			String const &         key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, key);
		}

		inline static auto estimate(
			Size &       plain_size,
			Size const & cipher_size
		) -> Void {
			restruct(plain_size);
			return estimate_whole(plain_size, cipher_size);
		}

	};

}
