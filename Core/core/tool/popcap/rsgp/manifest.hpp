#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsgp/version.hpp"
#include "core/tool/popcap/rsb/common.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSGP::Manifest {

	template <auto t_version>
	struct GenericResourceAdditional;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct GenericResourceAdditional<t_version> {
	};

	// ----------------

	template <auto t_version>
	struct TextureResourceAdditional;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct TextureResourceAdditional<t_version> {
		Integer         index;
		Size2D<Integer> size;
	};

	// ----------------

	template <auto t_version>
	struct Resource;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct Resource<t_version> {
		EnumerableVariant<ResourceType, GenericResourceAdditional<t_version>, TextureResourceAdditional<t_version>> additional;
	};

	// ----------------

	template <auto t_version>
	struct Package;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}))
	struct Package<t_version> {
		Map<Path, Resource<t_version>> resource;
		ResourceDataSectionStoreMode   resource_data_section_store_mode;
	};

	// ----------------

	using PackageVariant = VariantOfVersion<VersionEnum, Package>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Manifest::GenericResourceAdditional<t_version>),
		M_wrap(),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Manifest::TextureResourceAdditional<t_version>),
		M_wrap(index, size),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Manifest::Resource<t_version>),
		M_wrap(additional),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSGP::check_version(t_version, {3, 4}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSGP::Manifest::Package<t_version>),
		M_wrap(resource, resource_data_section_store_mode),
	);

}
