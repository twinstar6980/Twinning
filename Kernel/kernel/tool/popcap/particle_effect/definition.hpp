#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/particle_effect/version.hpp"

namespace TwinStar::Kernel::Tool::PopCap::ParticleEffect {

	template <auto version> requires (check_version(version, {}))
	struct Definition {

		#pragma region effect

		template <typename = None>
		struct Effect_;

		using Effect = Effect_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(Effect_<_>),
			M_wrap(
			),
		);

		#pragma endregion

	};

}
