#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/reanim/version.hpp"

namespace TwinStar::Core::Tool::PopCap::REANIM {

	template <auto version> requires (check_version(version, {}, {}))
	struct Manifest {

		#pragma region transform

		template <typename = None>
		struct Transform_;

		using Transform = Transform_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop()}, {}))
		M_record_of_map(
			M_wrap(Transform_<_>),
			M_wrap(
				(Floating) x,
				(Floating) y,
				(Floating) kx,
				(Floating) ky,
				(Floating) sx,
				(Floating) sy,
				(Floating) f,
				(Floating) a,
				(String) image,
				(String) font,
				(String) text,
			),
		);

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::mobile()}, {}))
		M_record_of_map(
			M_wrap(Transform_<_>),
			M_wrap(
				(Floating) x,
				(Floating) y,
				(Floating) kx,
				(Floating) ky,
				(Floating) sx,
				(Floating) sy,
				(Floating) f,
				(Floating) a,
				(Integer) image,
				(String) font,
				(String) text,
			),
		);

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Transform_<_>),
			M_wrap(
				(Floating) x,
				(Floating) y,
				(Floating) kx,
				(Floating) ky,
				(Floating) sx,
				(Floating) sy,
				(Floating) f,
				(Floating) a,
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
				(Floating) frame_rate,
				(List<Track>) track,
			),
		);

		#pragma endregion

	};

}
