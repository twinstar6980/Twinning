#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/wwise/sound_bank/version.hpp"
#include "core/tool/wwise/sound_bank/manifest.hpp"
#include "core/tool/common/wave_structure.hpp"
#include "core/tool/data/hash/fnv.hpp"

namespace TwinStar::Core::Tool::Wwise::SoundBank {

	template <auto version> requires (check_version(version, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using ChunkSign = WaveStructure::ChunkSign;

		struct ChunkSignFlag {
			inline static constexpr auto bkhd = FourCC{"BKHD"_fourcc};
			inline static constexpr auto didx = FourCC{"DIDX"_fourcc};
			inline static constexpr auto data = FourCC{"DATA"_fourcc};
			inline static constexpr auto init = FourCC{"INIT"_fourcc};
			inline static constexpr auto stmg = FourCC{"STMG"_fourcc};
			inline static constexpr auto hirc = FourCC{"HIRC"_fourcc};
			inline static constexpr auto stid = FourCC{"STID"_fourcc};
			inline static constexpr auto envs = FourCC{"ENVS"_fourcc};
			inline static constexpr auto plat = FourCC{"PLAT"_fourcc};
		};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		inline static constexpr auto k_data_block_padding_size = Size{0x10_sz};

		// ----------------

		struct IDWrapper {
			typename Manifest::ID value;
		};

		// ----------------

		using CommonPropertyValue = Variant<Boolean, Integer, Floater, Enumerated, IDWrapper>;

		template <typename Type> requires
			CategoryConstraint<>
			&& (IsPureInstance<Type>)
		struct CommonPropertyMap {
			Map<Type, Tuple<CommonPropertyValue>>                      regular;
			Map<Type, Tuple<CommonPropertyValue, CommonPropertyValue>> randomizer;
		};

		// ----------------

		template <typename Value>
		struct EnumerationAttribute;

		#pragma region enumeration attribute of common property type

		template <typename = None>
		struct EventActionCommonPropertyType_;

		using EventActionCommonPropertyType = EventActionCommonPropertyType_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(EventActionCommonPropertyType_<_>),
			M_wrap(
				delay,
				fade_time,
				probability,
			),
		);

		template <>
		struct EnumerationAttribute<EventActionCommonPropertyType> {
			using Attribute = decltype([] {
				if constexpr (check_version(version, {72, 118})) {
					using Type = TypePackage<
						// delay
						TypePackage<ValuePackage<14_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// fade_time
						TypePackage<ValuePackage<15_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// probability
						TypePackage<ValuePackage<16_e>, TypePackage<Floater>, ValuePackage<100.0_f>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {118})) {
					using Type = TypePackage<
						// delay
						TypePackage<ValuePackage<15_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// fade_time
						TypePackage<ValuePackage<16_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// probability
						TypePackage<ValuePackage<17_e>, TypePackage<Floater>, ValuePackage<100.0_f>>
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <typename = None>
		struct ModulatorCommonPropertyType_;

		using ModulatorCommonPropertyType = ModulatorCommonPropertyType_<>;

		template <typename _> requires (check_version(version, {112}))
		M_enumeration(
			M_wrap(ModulatorCommonPropertyType_<_>),
			M_wrap(
				scope,
				trigger_on,
				depth,
				frequency,
				waveform,
				smoothing,
				pulse_width_modulation,
				attack,
				initial_phase_offset,
				attack_time,
				attack_curve,
				decay_time,
				sustain_level,
				release_time,
				sustain_time,
				initial_delay,
				duration,
				loop,
				playback_rate,
				stop_playback,
			),
		);

		template <>
		struct EnumerationAttribute<ModulatorCommonPropertyType> {
			using Attribute = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = TypePackage<
						// scope,
						TypePackage<ValuePackage<0_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// trigger_on,
						TypePackage<ValuePackage<15_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// depth,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// frequency,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// waveform,
						TypePackage<ValuePackage<5_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// smoothing,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// pulse_width_modulation,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<50.0_f>>,
						// attack,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// initial_phase_offset,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// attack_time,
						TypePackage<ValuePackage<9_e>, TypePackage<Floater>, ValuePackage<0.2_f>>,
						// attack_curve,
						TypePackage<ValuePackage<10_e>, TypePackage<Floater>, ValuePackage<50.0_f>>,
						// decay_time,
						TypePackage<ValuePackage<11_e>, TypePackage<Floater>, ValuePackage<0.2_f>>,
						// sustain_level,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// release_time,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.5_f>>,
						// sustain_time,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// initial_delay,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// duration,
						TypePackage<ValuePackage<16_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// loop,
						TypePackage<ValuePackage<17_e>, TypePackage<Integer>, ValuePackage<1_i>>,
						// playback_rate,
						TypePackage<ValuePackage<18_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// stop_playback,
						TypePackage<ValuePackage<1_e>, TypePackage<Boolean>, ValuePackage<k_true>>
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <typename = None>
		struct AudioCommonPropertyType_;

		using AudioCommonPropertyType = AudioCommonPropertyType_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				game_defined_auxiliary_send_volume,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_loop,
				motion_volume_offset,
				motion_low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {88, 112}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_volume_make_up_gain,
				game_defined_auxiliary_send_volume,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter_id,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				hdr_envelope_tracking_active_range,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_initial_delay,
				playback_loop,
				motion_volume_offset,
				motion_low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {112, 128}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_volume_make_up_gain,
				game_defined_auxiliary_send_volume,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter_id,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				hdr_envelope_tracking_active_range,
				midi_note_tracking_root_note,
				midi_event_play_on,
				midi_transformation_transposition,
				midi_transformation_velocity_offset,
				midi_filter_key_range_minimum,
				midi_filter_key_range_maximum,
				midi_filter_velocity_minimum,
				midi_filter_velocity_maximum,
				midi_filter_channel,
				midi_clip_tempo_source,
				midi_target_id,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_initial_delay,
				playback_loop,
				playback_speed,
				motion_volume_offset,
				motion_low_pass_filter,
				mixer_id,
			),
		);

		template <typename _> requires (check_version(version, {128, 132}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_volume_make_up_gain,
				game_defined_auxiliary_send_volume,
				game_defined_auxiliary_send_low_pass_filter,
				game_defined_auxiliary_send_high_pass_filter,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				user_defined_auxiliary_send_low_pass_filter_0,
				user_defined_auxiliary_send_low_pass_filter_1,
				user_defined_auxiliary_send_low_pass_filter_2,
				user_defined_auxiliary_send_low_pass_filter_3,
				user_defined_auxiliary_send_high_pass_filter_0,
				user_defined_auxiliary_send_high_pass_filter_1,
				user_defined_auxiliary_send_high_pass_filter_2,
				user_defined_auxiliary_send_high_pass_filter_3,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter_id,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				hdr_envelope_tracking_active_range,
				midi_note_tracking_root_note,
				midi_event_play_on,
				midi_transformation_transposition,
				midi_transformation_velocity_offset,
				midi_filter_key_range_minimum,
				midi_filter_key_range_maximum,
				midi_filter_velocity_minimum,
				midi_filter_velocity_maximum,
				midi_filter_channel,
				midi_clip_tempo_source,
				midi_target_id,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_initial_delay,
				playback_loop,
				playback_speed,
				mixer_id,
			),
		);

		template <typename _> requires (check_version(version, {132, 135}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_volume_make_up_gain,
				game_defined_auxiliary_send_volume,
				game_defined_auxiliary_send_low_pass_filter,
				game_defined_auxiliary_send_high_pass_filter,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				user_defined_auxiliary_send_low_pass_filter_0,
				user_defined_auxiliary_send_low_pass_filter_1,
				user_defined_auxiliary_send_low_pass_filter_2,
				user_defined_auxiliary_send_low_pass_filter_3,
				user_defined_auxiliary_send_high_pass_filter_0,
				user_defined_auxiliary_send_high_pass_filter_1,
				user_defined_auxiliary_send_high_pass_filter_2,
				user_defined_auxiliary_send_high_pass_filter_3,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				positioning_listener_routing_speaker_panning_division_spatialization_mix,
				positioning_listener_routing_attenuation_id,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter_id,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				hdr_envelope_tracking_active_range,
				midi_note_tracking_root_note,
				midi_event_play_on,
				midi_transformation_transposition,
				midi_transformation_velocity_offset,
				midi_filter_key_range_minimum,
				midi_filter_key_range_maximum,
				midi_filter_velocity_minimum,
				midi_filter_velocity_maximum,
				midi_filter_channel,
				midi_clip_tempo_source,
				midi_target_id,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_initial_delay,
				playback_loop,
				playback_speed,
				mixer_id,
			),
		);

		template <typename _> requires (check_version(version, {135, 140}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_volume_make_up_gain,
				game_defined_auxiliary_send_volume,
				game_defined_auxiliary_send_low_pass_filter,
				game_defined_auxiliary_send_high_pass_filter,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				user_defined_auxiliary_send_low_pass_filter_0,
				user_defined_auxiliary_send_low_pass_filter_1,
				user_defined_auxiliary_send_low_pass_filter_2,
				user_defined_auxiliary_send_low_pass_filter_3,
				user_defined_auxiliary_send_high_pass_filter_0,
				user_defined_auxiliary_send_high_pass_filter_1,
				user_defined_auxiliary_send_high_pass_filter_2,
				user_defined_auxiliary_send_high_pass_filter_3,
				early_reflection_auxiliary_send_volume,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				positioning_listener_routing_speaker_panning_division_spatialization_mix,
				positioning_listener_routing_attenuation_id,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter_id,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				hdr_envelope_tracking_active_range,
				midi_note_tracking_root_note,
				midi_event_play_on,
				midi_transformation_transposition,
				midi_transformation_velocity_offset,
				midi_filter_key_range_minimum,
				midi_filter_key_range_maximum,
				midi_filter_velocity_minimum,
				midi_filter_velocity_maximum,
				midi_filter_channel,
				midi_clip_tempo_source,
				midi_target_id,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_initial_delay,
				playback_loop,
				playback_speed,
				mixer_id,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_volume_make_up_gain,
				game_defined_auxiliary_send_volume,
				game_defined_auxiliary_send_low_pass_filter,
				game_defined_auxiliary_send_high_pass_filter,
				user_defined_auxiliary_send_volume_0,
				user_defined_auxiliary_send_volume_1,
				user_defined_auxiliary_send_volume_2,
				user_defined_auxiliary_send_volume_3,
				user_defined_auxiliary_send_low_pass_filter_0,
				user_defined_auxiliary_send_low_pass_filter_1,
				user_defined_auxiliary_send_low_pass_filter_2,
				user_defined_auxiliary_send_low_pass_filter_3,
				user_defined_auxiliary_send_high_pass_filter_0,
				user_defined_auxiliary_send_high_pass_filter_1,
				user_defined_auxiliary_send_high_pass_filter_2,
				user_defined_auxiliary_send_high_pass_filter_3,
				early_reflection_auxiliary_send_volume,
				positioning_center_percent,
				positioning_speaker_panning_x,
				positioning_speaker_panning_y,
				positioning_speaker_panning_z,
				positioning_listener_routing_speaker_panning_division_spatialization_mix,
				positioning_listener_routing_attenuation_id,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter_id,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				hdr_envelope_tracking_active_range,
				midi_note_tracking_root_note,
				midi_event_play_on,
				midi_transformation_transposition,
				midi_transformation_velocity_offset,
				midi_filter_key_range_minimum,
				midi_filter_key_range_maximum,
				midi_filter_velocity_minimum,
				midi_filter_velocity_maximum,
				midi_filter_channel,
				midi_clip_tempo_source,
				midi_target_id,
				playback_priority_value,
				playback_priority_offset_at_maximum_distance,
				playback_initial_delay,
				playback_loop,
				playback_speed,
				mixer_id,
			),
		);

		template <>
		struct EnumerationAttribute<AudioCommonPropertyType> {
			using Attribute = decltype([] {
				if constexpr (check_version(version, {72, 88})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<18_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<11_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_value,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<7_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// motion_volume_offset,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// motion_low_pass_filter,
						TypePackage<ValuePackage<9_e>, TypePackage<Floater>, ValuePackage<0.0_f>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {88, 112})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<18_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<11_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_threshold,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<29_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<30_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// playback_priority_value,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<7_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// motion_volume_offset,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// motion_low_pass_filter,
						TypePackage<ValuePackage<9_e>, TypePackage<Floater>, ValuePackage<0.0_f>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {112, 118})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<18_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<11_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_threshold,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<29_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<30_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<53_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<54_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// motion_volume_offset,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// motion_low_pass_filter,
						TypePackage<ValuePackage<9_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// mixer_id,
						TypePackage<ValuePackage<57_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {118, 128})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<30_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<53_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<54_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// motion_volume_offset,
						TypePackage<ValuePackage<9_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// motion_low_pass_filter,
						TypePackage<ValuePackage<10_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// mixer_id,
						TypePackage<ValuePackage<57_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {128, 132})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_low_pass_filter,
						TypePackage<ValuePackage<68_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_high_pass_filter,
						TypePackage<ValuePackage<69_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_0,
						TypePackage<ValuePackage<60_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_1,
						TypePackage<ValuePackage<61_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_2,
						TypePackage<ValuePackage<62_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_3,
						TypePackage<ValuePackage<63_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_0,
						TypePackage<ValuePackage<64_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_1,
						TypePackage<ValuePackage<65_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_2,
						TypePackage<ValuePackage<66_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_3,
						TypePackage<ValuePackage<67_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<30_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<53_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<54_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// mixer_id,
						TypePackage<ValuePackage<57_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {132, 135})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_low_pass_filter,
						TypePackage<ValuePackage<68_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_high_pass_filter,
						TypePackage<ValuePackage<69_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_0,
						TypePackage<ValuePackage<60_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_1,
						TypePackage<ValuePackage<61_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_2,
						TypePackage<ValuePackage<62_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_3,
						TypePackage<ValuePackage<63_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_0,
						TypePackage<ValuePackage<64_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_1,
						TypePackage<ValuePackage<65_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_2,
						TypePackage<ValuePackage<66_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_3,
						TypePackage<ValuePackage<67_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_listener_routing_speaker_panning_division_spatialization_mix,
						TypePackage<ValuePackage<71_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// positioning_listener_routing_attenuation_id,
						TypePackage<ValuePackage<70_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<30_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<53_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<54_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// mixer_id,
						TypePackage<ValuePackage<57_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {135, 140})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_low_pass_filter,
						TypePackage<ValuePackage<68_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_high_pass_filter,
						TypePackage<ValuePackage<69_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_0,
						TypePackage<ValuePackage<60_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_1,
						TypePackage<ValuePackage<61_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_2,
						TypePackage<ValuePackage<62_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_3,
						TypePackage<ValuePackage<63_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_0,
						TypePackage<ValuePackage<64_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_1,
						TypePackage<ValuePackage<65_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_2,
						TypePackage<ValuePackage<66_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_3,
						TypePackage<ValuePackage<67_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// early_reflection_auxiliary_send_volume,
						TypePackage<ValuePackage<72_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_listener_routing_speaker_panning_division_spatialization_mix,
						TypePackage<ValuePackage<71_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// positioning_listener_routing_attenuation_id,
						TypePackage<ValuePackage<70_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<30_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<53_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<54_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// mixer_id,
						TypePackage<ValuePackage<57_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {140})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_low_pass_filter,
						TypePackage<ValuePackage<68_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_high_pass_filter,
						TypePackage<ValuePackage<69_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_0,
						TypePackage<ValuePackage<60_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_1,
						TypePackage<ValuePackage<61_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_2,
						TypePackage<ValuePackage<62_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_3,
						TypePackage<ValuePackage<63_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_0,
						TypePackage<ValuePackage<64_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_1,
						TypePackage<ValuePackage<65_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_2,
						TypePackage<ValuePackage<66_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_3,
						TypePackage<ValuePackage<67_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// early_reflection_auxiliary_send_volume,
						TypePackage<ValuePackage<72_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_z,
						TypePackage<ValuePackage<73_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_listener_routing_speaker_panning_division_spatialization_mix,
						TypePackage<ValuePackage<71_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// positioning_listener_routing_attenuation_id,
						TypePackage<ValuePackage<70_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<30_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<31_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<32_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<53_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<7_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<54_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// mixer_id,
						TypePackage<ValuePackage<57_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>
					>;
					return declare<Type>();
				}
			}());
		};

		#pragma endregion

		#pragma region enumeration attribute of manifest type

		template <>
		struct EnumerationAttribute<typename Manifest::Curve> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 4_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// constant,
						9_e,
						// linear,
						4_e,
						// s,
						5_e,
						// s_inverted,
						3_e,
						// sine,
						1_e,
						// sine_reciprocal,
						7_e,
						// logarithmic_1dot41,
						2_e,
						// logarithmic_3dot0,
						0_e,
						// exponential_1dot41,
						6_e,
						// exponential_3dot0,
						8_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::TimePoint> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 140})) {
					return 3_sz;
				}
				if constexpr (check_version(version, {140})) {
					return 4_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 140})) {
					using Type = ValuePackage<
						// immediate,
						0_e,
						// next_grid,
						1_e,
						// next_bar,
						2_e,
						// next_beat,
						3_e,
						// next_cue,
						4_e,
						// custom_cue,
						5_e,
						// entry_cue,
						6_e,
						// exit_cue,
						7_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// immediate,
						0_e,
						// next_grid,
						1_e,
						// next_bar,
						2_e,
						// next_beat,
						3_e,
						// next_cue,
						4_e,
						// custom_cue,
						5_e,
						// entry_cue,
						6_e,
						// exit_cue,
						7_e,
						// last_exit_position,
						9_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::CoordinateMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// linear,
						0_e,
						// scaled,
						2_e,
						// scaled_3,
						3_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::PropertyCategory> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 128})) {
					return 2_sz;
				}
				if constexpr (check_version(version, {128, 145})) {
					return 3_sz;
				}
				if constexpr (check_version(version, {145})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 128})) {
					using Type = ValuePackage<
						// unidirectional,
						0_e,
						// bidirectional,
						1_e,
						// bidirectional_ranged,
						2_e, // maybe, see playback_speed
						// boolean,
						3_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {128, 145})) {
					using Type = ValuePackage<
						// unidirectional,
						1_e,
						// bidirectional,
						2_e,
						// bidirectional_ranged,
						3_e, // maybe, see playback_speed
						// boolean,
						4_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {145})) {
					using Type = ValuePackage<
						// unidirectional,
						1_e,
						// bidirectional,
						2_e,
						// bidirectional_ranged,
						3_e, // maybe, see playback_speed
						// boolean,
						6_e,
						// unknown_6,
						4_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::ParameterCategory> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112, 145})) {
					return 2_sz;
				}
				if constexpr (check_version(version, {145})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112, 145})) {
					using Type = ValuePackage<
						// game_parameter,
						0_e,
						// midi_parameter,
						1_e,
						// modulator,
						2_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {145})) {
					using Type = ValuePackage<
						// game_parameter,
						0_e,
						// midi_parameter,
						1_e,
						// modulator,
						4_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// sequence,
						0_e,
						// random,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// step,
						0_e,
						// continuous,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 132})) {
					return 2_sz;
				}
				if constexpr (check_version(version, {132})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 132})) {
					using Type = ValuePackage<
						// user_defined,
						0_e,
						// game_defined,
						1_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {132})) {
					using Type = ValuePackage<
						// emitter,
						0_e,
						// emitter_with_automation,
						1_e,
						// listener_with_automation,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingListenerRoutingSpatialization> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {128})) {
					return 3_sz; // NOTE : use 3 byte even only need 2 byte
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {128})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// position,
						1_e,
						// position_and_orientation,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingSpeakerPanningMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {132, 140})) {
					return 2_sz;
				}
				if constexpr (check_version(version, {140})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {132, 140})) {
					using Type = ValuePackage<
						// direct_assignment,
						0_e,
						// balance_fade,
						1_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// direct_assignment,
						0_e,
						// balance_fade,
						1_e,
						// steering,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 132})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 132})) {
					using Type = ValuePackage<
						// two_dimension,
						0_e,
						// three_dimension,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::BusAutomaticDuckingSettingBusTarget> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 112})) {
					return 3_sz;
				}
				if constexpr (check_version(version, {112})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 112})) {
					using Type = ValuePackage<
						// voice_volume,
						0_e,
						// bus_volume,
						4_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// voice_volume,
						0_e,
						// bus_volume,
						5_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicTransitionSettingJumpMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {134})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {134})) {
					using Type = ValuePackage<
						// start,
						0_e,
						// specific,
						1_e,
						// next,
						3_e,
						// last_played,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicTransitionSettingSynchronizeMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// entry_cue,
						0_e,
						// random_cue,
						2_e,
						// custom_cue,
						3_e,
						// same_time_as_playing_segment,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::BusHDRSettingDynamicReleaseMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {88})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {88})) {
					using Type = ValuePackage<
						// linear,
						0_e,
						// exponential,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::SoundMIDISettingEventPlayOn> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// note_off,
						0_e,
						// note_on,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicMIDISettingClipTempoSource> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// hierarchy,
						0_e,
						// file,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlaybackLimitSettingScope> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// per_game_object,
						0_e,
						// globally,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlaybackLimitSettingWhenPriorityIsEqual> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// discard_oldest_instance,
						0_e,
						// discard_newest_instance,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlaybackLimitSettingWhenLimitIsReached> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// kill_voice,
						0_e,
						// use_virtual_voice_setting,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioVirtualVoiceSettingBehavior> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 140})) {
					using Type = ValuePackage<
						// continue_to_play,
						0_e,
						// kill_voice,
						1_e,
						// send_to_virtual_voice,
						2_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// continue_to_play,
						0_e,
						// kill_voice,
						1_e,
						// send_to_virtual_voice,
						2_e,
						// kill_if_finite_else_virtual,
						3_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioVirtualVoiceSettingOnReturnToPhysical> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// play_from_beginning,
						0_e,
						// play_from_elapsed_time,
						1_e,
						// resume,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioAssociationSettingMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// best_match,
						0_e,
						// weighted,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioSourceType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 112})) {
					return 2_sz;
				}
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 112})) {
					using Type = ValuePackage<
						// embedded,
						0_e,
						// streamed,
						1_e,
						// streamed_prefetched,
						2_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// embedded,
						0_e,
						// streamed,
						2_e,
						// streamed_prefetched,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayTypeRandomType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// standard,
						0_e,
						// shuffle,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayTypeSequenceAtEndOfPlaylist> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// restart,
						0_e,
						// play_in_reserve_order,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayModeContinuousTransitionType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// xfade_amp,
						1_e,
						// xfade_power,
						2_e,
						// delay,
						3_e,
						// sample_accurate,
						4_e,
						// trigger_rate,
						5_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::GameParameterBindToBuiltInParameterMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112, 128})) {
					return 3_sz;
				}
				if constexpr (check_version(version, {128})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112, 128})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// distance,
						1_e,
						// azimuth,
						2_e,
						// elevation,
						3_e,
						// object_to_listener_angle,
						4_e,
						// obstruction,
						5_e,
						// occlusion,
						6_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {128})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// distance,
						1_e,
						// azimuth,
						2_e,
						// elevation,
						3_e,
						// emitter_cone,
						4_e,
						// obstruction,
						5_e,
						// occlusion,
						6_e,
						// listener_cone,
						7_e,
						// diffraction,
						8_e

					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::GameParameterInterpolationMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// slew_rate,
						1_e,
						// filtering_over_time,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::EventActionProperty::ValueApplyMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// absolute,
						1_e,
						// relative,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::EventActionProperty::SeekType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// time,
						0_e,
						// percent,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::EventActionMode> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 125})) {
					return 3_sz;
				}
				if constexpr (check_version(version, {125})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 125})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// one,
						1_e,
						// all,
						2_e,
						// all_except,
						4_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {125})) {
					using Type = ValuePackage<
						// none,
						0_e,
						// one,
						1_e,
						// all,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::EventActionScope> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// global,
						0_e,
						// game_object,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorScope> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// voice,
						0_e,
						// note_or_event,
						1_e,
						// game_object,
						2_e,
						// global,
						3_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorTriggerOn> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// play,
						0_e,
						// note_off,
						2_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorWaveform> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112, 125})) {
					using Type = ValuePackage<
						// sine,
						0_e,
						// triangle,
						1_e,
						// square,
						2_e,
						// saw_up,
						3_e,
						// saw_down,
						4_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {125})) {
					using Type = ValuePackage<
						// sine,
						0_e,
						// triangle,
						1_e,
						// square,
						2_e,
						// saw_up,
						3_e,
						// saw_down,
						4_e,
						// random,
						5_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::SoundPlaylistContainerScope> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72})) {
					using Type = ValuePackage<
						// game_object,
						0_e,
						// global,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicTrackTrackType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 112})) {
					return 2_sz;
				}
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 112})) {
					using Type = ValuePackage<
						// normal,
						0_e,
						// random_step,
						1_e,
						// sequence_step,
						2_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// normal,
						0_e,
						// random_step,
						1_e,
						// sequence_step,
						2_e,
						// switcher,
						3_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicTrackClipCurveItemType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72, 112})) {
					return 3_sz;
				}
				if constexpr (check_version(version, {112})) {
					return 3_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 112})) {
					using Type = ValuePackage<
						// voice_volume,
						0_e,
						// voice_low_pass_filter,
						1_e,
						// clip_fade_in,
						2_e,
						// clip_fade_out,
						3_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// voice_volume,
						0_e,
						// voice_low_pass_filter,
						1_e,
						// voice_high_pass_filter,
						2_e,
						// clip_fade_in,
						3_e,
						// clip_fade_out,
						4_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::HierarchyType> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {72})) {
					return 8_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {72, 112})) {
					using Type = ValuePackage<
						// unknown,
						~0_e,
						// stateful_property_setting,
						1_e,
						// event_action,
						3_e,
						// event,
						4_e,
						// dialogue_event,
						15_e,
						// attenuation,
						14_e,
						// effect,
						18_e,
						// source,
						19_e,
						// audio_bus,
						8_e,
						// auxiliary_audio_bus,
						20_e,
						// sound,
						2_e,
						// sound_playlist_container,
						5_e,
						// sound_switch_container,
						6_e,
						// sound_blend_container,
						9_e,
						// actor_mixer,
						7_e,
						// music_track,
						11_e,
						// music_segment,
						10_e,
						// music_playlist_container,
						13_e,
						// music_switch_container,
						12_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {112, 128})) {
					using Type = ValuePackage<
						// unknown,
						~0_e,
						// stateful_property_setting,
						1_e,
						// event_action,
						3_e,
						// event,
						4_e,
						// dialogue_event,
						15_e,
						// attenuation,
						14_e,
						// low_frequency_oscillator_modulator,
						21_e,
						// envelope_modulator,
						22_e,
						// effect,
						18_e,
						// source,
						19_e,
						// audio_bus,
						8_e,
						// auxiliary_audio_bus,
						20_e,
						// sound,
						2_e,
						// sound_playlist_container,
						5_e,
						// sound_switch_container,
						6_e,
						// sound_blend_container,
						9_e,
						// actor_mixer,
						7_e,
						// music_track,
						11_e,
						// music_segment,
						10_e,
						// music_playlist_container,
						13_e,
						// music_switch_container,
						12_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {128, 132})) {
					using Type = ValuePackage<
						// unknown,
						~0_e,
						// stateful_property_setting,
						1_e,
						// event_action,
						3_e,
						// event,
						4_e,
						// dialogue_event,
						15_e,
						// attenuation,
						14_e,
						// low_frequency_oscillator_modulator,
						19_e,
						// envelope_modulator,
						20_e,
						// effect,
						16_e,
						// source,
						17_e,
						// audio_device,
						21_e,
						// audio_bus,
						8_e,
						// auxiliary_audio_bus,
						18_e,
						// sound,
						2_e,
						// sound_playlist_container,
						5_e,
						// sound_switch_container,
						6_e,
						// sound_blend_container,
						9_e,
						// actor_mixer,
						7_e,
						// music_track,
						11_e,
						// music_segment,
						10_e,
						// music_playlist_container,
						13_e,
						// music_switch_container,
						12_e
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {132})) {
					using Type = ValuePackage<
						// unknown,
						~0_e,
						// stateful_property_setting,
						1_e,
						// event_action,
						3_e,
						// event,
						4_e,
						// dialogue_event,
						15_e,
						// attenuation,
						14_e,
						// low_frequency_oscillator_modulator,
						19_e,
						// envelope_modulator,
						20_e,
						// time_modulator,
						22_e,
						// effect,
						16_e,
						// source,
						17_e,
						// audio_device,
						21_e,
						// audio_bus,
						8_e,
						// auxiliary_audio_bus,
						18_e,
						// sound,
						2_e,
						// sound_playlist_container,
						5_e,
						// sound_switch_container,
						6_e,
						// sound_blend_container,
						9_e,
						// actor_mixer,
						7_e,
						// music_track,
						11_e,
						// music_segment,
						10_e,
						// music_playlist_container,
						13_e,
						// music_switch_container,
						12_e
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::VoiceFilterBehavior> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {145})) {
					return 1_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {145})) {
					using Type = ValuePackage<
						// sum_all_value,
						0_e,
						// use_highest_value,
						1_e
					>;
					return declare<Type>();
				}
			}());
		};

		#pragma endregion

	};

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::VersionNumber;

		using typename Common::ChunkSign;

		using typename Common::ChunkSignFlag;

		using Common::k_data_block_padding_size;

		using typename Common::IDWrapper;

		using typename Common::CommonPropertyValue;

		template <typename _1>
		using CommonPropertyMap = typename Common::template CommonPropertyMap<_1>;

		using typename Common::EventActionCommonPropertyType;

		using typename Common::ModulatorCommonPropertyType;

		using typename Common::AudioCommonPropertyType;

		template <typename _1>
		using EnumerationAttribute = typename Common::template EnumerationAttribute<_1>;

		// ----------------

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsEnumerationWrapper<Value>)
		static auto exchange_enumeration_index (
			Enumerated &  index_value,
			Value const & value
		) -> Void {
			auto has_case = k_false;
			Generalization::each<typename EnumerationAttribute<Value>::Index>(
				[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
					if (index == static_cast<ZSize>(value.value)) {
						index_value = value_index;
						has_case = k_true;
					}
				}
			);
			assert_test(has_case);
			return;
		}

		// ----------------

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>, Value, Enumerated, IDWrapper>)
		static auto exchange_common_property (
			CommonPropertyMap<decltype(type)> & map,
			Value const &                       value
		) -> Void {
			using CurrentEnumerationAttribute = typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>;
			constexpr auto default_value = CurrentEnumerationAttribute::template Element<3_ixz>::template element<1_ixz>;
			if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Value>) {
				if (value != default_value) {
					auto & element = map.regular.append();
					element.key = type;
					element.value.template get<1_ix>().template set<Value>() = value;
				}
			}
			if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Enumerated>) {
				if (cbw<Enumerated>(value) != default_value) {
					auto & element = map.regular.append();
					element.key = type;
					exchange_enumeration_index(element.value.template get<1_ix>().template set<Enumerated>(), value);
				}
			}
			if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, IDWrapper>) {
				if (value != default_value.value) {
					auto & element = map.regular.append();
					element.key = type;
					element.value.template get<1_ix>().template set<IDWrapper>().value = value;
				}
			}
			return;
		}

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<Value, typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>>)
			&& (IsSame<Value, Boolean, Integer, Floater>)
		static auto exchange_common_property_as_regular (
			CommonPropertyMap<decltype(type)> &                     map,
			typename Manifest::template RegularValue<Value> const & value
		) -> Void {
			if (value.value != EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<3_ixz>::template element<1_ixz>) {
				auto & element = map.regular.append();
				element.key = type;
				element.value.template get<1_ix>().template set<Value>() = value.value;
			}
			return;
		}

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<Value, typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>>)
			&& (IsSame<Value, Boolean, Integer, Floater>)
		static auto exchange_common_property_as_randomizable (
			CommonPropertyMap<decltype(type)> &                          map,
			typename Manifest::template RandomizableValue<Value> const & value
		) -> Void {
			if (value.value != EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<3_ixz>::template element<1_ixz>) {
				auto & element = map.regular.append();
				element.key = type;
				element.value.template get<1_ix>().template set<Value>() = value.value;
			}
			if (value.minimum_value != mbw<Value>(0) || value.maximum_value != mbw<Value>(0)) {
				auto & element = map.randomizer.append();
				element.key = type;
				element.value.template get<1_ix>().template set<Value>() = value.minimum_value;
				element.value.template get<2_ix>().template set<Value>() = value.maximum_value;
			}
			return;
		}

		// ----------------

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::BusVoiceSetting const &   manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::voice_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::voice_pitch()>(map, manifest.pitch);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::AudioVoice const &        manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_randomizable<CPTC::voice_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_randomizable<CPTC::voice_pitch()>(map, manifest.pitch);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_randomizable<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_randomizable<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &         map,
			typename Manifest::BusVoiceVolumeGainSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {125})) {
				exchange_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &           map,
			typename Manifest::AudioVoiceVolumeGainSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88, 112})) {
				exchange_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_randomizable<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::BusBusSetting const &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::bus_volume()>(map, manifest.volume);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &   map,
			typename Manifest::BusOutputBusSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &     map,
			typename Manifest::AudioOutputBusSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &         map,
			typename Manifest::AudioAuxiliarySendSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_volume()>(map, manifest.game_defined.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_low_pass_filter()>(map, manifest.game_defined.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_high_pass_filter()>(map, manifest.game_defined.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_0()>(map, manifest.user_defined.item_1.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_0()>(map, manifest.user_defined.item_1.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_0()>(map, manifest.user_defined.item_1.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_1()>(map, manifest.user_defined.item_2.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_1()>(map, manifest.user_defined.item_2.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_1()>(map, manifest.user_defined.item_2.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_2()>(map, manifest.user_defined.item_3.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_2()>(map, manifest.user_defined.item_3.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_2()>(map, manifest.user_defined.item_3.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_3()>(map, manifest.user_defined.item_4.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_3()>(map, manifest.user_defined.item_4.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_3()>(map, manifest.user_defined.item_4.high_pass_filter);
			}
			if constexpr (check_version(version, {135})) {
				exchange_common_property_as_regular<CPTC::early_reflection_auxiliary_send_volume()>(map, manifest.early_reflection.volume);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &       map,
			typename Manifest::AudioPositioningSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::positioning_center_percent()>(map, manifest.center_percent);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property<CPTC::positioning_speaker_panning_x()>(map, manifest.speaker_panning.position.x);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property<CPTC::positioning_speaker_panning_y()>(map, manifest.speaker_panning.position.y);
			}
			if constexpr (check_version(version, {140})) {
				exchange_common_property<CPTC::positioning_speaker_panning_z()>(map, manifest.speaker_panning.position.z);
			}
			if constexpr (check_version(version, {132})) {
				exchange_common_property_as_regular<CPTC::positioning_listener_routing_speaker_panning_division_spatialization_mix()>(map, manifest.listener_routing.speaker_panning_divsion_spatialization_mix);
			}
			if constexpr (check_version(version, {132})) {
				exchange_common_property<CPTC::positioning_listener_routing_attenuation_id()>(map, manifest.listener_routing.attenuation.id);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::BusHDRSetting const &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_threshold()>(map, manifest.dynamic.threshold);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_ratio()>(map, manifest.dynamic.ratio);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_release_time()>(map, manifest.dynamic.release_time);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_window_tap_output_game_parameter_id()>(map, manifest.window_top_output_game_parameter.id);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_window_tap_output_game_parameter_minimum()>(map, manifest.window_top_output_game_parameter.minimum);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_window_tap_output_game_parameter_maximum()>(map, manifest.window_top_output_game_parameter.maximum);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::AudioHDRSetting const &   manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_envelope_tracking_active_range()>(map, manifest.envelope_tracking.active_range);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::SoundMIDISetting const &  manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_event_play_on()>(map, manifest.event.play_on);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_note_tracking_root_note()>(map, manifest.note_tracking.root_note);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::midi_transformation_transposition()>(map, manifest.transformation.transposition);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::midi_transformation_velocity_offset()>(map, manifest.transformation.velocity_offset);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_key_range_minimum()>(map, manifest.filter.key_range_minimum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_key_range_maximum()>(map, manifest.filter.key_range_maximum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_velocity_minimum()>(map, manifest.filter.velocity_minimum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_velocity_maximum()>(map, manifest.filter.velocity_maximum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_channel()>(map, manifest.filter.channel);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::MusicMIDISetting const &  manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_target_id()>(map, manifest.target.id);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_clip_tempo_source()>(map, manifest.clip_tempo.source);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &         map,
			typename Manifest::AudioPlaybackLimitSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &        map,
			typename Manifest::AudioVirtualVoiceSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &            map,
			typename Manifest::AudioPlaybackPrioritySetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::playback_priority_value()>(map, manifest.value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::playback_priority_offset_at_maximum_distance()>(map, manifest.offset_at_maximum_distance);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &  map,
			typename Manifest::AudioMotionSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72, 128})) {
				exchange_common_property_as_randomizable<CPTC::motion_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {72, 128})) {
				exchange_common_property_as_randomizable<CPTC::motion_volume_offset()>(map, manifest.volume_offset);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::AudioMixerSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::mixer_id()>(map, manifest.id);
			}
			return;
		}

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

		template <typename RawLength> requires
			CategoryConstraint<IsPureInstance<RawLength>>
		static auto exchange_unit_data (
			OByteStreamView & data,
			ByteList const &  value
		) -> Void {
			data.write(cbw<RawLength>(value.size()));
			data.write(value);
			return;
		}

		static auto exchange_unit_id (
			OByteStreamView &             data,
			typename Manifest::ID const & value
		) -> Void {
			data.write(cbw<IntegerU32>(value));
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<RawValue>(value));
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsFloaterWrapper<RawValue>)
		static auto exchange_unit_floater (
			OByteStreamView & data,
			Floater const &   value
		) -> Void {
			data.write(cbw<RawValue>(value));
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_enumerated (
			OByteStreamView &  data,
			Enumerated const & value
		) -> Void {
			data.write(cbw<RawValue>(value));
			return;
		}

		template <typename RawValue, auto ignore_reserve = k_false, typename ... Value> requires
			CategoryConstraint<IsPureInstance<RawValue> && IsInstance<Value ...>>
			&& (IsIntegerWrapper<RawValue>)
			&& (IsSameV<ignore_reserve, Boolean>)
			&& ((IsSame<Value, Boolean> || IsEnumerationWrapper<Value>) && ...)
		static auto exchange_unit_bit (
			OByteStreamView & data,
			Value const & ... value
		) -> Void {
			auto raw_value = RawValue{};
			auto bit_set = BitSet<k_type_bit_count<RawValue>>{};
			auto current_index = k_begin_index;
			Generalization::each_with<>(
				[&] <auto index, typename CurrentValue> (ValuePackage<index>, CurrentValue const & current_value) {
					if constexpr (IsSame<CurrentValue, Boolean>) {
						bit_set.set(current_index, current_value);
						++current_index;
					} else if constexpr (IsEnumerationWrapper<CurrentValue>) {
						auto index_value = Enumerated{};
						exchange_enumeration_index(index_value, current_value);
						for (auto & bit_index : SizeRange{EnumerationAttribute<CurrentValue>::size}) {
							bit_set.set(current_index, cbw<Boolean>(clip_bit(index_value, bit_index, 1_sz)));
							++current_index;
						}
					}
				},
				value ...
			);
			raw_value = bit_set.to_integer();
			data.write(raw_value);
			return;
		}

		template <typename RawLength, auto is_zeroed = k_false> requires
			CategoryConstraint<IsPure<RawLength>>
			&& (IsVoid<RawLength> || IsIntegerWrapper<RawLength>)
			&& (IsSameV<is_zeroed, Boolean>)
		static auto exchange_unit_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			if constexpr (IsVoid<RawLength>) {
				StringParser::write_string_until(self_cast<OCharacterStreamView>(data), value.as_view(), CharacterType::k_null);
				self_cast<OCharacterStreamView>(data).write_constant(CharacterType::k_null);
			}
			if constexpr (IsIntegerWrapper<RawLength>) {
				if constexpr (!is_zeroed) {
					data.write(cbw<RawLength>(value.size()));
					data.write(value);
				} else {
					data.write(cbw<RawLength>(value.size() + 1_sz));
					data.write(value);
					self_cast<OCharacterStreamView>(data).write_constant(CharacterType::k_null);
				}
			}
			return;
		}

		template <typename RawLength, typename Element, typename LeadingParser, typename ... ElementParser> requires
			CategoryConstraint<IsPure<RawLength> && IsPureInstance<Element> && IsPureInstance<LeadingParser> && IsPureInstance<ElementParser ...>>
			&& (IsVoid<RawLength> || IsIntegerWrapper<RawLength>)
			&& (IsGenericCallable<LeadingParser> && (IsGenericCallable<ElementParser> && ...))
		static auto exchange_unit_list (
			OByteStreamView &         data,
			List<Element> const &     list,
			LeadingParser const &     leading_parser,
			ElementParser const & ... element_parser
		) -> Void {
			auto count = Size{};
			count = list.size();
			auto length_position = data.position();
			if constexpr (!IsVoid<RawLength>) {
				data.forward(bs_static_size<RawLength>());
			}
			leading_parser(count);
			if constexpr (!IsVoid<RawLength>) {
				OByteStreamView{data.sub_view(length_position, bs_static_size<RawLength>())}.write(cbw<RawLength>(count));
			}
			Generalization::each_with<>(
				[&] (auto, auto & current_element_parser) {
					for (auto & element : list) {
						current_element_parser(data, element);
					}
				},
				element_parser ...
			);
			return;
		}

		template <typename ActualValue> requires
			CategoryConstraint<>
			&& (IsSame<ActualValue, Boolean, Integer, Floater, Enumerated, IDWrapper>)
		static auto process_unit_common_property_value (
			OByteStreamView &           data,
			CommonPropertyValue const & value
		) -> Void {
			if constexpr (IsSame<ActualValue, Boolean>) {
				exchange_unit_bit<IntegerU32>(data, value.template get<Boolean>());
			}
			if constexpr (IsSame<ActualValue, Integer>) {
				exchange_unit_integer<IntegerS32>(data, value.template get<Integer>());
			}
			if constexpr (IsSame<ActualValue, Floater>) {
				exchange_unit_floater<FloaterS32>(data, value.template get<Floater>());
			}
			if constexpr (IsSame<ActualValue, Enumerated>) {
				exchange_unit_enumerated<IntegerU32>(data, value.template get<Enumerated>());
			}
			if constexpr (IsSame<ActualValue, IDWrapper>) {
				exchange_unit_id(data, value.template get<IDWrapper>().value);
			}
			return;
		}

		// ----------------

		template <typename Type, typename Parser> requires
			CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Parser>>
			&& (IsEnumerationWrapper<Type>)
			&& (IsGenericCallable<Parser>)
		static auto process_section_sub (
			OByteStreamView & data,
			Boolean const &   randomizable,
			Parser const &    parser
		) -> Void {
			// NOTE : here
			auto map = CommonPropertyMap<Type>{};
			parser(map);
			exchange_unit_list<IntegerU8>(
				data,
				map.regular.as_list(),
				[] (auto & count) {
				},
				[] (auto & data, auto & element) {
					auto type = Enumerated{};
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							if (index == static_cast<ZSize>(element.key.value)) {
								type = Attribute::template Element<1_ixz>::template element<1_ixz>;
								has_case = k_true;
							}
						}
					);
					assert_test(has_case);
					exchange_unit_enumerated<IntegerU8>(data, type);
				},
				[] (auto & data, auto & element) {
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							if (index == static_cast<ZSize>(element.key.value)) {
								process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
								has_case = k_true;
							}
						}
					);
					assert_test(has_case);
				}
			);
			if (randomizable) {
				exchange_unit_list<IntegerU8>(
					data,
					map.randomizer.as_list(),
					[] (auto & count) {
					},
					[] (auto & data, auto & element) {
						auto type = Enumerated{};
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								if (index == static_cast<ZSize>(element.key.value)) {
									type = Attribute::template Element<1_ixz>::template element<1_ixz>;
									has_case = k_true;
								}
							}
						);
						assert_test(has_case);
						exchange_unit_enumerated<IntegerU8>(data, type);
					},
					[] (auto & data, auto & element) {
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								if (index == static_cast<ZSize>(element.key.value)) {
									process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
									process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<2_ix>());
									has_case = k_true;
								}
							}
						);
						assert_test(has_case);
					}
				);
			}
			return;
		}

		template <typename Count> requires
			CategoryConstraint<IsPureInstance<Count>>
			&& (IsIntegerWrapper<Count>)
		static auto process_section_sub (
			OByteStreamView &                   data,
			List<typename Manifest::ID> const & id
		) -> Void {
			exchange_unit_list<Count>(
				data,
				id,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                          data,
			typename Manifest::RealTimeParameterControlSetting const & real_time_parameter_control_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU16>(
					data,
					real_time_parameter_control_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.parameter.id);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.parameter.category);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.u1);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_integer<IntegerU32>(data, manifest.type);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u2);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                       data,
			typename Manifest::StateSetting const & state_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 125})) {
				exchange_unit_list<IntegerU32>(
					data,
					state_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 125})) {
							exchange_unit_id(data, manifest.group);
						}
						if constexpr (check_version(version, {72, 125})) {
							exchange_unit_bit<IntegerU8>(data, manifest.change_occur_at);
						}
						if constexpr (check_version(version, {72, 125})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.apply,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72, 125})) {
										exchange_unit_id(data, manifest.target);
									}
									if constexpr (check_version(version, {72, 125})) {
										exchange_unit_id(data, manifest.setting);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {125})) {
				exchange_unit_list<IntegerU8>(
					data,
					state_manifest.attribute,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {125})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
						if constexpr (check_version(version, {125})) {
							exchange_unit_bit<IntegerU8>(data, manifest.category);
						}
						if constexpr (check_version(version, {128})) {
							exchange_unit_integer<IntegerU8>(data, manifest.u1);
						}
					}
				);
				exchange_unit_list<IntegerU8>(
					data,
					state_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {125})) {
							exchange_unit_id(data, manifest.group);
						}
						if constexpr (check_version(version, {125})) {
							exchange_unit_bit<IntegerU8>(data, manifest.change_occur_at);
						}
						if constexpr (check_version(version, {125})) {
							exchange_unit_list<IntegerU8>(
								data,
								manifest.apply,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {125})) {
										exchange_unit_id(data, manifest.target);
									}
									if constexpr (check_version(version, {125})) {
										exchange_unit_id(data, manifest.setting);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                         data,
			List<typename Manifest::EffectU1> const & u1_manifest
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_list<IntegerU16>(
					data,
					u1_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {112})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
						if constexpr (check_version(version, {128})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_floater<FloaterS32>(data, manifest.value);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                      data,
			typename Manifest::AudioVoiceVolumeGainSetting const & voice_volume_gain_manifest,
			typename Manifest::AudioHDRSetting const &             hdr_manifest,
			Boolean const &                                        voice_volume_loudness_normalization_override,
			Boolean const &                                        hdr_envelope_tracking_override
		) -> Void {
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, hdr_envelope_tracking_override);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, voice_volume_loudness_normalization_override);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, voice_volume_gain_manifest.normalization);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, hdr_manifest.envelope_tracking.enable);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_envelope_tracking_override,
					voice_volume_loudness_normalization_override,
					voice_volume_gain_manifest.normalization,
					hdr_manifest.envelope_tracking.enable
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                data,
			typename Manifest::AudioOutputBusSetting const & output_bus_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, output_bus_manifest.bus);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                            data,
			typename Manifest::AudioMixerSetting const & mixer_manifest,
			Boolean const &                              mixer_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					mixer_override
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                    data,
			typename Manifest::AudioAuxiliarySendSetting const & auxiliary_send_manifest,
			Boolean const &                                      game_defined_auxiliary_send_override,
			Boolean const &                                      user_defined_auxiliary_send_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, game_defined_auxiliary_send_override);
				exchange_unit_bit<IntegerU8>(data, auxiliary_send_manifest.game_defined.enable);
				exchange_unit_bit<IntegerU8>(data, user_defined_auxiliary_send_override);
				exchange_unit_bit<IntegerU8>(data, auxiliary_send_manifest.user_defined.enable);
			}
			if constexpr (check_version(version, {112, 135})) {
				exchange_unit_bit<IntegerU8>(
					data,
					game_defined_auxiliary_send_override,
					auxiliary_send_manifest.game_defined.enable,
					user_defined_auxiliary_send_override,
					auxiliary_send_manifest.user_defined.enable
				);
			}
			if constexpr (check_version(version, {72, 135})) {
				if (auxiliary_send_manifest.user_defined.enable) {
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_1.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_2.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_3.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_4.bus);
					}
				}
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                    data,
			typename Manifest::AudioAuxiliarySendSetting const & auxiliary_send_manifest,
			Boolean const &                                      game_defined_auxiliary_send_override,
			Boolean const &                                      user_defined_auxiliary_send_override,
			Boolean const &                                      early_reflection_auxiliary_send_override
		) -> Void {
			if constexpr (check_version(version, {135})) {
				exchange_unit_bit<IntegerU8>(
					data,
					game_defined_auxiliary_send_override,
					auxiliary_send_manifest.game_defined.enable,
					user_defined_auxiliary_send_override,
					auxiliary_send_manifest.user_defined.enable,
					early_reflection_auxiliary_send_override
				);
			}
			if constexpr (check_version(version, {135})) {
				if (auxiliary_send_manifest.user_defined.enable) {
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_1.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_2.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_3.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_4.bus);
					}
				}
			}
			if constexpr (check_version(version, {135})) {
				exchange_unit_id(data, auxiliary_send_manifest.early_reflection.bus);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                  data,
			typename Manifest::AudioPositioningSetting const & positioning_manifest,
			Boolean const &                                    positioning_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, positioning_override);
				if (positioning_override) {
					// NOTE : here
					auto b1 = Boolean{};
					auto b2 = Boolean{};
					auto b3 = Boolean{};
					if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::two_dimension()) {
						if constexpr (check_version(version, {72, 88})) {
							b2 = k_false;
						}
						if constexpr (check_version(version, {88, 112})) {
							b3 = k_true;
						}
						b1 = positioning_manifest.speaker_panning.enable;
					}
					if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::three_dimension()) {
						if constexpr (check_version(version, {72, 88})) {
							b2 = k_true;
						}
						if constexpr (check_version(version, {88, 112})) {
							b3 = k_false;
						}
						b1 = positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::game_defined();
					}
					if constexpr (check_version(version, {88, 112})) {
						exchange_unit_bit<IntegerU8>(data, b3);
					}
					if constexpr (check_version(version, {72, 112})) {
						exchange_unit_bit<IntegerU8>(data, positioning_manifest.type);
					}
					if constexpr (check_version(version, {72, 88})) {
						exchange_unit_bit<IntegerU8>(
							data,
							b1,
							b2
						);
					}
					if constexpr (check_version(version, {88, 112})) {
						exchange_unit_bit<IntegerU8>(
							data,
							b1
						);
					}
					if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::two_dimension()) {
					}
					if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::three_dimension()) {
						exchange_unit_constant(data, 0_iu8);
						exchange_unit_constant(data, 0_iu8);
						exchange_unit_constant(data, 0_iu8);
						exchange_unit_id(data, positioning_manifest.listener_routing.attenuation.id);
						exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.spatialization);
						if (positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::game_defined()) {
							exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.position_source.update_at_each_frame);
						}
						if (positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) {
							exchange_unit_bit<IntegerU8>(
								data,
								positioning_manifest.listener_routing.position_source.automation.play_type,
								positioning_manifest.listener_routing.position_source.automation.play_mode,
								positioning_manifest.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
							exchange_unit_constant(data, 0_iu8);
							exchange_unit_constant(data, 0_iu8);
							exchange_unit_constant(data, 0_iu8);
							exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.position_source.automation.loop);
							exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_routing.position_source.automation.transition_time);
							exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.position_source.hold_listener_orientation);
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									exchange_unit_constant(data, 0_iu32);
									exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									exchange_unit_integer<IntegerU32>(data, manifest.duration);
								}
							);
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.path,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
									exchange_unit_integer<IntegerU32>(data, manifest.point.count);
								},
								[] (auto & data, auto & manifest) {
									exchange_unit_floater<FloaterS32>(data, manifest.random_range.left_right);
									exchange_unit_floater<FloaterS32>(data, manifest.random_range.front_back);
								}
							);
						}
					}
				}
			}
			if constexpr (check_version(version, {112, 132})) {
				auto b2 = Boolean{};
				if constexpr (check_version(version, {112, 125})) {
					exchange_unit_bit<IntegerU8>(
						data,
						positioning_override,
						b2,
						// TODO
						positioning_manifest.speaker_panning.enable,
						positioning_manifest.type,
						positioning_manifest.listener_routing.spatialization,
						positioning_manifest.listener_routing.position_source.automation.loop,
						positioning_manifest.listener_routing.position_source.update_at_each_frame,
						positioning_manifest.listener_routing.position_source.hold_listener_orientation
					);
				}
				if constexpr (check_version(version, {125, 132})) {
					exchange_unit_bit<IntegerU8>(
						data,
						positioning_override,
						positioning_manifest.enable,
						b2,
						// TODO
						positioning_manifest.speaker_panning.enable,
						positioning_manifest.type
					);
				}
				if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::three_dimension()) {
					if constexpr (check_version(version, {112, 125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.position_source.mode
						);
					}
					if constexpr (check_version(version, {125, 132})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.position_source.automation.loop,
							positioning_manifest.listener_routing.position_source.update_at_each_frame,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.mode
						);
					}
					if constexpr (check_version(version, {112, 132})) {
						exchange_unit_id(data, positioning_manifest.listener_routing.attenuation.id);
					}
					if (positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) {
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_bit<IntegerU8>(
								data,
								positioning_manifest.listener_routing.position_source.automation.play_type,
								positioning_manifest.listener_routing.position_source.automation.play_mode,
								positioning_manifest.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_routing.position_source.automation.transition_time);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.z);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.duration);
									}
								}
							);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.path,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.count);
									}
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.left_right);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.front_back);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.up_down);
									}
								}
							);
						}
					}
				}
			}
			if constexpr (check_version(version, {132})) {
				if constexpr (check_version(version, {132})) {
					exchange_unit_bit<IntegerU8>(
						data,
						positioning_override,
						positioning_manifest.listener_routing.enable,
						positioning_manifest.speaker_panning.mode,
						k_false,
						positioning_manifest.listener_routing.position_source.mode,
						k_false
					);
				}
				if (positioning_manifest.listener_routing.enable) {
					if constexpr (check_version(version, {132, 134})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.automation.loop
						);
					}
					if constexpr (check_version(version, {134, 140})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.attenuation.enable,
							positioning_manifest.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.automation.loop
						);
					}
					if constexpr (check_version(version, {140})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.attenuation.enable,
							positioning_manifest.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.automation.loop,
							positioning_manifest.listener_routing.position_source.diffraction_and_transmission
						);
					}
					if (positioning_manifest.listener_routing.position_source.mode != Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::emitter()) {
						if constexpr (check_version(version, {132})) {
							exchange_unit_bit<IntegerU8>(
								data,
								positioning_manifest.listener_routing.position_source.automation.play_type,
								positioning_manifest.listener_routing.position_source.automation.play_mode,
								positioning_manifest.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
						}
						if constexpr (check_version(version, {132})) {
							exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_routing.position_source.automation.transition_time);
						}
						if constexpr (check_version(version, {132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.z);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.duration);
									}
								}
							);
						}
						if constexpr (check_version(version, {132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.path,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.count);
									}
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.left_right);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.front_back);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.up_down);
									}
								}
							);
						}
					}
				}
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                           data,
			typename Manifest::MusicMIDISetting const & midi_manifest,
			Boolean const &                             midi_target_override,
			Boolean const &                             midi_clip_tempo_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					k_false,
					midi_clip_tempo_override,
					midi_target_override
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                       data,
			typename Manifest::SoundMIDISetting const &             midi_manifest,
			typename Manifest::AudioPlaybackPrioritySetting const & playback_priority_manifest,
			Boolean const &                                         midi_event_override,
			Boolean const &                                         midi_note_tracking_override,
			Boolean const &                                         playback_priority_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_priority_override,
					playback_priority_manifest.use_distance_factor,
					midi_event_override,
					midi_note_tracking_override,
					midi_manifest.note_tracking.enable,
					midi_manifest.event.break_on_note_off
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                       data,
			typename Manifest::AudioPlaybackPrioritySetting const & playback_priority_manifest,
			Boolean const &                                         playback_priority_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_priority_override);
				exchange_unit_bit<IntegerU8>(data, playback_priority_manifest.use_distance_factor);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_priority_override,
					playback_priority_manifest.use_distance_factor
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                    data,
			typename Manifest::AudioPlaybackLimitSetting const & playback_limit_manifest,
			Boolean const &                                      playback_limit_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_priority_is_equal);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_limit_is_reached);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_override);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                         data,
			typename Manifest::AudioPlaybackLimitSetting const &      playback_limit_manifest,
			typename Manifest::AudioBusMuteForBackgroundMusic const & mute_for_background_music_manifest,
			Boolean const &                                           playback_limit_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_limit_manifest.when_priority_is_equal,
					playback_limit_manifest.when_limit_is_reached,
					playback_limit_override,
					mute_for_background_music_manifest.enable
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                    data,
			typename Manifest::AudioPlaybackLimitSetting const & playback_limit_manifest,
			typename Manifest::AudioVirtualVoiceSetting const &  virtual_voice_manifest,
			Boolean const &                                      playback_limit_override,
			Boolean const &                                      virtual_voice_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.on_return_to_physical);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_priority_is_equal);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_limit_is_reached);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.scope);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.behavior);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_override);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_override);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_limit_manifest.when_priority_is_equal,
					playback_limit_manifest.when_limit_is_reached,
					playback_limit_manifest.scope,
					playback_limit_override,
					virtual_voice_override
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.on_return_to_physical);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.behavior);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                             data,
			typename Manifest::AudioEffectSetting const & effect_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU8>(
					data,
					effect_manifest.item,
					[&] (auto & count) {
						if constexpr (check_version(version, {72})) {
							if (count > 0_sz) {
								exchange_unit_bit<IntegerU8>(
									data,
									effect_manifest.bypass.template get<1_ix>(),
									effect_manifest.bypass.template get<2_ix>(),
									effect_manifest.bypass.template get<3_ix>(),
									effect_manifest.bypass.template get<4_ix>(),
									effect_manifest.bypass.template get<5_ix>()
								);
							}
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU8>(data, manifest.index);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							// TODO : in typical, render = 1 -> mode = 0 & u2 = 1, render = 0 -> mode = 1 & u2 = 0
							// TODO : if render, mode value will be changed ?
							exchange_unit_bit<IntegerU8>(data, manifest.use_share_set);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.u1);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                             data,
			typename Manifest::AudioEffectSetting const & effect_manifest,
			Boolean const &                               effect_override
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, effect_override);
			}
			process_section_sub(data, effect_manifest);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                               data,
			typename Manifest::AudioMetadataSetting const & metadata_manifest
		) -> Void {
			if constexpr (check_version(version, {140})) {
				exchange_unit_list<IntegerU8>(
					data,
					metadata_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {140})) {
							exchange_unit_integer<IntegerU8>(data, manifest.index);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_bit<IntegerU8>(data, manifest.use_share_set);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                               data,
			typename Manifest::AudioMetadataSetting const & metadata_manifest,
			Boolean const &                                 metadata_override
		) -> Void {
			if constexpr (check_version(version, {140})) {
				exchange_unit_bit<IntegerU8>(data, metadata_override);
			}
			process_section_sub(data, metadata_manifest);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                             data,
			typename Manifest::AudioSourceSetting const & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.plug_in);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU32>(data, manifest.type);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.resource);
			}
			if constexpr (check_version(version, {72, 113})) {
				exchange_unit_id(data, manifest.source);
			}
			if constexpr (check_version(version, {72, 113})) {
				if (manifest.type != Manifest::AudioSourceType::Constant::streamed()) {
					exchange_unit_integer<IntegerU32>(data, manifest.resource_offset);
				}
			}
			if constexpr (check_version(version, {72, 112})) {
				if (manifest.type != Manifest::AudioSourceType::Constant::streamed()) {
					if constexpr (check_version(version, {72})) {
						exchange_unit_integer<IntegerU32>(data, manifest.resource_size);
					}
				}
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_integer<IntegerU32>(data, manifest.resource_size);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.is_voice);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					manifest.is_voice,
					k_false,
					k_false,
					manifest.non_cachable_stream
				);
			}
			if constexpr (check_version(version, {72})) {
				// TODO
				if ((manifest.plug_in & 0x0000FFFF_i) >= 0x0002_i) {
					exchange_unit_constant(data, 0_iu32);
				}
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                   data,
			List<typename Manifest::AudioSourceSetting> const & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section_sub(data, manifest);
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                     data,
			typename Manifest::BusAutomaticDuckingSetting const & automatic_ducking_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU32>(data, automatic_ducking_manifest.recovery_time);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, automatic_ducking_manifest.maximum_ducking_volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					automatic_ducking_manifest.bus,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS32>(data, manifest.volume);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_out);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_in);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.curve);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.target);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                data,
			typename Manifest::AudioBusConfiguration const & bus_configuration_manifest
		) -> Void {
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerU32>(data, bus_configuration_manifest.u1);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                        data,
			typename Manifest::BusHDRSetting const & hdr_manifest
		) -> Void {
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_manifest.enable
				);
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_manifest.dynamic.release_mode
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_manifest.enable,
					hdr_manifest.dynamic.release_mode
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                           data,
			typename Manifest::AudioTimeSetting const & time_setting_manifest,
			Boolean const &                             time_setting_override
		) -> Void {
			if constexpr (check_version(version, {72})) {
				// TODO : test frequency mode-preset time and offset
				// NOTE : time = 960000 * signature / tempo, then with frequency mode-preset
				exchange_unit_floater<FloaterS64>(data, time_setting_manifest.time);
			}
			if constexpr (check_version(version, {72})) {
				// NOTE : 0 if mode.no, millisecond if mode.custom, else by mode-preset
				exchange_unit_floater<FloaterS64>(data, time_setting_manifest.offset);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, time_setting_manifest.tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.first);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.second);
			}
			if constexpr (check_version(version, {72, 140})) {
				auto b2 = Boolean{};
				auto b3 = Boolean{};
				auto b4 = Boolean{};
				auto b5 = Boolean{};
				auto b6 = Boolean{};
				auto b7 = Boolean{};
				auto b8 = Boolean{};
				exchange_unit_bit<IntegerU8>(data, time_setting_override, b2, b3, b4, b5, b6, b7, b8);
				assert_test(b2 == b3 && b3 == b4 && b4 == b5 && b5 == b6 && b6 == b7 && b7 == b8);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_bit<IntegerU8>(data, time_setting_override);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                         data,
			typename Manifest::MusicTrackClip const & clip_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					clip_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.source);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_id(data, manifest.event);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.offset);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.begin);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.end);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.duration);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				if (!clip_manifest.item.empty()) {
					exchange_unit_integer<IntegerU32>(data, clip_manifest.u1);
				}
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					clip_manifest.curve,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.index);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU32>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                              data,
			typename Manifest::MusicStingerSetting const & stinger_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					stinger_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.trigger);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.segment_to_play);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.play_at);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.cue_name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.do_not_play_this_stinger_again_for);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.allow_playing_stinger_in_next_segment);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                 data,
			typename Manifest::MusicTransitionSetting const & transition_manifest
		) -> Void {
			constexpr auto process_section_sub_of_fade =
				[] (
				OByteStreamView &                                         data,
				typename Manifest::MusicTransitionSettingItemFade const & manifest
			) {
				if constexpr (check_version(version, {72})) {
					exchange_unit_integer<IntegerU32>(data, manifest.time);
				}
				if constexpr (check_version(version, {72})) {
					exchange_unit_integer<IntegerU32>(data, manifest.curve);
				}
				if constexpr (check_version(version, {72})) {
					exchange_unit_integer<IntegerS32>(data, manifest.offset);
				}
				return;
			};
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					transition_manifest.item,
					[] (auto & count) {
					},
					// NOTE : avoid clang bug
					[&process_section_sub_of_fade] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {88})) {
							exchange_unit_constant(data, 1_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.source.id);
						}
						if constexpr (check_version(version, {88})) {
							exchange_unit_constant(data, 1_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.destination.id);
						}
						if constexpr (check_version(version, {72})) {
							process_section_sub_of_fade(data, manifest.source.fade_out);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.source.exit_source_at);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.source.exit_source_at_custom_cue_match);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_unit_bit<IntegerU8, k_true>(data, manifest.source.play_post_exit);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_bit<IntegerU8>(data, manifest.source.play_post_exit);
						}
						if constexpr (check_version(version, {72})) {
							process_section_sub_of_fade(data, manifest.destination.fade_in);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.destination.custom_cue_filter_match_target);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u1);
						}
						if constexpr (check_version(version, {134})) {
							exchange_unit_bit<IntegerU16>(data, manifest.destination.jump_to);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU16>(data, manifest.destination.synchronize_to);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_unit_bit<IntegerU8, k_true>(data, manifest.destination.play_pre_entry);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_bit<IntegerU8>(data, manifest.destination.play_pre_entry);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.destination.custom_cue_filter_match_source_cue_name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.segment.enable);
						}
						if constexpr (check_version(version, {72})) {
							auto has_segment_data = Boolean{};
							if constexpr (check_version(version, {72, 88})) {
								has_segment_data = k_true;
							}
							if constexpr (check_version(version, {88})) {
								has_segment_data = manifest.segment.enable;
							}
							if (has_segment_data) {
								if constexpr (check_version(version, {72})) {
									exchange_unit_id(data, manifest.segment.id);
								}
								if constexpr (check_version(version, {72})) {
									process_section_sub_of_fade(data, manifest.segment.fade_in);
								}
								if constexpr (check_version(version, {72})) {
									process_section_sub_of_fade(data, manifest.segment.fade_out);
								}
								if constexpr (check_version(version, {72, 140})) {
									exchange_unit_bit<IntegerU8, k_true>(data, manifest.segment.play_pre_entry);
								}
								if constexpr (check_version(version, {140})) {
									exchange_unit_bit<IntegerU8>(data, manifest.segment.play_pre_entry);
								}
								if constexpr (check_version(version, {72, 140})) {
									exchange_unit_bit<IntegerU8, k_true>(data, manifest.segment.play_post_exit);
								}
								if constexpr (check_version(version, {140})) {
									exchange_unit_bit<IntegerU8>(data, manifest.segment.play_pre_entry);
								}
							}
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                      data,
			typename Manifest::MusicTrackTransitionSetting const & transition_manifest
		) -> Void {
			constexpr auto process_section_sub_of_fade =
				[] (
				OByteStreamView &                                         data,
				typename Manifest::MusicTransitionSettingItemFade const & manifest
			) {
				if constexpr (check_version(version, {112})) {
					exchange_unit_integer<IntegerU32>(data, manifest.time);
				}
				if constexpr (check_version(version, {112})) {
					exchange_unit_integer<IntegerU32>(data, manifest.curve);
				}
				if constexpr (check_version(version, {112})) {
					exchange_unit_integer<IntegerS32>(data, manifest.offset);
				}
				return;
			};
			if constexpr (check_version(version, {112})) {
				exchange_unit_constant(data, 1_iu32);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_id(data, transition_manifest.switcher);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub_of_fade(data, transition_manifest.source.fade_out);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU32>(data, transition_manifest.source.exit_source_at);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_id(data, transition_manifest.source.exit_source_at_custom_cue_match);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub_of_fade(data, transition_manifest.destination.fade_in);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                               data,
			typename Manifest::AudioSwitcherSetting const & switcher_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU32>(data, switcher_manifest.is_state);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, switcher_manifest.is_state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, switcher_manifest.group);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, switcher_manifest.default_item);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                  data,
			typename Manifest::AudioAssociationSetting const & association_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					association_manifest.argument,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {88})) {
							exchange_unit_bit<IntegerU8>(data, manifest.is_state);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					association_manifest.path,
					[&] (auto & count) {
						// NOTE : here
						count *= 12_sz;
						if constexpr (check_version(version, {72, 88})) {
							exchange_unit_integer<IntegerU8>(data, association_manifest.probability);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(
								data,
								association_manifest.mode
							);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.object);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.weight);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.probability);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                      data,
			typename Manifest::SoundPlaylistContainerScope const & scope_manifest,
			typename Manifest::AudioPlayType const &               play_type_manifest,
			typename Manifest::AudioPlayTypeSetting const &        play_type_setting_manifest,
			typename Manifest::AudioPlayMode const &               play_mode_manifest,
			typename Manifest::AudioPlayModeSetting const &        play_mode_setting_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop.value);
			}
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop.minimum_value);
			}
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop.maximum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, play_mode_setting_manifest.continuous.transition_duration.value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, play_mode_setting_manifest.continuous.transition_duration.minimum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, play_mode_setting_manifest.continuous.transition_duration.maximum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU16>(data, play_type_setting_manifest.random.avoid_repeat);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_setting_manifest.continuous.transition_type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_type_setting_manifest.random.type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_type_manifest);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, k_false);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_setting_manifest.continuous.always_reset_playlist);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, play_type_setting_manifest.sequence.at_end_of_playlist);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_manifest);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, scope_manifest);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					k_false,
					play_mode_setting_manifest.continuous.always_reset_playlist,
					play_type_setting_manifest.sequence.at_end_of_playlist,
					play_mode_manifest,
					scope_manifest
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                   data,
			List<typename Manifest::SoundPlaylistContainerPlaylistItem> const & playlist_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU16>(
					data,
					playlist_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.weight);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                        data,
			List<typename Manifest::SoundSwitchContainerObjectAttributeItem> const & object_attribute_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					object_attribute_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.play_first_only);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.continue_to_play_across_switch);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.play_first_only, manifest.continue_to_play_across_switch);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_integer<IntegerU32>(data, manifest.u1);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_integer<IntegerU8>(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_out_time);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_in_time);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                     data,
			List<typename Manifest::SoundSwitchContainerObjectAssignItem> const & assigned_object_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					assigned_object_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU32>(
								data,
								manifest.object,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_id(data, manifest);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                             data,
			List<typename Manifest::SoundBlendContainerTrackItem> const & track_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					track_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							process_section_sub(data, manifest.real_time_parameter_control);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.cross_fade.id);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.cross_fade.category);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU32>(
								data,
								manifest.child,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_id(data, manifest.id);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_list<IntegerU32>(
											data,
											manifest.point,
											[] (auto & count) {
											},
											[] (auto & data, auto & manifest) {
												if constexpr (check_version(version, {72})) {
													exchange_unit_floater<FloaterS32>(data, manifest.position.x);
												}
												if constexpr (check_version(version, {72})) {
													exchange_unit_floater<FloaterS32>(data, manifest.position.y);
												}
												if constexpr (check_version(version, {72})) {
													exchange_unit_bit<IntegerU32>(data, manifest.curve);
												}
											}
										);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                              data,
			typename Manifest::MusicTrackTrackType const & track_type_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU32>(data, track_type_manifest);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                      data,
			typename Manifest::MusicTrackTrackType const &         track_type_manifest,
			typename Manifest::AudioSwitcherSetting const &        switcher_manifest,
			typename Manifest::MusicTrackTransitionSetting const & transition_manifest
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, track_type_manifest);
			}
			if constexpr (check_version(version, {112})) {
				if (track_type_manifest == Manifest::MusicTrackTrackType::Constant::switcher()) {
					if constexpr (check_version(version, {112})) {
						process_section_sub(data, switcher_manifest);
					}
					if constexpr (check_version(version, {112})) {
						process_section_sub(data, transition_manifest);
					}
				}
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                           data,
			typename Manifest::MusicTrackStream const & stream_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU16>(data, stream_manifest.look_ahead_time);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                          data,
			typename Manifest::MusicSegmentCue const & cue_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					cue_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.time);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_unit_constant(data, 0_iu32);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_constant(data, 0_iu8);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                   data,
			List<typename Manifest::MusicPlaylistContainerPlaylistItem> const & playlist_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					playlist_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.child_count);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32, k_true>(data, manifest.play_mode, manifest.play_type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.loop);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_constant(data, 0_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.weight);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.random_setting.avoid_repeat);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.group); // TODO : maybe
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.random_setting.type);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                    data,
			List<typename Manifest::MusicSwitchContainerAssociationItem> const & association_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 88})) {
				exchange_unit_list<IntegerU32>(
					data,
					association_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 88})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72, 88})) {
							exchange_unit_id(data, manifest.child);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                        data,
			typename Manifest::AudioPlayMode const & play_mode_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_manifest);
			}
			return;
		}

		// ----------------

		static auto process_section (
			OByteStreamView &                     data,
			typename Manifest::StateGroup const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU32>(data, manifest.default_transition);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.custom_transition,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.from);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.to);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.time);
						}
					}
				);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::SwitchGroup const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parameter.id);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.parameter.category);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.point,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS32>(data, manifest.position.x);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.position.y);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.curve);
						}
					}
				);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                        data,
			typename Manifest::GameParameter const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, manifest.range_default);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU32>(data, manifest.interpolation_mode);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_floater<FloaterS32>(data, manifest.interpolation_attack);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_floater<FloaterS32>(data, manifest.interpolation_release);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.bind_to_built_in_parameter);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                                data,
			typename Manifest::GameSynchronizationU1 const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u1);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u2);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u3);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u4);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u5);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u6);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                                  data,
			typename Manifest::StatefulPropertySetting const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72, 128})) {
				exchange_unit_list<IntegerU8>(
					data,
					manifest.value,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 128})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 128})) {
							exchange_unit_floater<FloaterS32>(data, manifest.value);
						}
					}
				);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_list<IntegerU16>(
					data,
					manifest.value,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {128})) {
							exchange_unit_integer<IntegerU16>(data, manifest.type);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {128})) {
							exchange_unit_floater<FloaterS32>(data, manifest.value);
						}
					}
				);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::EventAction const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			// NOTE : here
			auto type = Enumerated{};
			auto type_data_begin = Size{};
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, manifest.scope, manifest.mode);
			}
			if constexpr (check_version(version, {72})) {
				type_data_begin = data.position();
				data.forward_view(bs_static_size<IntegerU8>());
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.target);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU8>(data, manifest.u1);
			}
			auto process_section_sub_of_exception_list =
				[&] (
			) {
				if constexpr (check_version(version, {72, 125})) {
					exchange_unit_list<IntegerU32>(
						data,
						manifest.exception,
						[] (auto & size) {
						},
						[] (auto & data, auto & manifest) {
							if constexpr (check_version(version, {72, 125})) {
								exchange_unit_id(data, manifest.id);
							}
							if constexpr (check_version(version, {72, 125})) {
								exchange_unit_bit<IntegerU8>(data, manifest.u1);
							}
						}
					);
				}
				if constexpr (check_version(version, {125})) {
					exchange_unit_list<IntegerU8>(
						data,
						manifest.exception,
						[] (auto & size) {
						},
						[] (auto & data, auto & manifest) {
							if constexpr (check_version(version, {125})) {
								exchange_unit_id(data, manifest.id);
							}
							if constexpr (check_version(version, {125})) {
								exchange_unit_bit<IntegerU8>(data, manifest.u1);
							}
						}
					);
				}
			};
			auto has_case = k_false;
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::play_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::play_audio()>();
					type = 4_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property<CPTC::probability()>(common_property, property_manifest.probability);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.sound_bank);
					}
					if constexpr (check_version(version, {145})) {
						exchange_unit_constant(data, 0_iu32);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::stop_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::stop_audio()>();
					type = 1_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							k_false,
							property_manifest.resume_state_transition,
							property_manifest.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::pause_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::pause_audio()>();
					type = 2_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72, 125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.include_delayed_resume_action
						);
					}
					if constexpr (check_version(version, {125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.include_delayed_resume_action,
							property_manifest.resume_state_transition,
							property_manifest.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::resume_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::resume_audio()>();
					type = 3_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72, 125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.master_resume
						);
					}
					if constexpr (check_version(version, {125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.master_resume,
							property_manifest.resume_state_transition,
							property_manifest.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::break_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::break_audio()>();
					type = 28_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::seek_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::seek_audio()>();
					type = 30_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.seek_type);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.seek_value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.seek_value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.seek_value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.seek_to_nearest_marker
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {113})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::post_event()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::post_event()>();
					type = 33_e;
					if constexpr (check_version(version, {113})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {113})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_pitch()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_pitch()>();
					type = !property_manifest.reset ? (8_e) : (9_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_volume()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_volume()>();
					type = !property_manifest.reset ? (10_e) : (11_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_bus_volume()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_bus_volume()>();
					type = !property_manifest.reset ? (12_e) : (13_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_low_pass_filter()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_low_pass_filter()>();
					type = !property_manifest.reset ? (14_e) : (15_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {112})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_high_pass_filter()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_high_pass_filter()>();
					type = !property_manifest.reset ? (32_e) : (48_e);
					if constexpr (check_version(version, {112})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {112})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {112})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {112})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_mute()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_mute()>();
					type = !property_manifest.reset ? (6_e) : (7_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_game_parameter()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_game_parameter()>();
					type = !property_manifest.reset ? (19_e) : (20_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.bypass_game_parameter_interpolation);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_state_availability()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_state_availability()>();
					type = !property_manifest.enable ? (17_e) : (16_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::activate_state()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::activate_state()>();
					type = 18_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.group);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.item);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::activate_switch()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::activate_switch()>();
					type = 25_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.group);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.item);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::activate_trigger()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::activate_trigger()>();
					type = 29_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_bypass_effect()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_bypass_effect()>();
					type = !property_manifest.reset ? (26_e) : (27_e);
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.enable);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.value.template get<1_ix>(),
							property_manifest.value.template get<2_ix>(),
							property_manifest.value.template get<3_ix>(),
							property_manifest.value.template get<4_ix>(),
							property_manifest.value.template get<5_ix>(),
							as_constant(property_manifest.reset),
							as_constant(property_manifest.reset),
							as_constant(property_manifest.reset)
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {112})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::release_envelope()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::release_envelope()>();
					type = 31_e;
					if constexpr (check_version(version, {112})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {112})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {113})) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::reset_playlist()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::reset_playlist()>();
					type = 34_e;
					if constexpr (check_version(version, {113})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {113})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {113})) {
						exchange_unit_constant(data, 4_iu8); // TODO : maybe fade curve ?
					}
					if constexpr (check_version(version, {113, 115})) {
						exchange_unit_constant(data, 0_iu32);
					}
					if constexpr (check_version(version, {115})) {
						exchange_unit_constant(data, 0_iu8);
					}
					has_case = k_true;
				}
			}
			assert_test(has_case);
			if constexpr (check_version(version, {72})) {
				exchange_unit_enumerated<IntegerU8>(as_lvalue(OByteStreamView{data.sub_view(type_data_begin, bs_static_size<IntegerU8>())}), type);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                data,
			typename Manifest::Event const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72, 125})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {125})) {
				process_section_sub<IntegerU8>(data, manifest.child);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                        data,
			typename Manifest::DialogueEvent const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerU8>(data, manifest.probability);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.association);
			}
			if constexpr (check_version(version, {120})) {
				exchange_unit_constant(data, 0_iu16);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::Attenuation const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {140})) {
				exchange_unit_bit<IntegerU8>(data, manifest.height_spread);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, manifest.cone.enable);
				if (manifest.cone.enable) {
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.inner_angle);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.outer_angle);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.low_pass_filter);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.high_pass_filter);
					}
				}
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.spread);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.game_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.user_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.spread);
			}
			if constexpr (check_version(version, {112, 145})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.game_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.user_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.spread);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.focus);
			}
			if constexpr (check_version(version, {145})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_game_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_user_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_spread);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_focus);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.obstruction_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.obstruction_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.obstruction_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.occlusion_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.occlusion_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.occlusion_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.diffraction_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.diffraction_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.diffraction_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.transmission_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.transmission_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.transmission_high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU8>(
					data,
					manifest.curve,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                                          data,
			typename Manifest::LowFrequencyOscillatorModulator const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::depth()>(common_property, manifest.depth);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::frequency()>(common_property, manifest.frequency);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::waveform()>(common_property, manifest.waveform);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::smoothing()>(common_property, manifest.smoothing);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::pulse_width_modulation()>(common_property, manifest.pulse_width_modulation);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::attack()>(common_property, manifest.attack);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::initial_phase_offset()>(common_property, manifest.initial_phase_offset);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                            data,
			typename Manifest::EnvelopeModulator const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::attack_time()>(common_property, manifest.attack_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::attack_curve()>(common_property, manifest.attack_curve);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::decay_time()>(common_property, manifest.decay_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::sustain_level()>(common_property, manifest.sustain_level);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::release_time()>(common_property, manifest.release_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::sustain_time()>(common_property, manifest.sustain_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_after_release);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                        data,
			typename Manifest::TimeModulator const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {132})) {
				process_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_randomizable<CPTC::initial_delay()>(common_property, manifest.initial_delay);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_regular<CPTC::duration()>(common_property, manifest.duration);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_randomizable<CPTC::loop()>(common_property, manifest.loop);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_randomizable<CPTC::playback_rate()>(common_property, manifest.playback_rate);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_at_end);
						}
					}
				);
			}
			if constexpr (check_version(version, {132})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                 data,
			typename Manifest::Effect const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.plug_in);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_data<IntegerU32>(data, manifest.expand);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_constant(data, 0_iu8);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {125, 128})) {
				exchange_unit_constant(data, 0_iu16);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.u1);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                 data,
			typename Manifest::Source const & manifest
		) -> Void {
			return process_section(data, self_cast<typename Manifest::Effect>(manifest));
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::AudioDevice const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {128})) {
				exchange_unit_id(data, manifest.plug_in);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_data<IntegerU32>(data, manifest.expand);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_constant(data, 0_iu8);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.u1);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.effect);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                   data,
			typename Manifest::AudioBus const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {128})) {
				if (manifest.parent == 0_i) {
					if constexpr (check_version(version, {128})) {
						exchange_unit_id(data, manifest.audio_device);
					}
				}
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_false,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {125})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.bus);
						}
						if constexpr (check_version(version, {128})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {125})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
					}
				);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.override_positioning);
				exchange_unit_bit<IntegerU8>(data, manifest.positioning.speaker_panning.enable);
			}
			if constexpr (check_version(version, {112, 125})) {
				exchange_unit_bit<IntegerU8>(data, manifest.override_positioning, manifest.positioning.speaker_panning.enable);
			}
			if constexpr (check_version(version, {125})) {
				// NOTE : here
				auto override_positioning = Boolean{k_true};
				process_section_sub(data, manifest.positioning, override_positioning);
			}
			if constexpr (check_version(version, {125, 135})) {
				// NOTE : here
				auto override_game_defined_auxiliary_send = Boolean{k_true};
				auto override_user_defined_auxiliary_send = Boolean{k_true};
				process_section_sub(data, manifest.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				// NOTE : here
				auto override_game_defined_auxiliary_send = Boolean{k_true};
				auto override_user_defined_auxiliary_send = Boolean{k_true};
				auto override_early_reflection_auxiliary_send = Boolean{k_true};
				process_section_sub(data, manifest.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send, override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_limit, manifest.override_playback_limit);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.playback_limit, manifest.mute_for_background_music, manifest.override_playback_limit);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.bus_configuration);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.hdr);
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_unit_constant(data, 63_iu32);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.automatic_ducking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_id(data, manifest.mixer);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_constant(data, 0_iu16);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                            data,
			typename Manifest::AuxiliaryAudioBus const & manifest
		) -> Void {
			return process_section(data, self_cast<typename Manifest::AudioBus>(manifest));
		}

		static auto process_section (
			OByteStreamView &                data,
			typename Manifest::Sound const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.source);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							exchange_common_property_as_randomizable<CPTC::playback_loop()>(common_property, manifest.playback_setting.loop);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                                 data,
			typename Manifest::SoundPlaylistContainer const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.scope, manifest.playback_setting.type, manifest.playback_setting.type_setting, manifest.playback_setting.mode, manifest.playback_setting.mode_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.playlist);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                               data,
			typename Manifest::SoundSwitchContainer const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.switcher);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.mode);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.object_assign);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.object_attribute);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                              data,
			typename Manifest::SoundBlendContainer const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.track);
			}
			if constexpr (check_version(version, {120})) {
				process_section_sub(data, manifest.playback_setting.mode);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                     data,
			typename Manifest::ActorMixer const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                     data,
			typename Manifest::MusicTrack const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.source);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.clip);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_setting.type);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.playback_setting.type, manifest.playback_setting.switcher, manifest.playback_setting.transition);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stream);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_constant(data, 0_iu16);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                       data,
			typename Manifest::MusicSegment const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stinger);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS64>(data, manifest.playback_setting.duration);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.cue);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                                 data,
			typename Manifest::MusicPlaylistContainer const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stinger);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.transition);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.playlist);
			}
			return;
		}

		static auto process_section (
			OByteStreamView &                               data,
			typename Manifest::MusicSwitchContainer const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stinger);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.transition);
			}
			if constexpr (check_version(version, {72, 88})) {
				process_section_sub(data, manifest.playback_setting.switcher);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, manifest.playback_setting.continue_playing_on_switch_change);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.association);
			}
			return;
		}

		// ----------------

		static auto process_chunk_bkhd (
			OByteStreamView &                    data,
			typename Manifest::SoundBank const & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				data.write_constant(cbw<VersionNumber>(version.number));
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.id);
			}
			if constexpr (check_version(version, {72, 125})) {
				exchange_unit_integer<IntegerU32>(data, manifest.language);
			}
			if constexpr (check_version(version, {125})) {
				exchange_unit_id(data, manifest.language);
			}
			if constexpr (check_version(version, {72})) {
				// NOTE : here
				data.write(manifest.header_expand);
			}
			return;
		}

		static auto process_chunk_didx_data (
			OByteStreamView &                   didx_data,
			OByteStreamView &                   data_data,
			List<typename Manifest::ID> const & manifest,
			Path const &                        embedded_media_directory,
			Size const &                        data_begin_position
		) -> Void {
			if constexpr (check_version(version, {72})) {
				// NOTE : here
				exchange_unit_list<Void>(
					didx_data,
					manifest,
					[] (auto & count) {
					},
					[&] (auto & data, auto & manifest) {
						auto data_offset = Integer{};
						auto data_size = Integer{};
						if (manifest == 0_i) {
							data_offset = 0_i;
							data_size = 0_i;
						} else {
							data_data.write_space(k_null_byte, compute_padding_size(data_begin_position + data_data.position(), k_data_block_padding_size));
							data_offset = cbw<Integer>(data_data.position());
							data_size = cbw<Integer>(FileSystem::read_stream_file(embedded_media_directory / "{}.wem"_sf(manifest), data_data));
						}
						exchange_unit_id(data, manifest);
						exchange_unit_integer<IntegerU32>(data, data_offset);
						exchange_unit_integer<IntegerU32>(data, data_size);
					}
				);
			}
			return;
		}

		static auto process_chunk_init (
			OByteStreamView &                  data,
			typename Manifest::Setting const & manifest
		) -> Void {
			if constexpr (check_version(version, {118})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.plug_in,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {118})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {118, 140})) {
							exchange_unit_string<IntegerU32, k_true>(data, manifest.library);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_string<Void>(data, manifest.library);
						}
					}
				);
			}
			return;
		}

		static auto process_chunk_stmg (
			OByteStreamView &                              data,
			typename Manifest::Setting const &             manifest,
			typename Manifest::GameSynchronization const & game_synchronization_manifest
		) -> Void {
			if constexpr (check_version(version, {145})) {
				exchange_unit_bit<IntegerU16>(data, manifest.voice_filter_behavior);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, manifest.volume_threshold);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU16>(data, manifest.maximum_voice_instance);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_constant(data, 50_iu16);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.state_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.switch_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.game_parameter,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			if constexpr (check_version(version, {120, 125})) {
				exchange_unit_constant(data, 0_iu32);
				exchange_unit_constant(data, 0_iu32);
			}
			if constexpr (check_version(version, {125, 140})) {
				exchange_unit_constant(data, 0_iu32);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.u1,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			return;
		}

		static auto process_chunk_hirc (
			OByteStreamView &                          data,
			List<typename Manifest::Hierarchy> const & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						// NOTE : here
						auto type = Enumerated{};
						auto type_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU8>())};
						auto size_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU32>())};
						auto item_data = OByteStreamView{data.reserve_view()};
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<typename Manifest::HierarchyType>::Index>(
							[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
								constexpr auto variant_type = mbw<typename Manifest::HierarchyType>(index);
								if constexpr (variant_type != Manifest::HierarchyType::Constant::unknown()) {
									if (static_cast<ZSize>(manifest.index().value) == index) {
										type = cbw<Enumerated>(value_index);
										process_section(item_data, manifest.template get_of_type<variant_type>());
										has_case = k_true;
									}
								}
							}
						);
						if (!has_case) {
							auto & unknown_manifest = manifest.template get_of_type<Manifest::HierarchyType::Constant::unknown()>();
							type = cbw<Enumerated>(unknown_manifest.type);
							item_data.write(unknown_manifest.data);
						}
						exchange_unit_enumerated<IntegerU8>(type_data, type);
						exchange_unit_integer<IntegerU32>(size_data, cbw<Integer>(item_data.position()));
						data.forward(item_data.position());
						return;
					}
				);
			}
			return;
		}

		static auto process_chunk_stid (
			OByteStreamView &                                   data,
			List<typename Manifest::SoundBankReference> const & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_constant(data, 1_iu32);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_string<IntegerU8>(data, manifest.name);
						}
					}
				);
			}
			return;
		}

		static auto process_chunk_envs (
			OByteStreamView &                  data,
			typename Manifest::Setting const & manifest
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				// NOTE : here
				exchange_unit_list<Void>(
					data,
					as_lvalue(
						make_list<CWrapperView<typename Manifest::ObstructionSetting>>(
							manifest.obstruction.volume,
							manifest.obstruction.low_pass_filter,
							manifest.occlusion.volume,
							manifest.occlusion.low_pass_filter
						)
					),
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest_view) {
						auto & manifest = manifest_view.get();
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.enable);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72, 112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72, 112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72, 112})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				// NOTE : here
				exchange_unit_list<Void>(
					data,
					as_lvalue(
						make_list<CWrapperView<typename Manifest::ObstructionSetting>>(
							manifest.obstruction.volume,
							manifest.obstruction.low_pass_filter,
							manifest.obstruction.high_pass_filter,
							manifest.occlusion.volume,
							manifest.occlusion.low_pass_filter,
							manifest.occlusion.high_pass_filter
						)
					),
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest_view) {
						auto & manifest = manifest_view.get();
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.enable);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {112})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_chunk_plat (
			OByteStreamView &                  data,
			typename Manifest::Setting const & manifest
		) -> Void {
			if constexpr (check_version(version, {113, 118})) {
				exchange_unit_string<IntegerU32>(data, manifest.platform);
			}
			if constexpr (check_version(version, {118, 140})) {
				exchange_unit_string<IntegerU32, k_true>(data, manifest.platform);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_string<Void>(data, manifest.platform);
			}
			return;
		}

		// ----------------

		static auto process_sound_bank (
			OByteStreamView &                    sound_bank_data,
			typename Manifest::SoundBank const & sound_bank_manifest,
			Path const &                         embedded_media_directory
		) -> Void {
			if constexpr (check_version(version, {72})) {
				{
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_bkhd(chunk, sound_bank_manifest);
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::bkhd,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				constexpr auto didx_item_structure_size = bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>();
				if (!sound_bank_manifest.embedded_media.empty()) {
					auto didx_sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto didx_chunk = OByteStreamView{sound_bank_data.forward_view(didx_item_structure_size * sound_bank_manifest.embedded_media.size())};
					auto data_sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto data_chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_didx_data(didx_chunk, data_chunk, sound_bank_manifest.embedded_media, embedded_media_directory, sound_bank_data.position());
					didx_sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::didx,
							.size = cbw<IntegerU32>(didx_chunk.position()),
						}
					);
					data_sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::data,
							.size = cbw<IntegerU32>(data_chunk.position()),
						}
					);
					sound_bank_data.forward(data_chunk.position());
				}
			}
			if constexpr (check_version(version, {118})) {
				if (sound_bank_manifest.setting.has()) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_init(chunk, sound_bank_manifest.setting.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::init,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (sound_bank_manifest.setting.has()) {
					assert_test(sound_bank_manifest.game_synchronization.has());
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_stmg(chunk, sound_bank_manifest.setting.get(), sound_bank_manifest.game_synchronization.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::stmg,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (!sound_bank_manifest.hierarchy.empty()) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_hirc(chunk, sound_bank_manifest.hierarchy);
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::hirc,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (!sound_bank_manifest.reference.empty()) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_stid(chunk, sound_bank_manifest.reference);
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::stid,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (sound_bank_manifest.setting.has()) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_envs(chunk, sound_bank_manifest.setting.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::envs,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {113})) {
				if (sound_bank_manifest.setting.has()) {
					auto sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_plat(chunk, sound_bank_manifest.setting.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::plat,
							.size = cbw<IntegerU32>(chunk.position()),
						}
					);
					sound_bank_data.forward(chunk.position());
				}
			}
			return;
		}

	public:

		static auto do_process_sound_bank (
			OByteStreamView &                    sound_bank_data_,
			typename Manifest::SoundBank const & sound_bank_manifest,
			Path const &                         embedded_media_directory
		) -> Void {
			M_use_zps_of(sound_bank_data);
			return process_sound_bank(sound_bank_data, sound_bank_manifest, embedded_media_directory);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::VersionNumber;

		using typename Common::ChunkSign;

		using typename Common::ChunkSignFlag;

		using Common::k_data_block_padding_size;

		using typename Common::IDWrapper;

		using typename Common::CommonPropertyValue;

		template <typename _1>
		using CommonPropertyMap = typename Common::template CommonPropertyMap<_1>;

		using typename Common::EventActionCommonPropertyType;

		using typename Common::ModulatorCommonPropertyType;

		using typename Common::AudioCommonPropertyType;

		template <typename _1>
		using EnumerationAttribute = typename Common::template EnumerationAttribute<_1>;

		// ----------------

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsEnumerationWrapper<Value>)
		static auto exchange_enumeration_index (
			Enumerated const & index_value,
			Value &            value
		) -> Void {
			auto has_case = k_false;
			Generalization::each<typename EnumerationAttribute<Value>::Index>(
				[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
					if (value_index == index_value) {
						value = mbw<Value>(index);
						has_case = k_true;
					}
				}
			);
			assert_test(has_case);
			return;
		}

		// ----------------

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>, Value, Enumerated, IDWrapper>)
		static auto exchange_common_property (
			CommonPropertyMap<decltype(type)> const & map,
			Value &                                   value
		) -> Void {
			using CurrentEnumerationAttribute = typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>;
			if (auto element_if = map.regular.query_if(type)) {
				auto & element = element_if.get();
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Value>) {
					value = element.value.template get<1_ix>().template get<Value>();
				}
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Enumerated>) {
					exchange_enumeration_index(element.value.template get<1_ix>().template get<Enumerated>(), value);
				}
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, IDWrapper>) {
					value = element.value.template get<1_ix>().template get<IDWrapper>().value;
				}
			} else {
				constexpr auto default_value = CurrentEnumerationAttribute::template Element<3_ixz>::template element<1_ixz>;
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Value>) {
					value = default_value;
				}
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Enumerated>) {
					exchange_enumeration_index(default_value, value);
				}
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, IDWrapper>) {
					value = default_value.value;
				}
			}
			return;
		}

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<Value, typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>>)
			&& (IsSame<Value, Boolean, Integer, Floater>)
		static auto exchange_common_property_as_regular (
			CommonPropertyMap<decltype(type)> const &         map,
			typename Manifest::template RegularValue<Value> & value
		) -> Void {
			if (auto element_if = map.regular.query_if(type)) {
				auto & element = element_if.get();
				value.value = element.value.template get<1_ix>().template get<Value>();
			} else {
				value.value = EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<3_ixz>::template element<1_ixz>;
			}
			return;
		}

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<Value, typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>>)
			&& (IsSame<Value, Boolean, Integer, Floater>)
		static auto exchange_common_property_as_randomizable (
			CommonPropertyMap<decltype(type)> const &              map,
			typename Manifest::template RandomizableValue<Value> & value
		) -> Void {
			if (auto element_if = map.regular.query_if(type)) {
				auto & element = element_if.get();
				value.value = element.value.template get<1_ix>().template get<Value>();
			} else {
				value.value = EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<3_ixz>::template element<1_ixz>;
			}
			if (auto element_if = map.randomizer.query_if(type)) {
				auto & element = element_if.get();
				value.minimum_value = element.value.template get<1_ix>().template get<Value>();
				value.maximum_value = element.value.template get<2_ix>().template get<Value>();
			} else {
				value.minimum_value = mbw<Value>(0);
				value.maximum_value = mbw<Value>(0);
			}
			return;
		}

		// ----------------

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusVoiceSetting &               manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::voice_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::voice_pitch()>(map, manifest.pitch);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioVoice &                    manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_randomizable<CPTC::voice_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_randomizable<CPTC::voice_pitch()>(map, manifest.pitch);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_randomizable<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_randomizable<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusVoiceVolumeGainSetting &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {125})) {
				exchange_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioVoiceVolumeGainSetting &   manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88, 112})) {
				exchange_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_randomizable<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusBusSetting &                 manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::bus_volume()>(map, manifest.volume);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusOutputBusSetting &           manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioOutputBusSetting &         manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioAuxiliarySendSetting &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_volume()>(map, manifest.game_defined.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_low_pass_filter()>(map, manifest.game_defined.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_high_pass_filter()>(map, manifest.game_defined.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_0()>(map, manifest.user_defined.item_1.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_0()>(map, manifest.user_defined.item_1.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_0()>(map, manifest.user_defined.item_1.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_1()>(map, manifest.user_defined.item_2.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_1()>(map, manifest.user_defined.item_2.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_1()>(map, manifest.user_defined.item_2.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_2()>(map, manifest.user_defined.item_3.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_2()>(map, manifest.user_defined.item_3.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_2()>(map, manifest.user_defined.item_3.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_3()>(map, manifest.user_defined.item_4.volume);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_3()>(map, manifest.user_defined.item_4.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_3()>(map, manifest.user_defined.item_4.high_pass_filter);
			}
			if constexpr (check_version(version, {135})) {
				exchange_common_property_as_regular<CPTC::early_reflection_auxiliary_send_volume()>(map, manifest.early_reflection.volume);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioPositioningSetting &       manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::positioning_center_percent()>(map, manifest.center_percent);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property<CPTC::positioning_speaker_panning_x()>(map, manifest.speaker_panning.position.x);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property<CPTC::positioning_speaker_panning_y()>(map, manifest.speaker_panning.position.y);
			}
			if constexpr (check_version(version, {140})) {
				exchange_common_property<CPTC::positioning_speaker_panning_z()>(map, manifest.speaker_panning.position.z);
			}
			if constexpr (check_version(version, {132})) {
				exchange_common_property_as_regular<CPTC::positioning_listener_routing_speaker_panning_division_spatialization_mix()>(map, manifest.listener_routing.speaker_panning_divsion_spatialization_mix);
			}
			if constexpr (check_version(version, {132})) {
				exchange_common_property<CPTC::positioning_listener_routing_attenuation_id()>(map, manifest.listener_routing.attenuation.id);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusHDRSetting &                 manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_threshold()>(map, manifest.dynamic.threshold);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_ratio()>(map, manifest.dynamic.ratio);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_release_time()>(map, manifest.dynamic.release_time);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_window_tap_output_game_parameter_id()>(map, manifest.window_top_output_game_parameter.id);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_window_tap_output_game_parameter_minimum()>(map, manifest.window_top_output_game_parameter.minimum);
			}
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_window_tap_output_game_parameter_maximum()>(map, manifest.window_top_output_game_parameter.maximum);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioHDRSetting &               manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88})) {
				exchange_common_property<CPTC::hdr_envelope_tracking_active_range()>(map, manifest.envelope_tracking.active_range);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::SoundMIDISetting &              manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_event_play_on()>(map, manifest.event.play_on);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_note_tracking_root_note()>(map, manifest.note_tracking.root_note);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::midi_transformation_transposition()>(map, manifest.transformation.transposition);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property_as_regular<CPTC::midi_transformation_velocity_offset()>(map, manifest.transformation.velocity_offset);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_key_range_minimum()>(map, manifest.filter.key_range_minimum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_key_range_maximum()>(map, manifest.filter.key_range_maximum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_velocity_minimum()>(map, manifest.filter.velocity_minimum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_velocity_maximum()>(map, manifest.filter.velocity_maximum);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_filter_channel()>(map, manifest.filter.channel);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::MusicMIDISetting &              manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_target_id()>(map, manifest.target.id);
			}
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::midi_clip_tempo_source()>(map, manifest.clip_tempo.source);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioPlaybackLimitSetting &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioVirtualVoiceSetting &      manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioPlaybackPrioritySetting &  manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::playback_priority_value()>(map, manifest.value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_common_property_as_regular<CPTC::playback_priority_offset_at_maximum_distance()>(map, manifest.offset_at_maximum_distance);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioMotionSetting &            manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72, 128})) {
				exchange_common_property_as_randomizable<CPTC::motion_low_pass_filter()>(map, manifest.low_pass_filter);
			}
			if constexpr (check_version(version, {72, 128})) {
				exchange_common_property_as_randomizable<CPTC::motion_volume_offset()>(map, manifest.volume_offset);
			}
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioMixerSetting &             manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				exchange_common_property<CPTC::mixer_id()>(map, manifest.id);
			}
			return;
		}

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

		template <typename Length> requires
			CategoryConstraint<IsPureInstance<Length>>
		static auto exchange_unit_data (
			IByteStreamView & data,
			ByteList &        value
		) -> Void {
			auto size = Size{};
			size = cbw<Size>(data.read_of<Length>());
			value = data.forward_view(size);
			return;
		}

		static auto exchange_unit_id (
			IByteStreamView &       data,
			typename Manifest::ID & value
		) -> Void {
			auto raw_value = IntegerU32{};
			data.read(raw_value);
			value = cbw<Integer>(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			value = cbw<Integer>(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsFloaterWrapper<RawValue>)
		static auto exchange_unit_floater (
			IByteStreamView & data,
			Floater &         value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			value = cbw<Floater>(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_enumerated (
			IByteStreamView & data,
			Enumerated &      value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			value = cbw<Enumerated>(raw_value);
			return;
		}

		template <typename RawValue, auto ignore_reserve = k_false, typename ... Value> requires
			CategoryConstraint<IsPureInstance<RawValue> && IsInstance<Value ...>>
			&& (IsIntegerWrapper<RawValue>)
			&& (IsSameV<ignore_reserve, Boolean>)
			&& ((IsSame<Value, Boolean const> || IsSame<Value, Boolean> || IsEnumerationWrapper<Value>) && ...)
		static auto exchange_unit_bit (
			IByteStreamView & data,
			Value & ...       value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			auto bit_set = BitSet<k_type_bit_count<RawValue>>{};
			bit_set.from_integer(raw_value);
			auto current_index = k_begin_index;
			Generalization::each_with<>(
				[&] <auto index, typename CurrentValue> (ValuePackage<index>, CurrentValue & current_value) {
					if constexpr (IsSame<CurrentValue, Boolean const>) {
						assert_test(bit_set.get(current_index) == current_value);
						++current_index;
					} else if constexpr (IsSame<CurrentValue, Boolean>) {
						current_value = bit_set.get(current_index);
						++current_index;
					} else if constexpr (IsEnumerationWrapper<CurrentValue>) {
						auto index_value = Enumerated{};
						for (auto & bit_index : SizeRange{EnumerationAttribute<CurrentValue>::size}) {
							index_value |= cbw<Enumerated>(bit_set.get(current_index)) << bit_index;
							++current_index;
						}
						exchange_enumeration_index(index_value, current_value);
					}
				},
				value ...
			);
			if constexpr (!ignore_reserve) {
				for (auto & index : SizeRange{k_type_bit_count<RawValue> - current_index}) {
					assert_test(!bit_set.get(current_index + index));
				}
			}
			return;
		}

		template <typename RawLength, auto is_zeroed = k_false> requires
			CategoryConstraint<IsPure<RawLength>>
			&& (IsVoid<RawLength> || IsIntegerWrapper<RawLength>)
			&& (IsSameV<is_zeroed, Boolean>)
		static auto exchange_unit_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			if constexpr (IsVoid<RawLength>) {
				auto value_view = CStringView{};
				StringParser::read_string_until(self_cast<ICharacterStreamView>(data), value_view, CharacterType::k_null);
				self_cast<ICharacterStreamView>(data).read_constant(CharacterType::k_null);
				value = value_view;
			}
			if constexpr (IsIntegerWrapper<RawLength>) {
				auto length = data.read_of<RawLength>();
				if constexpr (!is_zeroed) {
					data.read(value, cbw<Size>(length));
				} else {
					assert_test(cbw<Size>(length) >= 1_sz);
					data.read(value, cbw<Size>(length) - 1_sz);
					self_cast<ICharacterStreamView>(data).read_constant(CharacterType::k_null);
				}
			}
			return;
		}

		template <typename RawLength, typename Element, typename LeadingParser, typename ... ElementParser> requires
			CategoryConstraint<IsPure<RawLength> && IsPureInstance<Element> && IsPureInstance<LeadingParser> && IsPureInstance<ElementParser ...>>
			&& (IsVoid<RawLength> || IsIntegerWrapper<RawLength>)
			&& (IsGenericCallable<LeadingParser> && (IsGenericCallable<ElementParser> && ...))
		static auto exchange_unit_list (
			IByteStreamView &         data,
			List<Element> &           list,
			LeadingParser const &     leading_parser,
			ElementParser const & ... element_parser
		) -> Void {
			auto count = Size{};
			if constexpr (!IsVoid<RawLength>) {
				count = cbw<Size>(data.read_of<RawLength>());
			} else {
				count = list.size();
			}
			leading_parser(count);
			if constexpr (!IsVoid<RawLength>) {
				list.allocate_full(count);
			}
			Generalization::each_with<>(
				[&] (auto, auto & current_element_parser) {
					for (auto & element : list) {
						current_element_parser(data, element);
					}
				},
				element_parser ...
			);
			return;
		}

		template <typename ActualValue> requires
			CategoryConstraint<>
			&& (IsSame<ActualValue, Boolean, Integer, Floater, Enumerated, IDWrapper>)
		static auto process_unit_common_property_value (
			IByteStreamView &     data,
			CommonPropertyValue & value
		) -> Void {
			if constexpr (IsSame<ActualValue, Boolean>) {
				exchange_unit_bit<IntegerU32>(data, value.template set<Boolean>());
			}
			if constexpr (IsSame<ActualValue, Integer>) {
				exchange_unit_integer<IntegerS32>(data, value.template set<Integer>());
			}
			if constexpr (IsSame<ActualValue, Floater>) {
				exchange_unit_floater<FloaterS32>(data, value.template set<Floater>());
			}
			if constexpr (IsSame<ActualValue, Enumerated>) {
				exchange_unit_enumerated<IntegerU32>(data, value.template set<Enumerated>());
			}
			if constexpr (IsSame<ActualValue, IDWrapper>) {
				exchange_unit_id(data, value.template set<IDWrapper>().value);
			}
			return;
		}

		// ----------------

		template <typename Type, typename Parser> requires
			CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Parser>>
			&& (IsEnumerationWrapper<Type>)
			&& (IsGenericCallable<Parser>)
		static auto process_section_sub (
			IByteStreamView & data,
			Boolean const &   randomizable,
			Parser const &    parser
		) -> Void {
			// NOTE : here
			auto map = CommonPropertyMap<Type>{};
			exchange_unit_list<IntegerU8>(
				data,
				map.regular.as_list(),
				[] (auto & count) {
				},
				[] (auto & data, auto & element) {
					auto type = Enumerated{};
					exchange_unit_enumerated<IntegerU8>(data, type);
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							if (type == Attribute::template Element<1_ixz>::template element<1_ixz>) {
								element.key = mbw<Type>(index);
								has_case = k_true;
							}
						}
					);
					assert_test(has_case);
				},
				[] (auto & data, auto & element) {
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							if (index == static_cast<ZSize>(element.key.value)) {
								process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
								has_case = k_true;
							}
						}
					);
					assert_test(has_case);
				}
			);
			if (randomizable) {
				exchange_unit_list<IntegerU8>(
					data,
					map.randomizer.as_list(),
					[] (auto & count) {
					},
					[] (auto & data, auto & element) {
						auto type = Enumerated{};
						exchange_unit_enumerated<IntegerU8>(data, type);
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								if (type == Attribute::template Element<1_ixz>::template element<1_ixz>) {
									element.key = mbw<Type>(index);
									has_case = k_true;
								}
							}
						);
						assert_test(has_case);
					},
					[] (auto & data, auto & element) {
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								if (index == static_cast<ZSize>(element.key.value)) {
									process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
									process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<2_ix>());
									has_case = k_true;
								}
							}
						);
						assert_test(has_case);
					}
				);
			}
			parser(map);
			return;
		}

		template <typename Count> requires
			CategoryConstraint<IsPureInstance<Count>>
			&& (IsIntegerWrapper<Count>)
		static auto process_section_sub (
			IByteStreamView &             data,
			List<typename Manifest::ID> & id
		) -> Void {
			exchange_unit_list<Count>(
				data,
				id,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                    data,
			typename Manifest::RealTimeParameterControlSetting & real_time_parameter_control_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU16>(
					data,
					real_time_parameter_control_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.parameter.id);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.parameter.category);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.u1);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_integer<IntegerU32>(data, manifest.type);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u2);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                 data,
			typename Manifest::StateSetting & state_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 125})) {
				exchange_unit_list<IntegerU32>(
					data,
					state_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 125})) {
							exchange_unit_id(data, manifest.group);
						}
						if constexpr (check_version(version, {72, 125})) {
							exchange_unit_bit<IntegerU8>(data, manifest.change_occur_at);
						}
						if constexpr (check_version(version, {72, 125})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.apply,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72, 125})) {
										exchange_unit_id(data, manifest.target);
									}
									if constexpr (check_version(version, {72, 125})) {
										exchange_unit_id(data, manifest.setting);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {125})) {
				exchange_unit_list<IntegerU8>(
					data,
					state_manifest.attribute,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {125})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
						if constexpr (check_version(version, {125})) {
							exchange_unit_bit<IntegerU8>(data, manifest.category);
						}
						if constexpr (check_version(version, {128})) {
							exchange_unit_integer<IntegerU8>(data, manifest.u1);
						}
					}
				);
				exchange_unit_list<IntegerU8>(
					data,
					state_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {125})) {
							exchange_unit_id(data, manifest.group);
						}
						if constexpr (check_version(version, {125})) {
							exchange_unit_bit<IntegerU8>(data, manifest.change_occur_at);
						}
						if constexpr (check_version(version, {125})) {
							exchange_unit_list<IntegerU8>(
								data,
								manifest.apply,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {125})) {
										exchange_unit_id(data, manifest.target);
									}
									if constexpr (check_version(version, {125})) {
										exchange_unit_id(data, manifest.setting);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                   data,
			List<typename Manifest::EffectU1> & u1_manifest
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_list<IntegerU16>(
					data,
					u1_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {112})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
						if constexpr (check_version(version, {128})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_floater<FloaterS32>(data, manifest.value);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                data,
			typename Manifest::AudioVoiceVolumeGainSetting & voice_volume_gain_manifest,
			typename Manifest::AudioHDRSetting &             hdr_manifest,
			Boolean &                                        voice_volume_loudness_normalization_override,
			Boolean &                                        hdr_envelope_tracking_override
		) -> Void {
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, hdr_envelope_tracking_override);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, voice_volume_loudness_normalization_override);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, voice_volume_gain_manifest.normalization);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, hdr_manifest.envelope_tracking.enable);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_envelope_tracking_override,
					voice_volume_loudness_normalization_override,
					voice_volume_gain_manifest.normalization,
					hdr_manifest.envelope_tracking.enable
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                          data,
			typename Manifest::AudioOutputBusSetting & output_bus_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, output_bus_manifest.bus);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                      data,
			typename Manifest::AudioMixerSetting & mixer_manifest,
			Boolean &                              mixer_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					mixer_override
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                              data,
			typename Manifest::AudioAuxiliarySendSetting & auxiliary_send_manifest,
			Boolean &                                      game_defined_auxiliary_send_override,
			Boolean &                                      user_defined_auxiliary_send_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, game_defined_auxiliary_send_override);
				exchange_unit_bit<IntegerU8>(data, auxiliary_send_manifest.game_defined.enable);
				exchange_unit_bit<IntegerU8>(data, user_defined_auxiliary_send_override);
				exchange_unit_bit<IntegerU8>(data, auxiliary_send_manifest.user_defined.enable);
			}
			if constexpr (check_version(version, {112, 135})) {
				exchange_unit_bit<IntegerU8>(
					data,
					game_defined_auxiliary_send_override,
					auxiliary_send_manifest.game_defined.enable,
					user_defined_auxiliary_send_override,
					auxiliary_send_manifest.user_defined.enable
				);
			}
			if constexpr (check_version(version, {72, 135})) {
				if (auxiliary_send_manifest.user_defined.enable) {
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_1.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_2.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_3.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_4.bus);
					}
				}
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                              data,
			typename Manifest::AudioAuxiliarySendSetting & auxiliary_send_manifest,
			Boolean &                                      game_defined_auxiliary_send_override,
			Boolean &                                      user_defined_auxiliary_send_override,
			Boolean &                                      early_reflection_auxiliary_send_override
		) -> Void {
			if constexpr (check_version(version, {135})) {
				exchange_unit_bit<IntegerU8>(
					data,
					game_defined_auxiliary_send_override,
					auxiliary_send_manifest.game_defined.enable,
					user_defined_auxiliary_send_override,
					auxiliary_send_manifest.user_defined.enable,
					early_reflection_auxiliary_send_override
				);
			}
			if constexpr (check_version(version, {135})) {
				if (auxiliary_send_manifest.user_defined.enable) {
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_1.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_2.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_3.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_unit_id(data, auxiliary_send_manifest.user_defined.item_4.bus);
					}
				}
			}
			if constexpr (check_version(version, {135})) {
				exchange_unit_id(data, auxiliary_send_manifest.early_reflection.bus);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                            data,
			typename Manifest::AudioPositioningSetting & positioning_manifest,
			Boolean &                                    positioning_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, positioning_override);
				if (positioning_override) {
					// NOTE : here
					auto b1 = Boolean{};
					auto b2 = Boolean{};
					auto b3 = Boolean{};
					if constexpr (check_version(version, {88, 112})) {
						exchange_unit_bit<IntegerU8>(data, b3);
					}
					if constexpr (check_version(version, {72, 112})) {
						exchange_unit_bit<IntegerU8>(data, positioning_manifest.type);
					}
					if constexpr (check_version(version, {72, 88})) {
						exchange_unit_bit<IntegerU8>(
							data,
							b1,
							b2
						);
					}
					if constexpr (check_version(version, {88, 112})) {
						exchange_unit_bit<IntegerU8>(
							data,
							b1
						);
					}
					if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::two_dimension()) {
						if constexpr (check_version(version, {72, 88})) {
							assert_test(!b2);
						}
						if constexpr (check_version(version, {88, 112})) {
							assert_test(b3);
						}
						positioning_manifest.speaker_panning.enable = b1;
					}
					if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::three_dimension()) {
						if constexpr (check_version(version, {72, 88})) {
							assert_test(b2);
						}
						if constexpr (check_version(version, {88, 112})) {
							assert_test(!b3);
						}
						positioning_manifest.listener_routing.position_source.mode = !b1 ? (Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) : (Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::game_defined());
						exchange_unit_constant(data, 0_iu8);
						exchange_unit_constant(data, 0_iu8);
						exchange_unit_constant(data, 0_iu8);
						exchange_unit_id(data, positioning_manifest.listener_routing.attenuation.id);
						exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.spatialization);
						if (positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::game_defined()) {
							exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.position_source.update_at_each_frame);
						}
						if (positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) {
							exchange_unit_bit<IntegerU8>(
								data,
								positioning_manifest.listener_routing.position_source.automation.play_type,
								positioning_manifest.listener_routing.position_source.automation.play_mode,
								positioning_manifest.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
							exchange_unit_constant(data, 0_iu8);
							exchange_unit_constant(data, 0_iu8);
							exchange_unit_constant(data, 0_iu8);
							exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.position_source.automation.loop);
							exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_routing.position_source.automation.transition_time);
							exchange_unit_bit<IntegerU8>(data, positioning_manifest.listener_routing.position_source.hold_listener_orientation);
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									exchange_unit_constant(data, 0_iu32);
									exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									exchange_unit_integer<IntegerU32>(data, manifest.duration);
								}
							);
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.path,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
									exchange_unit_integer<IntegerU32>(data, manifest.point.count);
								},
								[] (auto & data, auto & manifest) {
									exchange_unit_floater<FloaterS32>(data, manifest.random_range.left_right);
									exchange_unit_floater<FloaterS32>(data, manifest.random_range.front_back);
								}
							);
						}
					}
				}
			}
			if constexpr (check_version(version, {112, 132})) {
				auto b2 = Boolean{};
				if constexpr (check_version(version, {112, 125})) {
					exchange_unit_bit<IntegerU8>(
						data,
						positioning_override,
						b2,
						// TODO
						positioning_manifest.speaker_panning.enable,
						positioning_manifest.type,
						positioning_manifest.listener_routing.spatialization,
						positioning_manifest.listener_routing.position_source.automation.loop,
						positioning_manifest.listener_routing.position_source.update_at_each_frame,
						positioning_manifest.listener_routing.position_source.hold_listener_orientation
					);
				}
				if constexpr (check_version(version, {125, 132})) {
					exchange_unit_bit<IntegerU8>(
						data,
						positioning_override,
						positioning_manifest.enable,
						b2,
						// TODO
						positioning_manifest.speaker_panning.enable,
						positioning_manifest.type
					);
				}
				if (positioning_manifest.type == Manifest::AudioPositioningSettingType::Constant::three_dimension()) {
					if constexpr (check_version(version, {112, 125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.position_source.mode
						);
					}
					if constexpr (check_version(version, {125, 132})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.position_source.automation.loop,
							positioning_manifest.listener_routing.position_source.update_at_each_frame,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.mode
						);
					}
					if constexpr (check_version(version, {112, 132})) {
						exchange_unit_id(data, positioning_manifest.listener_routing.attenuation.id);
					}
					if (positioning_manifest.listener_routing.position_source.mode == Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) {
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_bit<IntegerU8>(
								data,
								positioning_manifest.listener_routing.position_source.automation.play_type,
								positioning_manifest.listener_routing.position_source.automation.play_mode,
								positioning_manifest.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_routing.position_source.automation.transition_time);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.z);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.duration);
									}
								}
							);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.path,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.count);
									}
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.left_right);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.front_back);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.up_down);
									}
								}
							);
						}
					}
				}
			}
			if constexpr (check_version(version, {132})) {
				if constexpr (check_version(version, {132})) {
					exchange_unit_bit<IntegerU8>(
						data,
						positioning_override,
						positioning_manifest.listener_routing.enable,
						positioning_manifest.speaker_panning.mode,
						k_false,
						positioning_manifest.listener_routing.position_source.mode,
						k_false
					);
				}
				if (positioning_manifest.listener_routing.enable) {
					if constexpr (check_version(version, {132, 134})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.automation.loop
						);
					}
					if constexpr (check_version(version, {134, 140})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.attenuation.enable,
							positioning_manifest.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.automation.loop
						);
					}
					if constexpr (check_version(version, {140})) {
						exchange_unit_bit<IntegerU8>(
							data,
							positioning_manifest.listener_routing.spatialization,
							positioning_manifest.listener_routing.attenuation.enable,
							positioning_manifest.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_manifest.listener_routing.position_source.hold_listener_orientation,
							positioning_manifest.listener_routing.position_source.automation.loop,
							positioning_manifest.listener_routing.position_source.diffraction_and_transmission
						);
					}
					if (positioning_manifest.listener_routing.position_source.mode != Manifest::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::emitter()) {
						if constexpr (check_version(version, {132})) {
							exchange_unit_bit<IntegerU8>(
								data,
								positioning_manifest.listener_routing.position_source.automation.play_type,
								positioning_manifest.listener_routing.position_source.automation.play_mode,
								positioning_manifest.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
						}
						if constexpr (check_version(version, {132})) {
							exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_routing.position_source.automation.transition_time);
						}
						if constexpr (check_version(version, {132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.z);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.duration);
									}
								}
							);
						}
						if constexpr (check_version(version, {132})) {
							exchange_unit_list<IntegerU32>(
								data,
								positioning_manifest.listener_routing.position_source.automation.path,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_integer<IntegerU32>(data, manifest.point.count);
									}
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.left_right);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.front_back);
									}
									if constexpr (check_version(version, {132})) {
										exchange_unit_floater<FloaterS32>(data, manifest.random_range.up_down);
									}
								}
							);
						}
					}
				}
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                     data,
			typename Manifest::MusicMIDISetting & midi_manifest,
			Boolean &                             midi_target_override,
			Boolean &                             midi_clip_tempo_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					k_false,
					midi_clip_tempo_override,
					midi_target_override
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                 data,
			typename Manifest::SoundMIDISetting &             midi_manifest,
			typename Manifest::AudioPlaybackPrioritySetting & playback_priority_manifest,
			Boolean &                                         midi_event_override,
			Boolean &                                         midi_note_tracking_override,
			Boolean &                                         playback_priority_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_priority_override,
					playback_priority_manifest.use_distance_factor,
					midi_event_override,
					midi_note_tracking_override,
					midi_manifest.note_tracking.enable,
					midi_manifest.event.break_on_note_off
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                 data,
			typename Manifest::AudioPlaybackPrioritySetting & playback_priority_manifest,
			Boolean &                                         playback_priority_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_priority_override);
				exchange_unit_bit<IntegerU8>(data, playback_priority_manifest.use_distance_factor);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_priority_override,
					playback_priority_manifest.use_distance_factor
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                              data,
			typename Manifest::AudioPlaybackLimitSetting & playback_limit_manifest,
			Boolean &                                      playback_limit_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_priority_is_equal);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_limit_is_reached);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_override);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                   data,
			typename Manifest::AudioPlaybackLimitSetting &      playback_limit_manifest,
			typename Manifest::AudioBusMuteForBackgroundMusic & mute_for_background_music_manifest,
			Boolean &                                           playback_limit_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_limit_manifest.when_priority_is_equal,
					playback_limit_manifest.when_limit_is_reached,
					playback_limit_override,
					mute_for_background_music_manifest.enable
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                              data,
			typename Manifest::AudioPlaybackLimitSetting & playback_limit_manifest,
			typename Manifest::AudioVirtualVoiceSetting &  virtual_voice_manifest,
			Boolean &                                      playback_limit_override,
			Boolean &                                      virtual_voice_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.on_return_to_physical);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_priority_is_equal);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.when_limit_is_reached);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_manifest.scope);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.behavior);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, playback_limit_override);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_override);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					playback_limit_manifest.when_priority_is_equal,
					playback_limit_manifest.when_limit_is_reached,
					playback_limit_manifest.scope,
					playback_limit_override,
					virtual_voice_override
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.on_return_to_physical);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_integer<IntegerU16>(data, playback_limit_manifest.value.value);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, virtual_voice_manifest.behavior);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                       data,
			typename Manifest::AudioEffectSetting & effect_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU8>(
					data,
					effect_manifest.item,
					[&] (auto & count) {
						if constexpr (check_version(version, {72})) {
							if (count > 0_sz) {
								exchange_unit_bit<IntegerU8>(
									data,
									effect_manifest.bypass.template get<1_ix>(),
									effect_manifest.bypass.template get<2_ix>(),
									effect_manifest.bypass.template get<3_ix>(),
									effect_manifest.bypass.template get<4_ix>(),
									effect_manifest.bypass.template get<5_ix>()
								);
							}
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU8>(data, manifest.index);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							// TODO : in typical, render = 1 -> mode = 0 & u2 = 1, render = 0 -> mode = 1 & u2 = 0
							// TODO : if render, mode value will be changed ?
							exchange_unit_bit<IntegerU8>(data, manifest.use_share_set);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.u1);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                       data,
			typename Manifest::AudioEffectSetting & effect_manifest,
			Boolean &                               effect_override
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, effect_override);
			}
			process_section_sub(data, effect_manifest);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                         data,
			typename Manifest::AudioMetadataSetting & metadata_manifest
		) -> Void {
			if constexpr (check_version(version, {140})) {
				exchange_unit_list<IntegerU8>(
					data,
					metadata_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {140})) {
							exchange_unit_integer<IntegerU8>(data, manifest.index);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_bit<IntegerU8>(data, manifest.use_share_set);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                         data,
			typename Manifest::AudioMetadataSetting & metadata_manifest,
			Boolean &                                 metadata_override
		) -> Void {
			if constexpr (check_version(version, {140})) {
				exchange_unit_bit<IntegerU8>(data, metadata_override);
			}
			process_section_sub(data, metadata_manifest);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                       data,
			typename Manifest::AudioSourceSetting & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.plug_in);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU32>(data, manifest.type);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.resource);
			}
			if constexpr (check_version(version, {72, 113})) {
				exchange_unit_id(data, manifest.source);
			}
			if constexpr (check_version(version, {72, 113})) {
				if (manifest.type != Manifest::AudioSourceType::Constant::streamed()) {
					exchange_unit_integer<IntegerU32>(data, manifest.resource_offset);
				}
			}
			if constexpr (check_version(version, {72, 112})) {
				if (manifest.type != Manifest::AudioSourceType::Constant::streamed()) {
					if constexpr (check_version(version, {72})) {
						exchange_unit_integer<IntegerU32>(data, manifest.resource_size);
					}
				}
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_integer<IntegerU32>(data, manifest.resource_size);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.is_voice);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					manifest.is_voice,
					k_false,
					k_false,
					manifest.non_cachable_stream
				);
			}
			if constexpr (check_version(version, {72})) {
				// TODO
				if ((manifest.plug_in & 0x0000FFFF_i) >= 0x0002_i) {
					exchange_unit_constant(data, 0_iu32);
				}
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                             data,
			List<typename Manifest::AudioSourceSetting> & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section_sub(data, manifest);
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                               data,
			typename Manifest::BusAutomaticDuckingSetting & automatic_ducking_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU32>(data, automatic_ducking_manifest.recovery_time);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, automatic_ducking_manifest.maximum_ducking_volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					automatic_ducking_manifest.bus,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS32>(data, manifest.volume);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_out);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_in);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.curve);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.target);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                          data,
			typename Manifest::AudioBusConfiguration & bus_configuration_manifest
		) -> Void {
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerU32>(data, bus_configuration_manifest.u1);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                  data,
			typename Manifest::BusHDRSetting & hdr_manifest
		) -> Void {
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_manifest.enable
				);
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_manifest.dynamic.release_mode
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					hdr_manifest.enable,
					hdr_manifest.dynamic.release_mode
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                     data,
			typename Manifest::AudioTimeSetting & time_setting_manifest,
			Boolean &                             time_setting_override
		) -> Void {
			if constexpr (check_version(version, {72})) {
				// TODO : test frequency mode-preset time and offset
				// NOTE : time = 960000 * signature / tempo, then with frequency mode-preset
				exchange_unit_floater<FloaterS64>(data, time_setting_manifest.time);
			}
			if constexpr (check_version(version, {72})) {
				// NOTE : 0 if mode.no, millisecond if mode.custom, else by mode-preset
				exchange_unit_floater<FloaterS64>(data, time_setting_manifest.offset);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, time_setting_manifest.tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.first);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.second);
			}
			if constexpr (check_version(version, {72, 140})) {
				auto b2 = Boolean{};
				auto b3 = Boolean{};
				auto b4 = Boolean{};
				auto b5 = Boolean{};
				auto b6 = Boolean{};
				auto b7 = Boolean{};
				auto b8 = Boolean{};
				exchange_unit_bit<IntegerU8>(data, time_setting_override, b2, b3, b4, b5, b6, b7, b8);
				assert_test(b2 == b3 && b3 == b4 && b4 == b5 && b5 == b6 && b6 == b7 && b7 == b8);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_bit<IntegerU8>(data, time_setting_override);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                   data,
			typename Manifest::MusicTrackClip & clip_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					clip_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.source);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_id(data, manifest.event);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.offset);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.begin);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.end);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.duration);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				if (!clip_manifest.item.empty()) {
					exchange_unit_integer<IntegerU32>(data, clip_manifest.u1);
				}
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					clip_manifest.curve,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.index);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU32>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                        data,
			typename Manifest::MusicStingerSetting & stinger_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					stinger_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.trigger);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.segment_to_play);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.play_at);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.cue_name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.do_not_play_this_stinger_again_for);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.allow_playing_stinger_in_next_segment);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                           data,
			typename Manifest::MusicTransitionSetting & transition_manifest
		) -> Void {
			constexpr auto process_section_sub_of_fade =
				[] (
				IByteStreamView &                                   data,
				typename Manifest::MusicTransitionSettingItemFade & manifest
			) {
				if constexpr (check_version(version, {72})) {
					exchange_unit_integer<IntegerU32>(data, manifest.time);
				}
				if constexpr (check_version(version, {72})) {
					exchange_unit_integer<IntegerU32>(data, manifest.curve);
				}
				if constexpr (check_version(version, {72})) {
					exchange_unit_integer<IntegerS32>(data, manifest.offset);
				}
				return;
			};
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					transition_manifest.item,
					[] (auto & count) {
					},
					// NOTE : avoid clang bug
					[&process_section_sub_of_fade] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {88})) {
							exchange_unit_constant(data, 1_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.source.id);
						}
						if constexpr (check_version(version, {88})) {
							exchange_unit_constant(data, 1_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.destination.id);
						}
						if constexpr (check_version(version, {72})) {
							process_section_sub_of_fade(data, manifest.source.fade_out);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.source.exit_source_at);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.source.exit_source_at_custom_cue_match);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_unit_bit<IntegerU8, k_true>(data, manifest.source.play_post_exit);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_bit<IntegerU8>(data, manifest.source.play_post_exit);
						}
						if constexpr (check_version(version, {72})) {
							process_section_sub_of_fade(data, manifest.destination.fade_in);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.destination.custom_cue_filter_match_target);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u1);
						}
						if constexpr (check_version(version, {134})) {
							exchange_unit_bit<IntegerU16>(data, manifest.destination.jump_to);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU16>(data, manifest.destination.synchronize_to);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_unit_bit<IntegerU8, k_true>(data, manifest.destination.play_pre_entry);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_bit<IntegerU8>(data, manifest.destination.play_pre_entry);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.destination.custom_cue_filter_match_source_cue_name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.segment.enable);
						}
						if constexpr (check_version(version, {72})) {
							auto has_segment_data = Boolean{};
							if constexpr (check_version(version, {72, 88})) {
								has_segment_data = k_true;
							}
							if constexpr (check_version(version, {88})) {
								has_segment_data = manifest.segment.enable;
							}
							if (has_segment_data) {
								if constexpr (check_version(version, {72})) {
									exchange_unit_id(data, manifest.segment.id);
								}
								if constexpr (check_version(version, {72})) {
									process_section_sub_of_fade(data, manifest.segment.fade_in);
								}
								if constexpr (check_version(version, {72})) {
									process_section_sub_of_fade(data, manifest.segment.fade_out);
								}
								if constexpr (check_version(version, {72, 140})) {
									exchange_unit_bit<IntegerU8, k_true>(data, manifest.segment.play_pre_entry);
								}
								if constexpr (check_version(version, {140})) {
									exchange_unit_bit<IntegerU8>(data, manifest.segment.play_pre_entry);
								}
								if constexpr (check_version(version, {72, 140})) {
									exchange_unit_bit<IntegerU8, k_true>(data, manifest.segment.play_post_exit);
								}
								if constexpr (check_version(version, {140})) {
									exchange_unit_bit<IntegerU8>(data, manifest.segment.play_pre_entry);
								}
							}
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                data,
			typename Manifest::MusicTrackTransitionSetting & transition_manifest
		) -> Void {
			constexpr auto process_section_sub_of_fade =
				[] (
				IByteStreamView &                                   data,
				typename Manifest::MusicTransitionSettingItemFade & manifest
			) {
				if constexpr (check_version(version, {112})) {
					exchange_unit_integer<IntegerU32>(data, manifest.time);
				}
				if constexpr (check_version(version, {112})) {
					exchange_unit_integer<IntegerU32>(data, manifest.curve);
				}
				if constexpr (check_version(version, {112})) {
					exchange_unit_integer<IntegerS32>(data, manifest.offset);
				}
				return;
			};
			if constexpr (check_version(version, {112})) {
				exchange_unit_constant(data, 1_iu32);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_id(data, transition_manifest.switcher);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub_of_fade(data, transition_manifest.source.fade_out);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU32>(data, transition_manifest.source.exit_source_at);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_id(data, transition_manifest.source.exit_source_at_custom_cue_match);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub_of_fade(data, transition_manifest.destination.fade_in);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                         data,
			typename Manifest::AudioSwitcherSetting & switcher_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU32>(data, switcher_manifest.is_state);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, switcher_manifest.is_state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, switcher_manifest.group);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, switcher_manifest.default_item);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                            data,
			typename Manifest::AudioAssociationSetting & association_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					association_manifest.argument,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {88})) {
							exchange_unit_bit<IntegerU8>(data, manifest.is_state);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					association_manifest.path,
					[&] (auto & count) {
						// NOTE : here
						assert_test(count % 12_sz == 0_sz);
						count /= 12_sz;
						if constexpr (check_version(version, {72, 88})) {
							exchange_unit_integer<IntegerU8>(data, association_manifest.probability);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(
								data,
								association_manifest.mode
							);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.object);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.weight);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.probability);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                data,
			typename Manifest::SoundPlaylistContainerScope & scope_manifest,
			typename Manifest::AudioPlayType &               play_type_manifest,
			typename Manifest::AudioPlayTypeSetting &        play_type_setting_manifest,
			typename Manifest::AudioPlayMode &               play_mode_manifest,
			typename Manifest::AudioPlayModeSetting &        play_mode_setting_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop.value);
			}
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop.minimum_value);
			}
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop.maximum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, play_mode_setting_manifest.continuous.transition_duration.value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, play_mode_setting_manifest.continuous.transition_duration.minimum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, play_mode_setting_manifest.continuous.transition_duration.maximum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU16>(data, play_type_setting_manifest.random.avoid_repeat);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_setting_manifest.continuous.transition_type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_type_setting_manifest.random.type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_type_manifest);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, k_false);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_setting_manifest.continuous.always_reset_playlist);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, play_type_setting_manifest.sequence.at_end_of_playlist);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_manifest);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU8>(data, scope_manifest);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(
					data,
					k_false,
					play_mode_setting_manifest.continuous.always_reset_playlist,
					play_type_setting_manifest.sequence.at_end_of_playlist,
					play_mode_manifest,
					scope_manifest
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                             data,
			List<typename Manifest::SoundPlaylistContainerPlaylistItem> & playlist_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU16>(
					data,
					playlist_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.weight);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                                  data,
			List<typename Manifest::SoundSwitchContainerObjectAttributeItem> & object_attribute_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					object_attribute_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.play_first_only);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.continue_to_play_across_switch);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.play_first_only, manifest.continue_to_play_across_switch);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_integer<IntegerU32>(data, manifest.u1);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_integer<IntegerU8>(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_out_time);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.fade_in_time);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                               data,
			List<typename Manifest::SoundSwitchContainerObjectAssignItem> & assigned_object_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					assigned_object_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU32>(
								data,
								manifest.object,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_id(data, manifest);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                       data,
			List<typename Manifest::SoundBlendContainerTrackItem> & track_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					track_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							process_section_sub(data, manifest.real_time_parameter_control);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.cross_fade.id);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.cross_fade.category);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU32>(
								data,
								manifest.child,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_id(data, manifest.id);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_list<IntegerU32>(
											data,
											manifest.point,
											[] (auto & count) {
											},
											[] (auto & data, auto & manifest) {
												if constexpr (check_version(version, {72})) {
													exchange_unit_floater<FloaterS32>(data, manifest.position.x);
												}
												if constexpr (check_version(version, {72})) {
													exchange_unit_floater<FloaterS32>(data, manifest.position.y);
												}
												if constexpr (check_version(version, {72})) {
													exchange_unit_bit<IntegerU32>(data, manifest.curve);
												}
											}
										);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                        data,
			typename Manifest::MusicTrackTrackType & track_type_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_unit_bit<IntegerU32>(data, track_type_manifest);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                data,
			typename Manifest::MusicTrackTrackType &         track_type_manifest,
			typename Manifest::AudioSwitcherSetting &        switcher_manifest,
			typename Manifest::MusicTrackTransitionSetting & transition_manifest
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, track_type_manifest);
			}
			if constexpr (check_version(version, {112})) {
				if (track_type_manifest == Manifest::MusicTrackTrackType::Constant::switcher()) {
					if constexpr (check_version(version, {112})) {
						process_section_sub(data, switcher_manifest);
					}
					if constexpr (check_version(version, {112})) {
						process_section_sub(data, transition_manifest);
					}
				}
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                     data,
			typename Manifest::MusicTrackStream & stream_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU16>(data, stream_manifest.look_ahead_time);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                    data,
			typename Manifest::MusicSegmentCue & cue_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					cue_manifest.item,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS64>(data, manifest.time);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_unit_constant(data, 0_iu32);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_constant(data, 0_iu8);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                             data,
			List<typename Manifest::MusicPlaylistContainerPlaylistItem> & playlist_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					playlist_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.child_count);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32, k_true>(data, manifest.play_mode, manifest.play_type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.loop);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_constant(data, 0_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.weight);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU16>(data, manifest.random_setting.avoid_repeat);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.group); // TODO : maybe
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.random_setting.type);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                              data,
			List<typename Manifest::MusicSwitchContainerAssociationItem> & association_manifest
		) -> Void {
			if constexpr (check_version(version, {72, 88})) {
				exchange_unit_list<IntegerU32>(
					data,
					association_manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 88})) {
							exchange_unit_id(data, manifest.item);
						}
						if constexpr (check_version(version, {72, 88})) {
							exchange_unit_id(data, manifest.child);
						}
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                  data,
			typename Manifest::AudioPlayMode & play_mode_manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, play_mode_manifest);
			}
			return;
		}

		// ----------------

		static auto process_section (
			IByteStreamView &               data,
			typename Manifest::StateGroup & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU32>(data, manifest.default_transition);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.custom_transition,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.from);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.to);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_integer<IntegerU32>(data, manifest.time);
						}
					}
				);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::SwitchGroup & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parameter.id);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.parameter.category);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.point,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_floater<FloaterS32>(data, manifest.position.x);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.position.y);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU32>(data, manifest.curve);
						}
					}
				);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                  data,
			typename Manifest::GameParameter & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, manifest.range_default);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU32>(data, manifest.interpolation_mode);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_floater<FloaterS32>(data, manifest.interpolation_attack);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_floater<FloaterS32>(data, manifest.interpolation_release);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.bind_to_built_in_parameter);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                          data,
			typename Manifest::GameSynchronizationU1 & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u1);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u2);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u3);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u4);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u5);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_floater<FloaterS32>(data, manifest.u6);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                            data,
			typename Manifest::StatefulPropertySetting & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72, 128})) {
				exchange_unit_list<IntegerU8>(
					data,
					manifest.value,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 128})) {
							exchange_unit_integer<IntegerU8>(data, manifest.type);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72, 128})) {
							exchange_unit_floater<FloaterS32>(data, manifest.value);
						}
					}
				);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_list<IntegerU16>(
					data,
					manifest.value,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {128})) {
							exchange_unit_integer<IntegerU16>(data, manifest.type);
						}
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {128})) {
							exchange_unit_floater<FloaterS32>(data, manifest.value);
						}
					}
				);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::EventAction & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			// NOTE : here
			auto type = Enumerated{};
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, manifest.scope, manifest.mode);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_enumerated<IntegerU8>(data, type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.target);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU8>(data, manifest.u1);
			}
			auto process_section_sub_of_exception_list =
				[&] (
			) {
				if constexpr (check_version(version, {72, 125})) {
					exchange_unit_list<IntegerU32>(
						data,
						manifest.exception,
						[] (auto & size) {
						},
						[] (auto & data, auto & manifest) {
							if constexpr (check_version(version, {72, 125})) {
								exchange_unit_id(data, manifest.id);
							}
							if constexpr (check_version(version, {72, 125})) {
								exchange_unit_bit<IntegerU8>(data, manifest.u1);
							}
						}
					);
				}
				if constexpr (check_version(version, {125})) {
					exchange_unit_list<IntegerU8>(
						data,
						manifest.exception,
						[] (auto & size) {
						},
						[] (auto & data, auto & manifest) {
							if constexpr (check_version(version, {125})) {
								exchange_unit_id(data, manifest.id);
							}
							if constexpr (check_version(version, {125})) {
								exchange_unit_bit<IntegerU8>(data, manifest.u1);
							}
						}
					);
				}
			};
			auto has_case = k_false;
			if constexpr (check_version(version, {72})) {
				if (type == 4_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::play_audio()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property<CPTC::probability()>(common_property, property_manifest.probability);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.sound_bank);
					}
					if constexpr (check_version(version, {145})) {
						exchange_unit_constant(data, 0_iu32);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 1_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::stop_audio()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							k_false,
							property_manifest.resume_state_transition,
							property_manifest.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 2_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::pause_audio()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72, 125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.include_delayed_resume_action
						);
					}
					if constexpr (check_version(version, {125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.include_delayed_resume_action,
							property_manifest.resume_state_transition,
							property_manifest.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 3_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::resume_audio()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72, 125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.master_resume
						);
					}
					if constexpr (check_version(version, {125})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.master_resume,
							property_manifest.resume_state_transition,
							property_manifest.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 28_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::break_audio()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 30_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::seek_audio()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.seek_type);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.seek_value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.seek_value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.seek_value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.seek_to_nearest_marker
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {113})) {
				if (type == 33_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::post_event()>();
					if constexpr (check_version(version, {113})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {113})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 8_e || type == 9_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_pitch()>();
					property_manifest.reset = type == 9_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 10_e || type == 11_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_volume()>();
					property_manifest.reset = type == 11_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 12_e || type == 13_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_bus_volume()>();
					property_manifest.reset = type == 13_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 14_e || type == 15_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_low_pass_filter()>();
					property_manifest.reset = type == 15_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {112})) {
				if (type == 32_e || type == 48_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_high_pass_filter()>();
					property_manifest.reset = type == 48_e;
					if constexpr (check_version(version, {112})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {112})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {112})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {112})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 6_e || type == 7_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_mute()>();
					property_manifest.reset = type == 7_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 19_e || type == 20_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_game_parameter()>();
					property_manifest.reset = type == 20_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_manifest.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.fade_curve);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.bypass_game_parameter_interpolation);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, property_manifest.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 16_e || type == 17_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_state_availability()>();
					property_manifest.enable = type == 16_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 18_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::activate_state()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.group);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.item);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 25_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::activate_switch()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.group);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_id(data, property_manifest.item);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 29_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::activate_trigger()>();
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (type == 26_e || type == 27_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_bypass_effect()>();
					property_manifest.reset = type == 27_e;
					if constexpr (check_version(version, {72})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(data, property_manifest.enable);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_bit<IntegerU8>(
							data,
							property_manifest.value.template get<1_ix>(),
							property_manifest.value.template get<2_ix>(),
							property_manifest.value.template get<3_ix>(),
							property_manifest.value.template get<4_ix>(),
							property_manifest.value.template get<5_ix>(),
							as_constant(property_manifest.reset),
							as_constant(property_manifest.reset),
							as_constant(property_manifest.reset)
						);
					}
					if constexpr (check_version(version, {72})) {
						process_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {112})) {
				if (type == 31_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::release_envelope()>();
					if constexpr (check_version(version, {112})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {112})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {113})) {
				if (type == 34_e) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::reset_playlist()>();
					if constexpr (check_version(version, {113})) {
						process_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {113})) {
									exchange_common_property_as_randomizable<CPTC::delay()>(common_property, property_manifest.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {113})) {
						exchange_unit_constant(data, 4_iu8); // TODO : maybe fade curve ?
					}
					if constexpr (check_version(version, {113, 115})) {
						exchange_unit_constant(data, 0_iu32);
					}
					if constexpr (check_version(version, {115})) {
						exchange_unit_constant(data, 0_iu8);
					}
					has_case = k_true;
				}
			}
			assert_test(has_case);
			return;
		}

		static auto process_section (
			IByteStreamView &          data,
			typename Manifest::Event & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72, 125})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {125})) {
				process_section_sub<IntegerU8>(data, manifest.child);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                  data,
			typename Manifest::DialogueEvent & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {88})) {
				exchange_unit_integer<IntegerU8>(data, manifest.probability);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.association);
			}
			if constexpr (check_version(version, {120})) {
				exchange_unit_constant(data, 0_iu16);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::Attenuation & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {140})) {
				exchange_unit_bit<IntegerU8>(data, manifest.height_spread);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, manifest.cone.enable);
				if (manifest.cone.enable) {
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.inner_angle);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.outer_angle);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.low_pass_filter);
					}
					if constexpr (check_version(version, {112})) {
						exchange_unit_floater<FloaterS32>(data, manifest.cone.high_pass_filter);
					}
				}
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.spread);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.game_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.user_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.spread);
			}
			if constexpr (check_version(version, {112, 145})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.game_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.user_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.spread);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.focus);
			}
			if constexpr (check_version(version, {145})) {
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_output_bus_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_game_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_user_defined_auxiliary_send_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_spread);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.distance_focus);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.obstruction_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.obstruction_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.obstruction_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.occlusion_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.occlusion_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.occlusion_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.diffraction_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.diffraction_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.diffraction_high_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.transmission_volume);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.transmission_low_pass_filter);
				exchange_unit_integer<IntegerU8>(data, manifest.apply.transmission_high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU8>(
					data,
					manifest.curve,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                                    data,
			typename Manifest::LowFrequencyOscillatorModulator & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::depth()>(common_property, manifest.depth);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::frequency()>(common_property, manifest.frequency);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::waveform()>(common_property, manifest.waveform);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::smoothing()>(common_property, manifest.smoothing);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::pulse_width_modulation()>(common_property, manifest.pulse_width_modulation);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::attack()>(common_property, manifest.attack);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::initial_phase_offset()>(common_property, manifest.initial_phase_offset);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                      data,
			typename Manifest::EnvelopeModulator & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::attack_time()>(common_property, manifest.attack_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::attack_curve()>(common_property, manifest.attack_curve);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::decay_time()>(common_property, manifest.decay_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::sustain_level()>(common_property, manifest.sustain_level);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::release_time()>(common_property, manifest.release_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_randomizable<CPTC::sustain_time()>(common_property, manifest.sustain_time);
						}
						if constexpr (check_version(version, {112})) {
							exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_after_release);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                  data,
			typename Manifest::TimeModulator & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {132})) {
				process_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_randomizable<CPTC::initial_delay()>(common_property, manifest.initial_delay);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_regular<CPTC::duration()>(common_property, manifest.duration);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_randomizable<CPTC::loop()>(common_property, manifest.loop);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property_as_randomizable<CPTC::playback_rate()>(common_property, manifest.playback_rate);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
						}
						if constexpr (check_version(version, {132})) {
							exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_at_end);
						}
					}
				);
			}
			if constexpr (check_version(version, {132})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &           data,
			typename Manifest::Effect & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.plug_in);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_data<IntegerU32>(data, manifest.expand);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_constant(data, 0_iu8);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {125, 128})) {
				exchange_unit_constant(data, 0_iu16);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.u1);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &           data,
			typename Manifest::Source & manifest
		) -> Void {
			return process_section(data, self_cast<typename Manifest::Effect>(manifest));
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::AudioDevice & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {128})) {
				exchange_unit_id(data, manifest.plug_in);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_data<IntegerU32>(data, manifest.expand);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_constant(data, 0_iu8);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {128})) {
				process_section_sub(data, manifest.u1);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.effect);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &             data,
			typename Manifest::AudioBus & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {128})) {
				if (manifest.parent == 0_i) {
					if constexpr (check_version(version, {128})) {
						exchange_unit_id(data, manifest.audio_device);
					}
				}
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_false,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {125})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.bus);
						}
						if constexpr (check_version(version, {128})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {125})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
					}
				);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_unit_bit<IntegerU8>(data, manifest.override_positioning);
				exchange_unit_bit<IntegerU8>(data, manifest.positioning.speaker_panning.enable);
			}
			if constexpr (check_version(version, {112, 125})) {
				exchange_unit_bit<IntegerU8>(data, manifest.override_positioning, manifest.positioning.speaker_panning.enable);
			}
			if constexpr (check_version(version, {125})) {
				// NOTE : here
				auto override_positioning = Boolean{};
				process_section_sub(data, manifest.positioning, override_positioning);
				assert_test(override_positioning);
			}
			if constexpr (check_version(version, {125, 135})) {
				// NOTE : here
				auto override_game_defined_auxiliary_send = Boolean{};
				auto override_user_defined_auxiliary_send = Boolean{};
				process_section_sub(data, manifest.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send);
				assert_test(override_game_defined_auxiliary_send);
				assert_test(override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				// NOTE : here
				auto override_game_defined_auxiliary_send = Boolean{};
				auto override_user_defined_auxiliary_send = Boolean{};
				auto override_early_reflection_auxiliary_send = Boolean{};
				process_section_sub(data, manifest.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send, override_early_reflection_auxiliary_send);
				assert_test(override_game_defined_auxiliary_send);
				assert_test(override_user_defined_auxiliary_send);
				assert_test(override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_limit, manifest.override_playback_limit);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.playback_limit, manifest.mute_for_background_music, manifest.override_playback_limit);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.bus_configuration);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.hdr);
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_unit_constant(data, 63_iu32);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.automatic_ducking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_id(data, manifest.mixer);
			}
			if constexpr (check_version(version, {112})) {
				exchange_unit_constant(data, 0_iu16);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                      data,
			typename Manifest::AuxiliaryAudioBus & manifest
		) -> Void {
			return process_section(data, self_cast<typename Manifest::AudioBus>(manifest));
		}

		static auto process_section (
			IByteStreamView &          data,
			typename Manifest::Sound & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.source);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							exchange_common_property_as_randomizable<CPTC::playback_loop()>(common_property, manifest.playback_setting.loop);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                           data,
			typename Manifest::SoundPlaylistContainer & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.scope, manifest.playback_setting.type, manifest.playback_setting.type_setting, manifest.playback_setting.mode, manifest.playback_setting.mode_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.playlist);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                         data,
			typename Manifest::SoundSwitchContainer & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.switcher);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.mode);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.object_assign);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.object_attribute);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                        data,
			typename Manifest::SoundBlendContainer & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							exchange_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, manifest.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.track);
			}
			if constexpr (check_version(version, {120})) {
				process_section_sub(data, manifest.playback_setting.mode);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &               data,
			typename Manifest::ActorMixer & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &               data,
			typename Manifest::MusicTrack & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.source);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.clip);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72, 112})) {
				process_section_sub(data, manifest.playback_setting.type);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.playback_setting.type, manifest.playback_setting.switcher, manifest.playback_setting.transition);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stream);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_constant(data, 0_iu16);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                 data,
			typename Manifest::MusicSegment & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stinger);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS64>(data, manifest.playback_setting.duration);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.cue);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                           data,
			typename Manifest::MusicPlaylistContainer & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stinger);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.transition);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.playlist);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                         data,
			typename Manifest::MusicSwitchContainer & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.effect, manifest.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				process_section_sub(data, manifest.metadata, manifest.override_metadata);
			}
			if constexpr (check_version(version, {112})) {
				process_section_sub(data, manifest.mixer, manifest.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.parent);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.voice);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.positioning);
						}
						if constexpr (check_version(version, {88})) {
							process_common_property(common_property, manifest.hdr);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.midi);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							process_common_property(common_property, manifest.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							process_common_property(common_property, manifest.motion);
						}
						if constexpr (check_version(version, {112})) {
							process_common_property(common_property, manifest.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.positioning, manifest.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.state);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub<IntegerU32>(data, manifest.child);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.stinger);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.transition);
			}
			if constexpr (check_version(version, {72, 88})) {
				process_section_sub(data, manifest.playback_setting.switcher);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_bit<IntegerU8>(data, manifest.playback_setting.continue_playing_on_switch_change);
			}
			if constexpr (check_version(version, {72})) {
				process_section_sub(data, manifest.playback_setting.association);
			}
			return;
		}

		// ----------------

		static auto process_chunk_bkhd (
			IByteStreamView &              data,
			typename Manifest::SoundBank & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				data.read_constant(cbw<VersionNumber>(version.number));
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_id(data, manifest.id);
			}
			if constexpr (check_version(version, {72, 125})) {
				exchange_unit_integer<IntegerU32>(data, manifest.language);
			}
			if constexpr (check_version(version, {125})) {
				exchange_unit_id(data, manifest.language);
			}
			if constexpr (check_version(version, {72})) {
				// NOTE : here
				manifest.header_expand = data.forward_view(data.reserve());
			}
			return;
		}

		static auto process_chunk_didx_data (
			IByteStreamView &             didx_data,
			IByteStreamView &             data_data,
			List<typename Manifest::ID> & manifest,
			Optional<Path> const &        embedded_media_directory
		) -> Void {
			if constexpr (check_version(version, {72})) {
				// NOTE : here
				constexpr auto didx_item_structure_size = bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>();
				assert_test(is_padded_size(didx_data.reserve(), didx_item_structure_size));
				manifest.allocate_full(didx_data.reserve() / didx_item_structure_size);
				exchange_unit_list<Void>(
					didx_data,
					manifest,
					[] (auto & count) {
					},
					[&] (auto & data, auto & manifest) {
						auto data_offset = Integer{};
						auto data_size = Integer{};
						exchange_unit_id(data, manifest);
						exchange_unit_integer<IntegerU32>(data, data_offset);
						exchange_unit_integer<IntegerU32>(data, data_size);
						if (manifest == 0_i) {
							assert_test(data_offset == 0_i);
							assert_test(data_size == 0_i);
						} else {
							auto item_data = data_data.sub_view(cbw<Size>(data_offset), cbw<Size>(data_size));
							if (embedded_media_directory) {
								FileSystem::write_file(embedded_media_directory.get() / "{}.wem"_sf(manifest), item_data);
							}
						}
					}
				);
				data_data.forward_to_end();
			}
			return;
		}

		static auto process_chunk_init (
			IByteStreamView &            data,
			typename Manifest::Setting & manifest
		) -> Void {
			if constexpr (check_version(version, {118})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.plug_in,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {118})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {118, 140})) {
							exchange_unit_string<IntegerU32, k_true>(data, manifest.library);
						}
						if constexpr (check_version(version, {140})) {
							exchange_unit_string<Void>(data, manifest.library);
						}
					}
				);
			}
			return;
		}

		static auto process_chunk_stmg (
			IByteStreamView &                        data,
			typename Manifest::Setting &             manifest,
			typename Manifest::GameSynchronization & game_synchronization_manifest
		) -> Void {
			if constexpr (check_version(version, {145})) {
				exchange_unit_bit<IntegerU16>(data, manifest.voice_filter_behavior);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_floater<FloaterS32>(data, manifest.volume_threshold);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_integer<IntegerU16>(data, manifest.maximum_voice_instance);
			}
			if constexpr (check_version(version, {128})) {
				exchange_unit_constant(data, 50_iu16);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.state_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.switch_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.game_parameter,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			if constexpr (check_version(version, {120, 125})) {
				exchange_unit_constant(data, 0_iu32);
				exchange_unit_constant(data, 0_iu32);
			}
			if constexpr (check_version(version, {125, 140})) {
				exchange_unit_constant(data, 0_iu32);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.u1,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
			}
			return;
		}

		static auto process_chunk_hirc (
			IByteStreamView &                    data,
			List<typename Manifest::Hierarchy> & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						// NOTE : here
						auto type = Enumerated{};
						auto item_size = Integer{};
						exchange_unit_enumerated<IntegerU8>(data, type);
						exchange_unit_integer<IntegerU32>(data, item_size);
						auto item_data = IByteStreamView{data.forward_view(cbw<Size>(item_size))};
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<typename Manifest::HierarchyType>::Index>(
							[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
								constexpr auto variant_type = mbw<typename Manifest::HierarchyType>(index);
								if constexpr (variant_type != Manifest::HierarchyType::Constant::unknown()) {
									if (type == value_index) {
										process_section(item_data, manifest.template set_of_type<variant_type>());
										has_case = k_true;
									}
								}
							}
						);
						if (!has_case) {
							auto & unknown_manifest = manifest.template set_of_type<Manifest::HierarchyType::Constant::unknown()>();
							unknown_manifest.type = cbw<Integer>(type);
							unknown_manifest.data = item_data.forward_view(item_data.reserve());
							M_log("warning : unknown hierarchy type {}"_sf(type));
						}
						assert_test(item_data.full());
					}
				);
			}
			return;
		}

		static auto process_chunk_stid (
			IByteStreamView &                             data,
			List<typename Manifest::SoundBankReference> & manifest
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_unit_constant(data, 1_iu32);
			}
			if constexpr (check_version(version, {72})) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						if constexpr (check_version(version, {72})) {
							exchange_unit_id(data, manifest.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_unit_string<IntegerU8>(data, manifest.name);
						}
					}
				);
			}
			return;
		}

		static auto process_chunk_envs (
			IByteStreamView &            data,
			typename Manifest::Setting & manifest
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				// NOTE : here
				exchange_unit_list<Void>(
					data,
					as_lvalue(
						make_list<VWrapperView<typename Manifest::ObstructionSetting>>(
							manifest.obstruction.volume,
							manifest.obstruction.low_pass_filter,
							manifest.occlusion.volume,
							manifest.occlusion.low_pass_filter
						)
					),
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest_view) {
						auto & manifest = manifest_view.get();
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.enable);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {72, 112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {72, 112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {72, 112})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				// NOTE : here
				exchange_unit_list<Void>(
					data,
					as_lvalue(
						make_list<VWrapperView<typename Manifest::ObstructionSetting>>(
							manifest.obstruction.volume,
							manifest.obstruction.low_pass_filter,
							manifest.obstruction.high_pass_filter,
							manifest.occlusion.volume,
							manifest.occlusion.low_pass_filter,
							manifest.occlusion.high_pass_filter
						)
					),
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest_view) {
						auto & manifest = manifest_view.get();
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.enable);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_bit<IntegerU8>(data, manifest.mode);
						}
						if constexpr (check_version(version, {112})) {
							exchange_unit_list<IntegerU16>(
								data,
								manifest.point,
								[] (auto & count) {
								},
								[] (auto & data, auto & manifest) {
									if constexpr (check_version(version, {112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.x);
									}
									if constexpr (check_version(version, {112})) {
										exchange_unit_floater<FloaterS32>(data, manifest.position.y);
									}
									if constexpr (check_version(version, {112})) {
										exchange_unit_bit<IntegerU32>(data, manifest.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		static auto process_chunk_plat (
			IByteStreamView &            data,
			typename Manifest::Setting & manifest
		) -> Void {
			if constexpr (check_version(version, {113, 118})) {
				exchange_unit_string<IntegerU32>(data, manifest.platform);
			}
			if constexpr (check_version(version, {118, 140})) {
				exchange_unit_string<IntegerU32, k_true>(data, manifest.platform);
			}
			if constexpr (check_version(version, {140})) {
				exchange_unit_string<Void>(data, manifest.platform);
			}
			return;
		}

		// ----------------

		static auto process_sound_bank (
			IByteStreamView &              sound_bank_data,
			typename Manifest::SoundBank & sound_bank_manifest,
			Optional<Path> const &         embedded_media_directory
		) -> Void {
			auto state = k_false;
			auto sign = ChunkSign{};
			auto chunk = IByteStreamView{};
			auto next_chunk =
				[&] (
			) -> auto {
				assert_test(chunk.full());
				state = !sound_bank_data.full();
				if (state) {
					sign = sound_bank_data.read_of<ChunkSign>();
					chunk = IByteStreamView{sound_bank_data.forward_view(cbw<Size>(sign.size))};
				}
				return;
			};
			next_chunk();
			if constexpr (check_version(version, {72})) {
				assert_test(state && sign.id == ChunkSignFlag::bkhd);
				{
					process_chunk_bkhd(chunk, sound_bank_manifest);
					next_chunk();
				}
			}
			if constexpr (check_version(version, {72})) {
				if (state && sign.id == ChunkSignFlag::didx) {
					auto didx_data = chunk;
					chunk.forward_to_end();
					next_chunk();
					assert_test(state && sign.id == ChunkSignFlag::data);
					auto data_data = chunk;
					chunk.forward_to_end();
					next_chunk();
					process_chunk_didx_data(didx_data, data_data, sound_bank_manifest.embedded_media, embedded_media_directory);
				}
			}
			if constexpr (check_version(version, {118})) {
				if (state && sign.id == ChunkSignFlag::init) {
					if (!sound_bank_manifest.setting.has()) {
						sound_bank_manifest.setting.set();
					}
					process_chunk_init(chunk, sound_bank_manifest.setting.get());
					next_chunk();
				}
			}
			if constexpr (check_version(version, {72})) {
				if (state && sign.id == ChunkSignFlag::stmg) {
					if (!sound_bank_manifest.setting.has()) {
						sound_bank_manifest.setting.set();
					}
					if (!sound_bank_manifest.game_synchronization.has()) {
						sound_bank_manifest.game_synchronization.set();
					}
					process_chunk_stmg(chunk, sound_bank_manifest.setting.get(), sound_bank_manifest.game_synchronization.get());
					next_chunk();
				}
			}
			if constexpr (check_version(version, {72})) {
				if (state && sign.id == ChunkSignFlag::hirc) {
					process_chunk_hirc(chunk, sound_bank_manifest.hierarchy);
					next_chunk();
				}
			}
			if constexpr (check_version(version, {72})) {
				if (state && sign.id == ChunkSignFlag::stid) {
					process_chunk_stid(chunk, sound_bank_manifest.reference);
					next_chunk();
				}
			}
			if constexpr (check_version(version, {72})) {
				if (state && sign.id == ChunkSignFlag::envs) {
					if (!sound_bank_manifest.setting.has()) {
						sound_bank_manifest.setting.set();
					}
					process_chunk_envs(chunk, sound_bank_manifest.setting.get());
					next_chunk();
				}
			}
			if constexpr (check_version(version, {113})) {
				if (state && sign.id == ChunkSignFlag::plat) {
					if (!sound_bank_manifest.setting.has()) {
						sound_bank_manifest.setting.set();
					}
					process_chunk_plat(chunk, sound_bank_manifest.setting.get());
					next_chunk();
				}
			}
			assert_test(sound_bank_manifest.setting.has() == sound_bank_manifest.game_synchronization.has());
			assert_test(!state);
			return;
		}

	public:

		static auto do_process_sound_bank (
			IByteStreamView &              sound_bank_data_,
			typename Manifest::SoundBank & sound_bank_manifest,
			Optional<Path> const &         embedded_media_directory
		) -> Void {
			M_use_zps_of(sound_bank_data);
			restruct(sound_bank_manifest);
			return process_sound_bank(sound_bank_data, sound_bank_manifest, embedded_media_directory);
		}

	};

}
