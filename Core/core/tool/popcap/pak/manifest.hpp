#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/pak/version.hpp"

namespace TwinKleS::Core::Tool::PopCap::PAK::Manifest {

	template <auto t_version>
	struct Resource;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {}))
	struct Resource<t_version> {
		Integer time;
	};

	// ----------------

	template <auto t_version>
	struct Package;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}, {}))
	struct Package<t_version> {
		Map<Path, Resource<t_version>> resource;
	};

	// ----------------

	using PackageVariant = VariantOfVersion<VersionEnum, Package>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAK::check_version(t_version, {0}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAK::Manifest::Resource<t_version>),
		M_wrap(time),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAK::check_version(t_version, {0}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAK::Manifest::Package<t_version>),
		M_wrap(resource),
	);

}
