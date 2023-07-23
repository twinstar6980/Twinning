#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/marmalade/dzip/version.hpp"

namespace TwinStar::Kernel::Tool::Marmalade::DZip {

	template <auto version> requires (check_version(version, {}))
	struct Definition {

		#pragma region chunk

		template <typename = None>
		struct Chunk_;

		using Chunk = Chunk_<>;

		template <typename _> requires (check_version(version, {0}))
		M_record_of_map(
			M_wrap(Chunk_<_>),
			M_wrap(
				(String) flag,
			),
		);

		#pragma endregion

		#pragma region resource

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {0}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(List<Chunk>) chunk,
			),
		);

		#pragma endregion

		#pragma region package

		template <typename = None>
		struct Package_;

		using Package = Package_<>;

		template <typename _> requires (check_version(version, {0}))
		M_record_of_map(
			M_wrap(Package_<_>),
			M_wrap(
				(Map<Path, Resource>) resource,
			),
		);

		#pragma endregion

	};

}
