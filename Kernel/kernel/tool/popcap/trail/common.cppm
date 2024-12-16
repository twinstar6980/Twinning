module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.trail.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.trail.version;
import twinning.kernel.tool.popcap.trail.definition;

export namespace Twinning::Kernel::Tool::PopCap::Trail {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = [] {
			if constexpr (check_version(version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicIdentifier{0xAB8B62B3_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {false})) {
				return MagicIdentifier{0xAB8B62B3_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {true})) {
				return MagicIdentifier{0x8488BC08_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {false})) {
				return MagicIdentifier{0xAB8B62B3_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<!version.variant_64.value, IntegerU32, IntegerU64>;

	};

}
