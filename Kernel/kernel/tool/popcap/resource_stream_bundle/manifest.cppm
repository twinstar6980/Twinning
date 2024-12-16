module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.manifest;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamBundle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Manifest {

		#pragma region resource

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {1}, {}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(String) id,
				(Path) path,
				(Integer) type,
				(Map<String, String>) property,
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
