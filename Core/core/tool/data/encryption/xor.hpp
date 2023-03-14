#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Data::Encryption::XOR {

	struct EncryptCommon {

	protected:

	};

	struct Encrypt :
		EncryptCommon {

	protected:

		static auto process_whole (
			IByteStreamView &     plain,
			OByteStreamView &     cipher,
			CByteListView const & key
		) -> Void {
			if (key.size() == 0_sz) {
				while (!plain.full()) {
					cipher.write(plain.read_of());
				}
			} else if (key.size() == 1_sz) {
				while (!plain.full()) {
					cipher.write(plain.read_of() ^ key.first());
				}
			} else {
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

	public:

		static auto do_process_whole (
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
