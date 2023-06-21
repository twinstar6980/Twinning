#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/particle_effect/version.hpp"

namespace TwinStar::Core::Tool::PopCap::ParticleEffect {

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
