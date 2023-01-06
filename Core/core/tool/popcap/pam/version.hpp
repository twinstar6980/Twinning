#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::PopCap::PAM {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 1_i},
		Version{.number = 2_i},
		Version{.number = 3_i},
		Version{.number = 4_i},
		Version{.number = 5_i},
		Version{.number = 6_i}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (number.size() == 0 || Range::has(number, it.number.value));
		return result;
	}

}
