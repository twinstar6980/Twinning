#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsb/version.hpp"
#include "core/tool/popcap/rsb/common.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSB::Description {

	template <auto t_version>
	struct Resource;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Resource<t_version> {
		Path                path;
		Integer             type;
		Map<String, String> property;
	};

	// ----------------

	template <auto t_version>
	struct Subgroup;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Subgroup<t_version> {
		SubgroupCategory                 category;
		Map<String, Resource<t_version>> resource;
	};

	// ----------------

	template <auto t_version>
	struct Group;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Group<t_version> {
		Boolean                          composite;
		Map<String, Subgroup<t_version>> subgroup;
	};

	// ----------------

	template <auto t_version>
	struct Package;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Package<t_version> {
		Map<String, Group<t_version>> group;
	};

	// ----------------

	using PackageOptionalVariant = VariantOfVersion<VersionEnum, Package, Optional>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Description::Resource<t_version>),
		M_wrap(path, type, property),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Description::Subgroup<t_version>),
		M_wrap(category, resource),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Description::Group<t_version>),
		M_wrap(composite, subgroup),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Description::Package<t_version>),
		M_wrap(group),
	);

}
