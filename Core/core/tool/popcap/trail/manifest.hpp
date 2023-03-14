#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/trail/version.hpp"

namespace TwinStar::Core::Tool::PopCap::Trail {

	template <auto version> requires (check_version(version, {}, {}))
	struct Manifest {

		#pragma region track node

		template <typename = None>
		struct TrackNode_;

		using TrackNode = TrackNode_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(TrackNode_<_>),
			M_wrap(
				(Floating) time,
				(Floating) low_value,
				(Floating) high_value,
				(Integer) curve,
				(Integer) distribution,
			),
		);

		#pragma endregion

		#pragma region trail

		template <typename = None>
		struct Trail_;

		using Trail = Trail_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop()}, {}))
		M_record_of_map(
			M_wrap(Trail_<_>),
			M_wrap(
				(Integer) maximum_point,
				(Floating) minimum_point_distance,
				(Integer) flag,
				(String) image,
				(List<TrackNode>) width_over_length,
				(List<TrackNode>) width_over_time,
				(List<TrackNode>) alpha_over_length,
				(List<TrackNode>) alpha_over_time,
				(List<TrackNode>) duration,
			),
		);

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::mobile()}, {}))
		M_record_of_map(
			M_wrap(Trail_<_>),
			M_wrap(
				(Integer) maximum_point,
				(Floating) minimum_point_distance,
				(Integer) flag,
				(Integer) image,
				(List<TrackNode>) width_over_length,
				(List<TrackNode>) width_over_time,
				(List<TrackNode>) alpha_over_length,
				(List<TrackNode>) alpha_over_time,
				(List<TrackNode>) duration,
			),
		);

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Trail_<_>),
			M_wrap(
				(Integer) maximum_point,
				(Floating) minimum_point_distance,
				(Integer) flag,
				(String) image,
				(String) image_resource,
				(List<TrackNode>) width_over_length,
				(List<TrackNode>) width_over_time,
				(List<TrackNode>) alpha_over_length,
				(List<TrackNode>) alpha_over_time,
				(List<TrackNode>) duration,
			),
		);

		#pragma endregion

	};

}
