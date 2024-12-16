module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.package.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.package.version;

export namespace Twinning::Kernel::Tool::PopCap::Package {

	template <auto version> requires (check_version(version, {}, {}))
	struct Definition {

		#pragma region resource

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {0}, {}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(Path) path,
				(Integer) time,
			),
		);

		#pragma endregion

		#pragma region package

		template <typename = None>
		struct Package_;

		using Package = Package_<>;

		template <typename _> requires (check_version(version, {0}, {}))
		M_record_of_map(
			M_wrap(Package_<_>),
			M_wrap(
				(List<Resource>) resource,
			),
		);

		#pragma endregion

	};

}
