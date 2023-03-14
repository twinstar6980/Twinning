#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/resource_stream_group/version.hpp"
#include "core/tool/popcap/resource_stream_bundle/common.hpp"

namespace TwinStar::Core::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
	struct Manifest {

		#pragma region resource

		template <typename = None>
		struct GenericResourceAdditional_;

		using GenericResourceAdditional = GenericResourceAdditional_<>;

		template <typename _> requires (check_version(version, {3, 4}))
		M_record_of_map(
			M_wrap(GenericResourceAdditional_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct TextureResourceAdditional_;

		using TextureResourceAdditional = TextureResourceAdditional_<>;

		template <typename _> requires (check_version(version, {3, 4}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Integer) index,
				(Size2D<Integer>) size,
			),
		);

		// ----------------

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {3, 4}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(EnumerableVariant<ResourceType, GenericResourceAdditional, TextureResourceAdditional>) additional,
			),
		);

		#pragma endregion

		#pragma region package

		template <typename = None>
		struct Package_;

		using Package = Package_<>;

		template <typename _> requires (check_version(version, {3, 4}))
		M_record_of_map(
			M_wrap(Package_<_>),
			M_wrap(
				(Map<Path, Resource>) resource,
				(ResourceDataSectionStoreMode) resource_data_section_store_mode,
			),
		);

		#pragma endregion

	};

}
