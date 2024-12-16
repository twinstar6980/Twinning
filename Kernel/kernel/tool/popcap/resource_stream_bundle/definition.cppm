module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Definition {

		#pragma region resource

		template <typename = None>
		struct GeneralResourceAdditional_;

		using GeneralResourceAdditional = GeneralResourceAdditional_<>;

		template <typename _> requires (check_version(version, {1}, {}))
		M_record_of_map(
			M_wrap(GeneralResourceAdditional_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct TextureResourceAdditional_;

		using TextureResourceAdditional = TextureResourceAdditional_<>;

		template <typename _> requires (check_version(version, {1}, {0, 1}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Size2<Integer>) size,
				(Integer) format,
				(Integer) pitch,
			),
		);

		template <typename _> requires (check_version(version, {4}, {1, 2}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Size2<Integer>) size,
				(Integer) format,
				(Integer) pitch,
				(Integer) additional_byte_count,
			),
		);

		template <typename _> requires (check_version(version, {4}, {2}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Size2<Integer>) size,
				(Integer) format,
				(Integer) pitch,
				(Integer) additional_byte_count,
				(Integer) scale,
			),
		);

		// ----------------

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {1}, {}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(Path) path,
				(EnumerableVariant<ResourceType, GeneralResourceAdditional, TextureResourceAdditional>) additional,
			),
		);

		#pragma endregion

		#pragma region group

		template <typename = None>
		struct SubgroupCategory_;

		using SubgroupCategory = SubgroupCategory_<>;

		template <typename _> requires (check_version(version, {1, 3}, {}))
		M_record_of_map(
			M_wrap(SubgroupCategory_<_>),
			M_wrap(
				(Optional<Integer>) resolution,
			),
		);

		template <typename _> requires (check_version(version, {3}, {}))
		M_record_of_map(
			M_wrap(SubgroupCategory_<_>),
			M_wrap(
				(Optional<Integer>) resolution,
				(Optional<String>) locale,
			),
		);

		// ----------------

		template <typename = None>
		struct Subgroup_;

		using Subgroup = Subgroup_<>;

		template <typename _> requires (check_version(version, {1}, {}))
		M_record_of_map(
			M_wrap(Subgroup_<_>),
			M_wrap(
				(String) id,
				(SubgroupCategory) category,
				(PacketCompression) compression,
				(List<Resource>) resource,
			),
		);

		// ----------------

		template <typename = None>
		struct Group_;

		using Group = Group_<>;

		template <typename _> requires (check_version(version, {1}, {}))
		M_record_of_map(
			M_wrap(Group_<_>),
			M_wrap(
				(String) id,
				(Boolean) composite,
				(List<Subgroup>) subgroup,
			),
		);

		#pragma endregion

		#pragma region package

		template <typename = None>
		struct Package_;

		using Package = Package_<>;

		template <typename _> requires (check_version(version, {1}, {}))
		M_record_of_map(
			M_wrap(Package_<_>),
			M_wrap(
				(List<Group>) group,
			),
		);

		#pragma endregion

	};

}
