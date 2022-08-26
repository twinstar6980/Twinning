#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rsb/version.hpp"
#include "core/tool/popcap/rsb/common.hpp"

namespace TwinKleS::Core::Tool::PopCap::RSB::Manifest {

	template <auto t_version>
	struct GenericResourceAdditional;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct GenericResourceAdditional<t_version> {
	};

	// ----------------

	template <auto t_version>
	struct TextureResourceAdditional;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {0, 1}))
	struct TextureResourceAdditional<t_version> {
		Size2D<Integer> size;
		Integer         format;
		Integer         bit_per_channel;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {2}))
	struct TextureResourceAdditional<t_version> {
		Size2D<Integer> size;
		Integer         scale;
		Integer         format;
		Integer         bit_per_channel;
	};

	// ----------------

	template <auto t_version>
	struct Resource;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Resource<t_version> {
		EnumerableVariant<ResourceType, GenericResourceAdditional<t_version>, TextureResourceAdditional<t_version>> additional;
	};

	// ----------------

	template <auto t_version>
	struct Subgroup;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {3, 4}, {}))
	struct Subgroup<t_version> {
		SubgroupCategory               category;
		Map<Path, Resource<t_version>> resource;
		ResourceDataSectionStoreMode   resource_data_section_store_mode;
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

	using PackageVariant = VariantOfVersion<VersionEnum, Package>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::GenericResourceAdditional<t_version>),
		M_wrap(),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {0, 1}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::TextureResourceAdditional<t_version>),
		M_wrap(size, format, bit_per_channel),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {2}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::TextureResourceAdditional<t_version>),
		M_wrap(size, scale, format, bit_per_channel),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::Resource<t_version>),
		M_wrap(additional),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::Subgroup<t_version>),
		M_wrap(category, resource, resource_data_section_store_mode),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::Group<t_version>),
		M_wrap(composite, subgroup),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::RSB::check_version(t_version, {3, 4}, {}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::RSB::Manifest::Package<t_version>),
		M_wrap(group),
	);

}
