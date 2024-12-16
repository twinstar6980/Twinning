module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.particle.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.particle.version;

export namespace Twinning::Kernel::Tool::PopCap::Particle {

	template <auto version> requires (check_version(version, {}, {}))
	struct Definition {

		#pragma region track node

		template <typename = None>
		struct TrackNode_;

		using TrackNode = TrackNode_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(TrackNode_<_>),
			M_wrap(
				(Floater) time,
				(Floater) low_value,
				(Floater) high_value,
				(Integer) curve,
				(Integer) distribution,
			),
		);

		#pragma endregion

		#pragma region field

		template <typename = None>
		struct Field_;

		using Field = Field_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Field_<_>),
			M_wrap(
				(Integer) type,
				(List<TrackNode>) offset_x,
				(List<TrackNode>) offset_y,
			),
		);

		#pragma endregion

		#pragma region emitter

		template <typename = None>
		struct Emitter_;

		using Emitter = Emitter_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop()}, {}))
		M_record_of_map(
			M_wrap(Emitter_<_>),
			M_wrap(
				(String) name,
				(String) image,
				(Integer) image_column,
				(Integer) image_row,
				(Integer) image_frame,
				(Integer) animated,
				(Integer) flag,
				(Integer) type,
				(String) on_duration,
				(List<TrackNode>) system_duration,
				(List<TrackNode>) cross_fade_duration,
				(List<TrackNode>) spawn_rate,
				(List<TrackNode>) spawn_minimum_active,
				(List<TrackNode>) spawn_maximum_active,
				(List<TrackNode>) spawn_maximum_launched,
				(List<TrackNode>) emitter_radius,
				(List<TrackNode>) emitter_offset_x,
				(List<TrackNode>) emitter_offset_y,
				(List<TrackNode>) emitter_box_x,
				(List<TrackNode>) emitter_box_y,
				(List<TrackNode>) emitter_path,
				(List<TrackNode>) emitter_skew_x,
				(List<TrackNode>) emitter_skew_y,
				(List<TrackNode>) particle_duration,
				(List<TrackNode>) system_red,
				(List<TrackNode>) system_green,
				(List<TrackNode>) system_blue,
				(List<TrackNode>) system_alpha,
				(List<TrackNode>) system_brightness,
				(List<TrackNode>) launch_speed,
				(List<TrackNode>) launch_angle,
				(List<Field>) field,
				(List<Field>) system_field,
				(List<TrackNode>) particle_red,
				(List<TrackNode>) particle_green,
				(List<TrackNode>) particle_blue,
				(List<TrackNode>) particle_alpha,
				(List<TrackNode>) particle_brightness,
				(List<TrackNode>) particle_spin_angle,
				(List<TrackNode>) particle_spin_speed,
				(List<TrackNode>) particle_scale,
				(List<TrackNode>) particle_stretch,
				(List<TrackNode>) collision_reflect,
				(List<TrackNode>) collision_spin,
				(List<TrackNode>) clip_top,
				(List<TrackNode>) clip_bottom,
				(List<TrackNode>) clip_left,
				(List<TrackNode>) clip_right,
				(List<TrackNode>) animation_rate,
			),
		);

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::mobile()}, {}))
		M_record_of_map(
			M_wrap(Emitter_<_>),
			M_wrap(
				(String) name,
				(Integer) image,
				(Integer) image_column,
				(Integer) image_row,
				(Integer) image_frame,
				(Integer) animated,
				(Integer) flag,
				(Integer) type,
				(String) on_duration,
				(List<TrackNode>) system_duration,
				(List<TrackNode>) cross_fade_duration,
				(List<TrackNode>) spawn_rate,
				(List<TrackNode>) spawn_minimum_active,
				(List<TrackNode>) spawn_maximum_active,
				(List<TrackNode>) spawn_maximum_launched,
				(List<TrackNode>) emitter_radius,
				(List<TrackNode>) emitter_offset_x,
				(List<TrackNode>) emitter_offset_y,
				(List<TrackNode>) emitter_box_x,
				(List<TrackNode>) emitter_box_y,
				(List<TrackNode>) emitter_path,
				(List<TrackNode>) emitter_skew_x,
				(List<TrackNode>) emitter_skew_y,
				(List<TrackNode>) particle_duration,
				(List<TrackNode>) system_red,
				(List<TrackNode>) system_green,
				(List<TrackNode>) system_blue,
				(List<TrackNode>) system_alpha,
				(List<TrackNode>) system_brightness,
				(List<TrackNode>) launch_speed,
				(List<TrackNode>) launch_angle,
				(List<Field>) field,
				(List<Field>) system_field,
				(List<TrackNode>) particle_red,
				(List<TrackNode>) particle_green,
				(List<TrackNode>) particle_blue,
				(List<TrackNode>) particle_alpha,
				(List<TrackNode>) particle_brightness,
				(List<TrackNode>) particle_spin_angle,
				(List<TrackNode>) particle_spin_speed,
				(List<TrackNode>) particle_scale,
				(List<TrackNode>) particle_stretch,
				(List<TrackNode>) collision_reflect,
				(List<TrackNode>) collision_spin,
				(List<TrackNode>) clip_top,
				(List<TrackNode>) clip_bottom,
				(List<TrackNode>) clip_left,
				(List<TrackNode>) clip_right,
				(List<TrackNode>) animation_rate,
			),
		);

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Emitter_<_>),
			M_wrap(
				(String) name,
				(String) image,
				(String) image_path,
				(Integer) image_column,
				(Integer) image_row,
				(Integer) image_frame,
				(Integer) animated,
				(Integer) flag,
				(Integer) type,
				(String) on_duration,
				(List<TrackNode>) system_duration,
				(List<TrackNode>) cross_fade_duration,
				(List<TrackNode>) spawn_rate,
				(List<TrackNode>) spawn_minimum_active,
				(List<TrackNode>) spawn_maximum_active,
				(List<TrackNode>) spawn_maximum_launched,
				(List<TrackNode>) emitter_radius,
				(List<TrackNode>) emitter_offset_x,
				(List<TrackNode>) emitter_offset_y,
				(List<TrackNode>) emitter_box_x,
				(List<TrackNode>) emitter_box_y,
				(List<TrackNode>) emitter_path,
				(List<TrackNode>) emitter_skew_x,
				(List<TrackNode>) emitter_skew_y,
				(List<TrackNode>) particle_duration,
				(List<TrackNode>) system_red,
				(List<TrackNode>) system_green,
				(List<TrackNode>) system_blue,
				(List<TrackNode>) system_alpha,
				(List<TrackNode>) system_brightness,
				(List<TrackNode>) launch_speed,
				(List<TrackNode>) launch_angle,
				(List<Field>) field,
				(List<Field>) system_field,
				(List<TrackNode>) particle_red,
				(List<TrackNode>) particle_green,
				(List<TrackNode>) particle_blue,
				(List<TrackNode>) particle_alpha,
				(List<TrackNode>) particle_brightness,
				(List<TrackNode>) particle_spin_angle,
				(List<TrackNode>) particle_spin_speed,
				(List<TrackNode>) particle_scale,
				(List<TrackNode>) particle_stretch,
				(List<TrackNode>) collision_reflect,
				(List<TrackNode>) collision_spin,
				(List<TrackNode>) clip_top,
				(List<TrackNode>) clip_bottom,
				(List<TrackNode>) clip_left,
				(List<TrackNode>) clip_right,
				(List<TrackNode>) animation_rate,
			),
		);

		#pragma endregion

		#pragma region particle

		template <typename = None>
		struct Particle_;

		using Particle = Particle_<>;

		template <typename _> requires (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {}))
		M_record_of_map(
			M_wrap(Particle_<_>),
			M_wrap(
				(List<Emitter>) emitter,
			),
		);

		#pragma endregion

	};

}
