module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encryption.exor.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encryption.exor.common;

export namespace Twinning::Kernel::Tool::Data::Encryption::Exor {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &        raw,
			OutputByteStreamView &       ripe,
			ConstantByteListView const & key
		) -> Void {
			if (key.size() == 0_sz) {
				while (!raw.full()) {
					ripe.write(raw.read_of());
				}
			}
			else if (key.size() == 1_sz) {
				while (!raw.full()) {
					ripe.write(raw.read_of() ^ key.first());
				}
			}
			else {
				auto key_index = key.begin_index();
				while (!raw.full()) {
					ripe.write(raw.read_of() ^ key[key_index]);
					++key_index;
					if (key_index == key.end_index()) {
						key_index = key.begin_index();
					}
				}
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &        raw_,
			OutputByteStreamView &       ripe_,
			ConstantByteListView const & key
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, key);
		}

	};

}
