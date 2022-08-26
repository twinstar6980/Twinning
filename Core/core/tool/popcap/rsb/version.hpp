#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/version_utility.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSB {

	struct Version {

		Integer number;
		Integer additional_texture_information_for_pvz_2_chinese_android;

		// ----------------

		friend constexpr auto operator == (
			Version const & thix,
			Version const & that
		) -> bool = default;

	};

	using VersionEnum = ValuePackage<
		Version{3_i, 0_i},
		Version{4_i, 0_i},
		Version{4_i, 1_i},
		Version{4_i, 2_i}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number,
		std::initializer_list<Integer::Value> const & additional_texture_information_for_pvz_2_chinese_android
	) -> ZBoolean {
		auto result = true;
		result &= (number.size() == 0 || Range::has(number, it.number.value));
		result &= (additional_texture_information_for_pvz_2_chinese_android.size() == 0 || Range::has(additional_texture_information_for_pvz_2_chinese_android, it.additional_texture_information_for_pvz_2_chinese_android.value));
		return result;
	}

}

namespace TwinKleS::Core::JSON {

	M_json_value_adapter_for_aggregate_as_object_by_field_of_unique(
		M_wrap(Tool::PopCap::RSB::Version),
		M_wrap(number, additional_texture_information_for_pvz_2_chinese_android),
	);

}
