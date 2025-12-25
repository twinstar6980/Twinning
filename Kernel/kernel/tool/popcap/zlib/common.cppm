module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.zlib.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.zlib.version;

export namespace Twinning::Kernel::Tool::Popcap::Zlib {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using MagicMarker = IntegerU32;

		inline static constexpr auto k_magic_marker = MagicMarker{0xDEADFED4_iu32};

		// ----------------

		using IntegerOfPlatform = AsSwitch<!version.variant_64.value, IntegerU32, IntegerU64>;

		// ----------------

		M_record_of_data(
			M_wrap(Header),
			M_wrap(
				(IntegerOfPlatform) raw_size,
			),
		);

	};

}
