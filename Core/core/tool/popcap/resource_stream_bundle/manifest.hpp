#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_bundle/version.hpp"
#include "core/tool/popcap/resource_stream_bundle/common.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Manifest {

		#pragma region resource

		template <typename = None>
		struct GenericResourceAdditional_;

		using GenericResourceAdditional = GenericResourceAdditional_<>;

		template <typename _> requires (check_version(version, {3, 4}, {}))
		M_record_of_map(
			M_wrap(GenericResourceAdditional_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct TextureResourceAdditional_;

		using TextureResourceAdditional = TextureResourceAdditional_<>;

		template <typename _> requires (check_version(version, {3, 4}, {0, 1}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Size2D<Integer>) size,
				(Integer) format,
				(Integer) row_byte_count,
			),
		);

		template <typename _> requires (check_version(version, {3, 4}, {2}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Size2D<Integer>) size,
				(Integer) format,
				(Integer) row_byte_count,
				(Integer) scale,
			),
		);

		// ----------------

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {3, 4}, {}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(EnumerableVariant<ResourceType, GenericResourceAdditional, TextureResourceAdditional>) additional,
			),
		);

		#pragma endregion

		#pragma region group

		template <typename = None>
		struct Subgroup_;

		using Subgroup = Subgroup_<>;

		template <typename _> requires (check_version(version, {3, 4}, {}))
		M_record_of_map(
			M_wrap(Subgroup_<_>),
			M_wrap(
				(SubgroupCategory) category,
				(Map<Path, Resource>) resource,
				(ResourceDataSectionStoreMode) resource_data_section_store_mode,
			),
		);

		// ----------------

		template <typename = None>
		struct Group_;

		using Group = Group_<>;

		template <typename _> requires (check_version(version, {3, 4}, {}))
		M_record_of_map(
			M_wrap(Group_<_>),
			M_wrap(
				(Boolean) composite,
				(Map<String, Subgroup>) subgroup,
			),
		);

		#pragma endregion

		#pragma region package

		template <typename = None>
		struct Package_;

		using Package = Package_<>;

		template <typename _> requires (check_version(version, {3, 4}, {}))
		M_record_of_map(
			M_wrap(Package_<_>),
			M_wrap(
				(Map<String, Group>) group,
			),
		);

		#pragma endregion

	};

}
