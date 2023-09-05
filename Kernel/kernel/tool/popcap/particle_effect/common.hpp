#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/particle_effect/version.hpp"
#include "kernel/tool/popcap/particle_effect/definition.hpp"

namespace TwinStar::Kernel::Tool::PopCap::ParticleEffect {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using MagicIdentifier = StaticByteArray<5_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x04_b, 'P'_b, 'P'_b, 'F'_b, '1'_b}};

		// ----------------

		using VersionNumber = IntegerU32;

	};

}
