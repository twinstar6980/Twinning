module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.render_effect.version;
import twinning.kernel.tool.popcap.render_effect.definition;

export namespace Twinning::Kernel::Tool::Popcap::RenderEffect {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Common {

		using Definition = Definition<t_version>;

		// ----------------

		using MagicMarker = IntegerU32;

		inline static constexpr auto k_magic_marker = MagicMarker{0x70636678_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

	};

}
