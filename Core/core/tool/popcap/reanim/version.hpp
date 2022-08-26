#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/version_utility.hpp"

namespace TwinKleS::Core::Tool::PopCap::Reanim {

	struct Version {

		enum class Platform : ZIntegerU8 {
			desktop,
			phone,
		};

		// ----------------

		Platform platform;
		Boolean  variant_64;

		// ----------------

		friend constexpr auto operator == (
			Version const & thix,
			Version const & that
		) -> bool = default;

	};

	using VersionEnum = ValuePackage<
		Version{Version::Platform::desktop, k_false},
		Version{Version::Platform::phone, k_false},
		Version{Version::Platform::phone, k_true}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                                  it,
		std::initializer_list<Version::Platform> const & platform,
		std::initializer_list<Boolean::Value> const &    variant_64
	) -> ZBoolean {
		auto result = true;
		result &= (platform.size() == 0 || Range::has(platform, it.platform));
		result &= (variant_64.size() == 0 || Range::has(variant_64, it.variant_64.value));
		return result;
	}

}

namespace TwinKleS::Core::Trait::Reflection {

	M_reflection_enum(
		M_wrap(Tool::PopCap::Reanim::Version::Platform),
		M_wrap(Platform),
		M_wrap(desktop, phone),
	);

}

namespace TwinKleS::Core::JSON {

	M_json_value_adapter_for_aggregate_as_object_by_field_of_unique(
		M_wrap(Tool::PopCap::Reanim::Version),
		M_wrap(platform, variant_64),
	);

}
