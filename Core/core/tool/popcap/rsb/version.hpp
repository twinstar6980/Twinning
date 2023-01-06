#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::PopCap::RSB {

	M_record_of_map(
		M_wrap(Version),
		M_wrap(
			(Integer) number,
			(Integer) additional_texture_information_for_pvz_2_chinese_android,
		),
	);

	using VersionPackage = ValuePackage<
		Version{.number = 3_i, .additional_texture_information_for_pvz_2_chinese_android = 0_i},
		Version{.number = 4_i, .additional_texture_information_for_pvz_2_chinese_android = 0_i},
		Version{.number = 4_i, .additional_texture_information_for_pvz_2_chinese_android = 1_i},
		Version{.number = 4_i, .additional_texture_information_for_pvz_2_chinese_android = 2_i}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number,
		std::initializer_list<Integer::Value> const & additional_texture_information_for_pvz_2_chinese_android
	) -> ZBoolean {
		auto result = true;
		result &= VersionPackage::has(it);
		result &= (number.size() == 0 || Range::has(number, it.number.value));
		result &= (additional_texture_information_for_pvz_2_chinese_android.size() == 0 || Range::has(additional_texture_information_for_pvz_2_chinese_android, it.additional_texture_information_for_pvz_2_chinese_android.value));
		return result;
	}

}
