module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.trail.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.trail.version;

export namespace Twinning::Kernel::Tool::Popcap::Trail {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Definition {

		#pragma region track node

		M_nested_template_declaration(TrackNode);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(TrackNode),
			M_wrap(
				(Floater) time,
				(Floater) low_value,
				(Floater) high_value,
				(Integer) curve,
				(Integer) distribution,
			),
		);

		#pragma endregion

		#pragma region trail

		M_nested_template_declaration(Trail);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::desktop()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Trail),
			M_wrap(
				(Integer) maximum_point,
				(Floater) minimum_point_distance,
				(Integer) flag,
				(String) image,
				(List<TrackNode>) width_over_length,
				(List<TrackNode>) width_over_time,
				(List<TrackNode>) alpha_over_length,
				(List<TrackNode>) alpha_over_time,
				(List<TrackNode>) duration,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::mobile()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Trail),
			M_wrap(
				(Integer) maximum_point,
				(Floater) minimum_point_distance,
				(Integer) flag,
				(Integer) image,
				(List<TrackNode>) width_over_length,
				(List<TrackNode>) width_over_time,
				(List<TrackNode>) alpha_over_length,
				(List<TrackNode>) alpha_over_time,
				(List<TrackNode>) duration,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Trail),
			M_wrap(
				(Integer) maximum_point,
				(Floater) minimum_point_distance,
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
