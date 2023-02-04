#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Data::Encryption::XOR {

	struct EncryptCommon {

	protected:

	};

	struct Encrypt :
		EncryptCommon {

	protected:

		// TODO : key -> CByteListView
		static auto process_whole (
			IByteStreamView & plain,
			OByteStreamView & cipher,
			Byte const &      key
		) -> Void {
			while (!plain.full()) {
				cipher.write(plain.read_of() ^ key);
			}
			return;
		}

	public:

		static auto do_process_whole (
			IByteStreamView & plain_,
			OByteStreamView & cipher_,
			Byte const &      key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, key);
		}

	};

}
