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

		using IDNumber = IntegerU32;

		static auto find_id (
			IDNumber const & number
		) -> typename Manifest::ID {
			auto result = typename Manifest::ID{};
			result = cbw<Integer>(number);
			return result;
		}

		// ----------------

		using CommonPropertyValue = Variant<Boolean, Integer, Floating, Size, IDWrapper>;

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

		template <typename _> requires (check_version(version, {140}))
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
				if constexpr (check_version(version, {140})) {
					using Type = TypePackage<
						// delay
						TypePackage<ValuePackage<15_sz>, TypePackage<Integer>, ValuePackage<0_i>>,
						// fade_time
						TypePackage<ValuePackage<16_sz>, TypePackage<Integer>, ValuePackage<0_i>>,
						// probability
						TypePackage<ValuePackage<17_sz>, TypePackage<Floating>, ValuePackage<100.0_f>>
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <typename = None>
		struct ModulatorCommonPropertyType_;

		using ModulatorCommonPropertyType = ModulatorCommonPropertyType_<>;

		template <typename _> requires (check_version(version, {140}))
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
				loop_count,
				playback_rate,
				stop_playback,
			),
		);

		template <>
		struct EnumerationAttribute<ModulatorCommonPropertyType> {
			using Attribute = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = TypePackage<
						// scope,
						TypePackage<ValuePackage<0_sz>, TypePackage<Size>, ValuePackage<0_sz>>,
						// trigger_on,
						TypePackage<ValuePackage<15_sz>, TypePackage<Size>, ValuePackage<0_sz>>,
						// depth,
						TypePackage<ValuePackage<2_sz>, TypePackage<Floating>, ValuePackage<100.0_f>>,
						// frequency,
						TypePackage<ValuePackage<4_sz>, TypePackage<Floating>, ValuePackage<1.0_f>>,
						// waveform,
						TypePackage<ValuePackage<5_sz>, TypePackage<Size>, ValuePackage<0_sz>>,
						// smoothing,
						TypePackage<ValuePackage<6_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// pulse_width_modulation,
						TypePackage<ValuePackage<7_sz>, TypePackage<Floating>, ValuePackage<50.0_f>>,
						// attack,
						TypePackage<ValuePackage<3_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// initial_phase_offset,
						TypePackage<ValuePackage<8_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// attack_time,
						TypePackage<ValuePackage<9_sz>, TypePackage<Floating>, ValuePackage<0.2_f>>,
						// attack_curve,
						TypePackage<ValuePackage<10_sz>, TypePackage<Floating>, ValuePackage<50.0_f>>,
						// decay_time,
						TypePackage<ValuePackage<11_sz>, TypePackage<Floating>, ValuePackage<0.2_f>>,
						// sustain_level,
						TypePackage<ValuePackage<12_sz>, TypePackage<Floating>, ValuePackage<100.0_f>>,
						// release_time,
						TypePackage<ValuePackage<14_sz>, TypePackage<Floating>, ValuePackage<0.5_f>>,
						// sustain_time,
						TypePackage<ValuePackage<13_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// initial_delay,
						TypePackage<ValuePackage<19_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// duration,
						TypePackage<ValuePackage<16_sz>, TypePackage<Floating>, ValuePackage<1.0_f>>,
						// loop_count,
						TypePackage<ValuePackage<17_sz>, TypePackage<Integer>, ValuePackage<1_i>>,
						// playback_rate,
						TypePackage<ValuePackage<18_sz>, TypePackage<Floating>, ValuePackage<1.0_f>>,
						// stop_playback,
						TypePackage<ValuePackage<1_sz>, TypePackage<Boolean>, ValuePackage<k_true>>
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <typename = None>
		struct AudioCommonPropertyType_;

		using AudioCommonPropertyType = AudioCommonPropertyType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioCommonPropertyType_<_>),
			M_wrap(
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_volume_make_up_gain,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
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
				position_speaker_panning_x,
				position_speaker_panning_y,
				position_speaker_panning_z,
				position_listener_relative_routing_speaker_panning_div_3d_spatialization_mix,
				position_listener_relative_routing_attenuation_id,
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
				playback_speed,
				bus_volume,
				hdr_threshold,
				hdr_ratio,
				hdr_release_time,
				hdr_window_tap_output_game_parameter,
				hdr_window_tap_output_game_parameter_minimum,
				hdr_window_tap_output_game_parameter_maximum,
				playback_priority_value,
				playback_priority_offset_at_max_distance,
				initial_delay,
				loop_count,
			),
		);

		template <>
		struct EnumerationAttribute<AudioCommonPropertyType> {
			using Attribute = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = TypePackage<
						// voice_volume,
						TypePackage<ValuePackage<0_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// voice_pitch,
						TypePackage<ValuePackage<2_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// voice_low_pass_filter,
						TypePackage<ValuePackage<3_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// voice_high_pass_filter,
						TypePackage<ValuePackage<4_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// voice_volume_make_up_gain,
						TypePackage<ValuePackage<6_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// output_bus_volume,
						TypePackage<ValuePackage<24_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// output_bus_low_pass_filter,
						TypePackage<ValuePackage<26_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// output_bus_high_pass_filter,
						TypePackage<ValuePackage<25_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_volume,
						TypePackage<ValuePackage<23_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_low_pass_filter,
						TypePackage<ValuePackage<68_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// game_defined_auxiliary_send_high_pass_filter,
						TypePackage<ValuePackage<69_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_0,
						TypePackage<ValuePackage<19_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_1,
						TypePackage<ValuePackage<20_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_2,
						TypePackage<ValuePackage<21_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_volume_3,
						TypePackage<ValuePackage<22_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_0,
						TypePackage<ValuePackage<60_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_1,
						TypePackage<ValuePackage<61_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_2,
						TypePackage<ValuePackage<62_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_low_pass_filter_3,
						TypePackage<ValuePackage<63_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_0,
						TypePackage<ValuePackage<64_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_1,
						TypePackage<ValuePackage<65_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_2,
						TypePackage<ValuePackage<66_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// user_defined_auxiliary_send_high_pass_filter_3,
						TypePackage<ValuePackage<67_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// early_reflection_auxiliary_send_volume,
						TypePackage<ValuePackage<72_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// positioning_center_percent,
						TypePackage<ValuePackage<14_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// position_speaker_panning_x,
						TypePackage<ValuePackage<12_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// position_speaker_panning_y,
						TypePackage<ValuePackage<13_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// position_speaker_panning_z,
						TypePackage<ValuePackage<73_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// position_listener_relative_routing_speaker_panning_div_3d_spatialization_mix,
						TypePackage<ValuePackage<71_sz>, TypePackage<Floating>, ValuePackage<100.0_f>>,
						// position_listener_relative_routing_attenuation_id,
						TypePackage<ValuePackage<70_sz>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_envelope_tracking_active_range,
						TypePackage<ValuePackage<33_sz>, TypePackage<Floating>, ValuePackage<12.0_f>>,
						// midi_note_tracking_root_note,
						TypePackage<ValuePackage<45_sz>, TypePackage<Integer>, ValuePackage<60_i>>,
						// midi_event_play_on,
						TypePackage<ValuePackage<46_sz>, TypePackage<Size>, ValuePackage<0_sz>>,
						// midi_transformation_transposition,
						TypePackage<ValuePackage<47_sz>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_transformation_velocity_offset,
						TypePackage<ValuePackage<48_sz>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_minimum,
						TypePackage<ValuePackage<49_sz>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_key_range_maximum,
						TypePackage<ValuePackage<50_sz>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_velocity_minimum,
						TypePackage<ValuePackage<51_sz>, TypePackage<Integer>, ValuePackage<0_i>>,
						// midi_filter_velocity_maximum,
						TypePackage<ValuePackage<52_sz>, TypePackage<Integer>, ValuePackage<127_i>>,
						// midi_filter_channel,
						// todo : bitset ?
						TypePackage<ValuePackage<53_sz>, TypePackage<Integer>, ValuePackage<65535_i>>,
						// midi_clip_tempo_source,
						TypePackage<ValuePackage<55_sz>, TypePackage<Size>, ValuePackage<0_sz>>,
						// midi_target_id,
						TypePackage<ValuePackage<56_sz>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// playback_speed,
						TypePackage<ValuePackage<54_sz>, TypePackage<Floating>, ValuePackage<1.0_f>>,
						// bus_volume,
						TypePackage<ValuePackage<5_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// hdr_threshold,
						TypePackage<ValuePackage<27_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// hdr_ratio,
						TypePackage<ValuePackage<28_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// hdr_release_time,
						TypePackage<ValuePackage<29_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter,
						TypePackage<ValuePackage<30_sz>, TypePackage<IDWrapper>, ValuePackage<IDWrapper{.value = 0_i}>>,
						// hdr_window_tap_output_game_parameter_minimum,
						TypePackage<ValuePackage<31_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// hdr_window_tap_output_game_parameter_maximum,
						TypePackage<ValuePackage<32_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// playback_priority_value,
						TypePackage<ValuePackage<7_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// playback_priority_offset_at_max_distance,
						TypePackage<ValuePackage<8_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// initial_delay,
						TypePackage<ValuePackage<59_sz>, TypePackage<Floating>, ValuePackage<0.0_f>>,
						// loop_count,
						TypePackage<ValuePackage<58_sz>, TypePackage<Integer>, ValuePackage<0_i>>
					>;
					return declare<Type>();
				}
			}());
		};

		#pragma endregion

		#pragma region enumeration attribute of manifest type

		template <>
		struct EnumerationAttribute<typename Manifest::MusicTrackTrackType> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// normal,
						0_sz,
						// random_step,
						1_sz,
						// sequence_step,
						2_sz,
						// switcher,
						3_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioSwitcherAssociationSettingMode> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// best_match,
						0_sz,
						// weighted,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorScope> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// voice,
						0_sz,
						// note_or_event,
						1_sz,
						// game_object,
						2_sz,
						// global,
						3_sz
					>;
					return declare<Type>();
				}
			}());
		};

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorTriggerOn> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// play,
						0_sz,
						// note_off,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorWaveform> {
			inline static constexpr auto size = Size{3_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// sine,
						0_sz,
						// triangle,
						1_sz,
						// square,
						2_sz,
						// saw_up,
						3_sz,
						// saw_down,
						4_sz,
						// random,
						5_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::Curve> {
			inline static constexpr auto size = Size{4_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// constant,
						9_sz,
						// linear,
						4_sz,
						// s,
						5_sz,
						// s_inverted,
						3_sz,
						// sine,
						1_sz,
						// sine_reciprocal,
						7_sz,
						// logarithmic_1dot41,
						2_sz,
						// logarithmic_3dot0,
						0_sz,
						// exponential_1dot41,
						6_sz,
						// exponential_3dot0,
						8_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::CurveShape> {
			inline static constexpr auto size = Size{4_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// a_1,
						0_sz,
						// a_2,
						8_sz,
						// b_1,
						1_sz,
						// b_2,
						7_sz,
						// c_1,
						2_sz,
						// c_2,
						6_sz,
						// d_1,
						3_sz,
						// d_2,
						5_sz,
						// e,
						4_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::TimePoint> {
			inline static constexpr auto size = Size{4_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// immediate,
						0_sz,
						// next_grid,
						1_sz,
						// next_bar,
						2_sz,
						// next_beta,
						3_sz,
						// next_cue,
						4_sz,
						// custom_cue,
						5_sz,
						// entry_cue,
						6_sz,
						// exit_cue,
						7_sz,
						// last_exit_position,
						9_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::PropertyCategory> {
			inline static constexpr auto size = Size{3_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// unidirectional,
						1_sz,
						// bidirectional,
						2_sz,
						// bidirectional_ranged,
						3_sz, // maybe, see playback_speed
						// boolean,
						4_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::RealTimeParameterControlXAxisCategory> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// game_parameter,
						0_sz,
						// midi_parameter,
						1_sz,
						// modulator,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AttenuationPropertyType> {
			inline static constexpr auto size = Size{7_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// cone_maximum_attenuation,
						26_sz,
						// cone_low_pass_filter,
						27_sz,
						// cone_high_pass_filter,
						28_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::ModulatorPropertyType> {
			inline static constexpr auto size = Size{7_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// lfo_depth,
						0_sz,
						// lfo_frequency,
						2_sz,
						// lfo_waveform,
						3_sz,
						// lfo_smoothing,
						4_sz,
						// lfo_pulse_width_modulation,
						5_sz,
						// lfo_attack,
						1_sz,
						// lfo_initial_phase_offset,
						6_sz,
						// envelope_attack_time,
						8_sz,
						// envelope_attack_curve,
						9_sz,
						// envelope_decay_time,
						10_sz,
						// envelope_sustain_level,
						11_sz,
						// envelope_maximum_sustain_time,
						12_sz,
						// envelope_release_time,
						13_sz,
						// time_initial_delay,
						15_sz,
						// time_playback_rate,
						14_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPropertyType> {
			inline static constexpr auto size = Size{7_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// bus_volume,
						5_sz,
						// output_bus_volume,
						43_sz,
						// output_bus_low_pass_filter,
						45_sz,
						// output_bus_high_pass_filter,
						44_sz,
						// voice_volume,
						0_sz,
						// voice_pitch,
						2_sz,
						// voice_low_pass_filter,
						3_sz,
						// voice_high_pass_filter,
						4_sz,
						// voice_initial_delay,
						6_sz,
						// audio_make_up_gain,
						7_sz,
						// game_defined_auxiliary_send_volume,
						38_sz,
						// game_defined_auxiliary_send_low_pass_filter,
						56_sz,
						// game_defined_auxiliary_send_high_pass_filter,
						57_sz,
						// user_defined_auxiliary_send_volume_0,
						39_sz,
						// user_defined_auxiliary_send_volume_1,
						40_sz,
						// user_defined_auxiliary_send_volume_2,
						41_sz,
						// user_defined_auxiliary_send_volume_3,
						42_sz,
						// user_defined_auxiliary_send_low_pass_filter_0,
						48_sz,
						// user_defined_auxiliary_send_low_pass_filter_1,
						49_sz,
						// user_defined_auxiliary_send_low_pass_filter_2,
						50_sz,
						// user_defined_auxiliary_send_low_pass_filter_3,
						51_sz,
						// user_defined_auxiliary_send_high_pass_filter_0,
						52_sz,
						// user_defined_auxiliary_send_high_pass_filter_1,
						53_sz,
						// user_defined_auxiliary_send_high_pass_filter_2,
						54_sz,
						// user_defined_auxiliary_send_high_pass_filter_3,
						55_sz,
						// early_reflection_auxiliary_send_volume,
						47_sz,
						// playback_limit_value,
						16_sz,
						// playback_priority_value,
						17_sz,
						// playback_speed,
						13_sz,
						// midi_transposition,
						11_sz,
						// midi_velocity_offset,
						12_sz,
						// transition_duration,
						15_sz,
						// effect_bypass_all,
						33_sz,
						// effect_bypass_0,
						29_sz,
						// effect_bypass_1,
						30_sz,
						// effect_bypass_2,
						31_sz,
						// effect_bypass_3,
						32_sz,
						// positioning_center_percent,
						24_sz,
						// speaker_panning_div_3d_spatialization_mix,
						23_sz,
						// speaker_panning_pan_front_rear,
						19_sz,
						// speaker_panning_pan_left_right,
						18_sz,
						// speaker_panning_pan_up_down,
						58_sz,
						// spatialization_3d_automation_pan_front_rear,
						21_sz,
						// spatialization_3d_automation_pan_left_right,
						20_sz,
						// spatialization_3d_automation_pan_up_down,
						22_sz,
						// attenuation_enable,
						46_sz,
						// hdr_threshold,
						34_sz,
						// hdr_release_time,
						35_sz,
						// hdr_ratio,
						36_sz,
						// hdr_active_range,
						37_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::BusAutoDuckingSettingBusTarget> {
			inline static constexpr auto size = Size{3_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// voice_volume,
						0_sz,
						// bus_volume,
						5_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::SoundMIDISettingEventPlayOn> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// note_off,
						0_sz,
						// note_on,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlaybackLimitSettingWhenPriorityIsEqual> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// discard_oldest_instance,
						0_sz,
						// discard_newest_instance,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlaybackLimitSettingWhenLimitIsReached> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// kill_voice,
						0_sz,
						// use_virtual_voice_setting,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlaybackLimitSettingScope> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// per_game_object,
						0_sz,
						// globally,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioVirtualVoiceSettingOnReturnToPhysical> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// play_from_beginning,
						0_sz,
						// play_from_elapsed_time,
						1_sz,
						// resume,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioVirtualVoiceSettingBehavior> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// continue_to_play,
						0_sz,
						// kill_voice,
						1_sz,
						// send_to_virtual_voice,
						2_sz,
						// kill_if_finite_else_virtual,
						3_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayType> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// sequence,
						0_sz,
						// random,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayMode> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// step,
						0_sz,
						// continuous,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingSpeakerPanningMode> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// direct_assignment,
						0_sz,
						// balance_fade,
						1_sz,
						// steering,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingListenerRelativeRoutingPosition3DMode> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// emitter,
						0_sz,
						// emitter_with_automation,
						1_sz,
						// listener_with_automation,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPositioningSettingListenerRelativeRoutingSpatialization3D> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// none,
						0_sz,
						// position,
						1_sz,
						// position_and_orientation,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioSourceType> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// internal,
						0_sz,
						// external,
						2_sz,
						// external_prefetch,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::SoundPlaylistContainerScope> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// game_object,
						0_sz,
						// global,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayTypeRandomType> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// standard,
						0_sz,
						// shuffle,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayTypeSequenceAtEndOfPlaylist> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// restart,
						0_sz,
						// play_in_reserve_order,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::AudioPlayModeContinuousTransitionType> {
			inline static constexpr auto size = Size{3_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// none,
						0_sz,
						// xfade_amp,
						1_sz,
						// xfade_power,
						2_sz,
						// delay,
						3_sz,
						// sample_accurate,
						4_sz,
						// trigger_rate,
						5_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicMIDISettingClipTempoSource> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// hierarchy,
						0_sz,
						// file,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::MusicTrackClipCurveItemType> {
			inline static constexpr auto size = Size{3_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// voice_volume,
						0_sz,
						// voice_low_pass_filter,
						1_sz,
						// voice_high_pass_filter,
						2_sz,
						// clip_fade_in,
						3_sz,
						// clip_fade_out,
						4_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::EventActionProperty::ValueApplyMode> {
			inline static constexpr auto size = Size{2_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// absolute,
						1_sz,
						// relative,
						2_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::EventActionProperty::SeekType> {
			inline static constexpr auto size = Size{1_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// time,
						0_sz,
						// percent,
						1_sz
					>;
					return declare<Type>();
				}
			}());
		};

		// ----------------

		template <>
		struct EnumerationAttribute<typename Manifest::HierarchyType> {
			inline static constexpr auto size = Size{8_sz};
			using Index = decltype([] {
				if constexpr (check_version(version, {88, 112})) {
					using Type = ValuePackage<
						// unknown,
						~0_sz
					>;
					return declare<Type>();
				}
				if constexpr (check_version(version, {140})) {
					using Type = ValuePackage<
						// unknown,
						~0_sz,
						// source_plug_in_reference,
						17_sz,
						// unknown_plug_in_reference,
						16_sz,
						// stateful_audio_property_setting,
						1_sz,
						// event_action,
						3_sz,
						// event,
						4_sz,
						// dialogue_event,
						15_sz,
						// attenuation,
						14_sz,
						// low_frequency_oscillator_modulator,
						19_sz,
						// envelope_modulator,
						20_sz,
						// time_modulator,
						22_sz,
						// audio_device,
						21_sz,
						// audio_bus,
						8_sz,
						// auxiliary_audio_bus,
						18_sz,
						// sound,
						2_sz,
						// sound_playlist_container,
						5_sz,
						// sound_switch_container,
						6_sz,
						// sound_blend_container,
						9_sz,
						// actor_mixer,
						7_sz,
						// music_track,
						11_sz,
						// music_segment,
						10_sz,
						// music_playlist_container,
						13_sz,
						// music_switch_container,
						12_sz
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

		using typename Common::IDNumber;

		using typename Common::IDWrapper;

		using typename Common::CommonPropertyValue;

		template <typename _1>
		using CommonPropertyMap = typename Common::template CommonPropertyMap<_1>;

		using typename Common::EventActionCommonPropertyType;

		using typename Common::ModulatorCommonPropertyType;

		using typename Common::AudioCommonPropertyType;

		template <typename _1>
		using EnumerationAttribute = typename Common::template EnumerationAttribute<_1>;

		using Common::find_id;

		// ----------------

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsEnumerationWrapper<Value>)
		static auto exchange_enumeration_index (
			Size &        index_value,
			Value const & value
		) -> Void {
			auto has_case = k_false;
			Generalization::each<typename EnumerationAttribute<Value>::Index>(
				[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
					if (index == cbw<Size>(value).value) {
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
			&& (IsSame<typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>, Value, Size, IDWrapper>)
		static auto exchange_common_property (
			CommonPropertyMap<decltype(type)> & map,
			Value const &                       value
		) -> Void {
			using CurrentEnumerationAttribute = typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>;
			constexpr auto default_value = CurrentEnumerationAttribute::template Element<3_ixz>::template element<1_ixz>;
			if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, IDWrapper>) {
				if (value != default_value.value) {
					auto & element = map.regular.append();
					element.key = type;
					element.value.template get<1_ix>().template set<IDWrapper>().value = value;
				}
			} else if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Size>) {
				if (cbw<Size>(value) != default_value) {
					auto & element = map.regular.append();
					element.key = type;
					exchange_enumeration_index(element.value.template get<1_ix>().template set<Size>(), value);
				}
			} else {
				if (value != default_value) {
					auto & element = map.regular.append();
					element.key = type;
					element.value.template get<1_ix>().template set<Value>() = value;
				}
			}
			return;
		}

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<Value, typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>>)
			&& (IsSame<Value, Boolean, Integer, Floating>)
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
			&& (IsSame<Value, Integer, Floating>)
		static auto exchange_common_property_as_randomized (
			CommonPropertyMap<decltype(type)> &                        map,
			typename Manifest::template RandomizedValue<Value> const & value
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
			exchange_common_property_as_regular<CPTC::voice_volume()>(map, manifest.volume);
			exchange_common_property_as_regular<CPTC::voice_pitch()>(map, manifest.pitch);
			exchange_common_property_as_regular<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_regular<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::AudioVoice const &        manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_randomized<CPTC::voice_volume()>(map, manifest.volume);
			exchange_common_property_as_randomized<CPTC::voice_pitch()>(map, manifest.pitch);
			exchange_common_property_as_randomized<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_randomized<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &         map,
			typename Manifest::BusVoiceVolumeGainSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &           map,
			typename Manifest::AudioVoiceVolumeGainSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_randomized<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::BusBusSetting const &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::bus_volume()>(map, manifest.volume);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &   map,
			typename Manifest::BusOutputBusSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &     map,
			typename Manifest::AudioOutputBusSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &         map,
			typename Manifest::AudioAuxiliarySendSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_volume()>(map, manifest.game_defined.volume);
			exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_low_pass_filter()>(map, manifest.game_defined.low_pass_filter);
			exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_high_pass_filter()>(map, manifest.game_defined.high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_0()>(map, manifest.user_defined.item[1_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_0()>(map, manifest.user_defined.item[1_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_0()>(map, manifest.user_defined.item[1_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_1()>(map, manifest.user_defined.item[2_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_1()>(map, manifest.user_defined.item[2_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_1()>(map, manifest.user_defined.item[2_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_2()>(map, manifest.user_defined.item[3_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_2()>(map, manifest.user_defined.item[3_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_2()>(map, manifest.user_defined.item[3_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_3()>(map, manifest.user_defined.item[4_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_3()>(map, manifest.user_defined.item[4_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_3()>(map, manifest.user_defined.item[4_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::early_reflection_auxiliary_send_volume()>(map, manifest.early_reflection.volume);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &       map,
			typename Manifest::AudioPositioningSetting const & manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::positioning_center_percent()>(map, manifest.center_percent);
			exchange_common_property<CPTC::position_speaker_panning_x()>(map, manifest.speaker_panning.position.x);
			exchange_common_property<CPTC::position_speaker_panning_y()>(map, manifest.speaker_panning.position.y);
			exchange_common_property<CPTC::position_speaker_panning_z()>(map, manifest.speaker_panning.position.z);
			exchange_common_property_as_regular<CPTC::position_listener_relative_routing_speaker_panning_div_3d_spatialization_mix()>(map, manifest.listener_relative_routing.speaker_panning_div_spatialization_3d_mix);
			exchange_common_property<CPTC::position_listener_relative_routing_attenuation_id()>(map, manifest.listener_relative_routing.attenuation.id);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::BusHDRSetting const &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::hdr_threshold()>(map, manifest.dynamic.threshold);
			exchange_common_property_as_regular<CPTC::hdr_ratio()>(map, manifest.dynamic.ratio);
			exchange_common_property_as_regular<CPTC::hdr_release_time()>(map, manifest.dynamic.release_time);
			exchange_common_property<CPTC::hdr_window_tap_output_game_parameter()>(map, manifest.window_top_output_game_parameter.id);
			exchange_common_property_as_regular<CPTC::hdr_window_tap_output_game_parameter_minimum()>(map, manifest.window_top_output_game_parameter.minimum);
			exchange_common_property_as_regular<CPTC::hdr_window_tap_output_game_parameter_maximum()>(map, manifest.window_top_output_game_parameter.maximum);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::AudioHDRSetting const &   manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::hdr_envelope_tracking_active_range()>(map, manifest.envelope_tracking.active_range);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::SoundMIDISetting const &  manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property<CPTC::midi_event_play_on()>(map, manifest.event.play_on);
			exchange_common_property<CPTC::midi_note_tracking_root_note()>(map, manifest.note_tracking.root_note);
			exchange_common_property_as_regular<CPTC::midi_transformation_transposition()>(map, manifest.transformation.transposition);
			exchange_common_property_as_regular<CPTC::midi_transformation_velocity_offset()>(map, manifest.transformation.velocity_offset);
			exchange_common_property<CPTC::midi_filter_key_range_minimum()>(map, manifest.filter.key_range_minimum);
			exchange_common_property<CPTC::midi_filter_key_range_maximum()>(map, manifest.filter.key_range_maximum);
			exchange_common_property<CPTC::midi_filter_velocity_minimum()>(map, manifest.filter.velocity_minimum);
			exchange_common_property<CPTC::midi_filter_velocity_maximum()>(map, manifest.filter.velocity_maximum);
			exchange_common_property<CPTC::midi_filter_channel()>(map, manifest.filter.channel);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Manifest::MusicMIDISetting const &  manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property<CPTC::midi_target_id()>(map, manifest.target.id);
			exchange_common_property<CPTC::midi_clip_tempo_source()>(map, manifest.clip_tempo.source);
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
			exchange_common_property_as_regular<CPTC::playback_priority_value()>(map, manifest.value);
			exchange_common_property_as_regular<CPTC::playback_priority_offset_at_max_distance()>(map, manifest.offset_at_max_distance);
			return;
		}

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_raw (
			OByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.write(value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			OByteStreamView & data,
			RawValue const &  value
		) -> Void {
			auto raw_value = RawValue{};
			raw_value = value;
			data.write(raw_value);
			return;
		}

		static auto exchange_unit_id (
			OByteStreamView &             data,
			typename Manifest::ID const & value
		) -> Void {
			auto raw_value = IDNumber{};
			raw_value = cbw<IDNumber>(value);
			data.write(raw_value);
			return;
		}

		static auto exchange_unit_plug_in_id (
			OByteStreamView &                   data,
			typename Manifest::PlugInID const & value
		) -> Void {
			auto raw_value = IntegerU16{};
			raw_value = cbw<IntegerU16>(value.u1);
			data.write(raw_value);
			raw_value = cbw<IntegerU16>(value.u2);
			data.write(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			auto raw_value = RawValue{};
			raw_value = cbw<RawValue>(value);
			data.write(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsFloatingWrapper<RawValue>)
		static auto exchange_unit_floating (
			OByteStreamView & data,
			Floating const &  value
		) -> Void {
			auto raw_value = RawValue{};
			raw_value = cbw<RawValue>(value);
			data.write(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_size (
			OByteStreamView & data,
			Size const &      value
		) -> Void {
			auto raw_value = RawValue{};
			raw_value = cbw<RawValue>(value);
			data.write(raw_value);
			return;
		}

		template <typename RawValue, typename Value> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_enumeration (
			OByteStreamView & data,
			Value const &     value
		) -> Void {
			auto raw_value = RawValue{};
			auto index_value = Size{};
			exchange_enumeration_index(index_value, value);
			raw_value = cbw<RawValue>(index_value);
			data.write(raw_value);
			return;
		}

		template <typename RawValue, typename ... Value> requires
			CategoryConstraint<IsPureInstance<RawValue> && IsInstance<Value ...>>
			&& (IsIntegerWrapper<RawValue>)
			&& ((IsSame<Value, Boolean> || IsEnumerationWrapper<Value>) && ...)
		static auto exchange_unit_bitset (
			OByteStreamView & data,
			Value const & ... value
		) -> Void {
			auto raw_value = RawValue{};
			auto bitset = BitSet<k_type_bit_count<RawValue>>{};
			auto current_index = k_begin_index;
			Generalization::each_with<>(
				[&] <auto index, typename CurrentValue> (ValuePackage<index>, CurrentValue const & current_value) {
					if constexpr (IsSame<CurrentValue, Boolean>) {
						bitset.set(current_index, current_value);
						++current_index;
					} else {
						auto index_value = Size{};
						exchange_enumeration_index(index_value, current_value);
						for (auto & bit_index : SizeRange{EnumerationAttribute<CurrentValue>::size}) {
							bitset.set(current_index, cbw<Boolean>(clip_bit(index_value, bit_index, 1_sz)));
							++current_index;
						}
					}
				},
				value ...
			);
			raw_value = bitset.to_integer();
			data.write(raw_value);
			return;
		}

		template <typename RawSizeValue> requires
			CategoryConstraint<IsPure<RawSizeValue>>
			&& (IsVoid<RawSizeValue> || IsIntegerWrapper<RawSizeValue>)
		static auto exchange_unit_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			data.write(self_cast<StringBlock<RawSizeValue>>(value));
			return;
		}

		template <typename Length, typename Element, typename LeadingParser, typename ... ElementParser> requires
			CategoryConstraint<IsPure<Length> && IsPureInstance<LeadingParser> && IsPureInstance<ElementParser ...>>
			&& (IsVoid<Length> || IsIntegerWrapper<Length>)
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
			if constexpr (!IsVoid<Length>) {
				data.forward(bs_static_size<Length>());
			}
			leading_parser(count);
			if constexpr (!IsVoid<Length>) {
				OByteStreamView{data.sub_view(length_position, bs_static_size<Length>())}.write<Length>(cbw<Length>(count));
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
			&& (IsSame<ActualValue, Boolean, Integer, Floating, Size, IDWrapper>)
		static auto process_unit_common_property_value (
			OByteStreamView &           data,
			CommonPropertyValue const & value
		) -> Void {
			if constexpr (IsSame<ActualValue, Boolean>) {
				exchange_unit_bitset<IntegerU32>(data, value.template get<Boolean>());
			}
			if constexpr (IsSame<ActualValue, Integer>) {
				exchange_unit_integer<IntegerS32>(data, value.template get<Integer>());
			}
			if constexpr (IsSame<ActualValue, Floating>) {
				exchange_unit_floating<Floating32>(data, value.template get<Floating>());
			}
			if constexpr (IsSame<ActualValue, Size>) {
				exchange_unit_size<IntegerU32>(data, value.template get<Size>());
			}
			if constexpr (IsSame<ActualValue, IDWrapper>) {
				exchange_unit_id(data, value.template get<IDWrapper>().value);
			}
			return;
		}

		// ----------------

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
			&& (IsEnumerationWrapper<Type>)
		static auto process_section_sub (
			OByteStreamView &               data,
			CommonPropertyMap<Type> const & map,
			Boolean const &                 exist_randomizer
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				map.regular.as_list(),
				[] (auto & count) {
				},
				[] (auto & data, auto & element) {
					auto type = Size{};
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							if (!has_case) {
								constexpr auto case_index = Attribute::template Element<1_ixz>::template element<1_ixz>;
								if (cbw<Size>(element.key).value == index) {
									type = case_index;
									has_case = k_true;
								}
							}
						}
					);
					assert_test(has_case);
					exchange_unit_size<IntegerU8>(data, type);
				},
				[] (auto & data, auto & element) {
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							if (!has_case) {
								if (index == static_cast<ZSize>(element.key.value)) {
									process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
									has_case = k_true;
								}
							}
						}
					);
					assert_test(has_case);
				}
			);
			if (exist_randomizer) {
				exchange_unit_list<IntegerU8>(
					data,
					map.randomizer.as_list(),
					[] (auto & count) {
					},
					[] (auto & data, auto & element) {
						auto type = Size{};
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								if (!has_case) {
									constexpr auto case_index = Attribute::template Element<1_ixz>::template element<1_ixz>;
									if (cbw<Size>(element.key).value == index) {
										type = case_index;
										has_case = k_true;
									}
								}
							}
						);
						assert_test(has_case);
						exchange_unit_size<IntegerU8>(data, type);
					},
					[] (auto & data, auto & element) {
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								if (!has_case) {
									if (index == static_cast<ZSize>(element.key.value)) {
										process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
										process_unit_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<2_ix>());
										has_case = k_true;
									}
								}
							}
						);
						assert_test(has_case);
					}
				);
			}
			return;
		}

		template <typename Length> requires
			CategoryConstraint<IsPureInstance<Length>>
			&& (IsIntegerWrapper<Length>)
		static auto process_section_sub (
			OByteStreamView &                   data,
			List<typename Manifest::ID> const & id
		) -> Void {
			exchange_unit_list<IntegerU32>(
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
			OByteStreamView &                                      data,
			typename Manifest::AudioVoiceVolumeGainSetting const & voice_volume_gain_manifest,
			typename Manifest::AudioHDRSetting const &             hdr_manifest,
			Boolean const &                                        voice_volume_loudness_normalization_override,
			Boolean const &                                        hdr_envelope_tracking_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				hdr_envelope_tracking_override,
				voice_volume_loudness_normalization_override,
				voice_volume_gain_manifest.normalization,
				hdr_manifest.envelope_tracking.enable
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                data,
			typename Manifest::AudioOutputBusSetting const & output_bus_manifest
		) -> Void {
			exchange_unit_id(data, output_bus_manifest.bus);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                    data,
			typename Manifest::AudioAuxiliarySendSetting const & auxiliary_send_manifest,
			Boolean const &                                      game_defined_auxiliary_send_override,
			Boolean const &                                      user_defined_auxiliary_send_override,
			Boolean const &                                      early_reflection_auxiliary_send_override
		) -> Void {
			assert_test(auxiliary_send_manifest.user_defined.item.size() == 4_sz);
			exchange_unit_bitset<IntegerU8>(
				data,
				game_defined_auxiliary_send_override,
				auxiliary_send_manifest.game_defined.enable,
				user_defined_auxiliary_send_override,
				auxiliary_send_manifest.user_defined.enable,
				early_reflection_auxiliary_send_override
			);
			if (auxiliary_send_manifest.user_defined.enable) {
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[1_ix].bus);
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[2_ix].bus);
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[3_ix].bus);
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[4_ix].bus);
			}
			exchange_unit_id(data, auxiliary_send_manifest.early_reflection.bus);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                  data,
			typename Manifest::AudioPositioningSetting const & positioning_manifest,
			Boolean const &                                    positioning_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				positioning_override,
				positioning_manifest.listener_relative_routing.enable,
				positioning_manifest.speaker_panning.mode,
				k_false,
				positioning_manifest.listener_relative_routing.position_3d.mode,
				k_false
			);
			if (positioning_manifest.listener_relative_routing.enable) {
				exchange_unit_bitset<IntegerU8>(
					data,
					positioning_manifest.listener_relative_routing.spatialization_3d,
					k_false,
					positioning_manifest.listener_relative_routing.attenuation.enable,
					positioning_manifest.listener_relative_routing.position_3d.hold_emitter_position_and_orientation,
					positioning_manifest.listener_relative_routing.position_3d.hold_listener_orientation,
					positioning_manifest.listener_relative_routing.position_3d.automation.loop,
					positioning_manifest.listener_relative_routing.position_3d.diffraction_and_transmission
				);
			}
			if (positioning_manifest.listener_relative_routing.position_3d.mode != Manifest::AudioPositioningSettingListenerRelativeRoutingPosition3DMode::Constant::emitter()) {
				exchange_unit_bitset<IntegerU8>(
					data,
					positioning_manifest.listener_relative_routing.position_3d.automation.play_type,
					positioning_manifest.listener_relative_routing.position_3d.automation.play_mode,
					positioning_manifest.listener_relative_routing.position_3d.automation.pick_new_path_when_sound_start
				);
				exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_relative_routing.position_3d.automation.transition_time);
				exchange_unit_list<IntegerU32>(
					data,
					positioning_manifest.listener_relative_routing.position_3d.automation.point,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_floating<Floating32>(data, manifest.position.x);
						exchange_unit_floating<Floating32>(data, manifest.position.z);
						exchange_unit_floating<Floating32>(data, manifest.position.y);
						exchange_unit_integer<IntegerS32>(data, manifest.duration);
					}
				);
				exchange_unit_list<IntegerU32>(
					data,
					positioning_manifest.listener_relative_routing.position_3d.automation.path,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
						exchange_unit_integer<IntegerU32>(data, manifest.point.count);
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_floating<Floating32>(data, manifest.random_range.left_right);
						exchange_unit_floating<Floating32>(data, manifest.random_range.front_back);
						exchange_unit_floating<Floating32>(data, manifest.random_range.up_down);
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                           data,
			typename Manifest::MusicMIDISetting const & midi_manifest,
			Boolean const &                             midi_target_override,
			Boolean const &                             midi_clip_tempo_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				k_false,
				midi_clip_tempo_override,
				midi_target_override
			);
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
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_priority_override,
				playback_priority_manifest.use_distance_factor,
				midi_event_override,
				midi_note_tracking_override,
				midi_manifest.note_tracking.enable,
				midi_manifest.event.break_on_note_off
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                       data,
			typename Manifest::AudioPlaybackPrioritySetting const & playback_priority_manifest,
			Boolean const &                                         playback_priority_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_priority_override,
				playback_priority_manifest.use_distance_factor
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                    data,
			typename Manifest::AudioPlaybackLimitSetting const & playback_limit_manifest,
			typename Manifest::AudioVirtualVoiceSetting const &  virtual_voice_manifest,
			Boolean const &                                      playback_limit_override,
			Boolean const &                                      virtual_voice_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_limit_manifest.when_priority_is_equal,
				playback_limit_manifest.when_limit_is_reached,
				playback_limit_manifest.scope,
				playback_limit_override,
				virtual_voice_override
			);
			exchange_unit_enumeration<IntegerU8>(data, virtual_voice_manifest.on_return_to_physical);
			exchange_unit_integer<IntegerS16>(data, playback_limit_manifest.value.value);
			exchange_unit_enumeration<IntegerU8>(data, virtual_voice_manifest.behavior);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                         data,
			typename Manifest::AudioPlaybackLimitSetting const &      playback_limit_manifest,
			typename Manifest::AudioBusMuteForBackgroundMusic const & mute_for_background_music_manifest
		) -> Void {
			auto b3 = Boolean{}; // todo
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_limit_manifest.when_priority_is_equal,
				playback_limit_manifest.when_limit_is_reached,
				b3,
				mute_for_background_music_manifest.enable
			);
			exchange_unit_integer<IntegerS16>(data, playback_limit_manifest.value.value);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                             data,
			typename Manifest::AudioEffectSetting const & effect_manifest
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				effect_manifest.item,
				[&] (auto & count) {
					assert_test(effect_manifest.bypass.size() == 5_sz);
					if (count > 0_sz) {
						exchange_unit_bitset<IntegerU8>(
							data,
							effect_manifest.bypass[1_ix],
							effect_manifest.bypass[2_ix],
							effect_manifest.bypass[3_ix],
							effect_manifest.bypass[4_ix],
							effect_manifest.bypass[5_ix]
						);
					} else {
						assert_test(effect_manifest.bypass[1_ix] == k_false);
						assert_test(effect_manifest.bypass[2_ix] == k_false);
						assert_test(effect_manifest.bypass[3_ix] == k_false);
						assert_test(effect_manifest.bypass[4_ix] == k_false);
						assert_test(effect_manifest.bypass[5_ix] == k_false);
					}
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.index);
					exchange_unit_id(data, manifest.id);
					// todo : in typical, render = 1 -> mode = 0 & u2 = 1, render = 0 -> mode = 1 & u2 = 0
					// todo : if render, mode value will be changed ?
					exchange_unit_bitset<IntegerU8>(data, manifest.use_share_set);
					exchange_unit_bitset<IntegerU8>(data, manifest.u1);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                             data,
			typename Manifest::AudioEffectSetting const & effect_manifest,
			Boolean const &                               effect_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, effect_override);
			process_section_sub(data, effect_manifest);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                               data,
			typename Manifest::AudioMetadataSetting const & metadata_manifest
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				metadata_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.index);
					exchange_unit_id(data, manifest.id);
					exchange_unit_bitset<IntegerU8>(data, manifest.use_share_set);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                               data,
			typename Manifest::AudioMetadataSetting const & metadata_manifest,
			Boolean const &                                 metadata_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, metadata_override);
			process_section_sub(data, metadata_manifest);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                     data,
			typename Manifest::AttenuationRealTimeParameterControlSetting const & real_time_parameter_control_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				real_time_parameter_control_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.x_axis);
					exchange_unit_enumeration<IntegerU8>(data, manifest.x_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_type);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_bitset<IntegerU8>(
						data,
						manifest.u2,
						manifest.u3
					);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                   data,
			typename Manifest::ModulatorRealTimeParameterControlSetting const & real_time_parameter_control_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				real_time_parameter_control_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.x_axis);
					exchange_unit_enumeration<IntegerU8>(data, manifest.x_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_type);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_bitset<IntegerU8>(
						data,
						manifest.u2,
						manifest.u3
					);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                               data,
			typename Manifest::AudioRealTimeParameterControlSetting const & real_time_parameter_control_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				real_time_parameter_control_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.x_axis);
					exchange_unit_enumeration<IntegerU8>(data, manifest.x_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_type);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_bitset<IntegerU8>(
						data,
						manifest.u2,
						manifest.u3
					);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                            data,
			typename Manifest::AudioStateSetting const & state_manifest
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				state_manifest.attribute,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_enumeration<IntegerU8>(data, manifest.type);
					exchange_unit_enumeration<IntegerU8>(data, manifest.category);
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
				}
			);
			exchange_unit_list<IntegerU8>(
				data,
				state_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.group);
					exchange_unit_enumeration<IntegerU8>(data, manifest.change_occur_at);
					exchange_unit_list<IntegerU8>(
						data,
						manifest.apply,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_id(data, manifest.target);
							exchange_unit_id(data, manifest.setting);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                             data,
			typename Manifest::AudioSourceSetting const & manifest
		) -> Void {
			exchange_unit_plug_in_id(data, manifest.plug_in);
			assert_test(manifest.plug_in.u1 == 0_i || manifest.plug_in.u1 == 1_i || manifest.plug_in.u1 == 2_i);
			exchange_unit_enumeration<IntegerU8>(data, manifest.type);
			exchange_unit_id(data, manifest.resource);
			// note : if external_prefetch, is internal data size
			exchange_unit_integer<IntegerU32>(data, manifest.resource_size);
			exchange_unit_bitset<IntegerU8>(
				data,
				k_false,
				k_false,
				k_false,
				manifest.non_cachable_stream
			);
			// todo
			if (manifest.plug_in.u1 == 2_i) {
				exchange_unit_constant(data, 0_iu32);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                   data,
			List<typename Manifest::AudioSourceSetting> const & manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					process_section_sub(data, manifest);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                data,
			typename Manifest::BusAutoDuckingSetting const & auto_ducking_manifest
		) -> Void {
			exchange_unit_integer<IntegerU32>(data, auto_ducking_manifest.recovery_time);
			exchange_unit_floating<Floating32>(data, auto_ducking_manifest.maximum_ducking_volume);
			exchange_unit_list<IntegerU32>(
				data,
				auto_ducking_manifest.bus,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.id);
					exchange_unit_floating<Floating32>(data, manifest.volume);
					exchange_unit_integer<IntegerS32>(data, manifest.fade_out);
					exchange_unit_integer<IntegerS32>(data, manifest.fade_in);
					exchange_unit_enumeration<IntegerU8>(data, manifest.curve);
					exchange_unit_enumeration<IntegerU8>(data, manifest.target);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                data,
			typename Manifest::AudioBusConfiguration const & bus_configuration_manifest
		) -> Void {
			exchange_unit_integer<IntegerU32>(data, bus_configuration_manifest.u1);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                        data,
			typename Manifest::BusHDRSetting const & hdr_manifest
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				hdr_manifest.enable,
				hdr_manifest.u1
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                           data,
			typename Manifest::AudioTimeSetting const & time_setting_manifest,
			Boolean const &                             time_setting_override
		) -> Void {
			// todo : test frequency mode-preset time and offset
			// note : time = 960000 * signature / tempo, then with frequency mode-preset
			exchange_unit_floating<Floating64>(data, time_setting_manifest.time);
			// note : 0 if mode.no, millisecond if mode.custom, else by mode-preset
			exchange_unit_floating<Floating64>(data, time_setting_manifest.offset);
			exchange_unit_floating<Floating32>(data, time_setting_manifest.tempo);
			exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.template get<1_ix>());
			exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.template get<2_ix>());
			exchange_unit_bitset<IntegerU8>(data, time_setting_override);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                         data,
			typename Manifest::MusicTrackClip const & clip_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				clip_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
					exchange_unit_id(data, manifest.source);
					exchange_unit_id(data, manifest.event);
					exchange_unit_floating<Floating64>(data, manifest.offset);
					exchange_unit_floating<Floating64>(data, manifest.begin);
					exchange_unit_floating<Floating64>(data, manifest.end);
					exchange_unit_floating<Floating64>(data, manifest.duration);
				}
			);
			if (!clip_manifest.item.empty()) {
				exchange_unit_integer<IntegerU32>(data, clip_manifest.u1);
			}
			exchange_unit_list<IntegerU32>(
				data,
				clip_manifest.curve,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.index);
					exchange_unit_enumeration<IntegerU32>(data, manifest.type);
					exchange_unit_list<IntegerU32>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                              data,
			typename Manifest::MusicStingerSetting const & stinger_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				stinger_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.trigger);
					exchange_unit_id(data, manifest.segment_to_play);
					exchange_unit_enumeration<IntegerU32>(data, manifest.play_at);
					exchange_unit_id(data, manifest.cue_name);
					exchange_unit_integer<IntegerU32>(data, manifest.do_not_play_this_stinger_again_for);
					exchange_unit_bitset<IntegerU32>(data, manifest.allow_playing_stinger_in_next_segment);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                 data,
			typename Manifest::MusicTransitionSetting const & transition_manifest
		) -> Void {
			constexpr auto process_fade =
				[] (
				OByteStreamView &                                         data,
				typename Manifest::MusicTransitionSettingItemFade const & manifest
			) {
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.time);
				// note : always default = 4 if non-fade
				exchange_unit_integer<IntegerU32>(data, manifest.curve);
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.offset);
				return;
			};
			exchange_unit_list<IntegerU32>(
				data,
				transition_manifest.item,
				[] (auto & count) {
				},
				// NOTE : avoid clang bug
				[&process_fade] (auto & data, auto & manifest) {
					exchange_unit_constant(data, 1_iu32);
					exchange_unit_id(data, manifest.source.id);
					exchange_unit_constant(data, 1_iu32);
					exchange_unit_id(data, manifest.destination.id);
					process_fade(data, manifest.source.fade_out);
					exchange_unit_enumeration<IntegerU32>(data, manifest.source.exit_source_at);
					exchange_unit_id(data, manifest.source.exit_source_at_custom_cue_match);
					exchange_unit_bitset<IntegerU8>(data, manifest.source.play_post_exit);
					process_fade(data, manifest.destination.fade_in);
					exchange_unit_id(data, manifest.destination.custom_cue_filter_match_target);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_integer<IntegerU16>(data, manifest.destination.jump_to);
					exchange_unit_integer<IntegerU16>(data, manifest.destination.sync_to);
					exchange_unit_bitset<IntegerU8>(data, manifest.destination.play_pre_entry);
					exchange_unit_bitset<IntegerU8>(data, manifest.destination.custom_cue_filter_match_source_cue_name);
					exchange_unit_bitset<IntegerU8>(data, manifest.segment.enable);
					if (manifest.segment.enable) {
						exchange_unit_id(data, manifest.segment.id);
						process_fade(data, manifest.segment.fade_in);
						process_fade(data, manifest.segment.fade_out);
						exchange_unit_bitset<IntegerU8>(data, manifest.segment.play_pre_entry);
						exchange_unit_bitset<IntegerU8>(data, manifest.segment.play_post_exit);
					}
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                      data,
			typename Manifest::MusicTrackTransitionSetting const & transition_manifest
		) -> Void {
			constexpr auto process_fade =
				[] (
				OByteStreamView &                                         data,
				typename Manifest::MusicTransitionSettingItemFade const & manifest
			) {
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.time);
				// note : always default = 4 if non-fade
				exchange_unit_integer<IntegerU32>(data, manifest.curve);
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.offset);
				return;
			};
			exchange_unit_constant(data, 1_iu32);
			exchange_unit_id(data, transition_manifest.switcher);
			process_fade(data, transition_manifest.source.fade_out);
			exchange_unit_enumeration<IntegerU32>(data, transition_manifest.source.exit_source_at);
			exchange_unit_id(data, transition_manifest.source.exit_source_at_custom_cue_match);
			process_fade(data, transition_manifest.destination.fade_in);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                               data,
			typename Manifest::AudioSwitcherSetting const & switcher_manifest
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, switcher_manifest.is_state);
			exchange_unit_id(data, switcher_manifest.group);
			exchange_unit_id(data, switcher_manifest.default_item);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                          data,
			typename Manifest::AudioSwitcherAssociationSetting const & switcher_association_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				switcher_association_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.group);
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_bitset<IntegerU8>(data, manifest.is_state);
				}
			);
			exchange_unit_list<IntegerU32>(
				data,
				switcher_association_manifest.path,
				[&] (auto & count) {
					// NOTE : here
					count *= 12_sz;
					exchange_unit_bitset<IntegerU8>(
						data,
						switcher_association_manifest.mode
					);
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.u1);
					exchange_unit_id(data, manifest.object);
					exchange_unit_integer<IntegerU16>(data, manifest.weight);
					exchange_unit_integer<IntegerU16>(data, manifest.probability);
				}
			);
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
			exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop_count.value);
			exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop_count.minimum_value);
			exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop_count.maximum_value);
			exchange_unit_floating<Floating32>(data, play_mode_setting_manifest.continuous.transition_duration.value);
			exchange_unit_floating<Floating32>(data, play_mode_setting_manifest.continuous.transition_duration.minimum_value);
			exchange_unit_floating<Floating32>(data, play_mode_setting_manifest.continuous.transition_duration.maximum_value);
			exchange_unit_integer<IntegerU16>(data, play_type_setting_manifest.random.avoid_repeat);
			exchange_unit_enumeration<IntegerU8>(data, play_mode_setting_manifest.continuous.transition_type);
			exchange_unit_bitset<IntegerU8>(data, play_type_setting_manifest.random.type);
			exchange_unit_bitset<IntegerU8>(data, play_type_manifest);
			exchange_unit_bitset<IntegerU8>(
				data,
				k_false,
				play_mode_setting_manifest.continuous.always_reset_playlist,
				play_type_setting_manifest.sequence.at_end_of_playlist,
				play_mode_manifest,
				scope_manifest
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                   data,
			List<typename Manifest::SoundPlaylistContainerPlaylistItem> const & sequence_container_playlist_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				sequence_container_playlist_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.item);
					exchange_unit_integer<IntegerU32>(data, manifest.weight);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                        data,
			List<typename Manifest::SoundSwitchContainerObjectAttributeItem> const & switch_container_object_attribute_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				switch_container_object_attribute_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.id);
					exchange_unit_bitset<IntegerU8>(data, manifest.play_first_only, manifest.continue_to_play_across_switch);
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_integer<IntegerU32>(data, manifest.fade_out_time);
					exchange_unit_integer<IntegerU32>(data, manifest.fade_in_time);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                     data,
			List<typename Manifest::SoundSwitchContainerObjectAssignItem> const & switch_container_assigned_object_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				switch_container_assigned_object_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.item);
					exchange_unit_list<IntegerU32>(
						data,
						manifest.object,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_id(data, manifest);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                             data,
			List<typename Manifest::SoundBlendContainerTrackItem> const & blend_container_track_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				blend_container_track_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.id);
					process_section_sub(data, manifest.real_time_parameter_control);
					exchange_unit_id(data, manifest.cross_fade);
					exchange_unit_enumeration<IntegerU8>(data, manifest.cross_fade_category);
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                                   data,
			List<typename Manifest::MusicPlaylistContainerPlaylistItem> const & music_playlist_container_playlist_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				music_playlist_container_playlist_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.item);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_integer<IntegerU32>(data, manifest.child_count);
					{
						// NOTE : here
						assert_test(manifest.play_type.has() == manifest.play_mode.has());
						if (manifest.play_type.has()) {
							exchange_unit_bitset<IntegerU32>(
								data,
								manifest.play_mode.get(),
								manifest.play_type.get()
							);
						} else {
							exchange_unit_constant(data, ~0_iu32);
						}
					}
					exchange_unit_integer<IntegerU16>(data, manifest.loop_count);
					exchange_unit_constant(data, 0_iu32);
					exchange_unit_integer<IntegerU32>(data, manifest.weight);
					exchange_unit_integer<IntegerU16>(data, manifest.random_setting.avoid_repeat);
					exchange_unit_bitset<IntegerU8>(data, manifest.group); // todo : maybe
					exchange_unit_bitset<IntegerU8>(data, manifest.random_setting.type);
				}
			);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                                      data,
			typename Manifest::MusicTrackTrackType const &         track_type_manifest,
			typename Manifest::AudioSwitcherSetting const &        switcher_manifest,
			typename Manifest::MusicTrackTransitionSetting const & transition_manifest
		) -> Void {
			exchange_unit_enumeration<IntegerU8>(data, track_type_manifest);
			if (track_type_manifest == Manifest::MusicTrackTrackType::Constant::switcher()) {
				process_section_sub(data, switcher_manifest);
				process_section_sub(data, transition_manifest);
			}
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                           data,
			typename Manifest::MusicTrackStream const & stream_manifest
		) -> Void {
			exchange_unit_integer<IntegerU16>(data, stream_manifest.look_ahead_time);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                        data,
			typename Manifest::AudioPlayMode const & play_mode_manifest
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, play_mode_manifest);
			return;
		}

		static auto process_section_sub (
			OByteStreamView &                          data,
			typename Manifest::MusicSegmentCue const & cue_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				cue_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.name);
					exchange_unit_floating<Floating64>(data, manifest.time);
					exchange_unit_constant(data, 0_iu8);
				}
			);
			return;
		}

		// ----------------

		static auto process_section (
			OByteStreamView &                     data,
			typename Manifest::StateGroup const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_integer<IntegerU32>(data, manifest.default_transition);
			exchange_unit_list<IntegerU32>(
				data,
				manifest.custom_transition,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.from);
					exchange_unit_id(data, manifest.to);
					exchange_unit_integer<IntegerS32>(data, manifest.time);
				}
			);
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::SwitchGroup const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_id(data, manifest.parameter);
			exchange_unit_enumeration<IntegerU8>(data, manifest.parameter_category);
			exchange_unit_list<IntegerU32>(
				data,
				manifest.point,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_floating<Floating32>(data, manifest.position.x);
					exchange_unit_id(data, manifest.position.y);
					exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
				}
			);
			return;
		}

		static auto process_section (
			OByteStreamView &                        data,
			typename Manifest::GameParameter const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_floating<Floating32>(data, manifest.range_default);
			exchange_unit_integer<IntegerU32>(data, manifest.interpolation_mode);
			exchange_unit_floating<Floating32>(data, manifest.interpolation_attack);
			exchange_unit_floating<Floating32>(data, manifest.interpolation_release);
			exchange_unit_integer<IntegerU8>(data, manifest.bind_to_built_in_parameter);
			return;
		}

		static auto process_section (
			OByteStreamView &                                data,
			typename Manifest::GameSynchronizationU1 const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_floating<Floating32>(data, manifest.u1);
			exchange_unit_floating<Floating32>(data, manifest.u2);
			exchange_unit_floating<Floating32>(data, manifest.u3);
			exchange_unit_floating<Floating32>(data, manifest.u4);
			exchange_unit_floating<Floating32>(data, manifest.u5);
			exchange_unit_floating<Floating32>(data, manifest.u6);
			return;
		}

		static auto process_section (
			OByteStreamView &                                data,
			typename Manifest::SourcePlugInReference const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_plug_in_id(data, manifest.plug_in);
			{
				// NOTE : here
				exchange_unit_size<IntegerU32>(data, manifest.expand.size());
				data.write(manifest.expand);
			}
			exchange_unit_constant(data, 0_iu8);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u2,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_integer<IntegerU8>(data, manifest.u3);
					exchange_unit_integer<IntegerU8>(data, manifest.u4);
					exchange_unit_integer<IntegerU32>(data, manifest.u5);
					exchange_unit_integer<IntegerU8>(data, manifest.u6);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			exchange_unit_constant(data, 0_iu16);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u4,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_floating<Floating32>(data, manifest.u3);
				}
			);
			return;
		}

		static auto process_section (
			OByteStreamView &                                 data,
			typename Manifest::UnknownPlugInReference const & manifest
		) -> Void {
			return process_section(data, self_cast<typename Manifest::SourcePlugInReference>(manifest));
		}

		static auto process_section (
			OByteStreamView &                                       data,
			typename Manifest::StatefulAudioPropertySetting const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.value.as_list(),
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_enumeration<IntegerU16>(data, manifest.key);
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_floating<Floating32>(data, manifest.value);
				}
			);
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::EventAction const & manifest
		) -> Void {
			// NOTE : here
			exchange_unit_id(data, manifest.id);
			auto scope_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU8>())};
			auto type_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU8>())};
			auto scope = Size{};
			auto type = Size{};
			exchange_unit_id(data, manifest.target);
			exchange_unit_integer<IntegerU8>(data, manifest.u1);
			auto common_property = CommonPropertyMap<EventActionCommonPropertyType>{};
			using CPTC = typename EventActionCommonPropertyType::Constant;
			auto process_section_sub_of_exception_list =
				[&] (
			) {
				exchange_unit_list<IntegerU8>(
					data,
					manifest.exception,
					[] (auto & size) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_id(data, manifest.id);
						exchange_unit_bitset<IntegerU8>(data, manifest.u1);
					}
				);
			};
			auto has_case = k_false;
			if constexpr (version.number >= 140_i) {
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::play_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::play_audio()>();
					type = 4_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						exchange_common_property<CPTC::probability()>(common_property, property_manifest.probability);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_id(data, property_manifest.sound_bank);
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::stop_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::stop_audio()>();
					type = 1_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(
						data,
						k_false,
						property_manifest.resume_state_transition,
						property_manifest.apply_to_dynamic_sequence
					);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::pause_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::pause_audio()>();
					type = 2_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.include_delayed_resume_action,
						property_manifest.resume_state_transition,
						property_manifest.apply_to_dynamic_sequence
					);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::resume_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::resume_audio()>();
					type = 3_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.master_resume,
						property_manifest.resume_state_transition,
						property_manifest.apply_to_dynamic_sequence
					);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::break_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::break_audio()>();
					type = 28_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::seek_audio()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::seek_audio()>();
					type = 30_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.seek_type);
					exchange_unit_floating<Floating32>(data, property_manifest.seek_value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.seek_value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.seek_value.maximum_value);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.seek_to_nearest_marker
					);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::post_event()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::post_event()>();
					type = 33_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_pitch()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_pitch()>();
					type = property_manifest.reset ? 9_sz : 8_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_volume()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_volume()>();
					type = property_manifest.reset ? 11_sz : 10_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_bus_volume()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_bus_volume()>();
					type = property_manifest.reset ? 13_sz : 12_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_low_pass_filter()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_low_pass_filter()>();
					type = property_manifest.reset ? 15_sz : 14_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_voice_high_pass_filter()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_high_pass_filter()>();
					type = property_manifest.reset ? 48_sz : 32_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_mute()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_mute()>();
					type = property_manifest.reset ? 7_sz : 6_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_game_parameter()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_game_parameter()>();
					type = property_manifest.reset ? 20_sz : 19_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(data, property_manifest.bypass_game_parameter_interpolation);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_state_availability()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_state_availability()>();
					type = property_manifest.enable ? 16_sz : 17_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::activate_state()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::activate_state()>();
					type = 18_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_id(data, property_manifest.group);
					exchange_unit_id(data, property_manifest.item);
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::activate_switch()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::activate_switch()>();
					type = 25_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					exchange_unit_id(data, property_manifest.group);
					exchange_unit_id(data, property_manifest.item);
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::activate_trigger()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::activate_trigger()>();
					type = 29_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::set_bypass_effect()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::set_bypass_effect()>();
					type = property_manifest.reset ? 27_sz : 26_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					assert_test(property_manifest.value.size() == 5_sz);
					exchange_unit_bitset<IntegerU8>(data, property_manifest.enable);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.value[5_ix],
						property_manifest.value[1_ix],
						property_manifest.value[2_ix],
						property_manifest.value[3_ix],
						property_manifest.value[4_ix],
						as_constant(property_manifest.reset),
						as_constant(property_manifest.reset),
						as_constant(property_manifest.reset)
					);
					process_section_sub_of_exception_list();
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::release_envelope()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::release_envelope()>();
					type = 31_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					has_case = k_true;
				}
				if (manifest.property.type() == Manifest::EventActionProperty::Type::Constant::reset_playlist()) {
					auto & property_manifest = manifest.property.template get_of_type<Manifest::EventActionProperty::Type::Constant::reset_playlist()>();
					type = 34_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						process_section_sub(data, common_property, k_true);
					}
					// todo : maybe fade curve ?
					exchange_unit_integer<IntegerU8>(data, property_manifest.u1);
					exchange_unit_constant(data, 0_iu8);
					has_case = k_true;
				}
			}
			assert_test(has_case);
			if (!manifest.globally) {
				scope = manifest.target == 0_i ? 5_sz : 3_sz;
			} else {
				scope = manifest.target == 0_i ? 4_sz : 2_sz;
			}
			if (type == 4_sz) {
				// play
				assert_test(scope == 3_sz);
			}
			if (type == 33_sz) {
				// post_event
				assert_test(scope == 3_sz);
			}
			if (type == 16_sz || type == 17_sz) {
				// enable/disable state
				assert_test(scope == 2_sz);
			}
			if (type == 18_sz) {
				// set state
				assert_test(scope == 2_sz);
				// todo : maybe wwise bug?
				scope = 4_sz;
			}
			if (type == 25_sz) {
				// set switch
				assert_test(scope == 3_sz);
				// 1 is only used for set_switch
				// todo : maybe wwise bug?
				scope = 1_sz;
			}
			exchange_unit_size<IntegerU8>(scope_data, scope);
			exchange_unit_size<IntegerU8>(type_data, type);
			return;
		}

		static auto process_section (
			OByteStreamView &                data,
			typename Manifest::Event const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_list<IntegerU8>(
				data,
				manifest.child,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest);
				}
			);
			return;
		}

		static auto process_section (
			OByteStreamView &                        data,
			typename Manifest::DialogueEvent const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_integer<IntegerU8>(data, manifest.probability);
			process_section_sub(data, manifest.switcher_association);
			exchange_unit_constant(data, 0_iu16);
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::Attenuation const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_bitset<IntegerU8>(data, manifest.height_spread);
			{
				// NOTE : here
				exchange_unit_bitset<IntegerU8>(data, manifest.cone.has());
				if (manifest.cone.has()) {
					exchange_unit_floating<Floating32>(data, manifest.cone.get().inner_angle);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().outer_angle);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().max_value);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().low_pass_filter);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().high_pass_filter);
				}
			}
			{
				// NOTE : here
				auto process_curve_index =
					[&] (
					auto & value
				) {
					auto raw_value = IntegerU8{};
					if (!value.has()) {
						raw_value = ~0_iu8;
					} else {
						raw_value = cbw<IntegerU8>(value.get());
					}
					exchange_unit_raw(data, raw_value);
				};
				process_curve_index(manifest.apply.output_bus_volume);
				process_curve_index(manifest.apply.game_defined_auxiliary_send_volume);
				process_curve_index(manifest.apply.user_defined_auxiliary_send_volume);
				process_curve_index(manifest.apply.low_pass_filter);
				process_curve_index(manifest.apply.high_pass_filter);
				process_curve_index(manifest.apply.spread);
				process_curve_index(manifest.apply.focus);
			}
			exchange_unit_list<IntegerU8>(
				data,
				manifest.curve,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			process_section_sub(data, manifest.real_time_parameter_control);
			return;
		}

		static auto process_section (
			OByteStreamView &                                          data,
			typename Manifest::LowFrequencyOscillatorModulator const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<ModulatorCommonPropertyType>{};
			{
				using CPTC = typename ModulatorCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::depth()>(common_property, manifest.depth);
				exchange_common_property_as_randomized<CPTC::frequency()>(common_property, manifest.frequency);
				exchange_common_property<CPTC::waveform()>(common_property, manifest.waveform);
				exchange_common_property_as_randomized<CPTC::smoothing()>(common_property, manifest.smoothing);
				exchange_common_property_as_randomized<CPTC::pulse_width_modulation()>(common_property, manifest.pulse_width_modulation);
				exchange_common_property_as_randomized<CPTC::attack()>(common_property, manifest.attack);
				exchange_common_property_as_randomized<CPTC::initial_phase_offset()>(common_property, manifest.initial_phase_offset);
				exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.real_time_parameter_control);
			return;
		}

		static auto process_section (
			OByteStreamView &                            data,
			typename Manifest::EnvelopeModulator const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<ModulatorCommonPropertyType>{};
			{
				using CPTC = typename ModulatorCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::attack_time()>(common_property, manifest.attack_time);
				exchange_common_property_as_randomized<CPTC::attack_curve()>(common_property, manifest.attack_curve);
				exchange_common_property_as_randomized<CPTC::decay_time()>(common_property, manifest.decay_time);
				exchange_common_property_as_randomized<CPTC::sustain_level()>(common_property, manifest.sustain_level);
				exchange_common_property_as_randomized<CPTC::release_time()>(common_property, manifest.release_time);
				exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
				exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
				exchange_common_property_as_randomized<CPTC::sustain_time()>(common_property, manifest.sustain_time);
				exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_after_release);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.real_time_parameter_control);
			return;
		}

		static auto process_section (
			OByteStreamView &                        data,
			typename Manifest::TimeModulator const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<ModulatorCommonPropertyType>{};
			{
				using CPTC = typename ModulatorCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.initial_delay);
				exchange_common_property_as_regular<CPTC::duration()>(common_property, manifest.duration);
				exchange_common_property_as_randomized<CPTC::loop_count()>(common_property, manifest.loop_count);
				exchange_common_property_as_randomized<CPTC::playback_rate()>(common_property, manifest.playback_rate);
				exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
				exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
				exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_at_end);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.real_time_parameter_control);
			return;
		}

		static auto process_section (
			OByteStreamView &                      data,
			typename Manifest::AudioDevice const & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_plug_in_id(data, manifest.plug_in);
			// NOTE : here
			if (manifest.plug_in.u1 == 7_i && manifest.plug_in.u2 == 174_i) {
				assert_test(manifest.expand.system.has());
				exchange_unit_size<IntegerU32>(data, 12_sz);
				exchange_unit_bitset<IntegerU8>(data, manifest.expand.system.get().allow_3d_audio);
				exchange_unit_id(data, manifest.expand.system.get().main_mix_configuration_for_binauralization);
				exchange_unit_id(data, manifest.expand.system.get().main_mix_configuration_for_home_theater);
				exchange_unit_bitset<IntegerU8>(data, manifest.expand.system.get().allow_system_audio_object);
				exchange_unit_integer<IntegerU16>(data, manifest.expand.system.get().minimum_system_audio_object_required);
			} else {
				assert_test(!manifest.expand.system.has());
				exchange_unit_size<IntegerU32>(data, 0_sz);
			}
			exchange_unit_constant(data, 0_iu8);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u2,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_integer<IntegerU8>(data, manifest.u3);
					exchange_unit_integer<IntegerU8>(data, manifest.u4);
					exchange_unit_integer<IntegerU32>(data, manifest.u5);
					exchange_unit_integer<IntegerU8>(data, manifest.u6);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			exchange_unit_constant(data, 0_iu16);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u4,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_floating<Floating32>(data, manifest.u3);
				}
			);
			process_section_sub(data, manifest.effect);
			return;
		}

		static auto process_section (
			OByteStreamView &                   data,
			typename Manifest::AudioBus const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.bus);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.playback_limit);
			}
			exchange_unit_id(data, manifest.id);
			auto override_positioning = k_true;
			auto override_game_defined_auxiliary_send = k_true;
			auto override_user_defined_auxiliary_send = k_true;
			auto override_early_reflection_auxiliary_send = k_true;
			exchange_unit_id(data, manifest.parent);
			if (manifest.parent == 0_i) {
				exchange_unit_id(data, manifest.audio_device);
			} else {
				assert_test(manifest.audio_device == find_id(0_iu32));
			}
			process_section_sub(data, common_property, k_false);
			process_section_sub(data, manifest.positioning, override_positioning);
			process_section_sub(data, manifest.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send, override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.mute_for_background_music);
			process_section_sub(data, manifest.bus_configuration);
			process_section_sub(data, manifest.hdr);
			process_section_sub(data, manifest.auto_ducking);
			process_section_sub(data, manifest.effect);
			exchange_unit_constant(data, 0_iu32);
			exchange_unit_constant(data, 0_iu16);
			process_section_sub(data, manifest.metadata);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.state);
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
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				exchange_common_property_as_randomized<CPTC::loop_count()>(common_property, manifest.playback_setting.loop_count);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.source);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			return;
		}

		static auto process_section (
			OByteStreamView &                                 data,
			typename Manifest::SoundPlaylistContainer const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.playback_setting.scope, manifest.playback_setting.type, manifest.playback_setting.type_setting, manifest.playback_setting.mode, manifest.playback_setting.mode_setting);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.playback_setting.playlist);
			return;
		}

		static auto process_section (
			OByteStreamView &                               data,
			typename Manifest::SoundSwitchContainer const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.playback_setting.switcher);
			process_section_sub(data, manifest.playback_setting.mode);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.playback_setting.object_assign);
			process_section_sub(data, manifest.playback_setting.object_attribute);
			return;
		}

		static auto process_section (
			OByteStreamView &                              data,
			typename Manifest::SoundBlendContainer const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.playback_setting.track);
			process_section_sub(data, manifest.playback_setting.mode);
			return;
		}

		static auto process_section (
			OByteStreamView &                     data,
			typename Manifest::ActorMixer const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			return;
		}

		static auto process_section (
			OByteStreamView &                     data,
			typename Manifest::MusicTrack const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.source);
			process_section_sub(data, manifest.playback_setting.clip);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.playback_setting.type, manifest.playback_setting.switcher, manifest.playback_setting.transition);
			process_section_sub(data, manifest.stream);
			exchange_unit_constant(data, 0_iu16);
			return;
		}

		static auto process_section (
			OByteStreamView &                       data,
			typename Manifest::MusicSegment const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			process_section_sub(data, manifest.stinger);
			{
				exchange_unit_floating<Floating64>(data, manifest.playback_setting.duration);
			}
			process_section_sub(data, manifest.playback_setting.cue);
			return;
		}

		static auto process_section (
			OByteStreamView &                                 data,
			typename Manifest::MusicPlaylistContainer const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			process_section_sub(data, manifest.stinger);
			process_section_sub(data, manifest.transition);
			process_section_sub(data, manifest.playback_setting.playlist);
			return;
		}

		static auto process_section (
			OByteStreamView &                               data,
			typename Manifest::MusicSwitchContainer const & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			process_section_sub(data, manifest.stinger);
			process_section_sub(data, manifest.transition);
			{
				exchange_unit_bitset<IntegerU8>(data, manifest.playback_setting.continue_playing_on_switch_change);
			}
			process_section_sub(data, manifest.playback_setting.switcher_association);
			return;
		}

		// ----------------

		static auto process_chunk_bkhd (
			OByteStreamView &                    data,
			typename Manifest::SoundBank const & manifest
		) -> Void {
			if constexpr (version.number >= 88_i) {
				data.write_constant(cbw<VersionNumber>(version.number));
				exchange_unit_id(data, manifest.id);
				exchange_unit_id(data, manifest.language);
				data.write(manifest.header_expand);
			}
			return;
		}

		static auto process_chunk_didx_data (
			OByteStreamView &                   didx_data,
			OByteStreamView &                   data_data,
			List<typename Manifest::ID> const & manifest,
			Path const &                        embedded_audio_directory,
			Size const &                        data_begin_position
		) -> Void {
			if constexpr (version.number >= 88_i) {
				exchange_unit_list<Void>(
					didx_data,
					manifest,
					[] (auto & count) {
					},
					[&] (auto & data, auto & manifest) {
						auto data_offset = Size{};
						auto data_size = Size{};
						if (manifest == 0_i) {
							data_offset = 1_ix;
							data_size = 0_sz;
						} else {
							data_data.write_space(k_null_byte, compute_padding_size(data_begin_position + data_data.position(), k_data_block_padding_size));
							data_offset = data_data.position();
							data_size = FileSystem::read_stream_file(embedded_audio_directory / "{}.wem"_sf(manifest), data_data);
						}
						exchange_unit_id(data, manifest);
						exchange_unit_size<IntegerU32>(data, data_offset);
						exchange_unit_size<IntegerU32>(data, data_size);
					}
				);
			}
			return;
		}

		static auto process_chunk_init (
			OByteStreamView &                  data,
			typename Manifest::Setting const & manifest
		) -> Void {
			if constexpr (version.number >= 140_i) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.plug_in,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_plug_in_id(data, manifest.id);
						exchange_unit_string<Void>(data, manifest.name);
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
			if constexpr (version.number >= 140_i) {
				exchange_unit_floating<Floating32>(data, manifest.volume_threshold);
				exchange_unit_integer<IntegerS16>(data, manifest.maximum_voice_instance);
				exchange_unit_constant(data, 50_iu16);
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.state_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.switch_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.game_parameter,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
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
			if constexpr (version.number >= 88_i) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						auto type = Size{};
						auto type_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU8>())};
						auto size_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU32>())};
						auto data_position = data.position();
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<typename Manifest::HierarchyType>::Index>(
							[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
								constexpr auto variant_type = mbw<typename Manifest::HierarchyType>(index);
								if constexpr (variant_type != Manifest::HierarchyType::Constant::unknown()) {
									if (manifest.index().value == index) {
										type = value_index;
										process_section(data, manifest.template get_of_type<variant_type>());
										has_case = k_true;
									}
								}
							}
						);
						if (!has_case) {
							auto & unknown_manifest = manifest.template get_of_type<Manifest::HierarchyType::Constant::unknown()>();
							type = cbw<Size>(unknown_manifest.type);
							data.write(unknown_manifest.data);
						}
						exchange_unit_size<IntegerU8>(type_data, type);
						exchange_unit_size<IntegerU32>(size_data, data.position() - data_position);
						return;
					}
				);
			}
			return;
		}

		static auto process_chunk_stid (
			OByteStreamView &                          data,
			List<typename Manifest::Reference> const & manifest
		) -> Void {
			if constexpr (version.number >= 88_i) {
				exchange_unit_constant(data, 1_iu32);
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_id(data, manifest.id);
						exchange_unit_string<IntegerU8>(data, manifest.name);
					}
				);
			}
			return;
		}

		static auto process_chunk_envs (
			OByteStreamView &                  data,
			typename Manifest::Setting const & manifest
		) -> Void {
			if constexpr (version.number >= 140_i) {
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
						exchange_unit_bitset<IntegerU8>(data, manifest.enable);
						exchange_unit_integer<IntegerU8>(data, manifest.u1);
						exchange_unit_list<IntegerU16>(
							data,
							manifest.point,
							[] (auto & count) {
							},
							[] (auto & data, auto & manifest) {
								exchange_unit_floating<Floating32>(data, manifest.position.x);
								exchange_unit_floating<Floating32>(data, manifest.position.y);
								exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
							}
						);
					}
				);
			}
			return;
		}

		static auto process_chunk_plat (
			OByteStreamView &                  data,
			typename Manifest::Setting const & manifest
		) -> Void {
			if constexpr (version.number >= 140_i) {
				exchange_unit_string<Void>(data, manifest.platform);
			}
			return;
		}

		// ----------------

		static auto process_sound_bank (
			OByteStreamView &                    sound_bank_data,
			typename Manifest::SoundBank const & sound_bank_manifest,
			Path const &                         embedded_audio_directory
		) -> Void {
			if constexpr (version.number >= 88_i) {
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
			if constexpr (version.number >= 88_i) {
				constexpr auto didx_item_structure_size = bs_static_size<IDNumber>() + bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>();
				if (!sound_bank_manifest.embedded_audio.empty()) {
					auto didx_sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto didx_chunk = OByteStreamView{sound_bank_data.forward_view(didx_item_structure_size * sound_bank_manifest.embedded_audio.size())};
					auto data_sign_data = OByteStreamView{sound_bank_data.forward_view(bs_static_size<ChunkSign>())};
					auto data_chunk = OByteStreamView{sound_bank_data.reserve_view()};
					process_chunk_didx_data(didx_chunk, data_chunk, sound_bank_manifest.embedded_audio, embedded_audio_directory, sound_bank_data.position());
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
			if constexpr (version.number >= 140_i) {
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
			if constexpr (version.number >= 88_i) {
				if (sound_bank_manifest.setting.has()) {
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
			if constexpr (version.number >= 88_i) {
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
			if constexpr (version.number >= 88_i) {
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
			if constexpr (version.number >= 88_i) {
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
			if constexpr (version.number >= 140_i) {
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
			Path const &                         embedded_audio_directory
		) -> Void {
			M_use_zps_of(sound_bank_data);
			return process_sound_bank(sound_bank_data, sound_bank_manifest, embedded_audio_directory);
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

		using typename Common::IDNumber;

		using typename Common::IDWrapper;

		using typename Common::CommonPropertyValue;

		template <typename _1>
		using CommonPropertyMap = typename Common::template CommonPropertyMap<_1>;

		using typename Common::EventActionCommonPropertyType;

		using typename Common::ModulatorCommonPropertyType;

		using typename Common::AudioCommonPropertyType;

		template <typename _1>
		using EnumerationAttribute = typename Common::template EnumerationAttribute<_1>;

		using Common::find_id;

		// ----------------

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsEnumerationWrapper<Value>)
		static auto exchange_enumeration_index (
			Size const & index_value,
			Value &      value
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
			&& (IsSame<typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>, Value, Size, IDWrapper>)
		static auto exchange_common_property (
			CommonPropertyMap<decltype(type)> const & map,
			Value &                                   value
		) -> Void {
			using CurrentEnumerationAttribute = typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>;
			if (auto element_if = map.regular.query_if(type)) {
				auto & element = element_if.get();
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, IDWrapper>) {
					value = element.value.template get<1_ix>().template get<IDWrapper>().value;
				} else if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Size>) {
					exchange_enumeration_index(element.value.template get<1_ix>().template get<Size>(), value);
				} else {
					value = element.value.template get<1_ix>().template get<Value>();
				}
			} else {
				constexpr auto default_value = CurrentEnumerationAttribute::template Element<3_ixz>::template element<1_ixz>;
				if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, IDWrapper>) {
					value = default_value.value;
				} else if constexpr (IsSame<typename CurrentEnumerationAttribute::template Element<2_ixz>::template Element<1_ixz>, Size>) {
					exchange_enumeration_index(default_value, value);
				} else {
					value = default_value;
				}
			}
			return;
		}

		template <auto type, typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsSame<Value, typename EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<2_ixz>::template Element<1_ixz>>)
			&& (IsSame<Value, Boolean, Integer, Floating>)
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
			&& (IsSame<Value, Boolean, Integer, Floating>)
		static auto exchange_common_property_as_randomized (
			CommonPropertyMap<decltype(type)> const &            map,
			typename Manifest::template RandomizedValue<Value> & value
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
			exchange_common_property_as_regular<CPTC::voice_volume()>(map, manifest.volume);
			exchange_common_property_as_regular<CPTC::voice_pitch()>(map, manifest.pitch);
			exchange_common_property_as_regular<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_regular<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioVoice &                    manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_randomized<CPTC::voice_volume()>(map, manifest.volume);
			exchange_common_property_as_randomized<CPTC::voice_pitch()>(map, manifest.pitch);
			exchange_common_property_as_randomized<CPTC::voice_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_randomized<CPTC::voice_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusVoiceVolumeGainSetting &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioVoiceVolumeGainSetting &   manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_randomized<CPTC::voice_volume_make_up_gain()>(map, manifest.make_up);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusBusSetting &                 manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::bus_volume()>(map, manifest.volume);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusOutputBusSetting &           manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioOutputBusSetting &         manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::output_bus_volume()>(map, manifest.volume);
			exchange_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, manifest.low_pass_filter);
			exchange_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, manifest.high_pass_filter);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioAuxiliarySendSetting &     manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_volume()>(map, manifest.game_defined.volume);
			exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_low_pass_filter()>(map, manifest.game_defined.low_pass_filter);
			exchange_common_property_as_regular<CPTC::game_defined_auxiliary_send_high_pass_filter()>(map, manifest.game_defined.high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_0()>(map, manifest.user_defined.item[1_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_0()>(map, manifest.user_defined.item[1_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_0()>(map, manifest.user_defined.item[1_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_1()>(map, manifest.user_defined.item[2_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_1()>(map, manifest.user_defined.item[2_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_1()>(map, manifest.user_defined.item[2_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_2()>(map, manifest.user_defined.item[3_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_2()>(map, manifest.user_defined.item[3_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_2()>(map, manifest.user_defined.item[3_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_3()>(map, manifest.user_defined.item[4_ix].volume);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_3()>(map, manifest.user_defined.item[4_ix].low_pass_filter);
			exchange_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_3()>(map, manifest.user_defined.item[4_ix].high_pass_filter);
			exchange_common_property_as_regular<CPTC::early_reflection_auxiliary_send_volume()>(map, manifest.early_reflection.volume);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioPositioningSetting &       manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::positioning_center_percent()>(map, manifest.center_percent);
			exchange_common_property<CPTC::position_speaker_panning_x()>(map, manifest.speaker_panning.position.x);
			exchange_common_property<CPTC::position_speaker_panning_y()>(map, manifest.speaker_panning.position.y);
			exchange_common_property<CPTC::position_speaker_panning_z()>(map, manifest.speaker_panning.position.z);
			exchange_common_property_as_regular<CPTC::position_listener_relative_routing_speaker_panning_div_3d_spatialization_mix()>(map, manifest.listener_relative_routing.speaker_panning_div_spatialization_3d_mix);
			exchange_common_property<CPTC::position_listener_relative_routing_attenuation_id()>(map, manifest.listener_relative_routing.attenuation.id);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::BusHDRSetting &                 manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::hdr_threshold()>(map, manifest.dynamic.threshold);
			exchange_common_property_as_regular<CPTC::hdr_ratio()>(map, manifest.dynamic.ratio);
			exchange_common_property_as_regular<CPTC::hdr_release_time()>(map, manifest.dynamic.release_time);
			exchange_common_property<CPTC::hdr_window_tap_output_game_parameter()>(map, manifest.window_top_output_game_parameter.id);
			exchange_common_property_as_regular<CPTC::hdr_window_tap_output_game_parameter_minimum()>(map, manifest.window_top_output_game_parameter.minimum);
			exchange_common_property_as_regular<CPTC::hdr_window_tap_output_game_parameter_maximum()>(map, manifest.window_top_output_game_parameter.maximum);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::AudioHDRSetting &               manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property_as_regular<CPTC::hdr_envelope_tracking_active_range()>(map, manifest.envelope_tracking.active_range);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::SoundMIDISetting &              manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property<CPTC::midi_event_play_on()>(map, manifest.event.play_on);
			exchange_common_property<CPTC::midi_note_tracking_root_note()>(map, manifest.note_tracking.root_note);
			exchange_common_property_as_regular<CPTC::midi_transformation_transposition()>(map, manifest.transformation.transposition);
			exchange_common_property_as_regular<CPTC::midi_transformation_velocity_offset()>(map, manifest.transformation.velocity_offset);
			exchange_common_property<CPTC::midi_filter_key_range_minimum()>(map, manifest.filter.key_range_minimum);
			exchange_common_property<CPTC::midi_filter_key_range_maximum()>(map, manifest.filter.key_range_maximum);
			exchange_common_property<CPTC::midi_filter_velocity_minimum()>(map, manifest.filter.velocity_minimum);
			exchange_common_property<CPTC::midi_filter_velocity_maximum()>(map, manifest.filter.velocity_maximum);
			exchange_common_property<CPTC::midi_filter_channel()>(map, manifest.filter.channel);
			return;
		}

		static auto process_common_property (
			CommonPropertyMap<AudioCommonPropertyType> const & map,
			typename Manifest::MusicMIDISetting &              manifest
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			exchange_common_property<CPTC::midi_target_id()>(map, manifest.target.id);
			exchange_common_property<CPTC::midi_clip_tempo_source()>(map, manifest.clip_tempo.source);
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
			exchange_common_property_as_regular<CPTC::playback_priority_value()>(map, manifest.value);
			exchange_common_property_as_regular<CPTC::playback_priority_offset_at_max_distance()>(map, manifest.offset_at_max_distance);
			return;
		}

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_raw (
			IByteStreamView & data,
			RawValue &        value
		) -> Void {
			data.read(value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			IByteStreamView & data,
			RawValue const &  value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			assert_test(raw_value == value);
			return;
		}

		static auto exchange_unit_id (
			IByteStreamView &       data,
			typename Manifest::ID & value
		) -> Void {
			auto raw_value = IDNumber{};
			data.read(raw_value);
			value = cbw<Integer>(raw_value);
			return;
		}

		static auto exchange_unit_plug_in_id (
			IByteStreamView &             data,
			typename Manifest::PlugInID & value
		) -> Void {
			auto raw_value = IntegerU16{};
			data.read(raw_value);
			value.u1 = cbw<Integer>(raw_value);
			data.read(raw_value);
			value.u2 = cbw<Integer>(raw_value);
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
			&& (IsFloatingWrapper<RawValue>)
		static auto exchange_unit_floating (
			IByteStreamView & data,
			Floating &        value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			value = cbw<Floating>(raw_value);
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_size (
			IByteStreamView & data,
			Size &            value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			value = cbw<Size>(raw_value);
			return;
		}

		template <typename RawValue, typename Value> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_enumeration (
			IByteStreamView & data,
			Value &           value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			auto index_value = cbw<Size>(raw_value);
			exchange_enumeration_index(index_value, value);
			return;
		}

		template <typename RawValue, typename ... Value> requires
			CategoryConstraint<IsPureInstance<RawValue> && IsInstance<Value ...>>
			&& (IsIntegerWrapper<RawValue>)
			&& ((IsSame<Value, Boolean const> || IsSame<Value, Boolean> || IsEnumerationWrapper<Value>) && ...)
		static auto exchange_unit_bitset (
			IByteStreamView & data,
			Value & ...       value
		) -> Void {
			auto raw_value = RawValue{};
			data.read(raw_value);
			auto bitset = BitSet<k_type_bit_count<RawValue>>{};
			bitset.from_integer(raw_value);
			auto current_index = k_begin_index;
			Generalization::each_with<>(
				[&] <auto index, typename CurrentValue> (ValuePackage<index>, CurrentValue & current_value) {
					if constexpr (IsSame<CurrentValue, Boolean const>) {
						assert_test(bitset.get(current_index) == current_value);
						++current_index;
					} else if constexpr (IsSame<CurrentValue, Boolean>) {
						current_value = bitset.get(current_index);
						++current_index;
					} else {
						auto index_value = Size{};
						for (auto & bit_index : SizeRange{EnumerationAttribute<CurrentValue>::size}) {
							index_value |= cbw<Size>(bitset.get(current_index)) << bit_index;
							++current_index;
						}
						exchange_enumeration_index(index_value, current_value);
					}
				},
				value ...
			);
			for (auto & index : SizeRange{k_type_bit_count<RawValue> - current_index}) {
				assert_test(!bitset.get(current_index + index));
			}
			return;
		}

		template <typename RawSizeValue> requires
			CategoryConstraint<IsPure<RawSizeValue>>
			&& (IsVoid<RawSizeValue> || IsIntegerWrapper<RawSizeValue>)
		static auto exchange_unit_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			data.read(self_cast<StringBlock<RawSizeValue>>(value));
			return;
		}

		template <typename Length, typename Element, typename LeadingParser, typename ... ElementParser> requires
			CategoryConstraint<IsPure<Length> && IsPureInstance<LeadingParser> && IsPureInstance<ElementParser ...>>
			&& (IsVoid<Length> || IsIntegerWrapper<Length>)
			&& (IsGenericCallable<LeadingParser> && (IsGenericCallable<ElementParser> && ...))
		static auto exchange_unit_list (
			IByteStreamView &         data,
			List<Element> &           list,
			LeadingParser const &     leading_parser,
			ElementParser const & ... element_parser
		) -> Void {
			auto count = Size{};
			if constexpr (!IsVoid<Length>) {
				count = cbw<Size>(data.read_of<Length>());
			} else {
				count = list.size();
			}
			leading_parser(count);
			if constexpr (!IsVoid<Length>) {
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
			&& (IsSame<ActualValue, Boolean, Integer, Floating, Size, IDWrapper>)
		static auto process_unit_common_property_value (
			IByteStreamView &     data,
			CommonPropertyValue & value
		) -> Void {
			if constexpr (IsSame<ActualValue, Boolean>) {
				exchange_unit_bitset<IntegerU32>(data, value.template set<Boolean>());
			}
			if constexpr (IsSame<ActualValue, Integer>) {
				exchange_unit_integer<IntegerS32>(data, value.template set<Integer>());
			}
			if constexpr (IsSame<ActualValue, Floating>) {
				exchange_unit_floating<Floating32>(data, value.template set<Floating>());
			}
			if constexpr (IsSame<ActualValue, Size>) {
				exchange_unit_size<IntegerU32>(data, value.template set<Size>());
			}
			if constexpr (IsSame<ActualValue, IDWrapper>) {
				exchange_unit_id(data, value.template set<IDWrapper>().value);
			}
			return;
		}

		// ----------------

		template <typename Type> requires
			CategoryConstraint<IsPureInstance<Type>>
			&& (IsEnumerationWrapper<Type>)
		static auto process_section_sub (
			IByteStreamView &         data,
			CommonPropertyMap<Type> & map,
			Boolean const &           exist_randomizer
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				map.regular.as_list(),
				[] (auto & count) {
				},
				[] (auto & data, auto & element) {
					auto type = Size{};
					exchange_unit_size<IntegerU8>(data, type);
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
							constexpr auto case_index = Attribute::template Element<1_ixz>::template element<1_ixz>;
							if (type == case_index) {
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
			if (exist_randomizer) {
				exchange_unit_list<IntegerU8>(
					data,
					map.randomizer.as_list(),
					[] (auto & count) {
					},
					[] (auto & data, auto & element) {
						auto type = Size{};
						exchange_unit_size<IntegerU8>(data, type);
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) -> auto {
								constexpr auto case_index = Attribute::template Element<1_ixz>::template element<1_ixz>;
								if (type == case_index) {
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
			return;
		}

		template <typename Length> requires
			CategoryConstraint<IsPureInstance<Length>>
			&& (IsIntegerWrapper<Length>)
		static auto process_section_sub (
			IByteStreamView &             data,
			List<typename Manifest::ID> & id
		) -> Void {
			exchange_unit_list<IntegerU32>(
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
			IByteStreamView &                                data,
			typename Manifest::AudioVoiceVolumeGainSetting & voice_volume_gain_manifest,
			typename Manifest::AudioHDRSetting &             hdr_manifest,
			Boolean &                                        voice_volume_loudness_normalization_override,
			Boolean &                                        hdr_envelope_tracking_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				hdr_envelope_tracking_override,
				voice_volume_loudness_normalization_override,
				voice_volume_gain_manifest.normalization,
				hdr_manifest.envelope_tracking.enable
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                          data,
			typename Manifest::AudioOutputBusSetting & output_bus_manifest
		) -> Void {
			exchange_unit_id(data, output_bus_manifest.bus);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                              data,
			typename Manifest::AudioAuxiliarySendSetting & auxiliary_send_manifest,
			Boolean &                                      game_defined_auxiliary_send_override,
			Boolean &                                      user_defined_auxiliary_send_override,
			Boolean &                                      early_reflection_auxiliary_send_override
		) -> Void {
			auxiliary_send_manifest.user_defined.item.allocate_full(4_sz);
			exchange_unit_bitset<IntegerU8>(
				data,
				game_defined_auxiliary_send_override,
				auxiliary_send_manifest.game_defined.enable,
				user_defined_auxiliary_send_override,
				auxiliary_send_manifest.user_defined.enable,
				early_reflection_auxiliary_send_override
			);
			if (auxiliary_send_manifest.user_defined.enable) {
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[1_ix].bus);
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[2_ix].bus);
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[3_ix].bus);
				exchange_unit_id(data, auxiliary_send_manifest.user_defined.item[4_ix].bus);
			} else {
				auxiliary_send_manifest.user_defined.item[1_ix].bus = find_id(0_iu32);
				auxiliary_send_manifest.user_defined.item[2_ix].bus = find_id(0_iu32);
				auxiliary_send_manifest.user_defined.item[3_ix].bus = find_id(0_iu32);
				auxiliary_send_manifest.user_defined.item[4_ix].bus = find_id(0_iu32);
			}
			exchange_unit_id(data, auxiliary_send_manifest.early_reflection.bus);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                            data,
			typename Manifest::AudioPositioningSetting & positioning_manifest,
			Boolean &                                    positioning_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				positioning_override,
				positioning_manifest.listener_relative_routing.enable,
				positioning_manifest.speaker_panning.mode,
				k_false,
				positioning_manifest.listener_relative_routing.position_3d.mode,
				k_false
			);
			if (positioning_manifest.listener_relative_routing.enable) {
				exchange_unit_bitset<IntegerU8>(
					data,
					positioning_manifest.listener_relative_routing.spatialization_3d,
					k_false,
					positioning_manifest.listener_relative_routing.attenuation.enable,
					positioning_manifest.listener_relative_routing.position_3d.hold_emitter_position_and_orientation,
					positioning_manifest.listener_relative_routing.position_3d.hold_listener_orientation,
					positioning_manifest.listener_relative_routing.position_3d.automation.loop,
					positioning_manifest.listener_relative_routing.position_3d.diffraction_and_transmission
				);
			}
			if (positioning_manifest.listener_relative_routing.position_3d.mode != Manifest::AudioPositioningSettingListenerRelativeRoutingPosition3DMode::Constant::emitter()) {
				exchange_unit_bitset<IntegerU8>(
					data,
					positioning_manifest.listener_relative_routing.position_3d.automation.play_type,
					positioning_manifest.listener_relative_routing.position_3d.automation.play_mode,
					positioning_manifest.listener_relative_routing.position_3d.automation.pick_new_path_when_sound_start
				);
				exchange_unit_integer<IntegerU32>(data, positioning_manifest.listener_relative_routing.position_3d.automation.transition_time);
				exchange_unit_list<IntegerU32>(
					data,
					positioning_manifest.listener_relative_routing.position_3d.automation.point,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_floating<Floating32>(data, manifest.position.x);
						exchange_unit_floating<Floating32>(data, manifest.position.z);
						exchange_unit_floating<Floating32>(data, manifest.position.y);
						exchange_unit_integer<IntegerS32>(data, manifest.duration);
					}
				);
				exchange_unit_list<IntegerU32>(
					data,
					positioning_manifest.listener_relative_routing.position_3d.automation.path,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_integer<IntegerU32>(data, manifest.point.begin);
						exchange_unit_integer<IntegerU32>(data, manifest.point.count);
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_floating<Floating32>(data, manifest.random_range.left_right);
						exchange_unit_floating<Floating32>(data, manifest.random_range.front_back);
						exchange_unit_floating<Floating32>(data, manifest.random_range.up_down);
					}
				);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                     data,
			typename Manifest::MusicMIDISetting & midi_manifest,
			Boolean &                             midi_target_override,
			Boolean &                             midi_clip_tempo_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				k_false,
				midi_clip_tempo_override,
				midi_target_override
			);
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
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_priority_override,
				playback_priority_manifest.use_distance_factor,
				midi_event_override,
				midi_note_tracking_override,
				midi_manifest.note_tracking.enable,
				midi_manifest.event.break_on_note_off
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                 data,
			typename Manifest::AudioPlaybackPrioritySetting & playback_priority_manifest,
			Boolean &                                         playback_priority_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_priority_override,
				playback_priority_manifest.use_distance_factor
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                              data,
			typename Manifest::AudioPlaybackLimitSetting & playback_limit_manifest,
			typename Manifest::AudioVirtualVoiceSetting &  virtual_voice_manifest,
			Boolean &                                      playback_limit_override,
			Boolean &                                      virtual_voice_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_limit_manifest.when_priority_is_equal,
				playback_limit_manifest.when_limit_is_reached,
				playback_limit_manifest.scope,
				playback_limit_override,
				virtual_voice_override
			);
			exchange_unit_enumeration<IntegerU8>(data, virtual_voice_manifest.on_return_to_physical);
			exchange_unit_integer<IntegerS16>(data, playback_limit_manifest.value.value);
			exchange_unit_enumeration<IntegerU8>(data, virtual_voice_manifest.behavior);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                   data,
			typename Manifest::AudioPlaybackLimitSetting &      playback_limit_manifest,
			typename Manifest::AudioBusMuteForBackgroundMusic & mute_for_background_music_manifest
		) -> Void {
			auto b3 = Boolean{}; // todo
			exchange_unit_bitset<IntegerU8>(
				data,
				playback_limit_manifest.when_priority_is_equal,
				playback_limit_manifest.when_limit_is_reached,
				b3,
				mute_for_background_music_manifest.enable
			);
			exchange_unit_integer<IntegerS16>(data, playback_limit_manifest.value.value);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                       data,
			typename Manifest::AudioEffectSetting & effect_manifest
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				effect_manifest.item,
				[&] (auto & count) {
					effect_manifest.bypass.allocate_full(5_sz);
					if (count > 0_sz) {
						exchange_unit_bitset<IntegerU8>(
							data,
							effect_manifest.bypass[1_ix],
							effect_manifest.bypass[2_ix],
							effect_manifest.bypass[3_ix],
							effect_manifest.bypass[4_ix],
							effect_manifest.bypass[5_ix]
						);
					} else {
						effect_manifest.bypass[1_ix] = k_false;
						effect_manifest.bypass[2_ix] = k_false;
						effect_manifest.bypass[3_ix] = k_false;
						effect_manifest.bypass[4_ix] = k_false;
						effect_manifest.bypass[5_ix] = k_false;
					}
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.index);
					exchange_unit_id(data, manifest.id);
					// todo : in typical, render = 1 -> mode = 0 & u2 = 1, render = 0 -> mode = 1 & u2 = 0
					// todo : if render, mode value will be changed ?
					exchange_unit_bitset<IntegerU8>(data, manifest.use_share_set);
					exchange_unit_bitset<IntegerU8>(data, manifest.u1);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                       data,
			typename Manifest::AudioEffectSetting & effect_manifest,
			Boolean &                               effect_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, effect_override);
			process_section_sub(data, effect_manifest);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                         data,
			typename Manifest::AudioMetadataSetting & metadata_manifest
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				metadata_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.index);
					exchange_unit_id(data, manifest.id);
					exchange_unit_bitset<IntegerU8>(data, manifest.use_share_set);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                         data,
			typename Manifest::AudioMetadataSetting & metadata_manifest,
			Boolean &                                 metadata_override
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, metadata_override);
			process_section_sub(data, metadata_manifest);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                               data,
			typename Manifest::AttenuationRealTimeParameterControlSetting & real_time_parameter_control_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				real_time_parameter_control_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.x_axis);
					exchange_unit_enumeration<IntegerU8>(data, manifest.x_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_type);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_bitset<IntegerU8>(
						data,
						manifest.u2,
						manifest.u3
					);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                             data,
			typename Manifest::ModulatorRealTimeParameterControlSetting & real_time_parameter_control_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				real_time_parameter_control_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.x_axis);
					exchange_unit_enumeration<IntegerU8>(data, manifest.x_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_type);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_bitset<IntegerU8>(
						data,
						manifest.u2,
						manifest.u3
					);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                         data,
			typename Manifest::AudioRealTimeParameterControlSetting & real_time_parameter_control_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				real_time_parameter_control_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.x_axis);
					exchange_unit_enumeration<IntegerU8>(data, manifest.x_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_category);
					exchange_unit_enumeration<IntegerU8>(data, manifest.y_axis_type);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_bitset<IntegerU8>(
						data,
						manifest.u2,
						manifest.u3
					);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                      data,
			typename Manifest::AudioStateSetting & state_manifest
		) -> Void {
			exchange_unit_list<IntegerU8>(
				data,
				state_manifest.attribute,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_enumeration<IntegerU8>(data, manifest.type);
					exchange_unit_enumeration<IntegerU8>(data, manifest.category);
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
				}
			);
			exchange_unit_list<IntegerU8>(
				data,
				state_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.group);
					exchange_unit_enumeration<IntegerU8>(data, manifest.change_occur_at);
					exchange_unit_list<IntegerU8>(
						data,
						manifest.apply,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_id(data, manifest.target);
							exchange_unit_id(data, manifest.setting);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                       data,
			typename Manifest::AudioSourceSetting & manifest
		) -> Void {
			exchange_unit_plug_in_id(data, manifest.plug_in);
			assert_test(manifest.plug_in.u1 == 0_i || manifest.plug_in.u1 == 1_i || manifest.plug_in.u1 == 2_i);
			exchange_unit_enumeration<IntegerU8>(data, manifest.type);
			exchange_unit_id(data, manifest.resource);
			// note : if external_prefetch, is internal data size
			exchange_unit_integer<IntegerU32>(data, manifest.resource_size);
			exchange_unit_bitset<IntegerU8>(
				data,
				k_false,
				k_false,
				k_false,
				manifest.non_cachable_stream
			);
			// todo
			if (manifest.plug_in.u1 == 2_i) {
				exchange_unit_constant(data, 0_iu32);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                             data,
			List<typename Manifest::AudioSourceSetting> & manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					process_section_sub(data, manifest);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                          data,
			typename Manifest::BusAutoDuckingSetting & auto_ducking_manifest
		) -> Void {
			exchange_unit_integer<IntegerU32>(data, auto_ducking_manifest.recovery_time);
			exchange_unit_floating<Floating32>(data, auto_ducking_manifest.maximum_ducking_volume);
			exchange_unit_list<IntegerU32>(
				data,
				auto_ducking_manifest.bus,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.id);
					exchange_unit_floating<Floating32>(data, manifest.volume);
					exchange_unit_integer<IntegerS32>(data, manifest.fade_out);
					exchange_unit_integer<IntegerS32>(data, manifest.fade_in);
					exchange_unit_enumeration<IntegerU8>(data, manifest.curve);
					exchange_unit_enumeration<IntegerU8>(data, manifest.target);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                          data,
			typename Manifest::AudioBusConfiguration & bus_configuration_manifest
		) -> Void {
			exchange_unit_integer<IntegerU32>(data, bus_configuration_manifest.u1);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                  data,
			typename Manifest::BusHDRSetting & hdr_manifest
		) -> Void {
			exchange_unit_bitset<IntegerU8>(
				data,
				hdr_manifest.enable,
				hdr_manifest.u1
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                     data,
			typename Manifest::AudioTimeSetting & time_setting_manifest,
			Boolean &                             time_setting_override
		) -> Void {
			// todo : test frequency mode-preset time and offset
			// note : time = 960000 * signature / tempo, then with frequency mode-preset
			exchange_unit_floating<Floating64>(data, time_setting_manifest.time);
			// note : 0 if mode.no, millisecond if mode.custom, else by mode-preset
			exchange_unit_floating<Floating64>(data, time_setting_manifest.offset);
			exchange_unit_floating<Floating32>(data, time_setting_manifest.tempo);
			exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.template get<1_ix>());
			exchange_unit_integer<IntegerU8>(data, time_setting_manifest.signature.template get<2_ix>());
			exchange_unit_bitset<IntegerU8>(data, time_setting_override);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                   data,
			typename Manifest::MusicTrackClip & clip_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				clip_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
					exchange_unit_id(data, manifest.source);
					exchange_unit_id(data, manifest.event);
					exchange_unit_floating<Floating64>(data, manifest.offset);
					exchange_unit_floating<Floating64>(data, manifest.begin);
					exchange_unit_floating<Floating64>(data, manifest.end);
					exchange_unit_floating<Floating64>(data, manifest.duration);
				}
			);
			if (!clip_manifest.item.empty()) {
				exchange_unit_integer<IntegerU32>(data, clip_manifest.u1);
			}
			exchange_unit_list<IntegerU32>(
				data,
				clip_manifest.curve,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.index);
					exchange_unit_enumeration<IntegerU32>(data, manifest.type);
					exchange_unit_list<IntegerU32>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                        data,
			typename Manifest::MusicStingerSetting & stinger_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				stinger_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.trigger);
					exchange_unit_id(data, manifest.segment_to_play);
					exchange_unit_enumeration<IntegerU32>(data, manifest.play_at);
					exchange_unit_id(data, manifest.cue_name);
					exchange_unit_integer<IntegerU32>(data, manifest.do_not_play_this_stinger_again_for);
					exchange_unit_bitset<IntegerU32>(data, manifest.allow_playing_stinger_in_next_segment);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                           data,
			typename Manifest::MusicTransitionSetting & transition_manifest
		) -> Void {
			constexpr auto process_fade =
				[] (
				IByteStreamView &                                   data,
				typename Manifest::MusicTransitionSettingItemFade & manifest
			) {
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.time);
				// note : always default = 4 if non-fade
				exchange_unit_integer<IntegerU32>(data, manifest.curve);
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.offset);
				return;
			};
			exchange_unit_list<IntegerU32>(
				data,
				transition_manifest.item,
				[] (auto & count) {
				},
				// NOTE : avoid clang bug
				[&process_fade] (auto & data, auto & manifest) {
					exchange_unit_constant(data, 1_iu32);
					exchange_unit_id(data, manifest.source.id);
					exchange_unit_constant(data, 1_iu32);
					exchange_unit_id(data, manifest.destination.id);
					process_fade(data, manifest.source.fade_out);
					exchange_unit_enumeration<IntegerU32>(data, manifest.source.exit_source_at);
					exchange_unit_id(data, manifest.source.exit_source_at_custom_cue_match);
					exchange_unit_bitset<IntegerU8>(data, manifest.source.play_post_exit);
					process_fade(data, manifest.destination.fade_in);
					exchange_unit_id(data, manifest.destination.custom_cue_filter_match_target);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_integer<IntegerU16>(data, manifest.destination.jump_to);
					exchange_unit_integer<IntegerU16>(data, manifest.destination.sync_to);
					exchange_unit_bitset<IntegerU8>(data, manifest.destination.play_pre_entry);
					exchange_unit_bitset<IntegerU8>(data, manifest.destination.custom_cue_filter_match_source_cue_name);
					exchange_unit_bitset<IntegerU8>(data, manifest.segment.enable);
					if (manifest.segment.enable) {
						exchange_unit_id(data, manifest.segment.id);
						process_fade(data, manifest.segment.fade_in);
						process_fade(data, manifest.segment.fade_out);
						exchange_unit_bitset<IntegerU8>(data, manifest.segment.play_pre_entry);
						exchange_unit_bitset<IntegerU8>(data, manifest.segment.play_post_exit);
					}
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                data,
			typename Manifest::MusicTrackTransitionSetting & transition_manifest
		) -> Void {
			constexpr auto process_fade =
				[] (
				IByteStreamView &                                   data,
				typename Manifest::MusicTransitionSettingItemFade & manifest
			) {
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.time);
				// note : always default = 4 if non-fade
				exchange_unit_integer<IntegerU32>(data, manifest.curve);
				// note : always default = 0 if non-fade
				exchange_unit_integer<IntegerS32>(data, manifest.offset);
				return;
			};
			exchange_unit_constant(data, 1_iu32);
			exchange_unit_id(data, transition_manifest.switcher);
			process_fade(data, transition_manifest.source.fade_out);
			exchange_unit_enumeration<IntegerU32>(data, transition_manifest.source.exit_source_at);
			exchange_unit_id(data, transition_manifest.source.exit_source_at_custom_cue_match);
			process_fade(data, transition_manifest.destination.fade_in);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                         data,
			typename Manifest::AudioSwitcherSetting & switcher_manifest
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, switcher_manifest.is_state);
			exchange_unit_id(data, switcher_manifest.group);
			exchange_unit_id(data, switcher_manifest.default_item);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                    data,
			typename Manifest::AudioSwitcherAssociationSetting & switcher_association_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				switcher_association_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.group);
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_bitset<IntegerU8>(data, manifest.is_state);
				}
			);
			exchange_unit_list<IntegerU32>(
				data,
				switcher_association_manifest.path,
				[&] (auto & count) {
					// NOTE : here
					assert_test(count % 12_sz == 0_sz);
					count /= 12_sz;
					exchange_unit_bitset<IntegerU8>(
						data,
						switcher_association_manifest.mode
					);
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.u1);
					exchange_unit_id(data, manifest.object);
					exchange_unit_integer<IntegerU16>(data, manifest.weight);
					exchange_unit_integer<IntegerU16>(data, manifest.probability);
				}
			);
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
			exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop_count.value);
			exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop_count.minimum_value);
			exchange_unit_integer<IntegerS16>(data, play_mode_setting_manifest.continuous.loop_count.maximum_value);
			exchange_unit_floating<Floating32>(data, play_mode_setting_manifest.continuous.transition_duration.value);
			exchange_unit_floating<Floating32>(data, play_mode_setting_manifest.continuous.transition_duration.minimum_value);
			exchange_unit_floating<Floating32>(data, play_mode_setting_manifest.continuous.transition_duration.maximum_value);
			exchange_unit_integer<IntegerU16>(data, play_type_setting_manifest.random.avoid_repeat);
			exchange_unit_enumeration<IntegerU8>(data, play_mode_setting_manifest.continuous.transition_type);
			exchange_unit_bitset<IntegerU8>(data, play_type_setting_manifest.random.type);
			exchange_unit_bitset<IntegerU8>(data, play_type_manifest);
			exchange_unit_bitset<IntegerU8>(
				data,
				k_false,
				play_mode_setting_manifest.continuous.always_reset_playlist,
				play_type_setting_manifest.sequence.at_end_of_playlist,
				play_mode_manifest,
				scope_manifest
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                             data,
			List<typename Manifest::SoundPlaylistContainerPlaylistItem> & sequence_container_playlist_manifest
		) -> Void {
			exchange_unit_list<IntegerU16>(
				data,
				sequence_container_playlist_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.item);
					exchange_unit_integer<IntegerU32>(data, manifest.weight);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                                  data,
			List<typename Manifest::SoundSwitchContainerObjectAttributeItem> & switch_container_object_attribute_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				switch_container_object_attribute_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.id);
					exchange_unit_bitset<IntegerU8>(data, manifest.play_first_only, manifest.continue_to_play_across_switch);
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_integer<IntegerU32>(data, manifest.fade_out_time);
					exchange_unit_integer<IntegerU32>(data, manifest.fade_in_time);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                               data,
			List<typename Manifest::SoundSwitchContainerObjectAssignItem> & switch_container_assigned_object_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				switch_container_assigned_object_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.item);
					exchange_unit_list<IntegerU32>(
						data,
						manifest.object,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_id(data, manifest);
						}
					);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                       data,
			List<typename Manifest::SoundBlendContainerTrackItem> & blend_container_track_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				blend_container_track_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.id);
					process_section_sub(data, manifest.real_time_parameter_control);
					exchange_unit_id(data, manifest.cross_fade);
					exchange_unit_enumeration<IntegerU8>(data, manifest.cross_fade_category);
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                             data,
			List<typename Manifest::MusicPlaylistContainerPlaylistItem> & music_playlist_container_playlist_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				music_playlist_container_playlist_manifest,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.item);
					exchange_unit_id(data, manifest.u1);
					exchange_unit_integer<IntegerU32>(data, manifest.child_count);
					{
						// NOTE : here
						auto value = IntegerU32{};
						exchange_unit_raw<IntegerU32>(data, value);
						if (value != ~0_iu32) {
							manifest.play_type.set();
							manifest.play_mode.set();
							data.backward(k_type_size<IntegerU32>);
							exchange_unit_bitset<IntegerU32>(
								data,
								manifest.play_mode.get(),
								manifest.play_type.get()
							);
						} else {
							manifest.play_type.reset();
							manifest.play_mode.reset();
						}
					}
					exchange_unit_integer<IntegerU16>(data, manifest.loop_count);
					exchange_unit_constant(data, 0_iu32);
					exchange_unit_integer<IntegerU32>(data, manifest.weight);
					exchange_unit_integer<IntegerU16>(data, manifest.random_setting.avoid_repeat);
					exchange_unit_bitset<IntegerU8>(data, manifest.group); // todo : maybe
					exchange_unit_bitset<IntegerU8>(data, manifest.random_setting.type);
				}
			);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                                data,
			typename Manifest::MusicTrackTrackType &         track_type_manifest,
			typename Manifest::AudioSwitcherSetting &        switcher_manifest,
			typename Manifest::MusicTrackTransitionSetting & transition_manifest
		) -> Void {
			exchange_unit_enumeration<IntegerU8>(data, track_type_manifest);
			if (track_type_manifest == Manifest::MusicTrackTrackType::Constant::switcher()) {
				process_section_sub(data, switcher_manifest);
				process_section_sub(data, transition_manifest);
			}
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                     data,
			typename Manifest::MusicTrackStream & stream_manifest
		) -> Void {
			exchange_unit_integer<IntegerU16>(data, stream_manifest.look_ahead_time);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                  data,
			typename Manifest::AudioPlayMode & play_mode_manifest
		) -> Void {
			exchange_unit_bitset<IntegerU8>(data, play_mode_manifest);
			return;
		}

		static auto process_section_sub (
			IByteStreamView &                    data,
			typename Manifest::MusicSegmentCue & cue_manifest
		) -> Void {
			exchange_unit_list<IntegerU32>(
				data,
				cue_manifest.item,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.name);
					exchange_unit_floating<Floating64>(data, manifest.time);
					exchange_unit_constant(data, 0_iu8);
				}
			);
			return;
		}

		// ----------------

		static auto process_section (
			IByteStreamView &               data,
			typename Manifest::StateGroup & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_integer<IntegerU32>(data, manifest.default_transition);
			exchange_unit_list<IntegerU32>(
				data,
				manifest.custom_transition,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest.from);
					exchange_unit_id(data, manifest.to);
					exchange_unit_integer<IntegerS32>(data, manifest.time);
				}
			);
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::SwitchGroup & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_id(data, manifest.parameter);
			exchange_unit_enumeration<IntegerU8>(data, manifest.parameter_category);
			exchange_unit_list<IntegerU32>(
				data,
				manifest.point,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_floating<Floating32>(data, manifest.position.x);
					exchange_unit_id(data, manifest.position.y);
					exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
				}
			);
			return;
		}

		static auto process_section (
			IByteStreamView &                  data,
			typename Manifest::GameParameter & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_floating<Floating32>(data, manifest.range_default);
			exchange_unit_integer<IntegerU32>(data, manifest.interpolation_mode);
			exchange_unit_floating<Floating32>(data, manifest.interpolation_attack);
			exchange_unit_floating<Floating32>(data, manifest.interpolation_release);
			exchange_unit_integer<IntegerU8>(data, manifest.bind_to_built_in_parameter);
			return;
		}

		static auto process_section (
			IByteStreamView &                          data,
			typename Manifest::GameSynchronizationU1 & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_floating<Floating32>(data, manifest.u1);
			exchange_unit_floating<Floating32>(data, manifest.u2);
			exchange_unit_floating<Floating32>(data, manifest.u3);
			exchange_unit_floating<Floating32>(data, manifest.u4);
			exchange_unit_floating<Floating32>(data, manifest.u5);
			exchange_unit_floating<Floating32>(data, manifest.u6);
			return;
		}

		static auto process_section (
			IByteStreamView &                          data,
			typename Manifest::SourcePlugInReference & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_plug_in_id(data, manifest.plug_in);
			{
				// NOTE : here
				auto expand_size = Size{};
				exchange_unit_size<IntegerU32>(data, expand_size);
				manifest.expand = data.forward_view(expand_size);
			}
			exchange_unit_constant(data, 0_iu8);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u2,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_integer<IntegerU8>(data, manifest.u3);
					exchange_unit_integer<IntegerU8>(data, manifest.u4);
					exchange_unit_integer<IntegerU32>(data, manifest.u5);
					exchange_unit_integer<IntegerU8>(data, manifest.u6);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			exchange_unit_constant(data, 0_iu16);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u4,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_floating<Floating32>(data, manifest.u3);
				}
			);
			return;
		}

		static auto process_section (
			IByteStreamView &                           data,
			typename Manifest::UnknownPlugInReference & manifest
		) -> Void {
			return process_section(data, self_cast<typename Manifest::SourcePlugInReference>(manifest));
		}

		static auto process_section (
			IByteStreamView &                                 data,
			typename Manifest::StatefulAudioPropertySetting & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.value.as_list(),
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_enumeration<IntegerU16>(data, manifest.key);
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_floating<Floating32>(data, manifest.value);
				}
			);
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::EventAction & manifest
		) -> Void {
			// NOTE : here
			exchange_unit_id(data, manifest.id);
			auto scope = Size{};
			auto type = Size{};
			auto target = IDNumber{};
			exchange_unit_size<IntegerU8>(data, scope);
			exchange_unit_size<IntegerU8>(data, type);
			exchange_unit_raw(data, target);
			// todo
			if (type == 4_sz) {
				// play
				assert_test(scope == 3_sz);
			}
			if (type == 33_sz) {
				// post_event
				assert_test(scope == 3_sz);
			}
			if (type == 16_sz || type == 17_sz) {
				// enable/disable state
				assert_test(scope == 2_sz);
			}
			if (type == 18_sz) {
				// set state
				assert_test(scope == 4_sz);
				// todo : maybe wwise bug?
				scope = 2_sz;
			}
			if (type == 25_sz) {
				// set switch
				assert_test(scope == 1_sz);
				// 1 is only used for set_switch
				// todo : maybe wwise bug?
				scope = 3_sz;
			}
			manifest.target = find_id(target);
			switch (scope.value) {
				case 2 : {
					assert_test(target != 0_iu32);
					manifest.globally = k_true;
					break;
				}
				case 3 : {
					assert_test(target != 0_iu32);
					manifest.globally = k_false;
					break;
				}
				case 4 : {
					assert_test(target == 0_iu32);
					manifest.globally = k_true;
					break;
				}
				case 5 : {
					assert_test(target == 0_iu32);
					manifest.globally = k_false;
					break;
				}
				default : {
					throw SimpleException{};
				}
			}
			exchange_unit_integer<IntegerU8>(data, manifest.u1);
			auto common_property = CommonPropertyMap<EventActionCommonPropertyType>{};
			using CPTC = typename EventActionCommonPropertyType::Constant;
			process_section_sub(data, common_property, k_true);
			auto process_section_sub_of_exception_list =
				[&] (
			) {
				exchange_unit_list<IntegerU8>(
					data,
					manifest.exception,
					[] (auto & size) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_id(data, manifest.id);
						exchange_unit_bitset<IntegerU8>(data, manifest.u1);
					}
				);
			};
			auto has_case = k_false;
			if constexpr (version.number >= 140_i) {
				if (type == 4_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::play_audio()>();
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_id(data, property_manifest.sound_bank);
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
						exchange_common_property<CPTC::probability()>(common_property, property_manifest.probability);
					}
					has_case = k_true;
				}
				if (type == 1_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::stop_audio()>();
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(
						data,
						k_false,
						property_manifest.resume_state_transition,
						property_manifest.apply_to_dynamic_sequence
					);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 2_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::pause_audio()>();
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.include_delayed_resume_action,
						property_manifest.resume_state_transition,
						property_manifest.apply_to_dynamic_sequence
					);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 3_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::resume_audio()>();
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.master_resume,
						property_manifest.resume_state_transition,
						property_manifest.apply_to_dynamic_sequence
					);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 28_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::break_audio()>();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 30_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::seek_audio()>();
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.seek_type);
					exchange_unit_floating<Floating32>(data, property_manifest.seek_value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.seek_value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.seek_value.maximum_value);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.seek_to_nearest_marker
					);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 33_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::post_event()>();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 8_sz || type == 9_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_pitch()>();
					property_manifest.reset = type == 9_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 10_sz || type == 11_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_volume()>();
					property_manifest.reset = type == 11_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 12_sz || type == 13_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_bus_volume()>();
					property_manifest.reset = type == 13_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 14_sz || type == 15_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_low_pass_filter()>();
					property_manifest.reset = type == 15_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 32_sz || type == 48_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_voice_high_pass_filter()>();
					property_manifest.reset = type == 48_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 6_sz || type == 7_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_mute()>();
					property_manifest.reset = type == 7_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 19_sz || type == 20_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_game_parameter()>();
					property_manifest.reset = type == 20_sz;
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.fade_curve);
					exchange_unit_bitset<IntegerU8>(data, property_manifest.bypass_game_parameter_interpolation);
					exchange_unit_enumeration<IntegerU8>(data, property_manifest.apply_mode);
					exchange_unit_floating<Floating32>(data, property_manifest.value.value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.minimum_value);
					exchange_unit_floating<Floating32>(data, property_manifest.value.maximum_value);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
						exchange_common_property_as_randomized<CPTC::fade_time()>(common_property, property_manifest.fade_time);
					}
					has_case = k_true;
				}
				if (type == 16_sz || type == 17_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_state_availability()>();
					property_manifest.enable = type == 16_sz;
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 18_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::activate_state()>();
					exchange_unit_id(data, property_manifest.group);
					exchange_unit_id(data, property_manifest.item);
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 25_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::activate_switch()>();
					exchange_unit_id(data, property_manifest.group);
					exchange_unit_id(data, property_manifest.item);
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 29_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::activate_trigger()>();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 26_sz || type == 27_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::set_bypass_effect()>();
					property_manifest.reset = type == 27_sz;
					property_manifest.value.allocate_full(5_sz);
					exchange_unit_bitset<IntegerU8>(data, property_manifest.enable);
					exchange_unit_bitset<IntegerU8>(
						data,
						property_manifest.value[5_ix],
						property_manifest.value[1_ix],
						property_manifest.value[2_ix],
						property_manifest.value[3_ix],
						property_manifest.value[4_ix],
						as_constant(property_manifest.reset),
						as_constant(property_manifest.reset),
						as_constant(property_manifest.reset)
					);
					process_section_sub_of_exception_list();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 31_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::release_envelope()>();
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
					}
					has_case = k_true;
				}
				if (type == 34_sz) {
					auto & property_manifest = manifest.property.template set_of_type<Manifest::EventActionProperty::Type::Constant::reset_playlist()>();
					// todo : maybe fade curve ?
					exchange_unit_integer<IntegerU8>(data, property_manifest.u1);
					exchange_unit_constant(data, 0_iu8);
					{
						exchange_common_property_as_randomized<CPTC::delay()>(common_property, property_manifest.delay);
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
			exchange_unit_list<IntegerU8>(
				data,
				manifest.child,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_id(data, manifest);
				}
			);
			return;
		}

		static auto process_section (
			IByteStreamView &                  data,
			typename Manifest::DialogueEvent & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_integer<IntegerU8>(data, manifest.probability);
			process_section_sub(data, manifest.switcher_association);
			exchange_unit_constant(data, 0_iu16);
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::Attenuation & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_bitset<IntegerU8>(data, manifest.height_spread);
			{
				// NOTE : here
				auto has_cone = Boolean{};
				exchange_unit_bitset<IntegerU8>(data, has_cone);
				if (has_cone) {
					manifest.cone.set();
					exchange_unit_floating<Floating32>(data, manifest.cone.get().inner_angle);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().outer_angle);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().max_value);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().low_pass_filter);
					exchange_unit_floating<Floating32>(data, manifest.cone.get().high_pass_filter);
				} else {
					manifest.cone.reset();
				}
			}
			{
				// NOTE : here
				auto process_curve_index =
					[&] (
					auto & value
				) {
					auto raw_value = IntegerU8{};
					exchange_unit_raw(data, raw_value);
					if (raw_value == ~0_iu8) {
						value = k_null_optional;
					} else {
						value.set(cbw<Integer>(raw_value));
					}
				};
				process_curve_index(manifest.apply.output_bus_volume);
				process_curve_index(manifest.apply.game_defined_auxiliary_send_volume);
				process_curve_index(manifest.apply.user_defined_auxiliary_send_volume);
				process_curve_index(manifest.apply.low_pass_filter);
				process_curve_index(manifest.apply.high_pass_filter);
				process_curve_index(manifest.apply.spread);
				process_curve_index(manifest.apply.focus);
			}
			exchange_unit_list<IntegerU8>(
				data,
				manifest.curve,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			process_section_sub(data, manifest.real_time_parameter_control);
			return;
		}

		static auto process_section (
			IByteStreamView &                                    data,
			typename Manifest::LowFrequencyOscillatorModulator & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<ModulatorCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.real_time_parameter_control);
			{
				using CPTC = typename ModulatorCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::depth()>(common_property, manifest.depth);
				exchange_common_property_as_randomized<CPTC::frequency()>(common_property, manifest.frequency);
				exchange_common_property<CPTC::waveform()>(common_property, manifest.waveform);
				exchange_common_property_as_randomized<CPTC::smoothing()>(common_property, manifest.smoothing);
				exchange_common_property_as_randomized<CPTC::pulse_width_modulation()>(common_property, manifest.pulse_width_modulation);
				exchange_common_property_as_randomized<CPTC::attack()>(common_property, manifest.attack);
				exchange_common_property_as_randomized<CPTC::initial_phase_offset()>(common_property, manifest.initial_phase_offset);
				exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                      data,
			typename Manifest::EnvelopeModulator & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<ModulatorCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.real_time_parameter_control);
			{
				using CPTC = typename ModulatorCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::attack_time()>(common_property, manifest.attack_time);
				exchange_common_property_as_randomized<CPTC::attack_curve()>(common_property, manifest.attack_curve);
				exchange_common_property_as_randomized<CPTC::decay_time()>(common_property, manifest.decay_time);
				exchange_common_property_as_randomized<CPTC::sustain_level()>(common_property, manifest.sustain_level);
				exchange_common_property_as_randomized<CPTC::release_time()>(common_property, manifest.release_time);
				exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
				exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
				exchange_common_property_as_randomized<CPTC::sustain_time()>(common_property, manifest.sustain_time);
				exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_after_release);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                  data,
			typename Manifest::TimeModulator & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<ModulatorCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.real_time_parameter_control);
			{
				using CPTC = typename ModulatorCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.initial_delay);
				exchange_common_property_as_regular<CPTC::duration()>(common_property, manifest.duration);
				exchange_common_property_as_randomized<CPTC::loop_count()>(common_property, manifest.loop_count);
				exchange_common_property_as_randomized<CPTC::playback_rate()>(common_property, manifest.playback_rate);
				exchange_common_property<CPTC::scope()>(common_property, manifest.scope);
				exchange_common_property<CPTC::trigger_on()>(common_property, manifest.trigger_on);
				exchange_common_property<CPTC::stop_playback()>(common_property, manifest.stop_playback_at_end);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                data,
			typename Manifest::AudioDevice & manifest
		) -> Void {
			exchange_unit_id(data, manifest.id);
			exchange_unit_plug_in_id(data, manifest.plug_in);
			auto expand_size = Size{};
			exchange_unit_size<IntegerU32>(data, expand_size);
			if (manifest.plug_in.u1 == 7_i && manifest.plug_in.u2 == 174_i) {
				assert_test(expand_size == 12_sz);
				manifest.expand.system.set();
				exchange_unit_bitset<IntegerU8>(data, manifest.expand.system.get().allow_3d_audio);
				exchange_unit_id(data, manifest.expand.system.get().main_mix_configuration_for_binauralization);
				exchange_unit_id(data, manifest.expand.system.get().main_mix_configuration_for_home_theater);
				exchange_unit_bitset<IntegerU8>(data, manifest.expand.system.get().allow_system_audio_object);
				exchange_unit_integer<IntegerU16>(data, manifest.expand.system.get().minimum_system_audio_object_required);
			} else {
				assert_test(expand_size == 0_sz);
			}
			exchange_unit_constant(data, 0_iu8);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u2,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU32>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_integer<IntegerU8>(data, manifest.u3);
					exchange_unit_integer<IntegerU8>(data, manifest.u4);
					exchange_unit_integer<IntegerU32>(data, manifest.u5);
					exchange_unit_integer<IntegerU8>(data, manifest.u6);
					exchange_unit_list<IntegerU16>(
						data,
						manifest.point,
						[] (auto & count) {
						},
						[] (auto & data, auto & manifest) {
							exchange_unit_floating<Floating32>(data, manifest.position.x);
							exchange_unit_floating<Floating32>(data, manifest.position.y);
							exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
						}
					);
				}
			);
			exchange_unit_constant(data, 0_iu16);
			exchange_unit_list<IntegerU16>(
				data,
				manifest.u4,
				[] (auto & count) {
				},
				[] (auto & data, auto & manifest) {
					exchange_unit_integer<IntegerU8>(data, manifest.u1);
					exchange_unit_integer<IntegerU8>(data, manifest.u2);
					exchange_unit_floating<Floating32>(data, manifest.u3);
				}
			);
			process_section_sub(data, manifest.effect);
			return;
		}

		static auto process_section (
			IByteStreamView &             data,
			typename Manifest::AudioBus & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			auto override_positioning = Boolean{};
			auto override_game_defined_auxiliary_send = Boolean{};
			auto override_user_defined_auxiliary_send = Boolean{};
			auto override_early_reflection_auxiliary_send = Boolean{};
			exchange_unit_id(data, manifest.parent);
			if (manifest.parent == 0_i) {
				exchange_unit_id(data, manifest.audio_device);
			} else {
				manifest.audio_device = find_id(0_iu32);
			}
			process_section_sub(data, common_property, k_false);
			process_section_sub(data, manifest.positioning, override_positioning);
			process_section_sub(data, manifest.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send, override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.mute_for_background_music);
			process_section_sub(data, manifest.bus_configuration);
			process_section_sub(data, manifest.hdr);
			process_section_sub(data, manifest.auto_ducking);
			process_section_sub(data, manifest.effect);
			exchange_unit_constant(data, 0_iu32);
			exchange_unit_constant(data, 0_iu16);
			process_section_sub(data, manifest.metadata);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.state);
			assert_test(override_positioning);
			assert_test(override_game_defined_auxiliary_send);
			assert_test(override_user_defined_auxiliary_send);
			assert_test(override_early_reflection_auxiliary_send);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.bus);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.playback_limit);
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
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.source);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				exchange_common_property_as_randomized<CPTC::loop_count()>(common_property, manifest.playback_setting.loop_count);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                           data,
			typename Manifest::SoundPlaylistContainer & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.playback_setting.scope, manifest.playback_setting.type, manifest.playback_setting.type_setting, manifest.playback_setting.mode, manifest.playback_setting.mode_setting);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.playback_setting.playlist);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                         data,
			typename Manifest::SoundSwitchContainer & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.playback_setting.switcher);
			process_section_sub(data, manifest.playback_setting.mode);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.playback_setting.object_assign);
			process_section_sub(data, manifest.playback_setting.object_attribute);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                        data,
			typename Manifest::SoundBlendContainer & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.playback_setting.track);
			process_section_sub(data, manifest.playback_setting.mode);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_randomized<CPTC::initial_delay()>(common_property, manifest.playback_setting.initial_delay);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &               data,
			typename Manifest::ActorMixer & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.midi, manifest.playback_priority, manifest.override_midi_event, manifest.override_midi_note_tracking, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &               data,
			typename Manifest::MusicTrack & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.source);
			process_section_sub(data, manifest.playback_setting.clip);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub(data, manifest.playback_setting.type, manifest.playback_setting.switcher, manifest.playback_setting.transition);
			process_section_sub(data, manifest.stream);
			exchange_unit_constant(data, 0_iu16);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                 data,
			typename Manifest::MusicSegment & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			process_section_sub(data, manifest.stinger);
			{
				exchange_unit_floating<Floating64>(data, manifest.playback_setting.duration);
			}
			process_section_sub(data, manifest.playback_setting.cue);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                           data,
			typename Manifest::MusicPlaylistContainer & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			process_section_sub(data, manifest.stinger);
			process_section_sub(data, manifest.transition);
			process_section_sub(data, manifest.playback_setting.playlist);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		static auto process_section (
			IByteStreamView &                         data,
			typename Manifest::MusicSwitchContainer & manifest
		) -> Void {
			auto common_property = CommonPropertyMap<AudioCommonPropertyType>{};
			exchange_unit_id(data, manifest.id);
			process_section_sub(data, manifest.midi, manifest.override_midi_target, manifest.override_midi_clip_tempo);
			process_section_sub(data, manifest.effect, manifest.override_effect);
			process_section_sub(data, manifest.metadata, manifest.override_metadata);
			exchange_unit_constant(data, 0_iu8);
			process_section_sub(data, manifest.output_bus);
			exchange_unit_id(data, manifest.parent);
			process_section_sub(data, manifest.playback_priority, manifest.override_playback_priority);
			process_section_sub(data, common_property, k_true);
			process_section_sub(data, manifest.positioning, manifest.override_positioning);
			process_section_sub(data, manifest.auxiliary_send, manifest.override_game_defined_auxiliary_send, manifest.override_user_defined_auxiliary_send, manifest.override_early_reflection_auxiliary_send);
			process_section_sub(data, manifest.playback_limit, manifest.virtual_voice, manifest.override_playback_limit, manifest.override_virtual_voice);
			process_section_sub(data, manifest.voice_volume_gain, manifest.hdr, manifest.override_voice_volume_loudness_normalization, manifest.override_hdr_envelope_tracking);
			process_section_sub(data, manifest.state);
			process_section_sub(data, manifest.real_time_parameter_control);
			process_section_sub<IntegerU32>(data, manifest.child);
			process_section_sub(data, manifest.time_setting, manifest.override_time_setting);
			process_section_sub(data, manifest.stinger);
			process_section_sub(data, manifest.transition);
			{
				exchange_unit_bitset<IntegerU8>(data, manifest.playback_setting.continue_playing_on_switch_change);
			}
			process_section_sub(data, manifest.playback_setting.switcher_association);
			{
				using CPTC = typename AudioCommonPropertyType::Constant;
				exchange_common_property_as_regular<CPTC::playback_speed()>(common_property, manifest.playback_setting.speed);
				process_common_property(common_property, manifest.voice);
				process_common_property(common_property, manifest.voice_volume_gain);
				process_common_property(common_property, manifest.output_bus);
				process_common_property(common_property, manifest.auxiliary_send);
				process_common_property(common_property, manifest.positioning);
				process_common_property(common_property, manifest.hdr);
				process_common_property(common_property, manifest.midi);
				process_common_property(common_property, manifest.playback_limit);
				process_common_property(common_property, manifest.virtual_voice);
				process_common_property(common_property, manifest.playback_priority);
			}
			return;
		}

		// ----------------

		static auto process_chunk_bkhd (
			IByteStreamView &              data,
			typename Manifest::SoundBank & manifest
		) -> Void {
			if constexpr (version.number >= 88_i) {
				data.read_constant(cbw<VersionNumber>(version.number));
				exchange_unit_id(data, manifest.id);
				exchange_unit_id(data, manifest.language);
				manifest.header_expand = data.forward_view(data.reserve());
			}
			return;
		}

		static auto process_chunk_didx_data (
			IByteStreamView &             didx_data,
			IByteStreamView &             data_data,
			List<typename Manifest::ID> & manifest,
			Optional<Path> const &        embedded_audio_directory
		) -> Void {
			if constexpr (version.number >= 88_i) {
				constexpr auto didx_item_structure_size = bs_static_size<IDNumber>() + bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>();
				assert_test(is_padded_size(didx_data.reserve(), didx_item_structure_size));
				manifest.allocate_full(didx_data.reserve() / didx_item_structure_size);
				exchange_unit_list<Void>(
					didx_data,
					manifest,
					[] (auto & count) {
					},
					[&] (auto & data, auto & manifest) {
						auto data_offset = Size{};
						auto data_size = Size{};
						exchange_unit_id(data, manifest);
						exchange_unit_size<IntegerU32>(data, data_offset);
						exchange_unit_size<IntegerU32>(data, data_size);
						if (manifest == 0_i) {
							assert_test(data_offset == 1_ix);
							assert_test(data_size == 0_sz);
						} else {
							auto item_data = data_data.sub_view(data_offset, data_size);
							if (embedded_audio_directory) {
								FileSystem::write_file(embedded_audio_directory.get() / "{}.wem"_sf(manifest), item_data);
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
			if constexpr (version.number >= 140_i) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest.plug_in,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_plug_in_id(data, manifest.id);
						exchange_unit_string<Void>(data, manifest.name);
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
			if constexpr (version.number >= 140_i) {
				exchange_unit_floating<Floating32>(data, manifest.volume_threshold);
				exchange_unit_integer<IntegerS16>(data, manifest.maximum_voice_instance);
				exchange_unit_constant(data, 50_iu16);
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.state_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.switch_group,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
				exchange_unit_list<IntegerU32>(
					data,
					game_synchronization_manifest.game_parameter,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						process_section(data, manifest);
					}
				);
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
			if constexpr (version.number >= 88_i) {
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						auto type = Size{};
						auto size = Size{};
						exchange_unit_size<IntegerU8>(data, type);
						exchange_unit_size<IntegerU32>(data, size);
						auto data_position = data.position();
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<typename Manifest::HierarchyType>::Index>(
							[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) -> auto {
								constexpr auto variant_type = mbw<typename Manifest::HierarchyType>(index);
								if constexpr (variant_type != Manifest::HierarchyType::Constant::unknown()) {
									if (type == value_index) {
										process_section(data, manifest.template set_of_type<variant_type>());
										has_case = k_true;
									}
								}
							}
						);
						if (!has_case) {
							auto & unknown_manifest = manifest.template set_of_type<Manifest::HierarchyType::Constant::unknown()>();
							unknown_manifest.type = cbw<Integer>(type);
							unknown_manifest.data = data.forward_view(size);
							M_log("warning : unknown hierarchy type {}"_sf(type));
						}
						assert_test(data.position() - data_position == size);
					}
				);
			}
			return;
		}

		static auto process_chunk_stid (
			IByteStreamView &                    data,
			List<typename Manifest::Reference> & manifest
		) -> Void {
			if constexpr (version.number >= 88_i) {
				exchange_unit_constant(data, 1_iu32);
				exchange_unit_list<IntegerU32>(
					data,
					manifest,
					[] (auto & count) {
					},
					[] (auto & data, auto & manifest) {
						exchange_unit_id(data, manifest.id);
						exchange_unit_string<IntegerU8>(data, manifest.name);
					}
				);
			}
			return;
		}

		static auto process_chunk_envs (
			IByteStreamView &            data,
			typename Manifest::Setting & manifest
		) -> Void {
			if constexpr (version.number >= 140_i) {
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
						exchange_unit_bitset<IntegerU8>(data, manifest.enable);
						exchange_unit_integer<IntegerU8>(data, manifest.u1);
						exchange_unit_list<IntegerU16>(
							data,
							manifest.point,
							[] (auto & count) {
							},
							[] (auto & data, auto & manifest) {
								exchange_unit_floating<Floating32>(data, manifest.position.x);
								exchange_unit_floating<Floating32>(data, manifest.position.y);
								exchange_unit_enumeration<IntegerU32>(data, manifest.curve);
							}
						);
					}
				);
			}
			return;
		}

		static auto process_chunk_plat (
			IByteStreamView &            data,
			typename Manifest::Setting & manifest
		) -> Void {
			if constexpr (version.number >= 140_i) {
				exchange_unit_string<Void>(data, manifest.platform);
			}
			return;
		}

		// ----------------

		static auto process_sound_bank (
			IByteStreamView &              sound_bank_data,
			typename Manifest::SoundBank & sound_bank_manifest,
			Optional<Path> const &         embedded_audio_directory
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
			if constexpr (version.number >= 88_i) {
				assert_test(state && sign.id == ChunkSignFlag::bkhd);
				{
					process_chunk_bkhd(chunk, sound_bank_manifest);
					next_chunk();
				}
			}
			if constexpr (version.number >= 88_i) {
				if (state && sign.id == ChunkSignFlag::didx) {
					auto didx_data = chunk;
					chunk.forward_to_end();
					next_chunk();
					assert_test(state && sign.id == ChunkSignFlag::data);
					auto data_data = chunk;
					chunk.forward_to_end();
					next_chunk();
					process_chunk_didx_data(didx_data, data_data, sound_bank_manifest.embedded_audio, embedded_audio_directory);
				}
			}
			if constexpr (version.number >= 140_i) {
				if (state && sign.id == ChunkSignFlag::init) {
					sound_bank_manifest.setting.set();
					sound_bank_manifest.game_synchronization.set();
					process_chunk_init(chunk, sound_bank_manifest.setting.get());
					next_chunk();
				}
			}
			if constexpr (version.number >= 88_i) {
				if (state && sign.id == ChunkSignFlag::stmg) {
					process_chunk_stmg(chunk, sound_bank_manifest.setting.get(), sound_bank_manifest.game_synchronization.get());
					next_chunk();
				}
			}
			if constexpr (version.number >= 88_i) {
				if (state && sign.id == ChunkSignFlag::hirc) {
					process_chunk_hirc(chunk, sound_bank_manifest.hierarchy);
					next_chunk();
				}
			}
			if constexpr (version.number >= 88_i) {
				if (state && sign.id == ChunkSignFlag::stid) {
					process_chunk_stid(chunk, sound_bank_manifest.reference);
					next_chunk();
				}
			}
			if constexpr (version.number >= 88_i) {
				if (state && sign.id == ChunkSignFlag::envs) {
					process_chunk_envs(chunk, sound_bank_manifest.setting.get());
					next_chunk();
				}
			}
			if constexpr (version.number >= 140_i) {
				if (state && sign.id == ChunkSignFlag::plat) {
					process_chunk_plat(chunk, sound_bank_manifest.setting.get());
					next_chunk();
				}
			}
			assert_test(!state);
			return;
		}

	public:

		static auto do_process_sound_bank (
			IByteStreamView &              sound_bank_data_,
			typename Manifest::SoundBank & sound_bank_manifest,
			Optional<Path> const &         embedded_audio_directory
		) -> Void {
			M_use_zps_of(sound_bank_data);
			restruct(sound_bank_manifest);
			return process_sound_bank(sound_bank_data, sound_bank_manifest, embedded_audio_directory);
		}

	};

}
