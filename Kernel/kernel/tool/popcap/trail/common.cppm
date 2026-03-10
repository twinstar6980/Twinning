module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.trail.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.trail.version;
import twinning.kernel.tool.popcap.trail.definition;

export namespace Twinning::Kernel::Tool::Popcap::Trail {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Common {

		using Definition = Definition<t_version>;

		// ----------------

		using MagicMarker = IntegerU32;

		inline static constexpr auto k_magic_marker = [] {
			if constexpr (check_version(t_version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicMarker{0xAB8B62B3_iu32};
			}
			if constexpr (check_version(t_version, {VersionPlatform::Constant::mobile()}, {false})) {
				return MagicMarker{0xAB8B62B3_iu32};
			}
			if constexpr (check_version(t_version, {VersionPlatform::Constant::mobile()}, {true})) {
				return MagicMarker{0x8488BC08_iu32};
			}
			if constexpr (check_version(t_version, {VersionPlatform::Constant::television()}, {false})) {
				return MagicMarker{0xAB8B62B3_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<!t_version.variant_64.value, IntegerU32, IntegerU64>;

	};

}
