#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::PopCap::CryptData {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
		),
	);

	using VersionPackage = ValuePackage<
		Version{}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const & it
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		return result;
	}

}
