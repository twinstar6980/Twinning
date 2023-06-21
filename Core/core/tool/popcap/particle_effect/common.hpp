#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/particle_effect/version.hpp"
#include "core/tool/popcap/particle_effect/definition.hpp"

namespace TwinStar::Core::Tool::PopCap::ParticleEffect {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using VersionNumber = IntegerU32;

	};

}
