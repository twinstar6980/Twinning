#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::PopCap::SexyTexture {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 0_i}
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
