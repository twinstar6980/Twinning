#pragma once

#include "kernel/utility/utility.hpp"

namespace TwinStar::Kernel::Tool::PopCap::CharacterFontWidget2 {

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
