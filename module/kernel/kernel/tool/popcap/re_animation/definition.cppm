module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.re_animation.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.re_animation.version;

export namespace Twinning::Kernel::Tool::Popcap::ReAnimation {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Definition {

		#pragma region transform

		M_nested_template_declaration(Transform);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::desktop()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Transform),
			M_wrap(
				(Floater) x,
				(Floater) y,
				(Floater) kx,
				(Floater) ky,
				(Floater) sx,
				(Floater) sy,
				(Floater) f,
				(Floater) a,
				(String) image,
				(String) font,
				(String) text,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::mobile()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Transform),
			M_wrap(
				(Floater) x,
				(Floater) y,
				(Floater) kx,
				(Floater) ky,
				(Floater) sx,
				(Floater) sy,
				(Floater) f,
				(Floater) a,
				(Integer) image,
				(String) font,
				(String) text,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Transform),
			M_wrap(
				(Floater) x,
				(Floater) y,
				(Floater) kx,
				(Floater) ky,
				(Floater) sx,
				(Floater) sy,
				(Floater) f,
				(Floater) a,
				(String) image,
				(String) image_path,
				(String) image_another,
				(String) image_path_another,
				(String) font,
				(String) text,
			),
		);

		#pragma endregion

		#pragma region track

		M_nested_template_declaration(Track);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Track),
			M_wrap(
				(String) name,
				(List<Transform>) transform,
			),
		);

		#pragma endregion

		#pragma region animation

		M_nested_template_declaration(Animation);

		M_nested_template_definition_check(check_version(t_version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Animation),
			M_wrap(
				(Floater) frame_rate,
				(List<Track>) track,
			),
		);

		#pragma endregion

	};

}
