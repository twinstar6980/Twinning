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

		inline static auto process_whole(
			InputByteStreamView &        plain,
			OutputByteStreamView &       cipher,
			Mode const &                 mode,
			Integer const &              block_size,
			ConstantByteListView const & key,
			ConstantByteListView const & initialization_vector
		) -> Void {
			assert_test(is_padded_size(plain.reserve(), cbox<Size>(block_size)));
			assert_test(is_valid_block_size(block_size));
			assert_test(is_valid_block_size(cbox<Integer>(key.size())));
			assert_test(is_padded_size(bs_size(initialization_vector) * k_type_bit_count<Character>, cbox<Size>(block_size)));
			auto initialization_vector_view = mode == Mode::Constant::ecb() ? (k_empty_initialization_vector.view().head(cbox<Size>(block_size))) : (initialization_vector);
			auto plain_size = plain.reserve();
			if (plain_size != 0_sz) {
				auto rijndael = Third::Rijndael::CRijndael{};
				rijndael.MakeKey(rubox<char const *>(key.begin()), rubox<char const *>(initialization_vector_view.begin()), ubox<int>(key.size()), ubox<int>(block_size));
				rijndael.Encrypt(rubox<char const *>(plain.current_pointer()), rubox<char *>(cipher.current_pointer()), ubox<std::size_t>(plain.reserve()), ubox<int>(mode));
			}
			plain.forward(plain_size);
			cipher.forward(plain_size);
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &        plain_,
			OutputByteStreamView &       cipher_,
			Mode const &                 mode,
			Integer const &              block_size,
			ConstantByteListView const & key,
			ConstantByteListView const & initialization_vector
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, mode, block_size, key, initialization_vector);
		}

	};

}
