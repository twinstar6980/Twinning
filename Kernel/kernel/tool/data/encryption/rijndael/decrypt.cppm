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

		inline static auto process_whole(
			OutputByteStreamView &       plain,
			InputByteStreamView &        cipher,
			Mode const &                 mode,
			Integer const &              block_size,
			ConstantByteListView const & key,
			ConstantByteListView const & initialization_vector
		) -> Void {
			assert_test(is_padded_size(cipher.reserve(), cbox<Size>(block_size)));
			assert_test(is_valid_block_size(block_size));
			assert_test(is_valid_block_size(cbox<Integer>(key.size())));
			assert_test(is_padded_size(bs_size(initialization_vector) * k_type_bit_count<Character>, cbox<Size>(block_size)));
			auto initialization_vector_view = mode == Mode::Constant::ecb() ? (k_empty_initialization_vector.view().head(cbox<Size>(block_size))) : (initialization_vector);
			auto cipher_size = cipher.reserve();
			if (cipher_size != 0_sz) {
				auto rijndael = Third::Rijndael::CRijndael{};
				rijndael.MakeKey(rubox<char const *>(key.begin()), rubox<char const *>(initialization_vector_view.begin()), ubox<int>(key.size()), ubox<int>(block_size));
				rijndael.Decrypt(rubox<char const *>(cipher.current_pointer()), rubox<char *>(plain.current_pointer()), ubox<std::size_t>(cipher.reserve()), ubox<int>(mode));
			}
			cipher.forward(cipher_size);
			plain.forward(cipher_size);
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView &       plain_,
			InputByteStreamView &        cipher_,
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
