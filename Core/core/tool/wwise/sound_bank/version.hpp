#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Wwise::SoundBank {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 88_i},
		Version{.number = 112_i},
		Version{.number = 140_i}
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
