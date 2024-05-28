#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/encryption/xor/common.hpp"

namespace Twinning::Kernel::Tool::Data::Encryption::XOR {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
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

		static auto process (
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
