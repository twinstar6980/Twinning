module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.wwise.sound_bank.common;
import twinning.kernel.utility;
import twinning.kernel.tool.wwise.sound_bank.version;
import twinning.kernel.tool.wwise.sound_bank.definition;
import twinning.kernel.tool.common.wave_structure;

export namespace Twinning::Kernel::Tool::Wwise::SoundBank {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

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

			typename Definition::ID value{};

		};

		// ----------------

		using CommonPropertyValue = Variant<Boolean, Integer, Floater, Enumerated, IDWrapper>;

		template <typename Type> requires
			CategoryConstraint<>
			&& (IsPureInstance<Type>)
		struct CommonPropertyMap {

			Map<Type, Tuple<CommonPropertyValue>> regular{};

			Map<Type, Tuple<CommonPropertyValue, CommonPropertyValue>> randomizer{};

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
				if constexpr (check_version(version, {118, 150})) {
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
				if constexpr (check_version(version, {150})) {
					using Type = TypePackage<
						// delay
						TypePackage<ValuePackage<57_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// fade_time
						TypePackage<ValuePackage<58_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// probability
						TypePackage<ValuePackage<59_e>, TypePackage<Floater>, ValuePackage<100.0_f>>
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
				if constexpr (check_version(version, {112, 150})) {
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
				if constexpr (check_version(version, {150})) {
					using Type = TypePackage<
						// scope,
						TypePackage<ValuePackage<0_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// trigger_on,
						TypePackage<ValuePackage<16_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
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
						TypePackage<ValuePackage<10_e>, TypePackage<Floater>, ValuePackage<0.2_f>>,
						// attack_curve,
						TypePackage<ValuePackage<11_e>, TypePackage<Floater>, ValuePackage<50.0_f>>,
						// decay_time,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.2_f>>,
						// sustain_level,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// release_time,
						TypePackage<ValuePackage<15_e>, TypePackage<Floater>, ValuePackage<0.5_f>>,
						// sustain_time,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// initial_delay,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// duration,
						TypePackage<ValuePackage<17_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
						// loop,
						TypePackage<ValuePackage<18_e>, TypePackage<Integer>, ValuePackage<1_i>>,
						// playback_rate,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<1.0_f>>,
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

		template <typename _> requires (check_version(version, {140, 150}))
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

		template <typename _> requires (check_version(version, {150}))
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
				if constexpr (check_version(version, {140, 150})) {
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
				if constexpr (check_version(version, {150})) {
					using Type = TypePackage<
						// bus_volume,
						TypePackage<ValuePackage<4_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<13_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<15_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<14_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume,
						TypePackage<ValuePackage<0_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<1_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<2_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<3_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<5_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<12_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_low_pass_filter,
						TypePackage<ValuePackage<24_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_high_pass_filter,
						TypePackage<ValuePackage<25_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<8_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<9_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<10_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<11_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_0,
						TypePackage<ValuePackage<16_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_1,
						TypePackage<ValuePackage<17_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_2,
						TypePackage<ValuePackage<18_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_3,
						TypePackage<ValuePackage<19_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_0,
						TypePackage<ValuePackage<20_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_1,
						TypePackage<ValuePackage<21_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_2,
						TypePackage<ValuePackage<22_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_3,
						TypePackage<ValuePackage<23_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// early_reflection_auxiliary_send_volume,
						TypePackage<ValuePackage<26_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<41_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_x,
						TypePackage<ValuePackage<35_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_y,
						TypePackage<ValuePackage<36_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_speaker_panning_z,
						TypePackage<ValuePackage<37_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// positioning_listener_routing_speaker_panning_division_spatialization_mix,
						TypePackage<ValuePackage<42_e>, TypePackage<Floater>, ValuePackage<100.0_f>>,
						// positioning_listener_routing_attenuation_id,
						TypePackage<ValuePackage<85_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_id,
						TypePackage<ValuePackage<61_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<62_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<63_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<30_e>, TypePackage<Floater>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<75_e>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<76_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<31_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<32_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<77_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<78_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<79_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<80_e>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						TypePackage<ValuePackage<81_e>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<82_e>, TypePackage<Enumerated>, ValuePackage<0_e>>,
						// midi_target_id,
						TypePackage<ValuePackage<83_e>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_priority_value,
						TypePackage<ValuePackage<6_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_maximum_distance,
						TypePackage<ValuePackage<56_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_initial_delay,
						TypePackage<ValuePackage<34_e>, TypePackage<Floater>, ValuePackage<0.0_f>>,
						// playback_loop,
						TypePackage<ValuePackage<84_e>, TypePackage<Integer>, ValuePackage<0_i>>,
						// playback_speed,
						TypePackage<ValuePackage<33_e>, TypePackage<Floater>, ValuePackage<1.0_f>>
					>;
					return declare<Type>();
				}
			}());
		};

		#pragma endregion

		#pragma region enumeration attribute of definition type

		template <>
		struct EnumerationAttribute<typename Definition::Curve> {
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
		struct EnumerationAttribute<typename Definition::TimePoint> {
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
		struct EnumerationAttribute<typename Definition::CoordinateMode> {
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
		struct EnumerationAttribute<typename Definition::PropertyCategory> {
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
		struct EnumerationAttribute<typename Definition::ParameterCategory> {
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
		struct EnumerationAttribute<typename Definition::AudioPlayType> {
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
		struct EnumerationAttribute<typename Definition::AudioPlayMode> {
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
		struct EnumerationAttribute<typename Definition::AudioPositioningSettingListenerRoutingPositionSourceMode> {
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
		struct EnumerationAttribute<typename Definition::AudioPositioningSettingListenerRoutingSpatialization> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {128})) {
					return 3_sz; // NOTE : EXPLAIN - use 3 byte even only need 2 byte
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
		struct EnumerationAttribute<typename Definition::AudioPositioningSettingSpeakerPanningMode> {
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
		struct EnumerationAttribute<typename Definition::AudioPositioningSettingType> {
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
		struct EnumerationAttribute<typename Definition::BusAutomaticDuckingSettingBusTarget> {
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
		struct EnumerationAttribute<typename Definition::MusicTransitionSettingJumpMode> {
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
		struct EnumerationAttribute<typename Definition::MusicTransitionSettingSynchronizeMode> {
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
		struct EnumerationAttribute<typename Definition::BusHDRSettingDynamicReleaseMode> {
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
		struct EnumerationAttribute<typename Definition::SoundMIDISettingEventPlayOn> {
			inline static constexpr auto size = Size{[] {
				if constexpr (check_version(version, {112})) {
					return 2_sz;
				}
				return k_none_size;
			}()};
			using Index = decltype([] {
				if constexpr (check_version(version, {112})) {
					using Type = ValuePackage<
						// note_on,
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
		struct EnumerationAttribute<typename Definition::MusicMIDISettingClipTempoSource> {
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
		struct EnumerationAttribute<typename Definition::AudioPlaybackLimitSettingScope> {
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
		struct EnumerationAttribute<typename Definition::AudioPlaybackLimitSettingWhenPriorityIsEqual> {
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
		struct EnumerationAttribute<typename Definition::AudioPlaybackLimitSettingWhenLimitIsReached> {
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
		struct EnumerationAttribute<typename Definition::AudioVirtualVoiceSettingBehavior> {
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
		struct EnumerationAttribute<typename Definition::AudioVirtualVoiceSettingOnReturnToPhysical> {
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
		struct EnumerationAttribute<typename Definition::AudioAssociationSettingMode> {
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
		struct EnumerationAttribute<typename Definition::AudioSourceType> {
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
		struct EnumerationAttribute<typename Definition::AudioPlayTypeRandomType> {
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
		struct EnumerationAttribute<typename Definition::AudioPlayTypeSequenceAtEndOfPlaylist> {
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
		struct EnumerationAttribute<typename Definition::AudioPlayModeContinuousTransitionType> {
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
		struct EnumerationAttribute<typename Definition::GameParameterBindToBuiltInParameterMode> {
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
		struct EnumerationAttribute<typename Definition::GameParameterInterpolationMode> {
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
		struct EnumerationAttribute<typename Definition::EventActionProperty::ValueApplyMode> {
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
		struct EnumerationAttribute<typename Definition::EventActionProperty::SeekType> {
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
		struct EnumerationAttribute<typename Definition::EventActionMode> {
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
		struct EnumerationAttribute<typename Definition::EventActionScope> {
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
		struct EnumerationAttribute<typename Definition::ModulatorScope> {
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
		struct EnumerationAttribute<typename Definition::ModulatorTriggerOn> {
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
		struct EnumerationAttribute<typename Definition::ModulatorWaveform> {
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
		struct EnumerationAttribute<typename Definition::SoundPlaylistContainerScope> {
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
		struct EnumerationAttribute<typename Definition::MusicTrackTrackType> {
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
		struct EnumerationAttribute<typename Definition::MusicTrackClipCurveItemType> {
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
		struct EnumerationAttribute<typename Definition::HierarchyType> {
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
		struct EnumerationAttribute<typename Definition::VoiceFilterBehavior> {
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

}
