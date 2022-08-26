#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/common/version_utility.hpp"

namespace TwinKleS::Core::Tool::PopCap::PAK {

	struct Version {

		Integer number;
		Boolean compress_resource_data;

		// ----------------

		friend constexpr auto operator == (
			Version const & thix,
			Version const & that
		) -> bool = default;

	};

	using VersionEnum = ValuePackage<
		Version{0_i, k_false},
		Version{0_i, k_true}
	>;

	// ----------------

	inline constexpr auto check_version (
		Version const &                               it,
		std::initializer_list<Integer::Value> const & number,
		std::initializer_list<Boolean::Value> const & compress_resource_data
	) -> ZBoolean {
		auto result = true;
		result &= (number.size() == 0 || Range::has(number, it.number.value));
		result &= (compress_resource_data.size() == 0 || Range::has(compress_resource_data, it.compress_resource_data.value));
		return result;
	}

}

namespace TwinKleS::Core::JSON {

	M_json_value_adapter_for_aggregate_as_object_by_field_of_unique(
		M_wrap(Tool::PopCap::PAK::Version),
		M_wrap(number, compress_resource_data),
	);

}
