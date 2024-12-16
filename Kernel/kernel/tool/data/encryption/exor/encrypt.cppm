module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encryption.exor.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encryption.exor.common;

export namespace Twinning::Kernel::Tool::Data::Encryption::EXOR {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView &     plain,
			OByteStreamView &     cipher,
			CByteListView const & key
		) -> Void {
			if (key.size() == 0_sz) {
				while (!plain.full()) {
					cipher.write(plain.read_of());
				}
			}
			else if (key.size() == 1_sz) {
				while (!plain.full()) {
					cipher.write(plain.read_of() ^ key.first());
				}
			}
			else {
				auto key_index = key.begin_index();
				while (!plain.full()) {
					cipher.write(plain.read_of() ^ key[key_index]);
					++key_index;
					if (key_index == key.end_index()) {
						key_index = key.begin_index();
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &     plain_,
			OByteStreamView &     cipher_,
			CByteListView const & key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, key);
		}

	};

}
