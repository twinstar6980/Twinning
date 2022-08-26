#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/marmalade/dzip/version.hpp"

namespace TwinKleS::Core::Tool::Marmalade::DZip::Manifest {

	template <auto t_version>
	struct Chunk;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct Chunk<t_version> {
		String flag;
	};

	// ----------------

	template <auto t_version>
	struct Resource;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct Resource<t_version> {
		List<Chunk<t_version>> chunk;
	};

	// ----------------

	template <auto t_version>
	struct Package;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {0}))
	struct Package<t_version> {
		Map<Path, Resource<t_version>> resource;
	};

	// ----------------

	using PackageVariant = VariantOfVersion<VersionEnum, Package>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Manifest::Chunk<t_version>),
		M_wrap(flag),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Manifest::Resource<t_version>),
		M_wrap(chunk),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Marmalade::DZip::check_version(t_version, {0}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::Marmalade::DZip::Manifest::Package<t_version>),
		M_wrap(resource),
	);

}
