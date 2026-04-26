module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.re_animation.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.re_animation.version;
import twinning.kernel.tool.popcap.re_animation.definition;

export namespace Twinning::Kernel::Tool::Popcap::ReAnimation {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Common {

		using Definition = Definition<t_version>;

		// ----------------

		using MagicMarker = IntegerU32;

		inline static constexpr auto k_magic_marker = [] {
			if constexpr (check_version(t_version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicMarker{0xB393B4C0_iu32};
			}
			if constexpr (check_version(t_version, {VersionPlatform::Constant::mobile()}, {false})) {
				return MagicMarker{0xFF2565B5_iu32};
			}
			if constexpr (check_version(t_version, {VersionPlatform::Constant::mobile()}, {true})) {
				return MagicMarker{0xC046E570_iu32};
			}
			if constexpr (check_version(t_version, {VersionPlatform::Constant::television()}, {false})) {
				return MagicMarker{0x00000000_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<!t_version.variant_64.value, IntegerU32, IntegerU64>;

		// ----------------

		inline static constexpr auto k_transform_data_size = [] {
			auto size = 0_sz;
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (check_version(t_version, {VersionPlatform::Constant::television()}, {})) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			return size;
		}();

		inline static constexpr auto k_track_data_size = [] {
			auto size = 0_sz;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (check_version(t_version, {VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {})) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (check_version(t_version, {VersionPlatform::Constant::television()}, {})) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			return size;
		}();

	};

}
