module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.crypt_data.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.crypt_data.version;

export namespace Twinning::Kernel::Tool::PopCap::CryptData {

	template <auto version> requires (check_version(version))
	struct Common {

		using MagicIdentifier = StaticByteArray<11_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x43_b, 0x52_b, 0x59_b, 0x50_b, 0x54_b, 0x5F_b, 0x52_b, 0x45_b, 0x53_b, 0x0A_b, 0x00_b}};

		// ----------------

		M_record_of_data(
			M_wrap(Header),
			M_wrap(
				(IntegerU64) plain_size,
			),
		);

	};

}
