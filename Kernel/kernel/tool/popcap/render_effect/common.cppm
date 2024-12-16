module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.render_effect.version;
import twinning.kernel.tool.popcap.render_effect.definition;

export namespace Twinning::Kernel::Tool::PopCap::RenderEffect {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0x70636678_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

	};

}
