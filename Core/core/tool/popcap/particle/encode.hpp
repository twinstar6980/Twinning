#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/particle/version.hpp"
#include "core/tool/popcap/particle/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::Particle {

	template <auto version> requires (check_version(version, {}, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = [] {
			if constexpr (check_version(version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicIdentifier{0x411F994D_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {false})) {
				return MagicIdentifier{0x411F994D_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {true})) {
				return MagicIdentifier{0xE09295E9_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {false})) {
				return MagicIdentifier{0x00000000_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;

		// ----------------

		inline static constexpr auto k_field_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			return size;
		}();

		inline static constexpr auto k_emitter_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerU32>();
			size += bs_static_size<IntegerOfPlatform>() * 40_sz * 2_sz;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			return size;
		}();

	};

	template <auto version> requires (check_version(version, {}, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		using Common::k_field_data_size;

		using Common::k_emitter_data_size;

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			OByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.write_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU32>(value));
			return;
		}

		static auto exchange_unit_integer_platform (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerOfPlatform>(value));
			return;
		}

		static auto exchange_unit_floating (
			OByteStreamView & data,
			Floating const &  value
		) -> Void {
			data.write(cbw<FloatingS32>(value));
			return;
		}

		static auto exchange_unit_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			data.write(self_cast<StringBlock32>(value));
			return;
		}

		// ----------------

		static auto process_track_node_list (
			OByteStreamView &                          track_node_list_data,
			List<typename Manifest::TrackNode> const & track_node_list_manifest
		) -> Void {
			exchange_unit_integer(track_node_list_data, cbw<Integer>(track_node_list_manifest.size()));
			for (auto & track_node_manifest : track_node_list_manifest) {
				exchange_unit_floating(track_node_list_data, track_node_manifest.time);
				exchange_unit_floating(track_node_list_data, track_node_manifest.low_value);
				exchange_unit_floating(track_node_list_data, track_node_manifest.high_value);
				exchange_unit_integer(track_node_list_data, track_node_manifest.curve);
				exchange_unit_integer(track_node_list_data, track_node_manifest.distribution);
			}
			return;
		}

		static auto process_field_list (
			OByteStreamView &                      field_list_data,
			List<typename Manifest::Field> const & field_list_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_unit_constant(field_list_data, cbw<IntegerU32>(k_field_data_size));
			for (auto & field_manifest : field_list_manifest) {
				exchange_unit_integer_platform(field_list_data, field_manifest.type);
				exchange_unit_integer_platform(field_list_data, ignored);
				exchange_unit_integer_platform(field_list_data, ignored);
				exchange_unit_integer_platform(field_list_data, ignored);
				exchange_unit_integer_platform(field_list_data, ignored);
			}
			for (auto & field_manifest : field_list_manifest) {
				process_track_node_list(field_list_data, field_manifest.offset_x);
				process_track_node_list(field_list_data, field_manifest.offset_y);
			}
			return;
		}

		static auto process_emitter_list (
			OByteStreamView &                        emitter_list_data,
			List<typename Manifest::Emitter> const & emitter_list_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_unit_integer(emitter_list_data, cbw<Integer>(emitter_list_manifest.size()));
			if constexpr (check_version(version, {}, {true})) {
				exchange_unit_integer(emitter_list_data, ignored);
			}
			exchange_unit_constant(emitter_list_data, cbw<IntegerU32>(k_emitter_data_size));
			for (auto & emitter_manifest : emitter_list_manifest) {
				exchange_unit_integer_platform(emitter_list_data, ignored);
				exchange_unit_integer(emitter_list_data, emitter_manifest.image_column);
				exchange_unit_integer(emitter_list_data, emitter_manifest.image_row);
				exchange_unit_integer(emitter_list_data, emitter_manifest.image_frame);
				exchange_unit_integer(emitter_list_data, emitter_manifest.animated);
				exchange_unit_integer(emitter_list_data, emitter_manifest.flag);
				exchange_unit_integer(emitter_list_data, emitter_manifest.type);
				exchange_unit_integer_platform(emitter_list_data, ignored);
				exchange_unit_integer_platform(emitter_list_data, ignored);
				for (auto & index : SizeRange{22_sz}) {
					exchange_unit_integer_platform(emitter_list_data, ignored);
					exchange_unit_integer_platform(emitter_list_data, ignored);
				}
				exchange_unit_integer_platform(emitter_list_data, ignored);
				exchange_unit_integer_platform(emitter_list_data, cbw<Integer>(emitter_manifest.field.size()));
				exchange_unit_integer_platform(emitter_list_data, ignored);
				exchange_unit_integer_platform(emitter_list_data, cbw<Integer>(emitter_manifest.system_field.size()));
				for (auto & index : SizeRange{16_sz}) {
					exchange_unit_integer_platform(emitter_list_data, ignored);
					exchange_unit_integer_platform(emitter_list_data, ignored);
				}
			}
			for (auto & emitter_manifest : emitter_list_manifest) {
				if constexpr (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::television()}, {})) {
					exchange_unit_string(emitter_list_data, emitter_manifest.image);
				}
				if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {})) {
					exchange_unit_integer(emitter_list_data, emitter_manifest.image);
				}
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_unit_string(emitter_list_data, emitter_manifest.image_path);
				}
				exchange_unit_string(emitter_list_data, emitter_manifest.name);
				process_track_node_list(emitter_list_data, emitter_manifest.system_duration);
				exchange_unit_string(emitter_list_data, emitter_manifest.on_duration);
				process_track_node_list(emitter_list_data, emitter_manifest.cross_fade_duration);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_rate);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_minimum_active);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_maximum_active);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_maximum_launched);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_radius);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_offset_x);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_offset_y);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_box_x);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_box_y);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_path);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_skew_x);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_skew_y);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_duration);
				process_track_node_list(emitter_list_data, emitter_manifest.system_red);
				process_track_node_list(emitter_list_data, emitter_manifest.system_green);
				process_track_node_list(emitter_list_data, emitter_manifest.system_blue);
				process_track_node_list(emitter_list_data, emitter_manifest.system_alpha);
				process_track_node_list(emitter_list_data, emitter_manifest.system_brightness);
				process_track_node_list(emitter_list_data, emitter_manifest.launch_speed);
				process_track_node_list(emitter_list_data, emitter_manifest.launch_angle);
				process_field_list(emitter_list_data, emitter_manifest.field);
				process_field_list(emitter_list_data, emitter_manifest.system_field);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_red);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_green);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_blue);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_alpha);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_brightness);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_spin_angle);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_spin_speed);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_scale);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_stretch);
				process_track_node_list(emitter_list_data, emitter_manifest.collision_reflect);
				process_track_node_list(emitter_list_data, emitter_manifest.collision_spin);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_top);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_bottom);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_left);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_right);
				process_track_node_list(emitter_list_data, emitter_manifest.animation_rate);
			}
			return;
		}

		static auto process_particle (
			OByteStreamView &                   particle_data,
			typename Manifest::Particle const & particle_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			particle_data.write_constant(k_magic_identifier);
			exchange_unit_integer_platform(particle_data, ignored);
			process_emitter_list(particle_data, particle_manifest.emitter);
			return;
		}

	public:

		static auto do_process_particle (
			OByteStreamView &                   particle_data_,
			typename Manifest::Particle const & particle_manifest
		) -> Void {
			M_use_zps_of(particle_data);
			return process_particle(particle_data, particle_manifest);
		}

	};

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		using Common::k_field_data_size;

		using Common::k_emitter_data_size;

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			IByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.read_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU32>());
			return;
		}

		static auto exchange_unit_integer_platform (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerOfPlatform>());
			return;
		}

		static auto exchange_unit_floating (
			IByteStreamView & data,
			Floating &        value
		) -> Void {
			value = cbw<Floating>(data.read_of<FloatingS32>());
			return;
		}

		static auto exchange_unit_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			data.read(self_cast<StringBlock32>(value));
			return;
		}

		// ----------------

		static auto process_track_node_list (
			IByteStreamView &                    track_node_list_data,
			List<typename Manifest::TrackNode> & track_node_list_manifest
		) -> Void {
			track_node_list_manifest.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(track_node_list_data, it); }))));
			for (auto & track_node_manifest : track_node_list_manifest) {
				exchange_unit_floating(track_node_list_data, track_node_manifest.time);
				exchange_unit_floating(track_node_list_data, track_node_manifest.low_value);
				exchange_unit_floating(track_node_list_data, track_node_manifest.high_value);
				exchange_unit_integer(track_node_list_data, track_node_manifest.curve);
				exchange_unit_integer(track_node_list_data, track_node_manifest.distribution);
			}
			return;
		}

		static auto process_field_list (
			IByteStreamView &                field_list_data,
			List<typename Manifest::Field> & field_list_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_unit_constant(field_list_data, cbw<IntegerU32>(k_field_data_size));
			for (auto & field_manifest : field_list_manifest) {
				exchange_unit_integer_platform(field_list_data, field_manifest.type);
				exchange_unit_integer_platform(field_list_data, ignored);
				exchange_unit_integer_platform(field_list_data, ignored);
				exchange_unit_integer_platform(field_list_data, ignored);
				exchange_unit_integer_platform(field_list_data, ignored);
			}
			for (auto & field_manifest : field_list_manifest) {
				process_track_node_list(field_list_data, field_manifest.offset_x);
				process_track_node_list(field_list_data, field_manifest.offset_y);
			}
			return;
		}

		static auto process_emitter_list (
			IByteStreamView &                  emitter_list_data,
			List<typename Manifest::Emitter> & emitter_list_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			emitter_list_manifest.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(emitter_list_data, it); }))));
			if constexpr (check_version(version, {}, {true})) {
				exchange_unit_integer(emitter_list_data, ignored);
			}
			exchange_unit_constant(emitter_list_data, cbw<IntegerU32>(k_emitter_data_size));
			for (auto & emitter_manifest : emitter_list_manifest) {
				exchange_unit_integer_platform(emitter_list_data, ignored);
				exchange_unit_integer(emitter_list_data, emitter_manifest.image_column);
				exchange_unit_integer(emitter_list_data, emitter_manifest.image_row);
				exchange_unit_integer(emitter_list_data, emitter_manifest.image_frame);
				exchange_unit_integer(emitter_list_data, emitter_manifest.animated);
				exchange_unit_integer(emitter_list_data, emitter_manifest.flag);
				exchange_unit_integer(emitter_list_data, emitter_manifest.type);
				exchange_unit_integer_platform(emitter_list_data, ignored);
				exchange_unit_integer_platform(emitter_list_data, ignored);
				for (auto & index : SizeRange{22_sz}) {
					exchange_unit_integer_platform(emitter_list_data, ignored);
					exchange_unit_integer_platform(emitter_list_data, ignored);
				}
				exchange_unit_integer_platform(emitter_list_data, ignored);
				emitter_manifest.field.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer_platform(emitter_list_data, it); }))));
				exchange_unit_integer_platform(emitter_list_data, ignored);
				emitter_manifest.system_field.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer_platform(emitter_list_data, it); }))));
				for (auto & index : SizeRange{16_sz}) {
					exchange_unit_integer_platform(emitter_list_data, ignored);
					exchange_unit_integer_platform(emitter_list_data, ignored);
				}
			}
			for (auto & emitter_manifest : emitter_list_manifest) {
				if constexpr (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::television()}, {})) {
					exchange_unit_string(emitter_list_data, emitter_manifest.image);
				}
				if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {})) {
					exchange_unit_integer(emitter_list_data, emitter_manifest.image);
				}
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_unit_string(emitter_list_data, emitter_manifest.image_path);
				}
				exchange_unit_string(emitter_list_data, emitter_manifest.name);
				process_track_node_list(emitter_list_data, emitter_manifest.system_duration);
				exchange_unit_string(emitter_list_data, emitter_manifest.on_duration);
				process_track_node_list(emitter_list_data, emitter_manifest.cross_fade_duration);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_rate);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_minimum_active);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_maximum_active);
				process_track_node_list(emitter_list_data, emitter_manifest.spawn_maximum_launched);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_radius);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_offset_x);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_offset_y);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_box_x);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_box_y);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_path);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_skew_x);
				process_track_node_list(emitter_list_data, emitter_manifest.emitter_skew_y);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_duration);
				process_track_node_list(emitter_list_data, emitter_manifest.system_red);
				process_track_node_list(emitter_list_data, emitter_manifest.system_green);
				process_track_node_list(emitter_list_data, emitter_manifest.system_blue);
				process_track_node_list(emitter_list_data, emitter_manifest.system_alpha);
				process_track_node_list(emitter_list_data, emitter_manifest.system_brightness);
				process_track_node_list(emitter_list_data, emitter_manifest.launch_speed);
				process_track_node_list(emitter_list_data, emitter_manifest.launch_angle);
				process_field_list(emitter_list_data, emitter_manifest.field);
				process_field_list(emitter_list_data, emitter_manifest.system_field);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_red);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_green);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_blue);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_alpha);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_brightness);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_spin_angle);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_spin_speed);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_scale);
				process_track_node_list(emitter_list_data, emitter_manifest.particle_stretch);
				process_track_node_list(emitter_list_data, emitter_manifest.collision_reflect);
				process_track_node_list(emitter_list_data, emitter_manifest.collision_spin);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_top);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_bottom);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_left);
				process_track_node_list(emitter_list_data, emitter_manifest.clip_right);
				process_track_node_list(emitter_list_data, emitter_manifest.animation_rate);
			}
			return;
		}

		static auto process_particle (
			IByteStreamView &             particle_data,
			typename Manifest::Particle & particle_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			particle_data.read_constant(k_magic_identifier);
			exchange_unit_integer_platform(particle_data, ignored);
			process_emitter_list(particle_data, particle_manifest.emitter);
			return;
		}

	public:

		static auto do_process_particle (
			IByteStreamView &             particle_data_,
			typename Manifest::Particle & particle_manifest
		) -> Void {
			M_use_zps_of(particle_data);
			restruct(particle_manifest);
			return process_particle(particle_data, particle_manifest);
		}

	};

}
