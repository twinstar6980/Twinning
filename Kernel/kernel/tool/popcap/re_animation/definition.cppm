module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.re_animation.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.re_animation.version;

export namespace Twinning::Kernel::Tool::PopCap::ReAnimation {

	template <auto version> requires (check_version(version, {}, {}))
	struct Definition {

		#pragma region transform

		template <typename = None>
		struct Transform_;

		using Transform = Transform_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop()}, {}))
		M_record_of_map(
			M_wrap(Transform_<_>),
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

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::mobile()}, {}))
		M_record_of_map(
			M_wrap(Transform_<_>),
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

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Transform_<_>),
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

		template <typename = None>
		struct Track_;

		using Track = Track_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Track_<_>),
			M_wrap(
				(String) name,
				(List<Transform>) transform,
			),
		);

		#pragma endregion

		#pragma region animation

		template <typename = None>
		struct Animation_;

		using Animation = Animation_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Animation_<_>),
			M_wrap(
				(Floater) frame_rate,
				(List<Track>) track,
			),
		);

		#pragma endregion

	};

}
