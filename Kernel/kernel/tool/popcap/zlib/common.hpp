#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/zlib/version.hpp"

namespace Twinning::Kernel::Tool::PopCap::ZLib {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0xDEADFED4_iu32};

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
