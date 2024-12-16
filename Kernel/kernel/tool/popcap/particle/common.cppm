module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.particle.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.particle.version;
import twinning.kernel.tool.popcap.particle.definition;

export namespace Twinning::Kernel::Tool::PopCap::Particle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = [] {
			if constexpr (check_version(version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicIdentifier{0x411F994D_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {false})) {
				return MagicIdentifier{0x411F994D_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {true})) {
				return MagicIdentifier{0xE09295E9_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {false})) {
				return MagicIdentifier{0x00000000_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<!version.variant_64.value, IntegerU32, IntegerU64>;

		// ----------------

		inline static constexpr auto k_field_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			return size;
		}();

		inline static constexpr auto k_emitter_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerOfPlatform>() * 40_sz * 2_sz;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			return size;
		}();

	};

}
