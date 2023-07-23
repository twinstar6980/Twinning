#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/particle_effect/version.hpp"
#include "kernel/tool/popcap/particle_effect/definition.hpp"

namespace TwinStar::Kernel::Tool::PopCap::ParticleEffect {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using VersionNumber = IntegerU32;

	};

}
