#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/wwise/sound_bank/version.hpp"

namespace TwinStar::Core::Tool::Wwise::SoundBank {

	template <auto version> requires (check_version(version, {}))
	struct Manifest {

		#pragma region id

		using ID = Integer;

		// ----------------

		M_record_of_list(
			M_wrap(PlugInID),
			M_wrap(
				(Integer) u1,
				(Integer) u2,
			),
		);

		#pragma endregion

		#pragma region randomizable value

		template <typename Value>
		M_record_of_list(
			M_wrap(RegularValue),
			M_wrap(
				(Value) value,
			),
		);

		template <typename Value>
		M_record_of_list(
			M_wrap(RandomizedValue),
			M_wrap(
				(Value) value,
				(Value) minimum_value,
				(Value) maximum_value,
			),
		);

		#pragma endregion

		#pragma region common

		template <typename = None>
		struct Curve_;

		using Curve = Curve_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(Curve_<_>),
			M_wrap(
				constant,
				linear,
				s,
				s_inverted,
				sine,
				sine_reciprocal,
				logarithmic_1dot41,
				logarithmic_3dot0,
				exponential_1dot41,
				exponential_3dot0,
			),
		);

		// ----------------

		template <typename = None>
		struct CurveShape_;

		using CurveShape = CurveShape_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(CurveShape_<_>),
			M_wrap(
				a_1,
				a_2,
				b_1,
				b_2,
				c_1,
				c_2,
				d_1,
				d_2,
				e,
			),
		);

		// ----------------

		template <typename = None>
		struct TimePoint_;

		using TimePoint = TimePoint_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(TimePoint_<_>),
			M_wrap(
				immediate,
				next_grid,
				next_bar,
				next_beta,
				next_cue,
				custom_cue,
				entry_cue,
				exit_cue,
				last_exit_position,
			),
		);

		// ----------------

		template <typename = None>
		struct Point2D_;

		using Point2D = Point2D_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(Point2D_<_>),
			M_wrap(
				(Position2D<Floating>) position,
				(Curve) curve,
			),
		);

		#pragma endregion

		#pragma region audio property

		template <typename = None>
		struct PropertyCategory_;

		using PropertyCategory = PropertyCategory_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(PropertyCategory_<_>),
			M_wrap(
				unidirectional,
				bidirectional,
				bidirectional_ranged,
				boolean,
			),
		);

		// ----------------

		template <typename = None>
		struct AttenuationPropertyType_;

		using AttenuationPropertyType = AttenuationPropertyType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AttenuationPropertyType_<_>),
			M_wrap(
				cone_maximum_attenuation,
				cone_low_pass_filter,
				cone_high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct ModulatorPropertyType_;

		using ModulatorPropertyType = ModulatorPropertyType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(ModulatorPropertyType_<_>),
			M_wrap(
				lfo_depth,
				lfo_frequency,
				lfo_waveform,
				lfo_smoothing,
				lfo_pulse_width_modulation,
				lfo_attack,
				lfo_initial_phase_offset,
				envelope_attack_time,
				envelope_attack_curve,
				envelope_decay_time,
				envelope_sustain_level,
				envelope_maximum_sustain_time,
				envelope_release_time,
				time_initial_delay,
				time_playback_rate,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPropertyType_;

		using AudioPropertyType = AudioPropertyType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPropertyType_<_>),
			M_wrap(
				bus_volume,
				output_bus_volume,
				output_bus_low_pass_filter,
				output_bus_high_pass_filter,
				voice_volume,
				voice_pitch,
				voice_low_pass_filter,
				voice_high_pass_filter,
				voice_initial_delay,
				audio_make_up_gain,
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
				playback_limit_value,
				playback_priority_value,
				playback_speed,
				midi_transposition,
				midi_velocity_offset,
				transition_duration,
				effect_bypass_all,
				effect_bypass_0,
				effect_bypass_1,
				effect_bypass_2,
				effect_bypass_3,
				positioning_center_percent,
				speaker_panning_div_3d_spatialization_mix,
				speaker_panning_pan_front_rear,
				speaker_panning_pan_left_right,
				speaker_panning_pan_up_down,
				spatialization_3d_automation_pan_front_rear,
				spatialization_3d_automation_pan_left_right,
				spatialization_3d_automation_pan_up_down,
				attenuation_enable,
				attenuation_cone_maximum_attenuation,
				attenuation_cone_low_pass_filter,
				attenuation_cone_high_pass_filter,
				hdr_threshold,
				hdr_release_time,
				hdr_ratio,
				hdr_active_range,
			),
		);

		#pragma endregion

		#pragma region audio play setting

		template <typename = None>
		struct AudioPlayType_;

		using AudioPlayType = AudioPlayType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlayType_<_>),
			M_wrap(
				sequence,
				random,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayMode_;

		using AudioPlayMode = AudioPlayMode_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlayMode_<_>),
			M_wrap(
				step,
				continuous,
			),
		);

		#pragma endregion

		#pragma region bus voice setting

		template <typename = None>
		struct BusVoiceSetting_;

		using BusVoiceSetting = BusVoiceSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusVoiceSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) volume,
				(RegularValue<Floating>) pitch,
				(RegularValue<Floating>) low_pass_filter,
				(RegularValue<Floating>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioVoice_;

		using AudioVoice = AudioVoice_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioVoice_<_>),
			M_wrap(
				(RandomizedValue<Floating>) volume,
				(RandomizedValue<Floating>) pitch,
				(RandomizedValue<Floating>) low_pass_filter,
				(RandomizedValue<Floating>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct BusVoiceVolumeGainSetting_;

		using BusVoiceVolumeGainSetting = BusVoiceVolumeGainSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusVoiceVolumeGainSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) make_up,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioVoiceVolumeGainSetting_;

		using AudioVoiceVolumeGainSetting = AudioVoiceVolumeGainSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioVoiceVolumeGainSetting_<_>),
			M_wrap(
				(Boolean) normalization,
				(RandomizedValue<Floating>) make_up,
			),
		);

		// ----------------

		template <typename = None>
		struct BusBusSetting_;

		using BusBusSetting = BusBusSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusBusSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) volume,
			),
		);

		// ----------------

		template <typename = None>
		struct BusOutputBusSetting_;

		using BusOutputBusSetting = BusOutputBusSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusOutputBusSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) volume,
				(RegularValue<Floating>) low_pass_filter,
				(RegularValue<Floating>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioOutputBusSetting_;

		using AudioOutputBusSetting = AudioOutputBusSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioOutputBusSetting_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floating>) volume,
				(RegularValue<Floating>) low_pass_filter,
				(RegularValue<Floating>) high_pass_filter,
			),
		);

		#pragma endregion

		#pragma region audio auxiliary send setting

		template <typename = None>
		struct AudioGameDefinedAuxiliarySendSetting_;

		using AudioGameDefinedAuxiliarySendSetting = AudioGameDefinedAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioGameDefinedAuxiliarySendSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(RegularValue<Floating>) volume,
				(RegularValue<Floating>) low_pass_filter,
				(RegularValue<Floating>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioUserDefinedAuxiliarySendSettingItem_;

		using AudioUserDefinedAuxiliarySendSettingItem = AudioUserDefinedAuxiliarySendSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioUserDefinedAuxiliarySendSettingItem_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floating>) volume,
				(RegularValue<Floating>) low_pass_filter,
				(RegularValue<Floating>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioUserDefinedAuxiliarySendSetting_;

		using AudioUserDefinedAuxiliarySendSetting = AudioUserDefinedAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioUserDefinedAuxiliarySendSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(List<AudioUserDefinedAuxiliarySendSettingItem>) item,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioEarlyReflectionAuxiliarySendSetting_;

		using AudioEarlyReflectionAuxiliarySendSetting = AudioEarlyReflectionAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioEarlyReflectionAuxiliarySendSetting_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floating>) volume,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioAuxiliarySendSetting_;

		using AudioAuxiliarySendSetting = AudioAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioAuxiliarySendSetting_<_>),
			M_wrap(
				(AudioGameDefinedAuxiliarySendSetting) game_defined,
				(AudioUserDefinedAuxiliarySendSetting) user_defined,
				(AudioEarlyReflectionAuxiliarySendSetting) early_reflection,
			),
		);

		#pragma endregion

		#pragma region audio effect setting

		template <typename = None>
		struct AudioEffectSettingItem_;

		using AudioEffectSettingItem = AudioEffectSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioEffectSettingItem_<_>),
			M_wrap(
				(Integer) index,
				(ID) id,
				(Boolean) use_share_set,
				(Boolean) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioEffectSetting_;

		using AudioEffectSetting = AudioEffectSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioEffectSetting_<_>),
			M_wrap(
				(List<Boolean>) bypass,
				(List<AudioEffectSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio metadata setting

		template <typename = None>
		struct AudioMetadataSettingItem_;

		using AudioMetadataSettingItem = AudioMetadataSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioMetadataSettingItem_<_>),
			M_wrap(
				(Integer) index,
				(ID) id,
				(Boolean) use_share_set,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioMetadataSetting_;

		using AudioMetadataSetting = AudioMetadataSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioMetadataSetting_<_>),
			M_wrap(
				(List<AudioMetadataSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio positioning setting

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPoint_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPoint = AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPoint_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPoint_<_>),
			M_wrap(
				(Position3D<Floating>) position,
				(Integer) duration,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathPoint_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathPoint = AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathPoint_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_list(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathPoint_<_>),
			M_wrap(
				(Integer) begin,
				(Integer) count,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathRandomRange_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathRandomRange = AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathRandomRange_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_list(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathRandomRange_<_>),
			M_wrap(
				(Floating) left_right,
				(Floating) front_back,
				(Floating) up_down,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPath_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPath = AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPath_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPath_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathPoint) point,
				(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPathRandomRange) random_range,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3DAutomation_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3DAutomation = AudioPositioningSettingListenerRelativeRoutingPosition3DAutomation_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomation_<_>),
			M_wrap(
				(AudioPlayType) play_type,
				(AudioPlayMode) play_mode,
				(Boolean) pick_new_path_when_sound_start,
				(Boolean) loop,
				(Integer) transition_time,
				(List<AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPoint>) point,
				(List<AudioPositioningSettingListenerRelativeRoutingPosition3DAutomationPath>) path,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3DMode_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3DMode = AudioPositioningSettingListenerRelativeRoutingPosition3DMode_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3DMode_<_>),
			M_wrap(
				emitter,
				emitter_with_automation,
				listener_with_automation,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingPosition3D_;

		using AudioPositioningSettingListenerRelativeRoutingPosition3D = AudioPositioningSettingListenerRelativeRoutingPosition3D_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingPosition3D_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRelativeRoutingPosition3DMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) hold_emitter_position_and_orientation,
				(Boolean) diffraction_and_transmission,
				(AudioPositioningSettingListenerRelativeRoutingPosition3DAutomation) automation,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingAttenuation_;

		using AudioPositioningSettingListenerRelativeRoutingAttenuation = AudioPositioningSettingListenerRelativeRoutingAttenuation_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingAttenuation_<_>),
			M_wrap(
				(Boolean) enable,
				(ID) id,
				(Boolean) use_share_set,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRoutingSpatialization3D_;

		using AudioPositioningSettingListenerRelativeRoutingSpatialization3D = AudioPositioningSettingListenerRelativeRoutingSpatialization3D_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPositioningSettingListenerRelativeRoutingSpatialization3D_<_>),
			M_wrap(
				none,
				position,
				position_and_orientation,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRelativeRouting_;

		using AudioPositioningSettingListenerRelativeRouting = AudioPositioningSettingListenerRelativeRouting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRelativeRouting_<_>),
			M_wrap(
				(Boolean) enable,
				(AudioPositioningSettingListenerRelativeRoutingSpatialization3D) spatialization_3d,
				(RegularValue<Floating>) speaker_panning_div_spatialization_3d_mix,
				(AudioPositioningSettingListenerRelativeRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRelativeRoutingPosition3D) position_3d,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingSpeakerPanningMode_;

		using AudioPositioningSettingSpeakerPanningMode = AudioPositioningSettingSpeakerPanningMode_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPositioningSettingSpeakerPanningMode_<_>),
			M_wrap(
				direct_assignment,
				balance_fade,
				steering,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingSpeakerPanning_;

		using AudioPositioningSettingSpeakerPanning = AudioPositioningSettingSpeakerPanning_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingSpeakerPanning_<_>),
			M_wrap(
				(AudioPositioningSettingSpeakerPanningMode) mode,
				(Position3D<Floating>) position,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSetting_;

		using AudioPositioningSetting = AudioPositioningSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRelativeRouting) listener_relative_routing,
			),
		);

		#pragma endregion

		#pragma region audio real time parameter control setting

		template <typename = None>
		struct RealTimeParameterControlXAxisCategory_;

		using RealTimeParameterControlXAxisCategory = RealTimeParameterControlXAxisCategory_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(RealTimeParameterControlXAxisCategory_<_>),
			M_wrap(
				game_parameter,
				midi_parameter,
				modulator,
			),
		);

		// ----------------

		template <typename = None>
		struct AttenuationRealTimeParameterControlSettingItem_;

		using AttenuationRealTimeParameterControlSettingItem = AttenuationRealTimeParameterControlSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AttenuationRealTimeParameterControlSettingItem_<_>),
			M_wrap(
				(ID) x_axis,
				(RealTimeParameterControlXAxisCategory) x_axis_category,
				(PropertyCategory) y_axis_category,
				(AttenuationPropertyType) y_axis_type,
				(ID) u1,
				(Boolean) u2,
				(Boolean) u3,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct AttenuationRealTimeParameterControlSetting_;

		using AttenuationRealTimeParameterControlSetting = AttenuationRealTimeParameterControlSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AttenuationRealTimeParameterControlSetting_<_>),
			M_wrap(
				(List<AttenuationRealTimeParameterControlSettingItem>) item,
			),
		);

		// ----------------

		template <typename = None>
		struct ModulatorRealTimeParameterControlSettingItem_;

		using ModulatorRealTimeParameterControlSettingItem = ModulatorRealTimeParameterControlSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(ModulatorRealTimeParameterControlSettingItem_<_>),
			M_wrap(
				(ID) x_axis,
				(RealTimeParameterControlXAxisCategory) x_axis_category,
				(PropertyCategory) y_axis_category,
				(ModulatorPropertyType) y_axis_type,
				(ID) u1,
				(Boolean) u2,
				(Boolean) u3,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct ModulatorRealTimeParameterControlSetting_;

		using ModulatorRealTimeParameterControlSetting = ModulatorRealTimeParameterControlSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(ModulatorRealTimeParameterControlSetting_<_>),
			M_wrap(
				(List<ModulatorRealTimeParameterControlSettingItem>) item,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioRealTimeParameterControlSettingItem_;

		using AudioRealTimeParameterControlSettingItem = AudioRealTimeParameterControlSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioRealTimeParameterControlSettingItem_<_>),
			M_wrap(
				(ID) x_axis,
				(RealTimeParameterControlXAxisCategory) x_axis_category,
				(PropertyCategory) y_axis_category,
				(AudioPropertyType) y_axis_type,
				(ID) u1,
				(Boolean) u2,
				(Boolean) u3,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioRealTimeParameterControlSetting_;

		using AudioRealTimeParameterControlSetting = AudioRealTimeParameterControlSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioRealTimeParameterControlSetting_<_>),
			M_wrap(
				(List<AudioRealTimeParameterControlSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio state setting

		template <typename = None>
		struct AudioStateSettingAttribute_;

		using AudioStateSettingAttribute = AudioStateSettingAttribute_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioStateSettingAttribute_<_>),
			M_wrap(
				(AudioPropertyType) type,
				(PropertyCategory) category,
				(Integer) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioStateSettingApplyItem_;

		using AudioStateSettingApplyItem = AudioStateSettingApplyItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioStateSettingApplyItem_<_>),
			M_wrap(
				(ID) target,
				(ID) setting,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioStateSettingItem_;

		using AudioStateSettingItem = AudioStateSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioStateSettingItem_<_>),
			M_wrap(
				(ID) group,
				(TimePoint) change_occur_at,
				(List<AudioStateSettingApplyItem>) apply,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioStateSetting_;

		using AudioStateSetting = AudioStateSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioStateSetting_<_>),
			M_wrap(
				(List<AudioStateSettingAttribute>) attribute,
				(List<AudioStateSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region bus auto ducking setting

		template <typename = None>
		struct BusAutoDuckingSettingBusTarget_;

		using BusAutoDuckingSettingBusTarget = BusAutoDuckingSettingBusTarget_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(BusAutoDuckingSettingBusTarget_<_>),
			M_wrap(
				voice_volume,
				bus_volume,
			),
		);

		// ----------------

		template <typename = None>
		struct BusAutoDuckingSettingBus_;

		using BusAutoDuckingSettingBus = BusAutoDuckingSettingBus_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusAutoDuckingSettingBus_<_>),
			M_wrap(
				(ID) id,
				(Floating) volume,
				(Integer) fade_out,
				(Integer) fade_in,
				(CurveShape) curve,
				(BusAutoDuckingSettingBusTarget) target,
			),
		);

		// ----------------

		template <typename = None>
		struct BusAutoDuckingSetting_;

		using BusAutoDuckingSetting = BusAutoDuckingSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusAutoDuckingSetting_<_>),
			M_wrap(
				(Integer) recovery_time,
				(Floating) maximum_ducking_volume,
				(List<BusAutoDuckingSettingBus>) bus,
			),
		);

		#pragma endregion

		#pragma region music transition setting

		template <typename = None>
		struct MusicTransitionSettingItemFade_;

		using MusicTransitionSettingItemFade = MusicTransitionSettingItemFade_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItemFade_<_>),
			M_wrap(
				(Integer) time,
				(Integer) curve,
				(Integer) offset,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSettingItemSource_;

		using MusicTransitionSettingItemSource = MusicTransitionSettingItemSource_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItemSource_<_>),
			M_wrap(
				(ID) id,
				(TimePoint) exit_source_at,
				(ID) exit_source_at_custom_cue_match,
				(Boolean) play_post_exit,
				(MusicTransitionSettingItemFade) fade_out,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSettingItemDestination_;

		using MusicTransitionSettingItemDestination = MusicTransitionSettingItemDestination_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItemDestination_<_>),
			M_wrap(
				(ID) id,
				(Integer) jump_to,
				(Integer) sync_to,
				(Boolean) play_pre_entry,
				(Boolean) custom_cue_filter_match_source_cue_name,
				(ID) custom_cue_filter_match_target,
				(MusicTransitionSettingItemFade) fade_in,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSettingItemSegment_;

		using MusicTransitionSettingItemSegment = MusicTransitionSettingItemSegment_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItemSegment_<_>),
			M_wrap(
				(Boolean) enable,
				(ID) id,
				(Boolean) play_pre_entry,
				(MusicTransitionSettingItemFade) fade_in,
				(Boolean) play_post_exit,
				(MusicTransitionSettingItemFade) fade_out,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSettingItem_;

		using MusicTransitionSettingItem = MusicTransitionSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItem_<_>),
			M_wrap(
				(ID) u1,
				(MusicTransitionSettingItemSource) source,
				(MusicTransitionSettingItemDestination) destination,
				(MusicTransitionSettingItemSegment) segment,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSetting_;

		using MusicTransitionSetting = MusicTransitionSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTransitionSetting_<_>),
			M_wrap(
				(List<MusicTransitionSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region music track transition setting

		template <typename = None>
		struct MusicTrackTransitionSettingItemSource_;

		using MusicTrackTransitionSettingItemSource = MusicTrackTransitionSettingItemSource_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackTransitionSettingItemSource_<_>),
			M_wrap(
				(TimePoint) exit_source_at,
				(ID) exit_source_at_custom_cue_match,
				(MusicTransitionSettingItemFade) fade_out,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackTransitionSettingItemDestination_;

		using MusicTrackTransitionSettingItemDestination = MusicTrackTransitionSettingItemDestination_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackTransitionSettingItemDestination_<_>),
			M_wrap(
				(MusicTransitionSettingItemFade) fade_in,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackTransitionSetting_;

		using MusicTrackTransitionSetting = MusicTrackTransitionSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackTransitionSetting_<_>),
			M_wrap(
				(ID) switcher,
				(MusicTrackTransitionSettingItemSource) source,
				(MusicTrackTransitionSettingItemDestination) destination,
			),
		);

		#pragma endregion

		#pragma region music stinger setting

		template <typename = None>
		struct MusicStingerSettingItem_;

		using MusicStingerSettingItem = MusicStingerSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicStingerSettingItem_<_>),
			M_wrap(
				(ID) trigger,
				(ID) segment_to_play,
				(TimePoint) play_at,
				(ID) cue_name,
				(Integer) do_not_play_this_stinger_again_for,
				(Boolean) allow_playing_stinger_in_next_segment,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicStingerSetting_;

		using MusicStingerSetting = MusicStingerSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicStingerSetting_<_>),
			M_wrap(
				(List<MusicStingerSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region bus hdr setting

		template <typename = None>
		struct BusHDRSettingWindowTopOutputGameParameter_;

		using BusHDRSettingWindowTopOutputGameParameter = BusHDRSettingWindowTopOutputGameParameter_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusHDRSettingWindowTopOutputGameParameter_<_>),
			M_wrap(
				(ID) id,
				(RegularValue<Floating>) minimum,
				(RegularValue<Floating>) maximum,
			),
		);

		// ----------------

		template <typename = None>
		struct BusHDRSettingDynamic_;

		using BusHDRSettingDynamic = BusHDRSettingDynamic_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusHDRSettingDynamic_<_>),
			M_wrap(
				(RegularValue<Floating>) threshold,
				(RegularValue<Floating>) ratio,
				(RegularValue<Floating>) release_time,
				(RegularValue<Integer>) release_mode,
			),
		);

		// ----------------

		template <typename = None>
		struct BusHDRSetting_;

		using BusHDRSetting = BusHDRSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(BusHDRSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(Boolean) u1,
				(BusHDRSettingDynamic) dynamic,
				(BusHDRSettingWindowTopOutputGameParameter) window_top_output_game_parameter,
			),
		);

		#pragma endregion

		#pragma region audio hdr setting

		template <typename = None>
		struct AudioHDRSettingEnvelopeTracking_;

		using AudioHDRSettingEnvelopeTracking = AudioHDRSettingEnvelopeTracking_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioHDRSettingEnvelopeTracking_<_>),
			M_wrap(
				(Boolean) enable,
				(RegularValue<Floating>) active_range,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioHDRSetting_;

		using AudioHDRSetting = AudioHDRSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioHDRSetting_<_>),
			M_wrap(
				(AudioHDRSettingEnvelopeTracking) envelope_tracking,
			),
		);

		#pragma endregion

		#pragma region sound midi setting

		template <typename = None>
		struct SoundMIDISettingEventPlayOn_;

		using SoundMIDISettingEventPlayOn = SoundMIDISettingEventPlayOn_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(SoundMIDISettingEventPlayOn_<_>),
			M_wrap(
				note_on,
				note_off,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundMIDISettingEvent_;

		using SoundMIDISettingEvent = SoundMIDISettingEvent_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundMIDISettingEvent_<_>),
			M_wrap(
				(SoundMIDISettingEventPlayOn) play_on,
				(Boolean) break_on_note_off,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundMIDISettingNoteTracking_;

		using SoundMIDISettingNoteTracking = SoundMIDISettingNoteTracking_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundMIDISettingNoteTracking_<_>),
			M_wrap(
				(Boolean) enable,
				(Integer) root_note,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundMIDISettingTransformation_;

		using SoundMIDISettingTransformation = SoundMIDISettingTransformation_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundMIDISettingTransformation_<_>),
			M_wrap(
				(RegularValue<Integer>) transposition,
				(RegularValue<Integer>) velocity_offset,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundMIDISettingFilter_;

		using SoundMIDISettingFilter = SoundMIDISettingFilter_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundMIDISettingFilter_<_>),
			M_wrap(
				(Integer) key_range_minimum,
				(Integer) key_range_maximum,
				(Integer) velocity_minimum,
				(Integer) velocity_maximum,
				(Integer) channel,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundMIDISetting_;

		using SoundMIDISetting = SoundMIDISetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundMIDISetting_<_>),
			M_wrap(
				(SoundMIDISettingEvent) event,
				(SoundMIDISettingNoteTracking) note_tracking,
				(SoundMIDISettingTransformation) transformation,
				(SoundMIDISettingFilter) filter,
			),
		);

		#pragma endregion

		#pragma region music midi setting

		template <typename = None>
		struct MusicMIDISettingTarget_;

		using MusicMIDISettingTarget = MusicMIDISettingTarget_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicMIDISettingTarget_<_>),
			M_wrap(
				(ID) id,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicMIDISettingClipTempoSource_;

		using MusicMIDISettingClipTempoSource = MusicMIDISettingClipTempoSource_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(MusicMIDISettingClipTempoSource_<_>),
			M_wrap(
				hierarchy,
				file,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicMIDISettingClipTempo_;

		using MusicMIDISettingClipTempo = MusicMIDISettingClipTempo_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicMIDISettingClipTempo_<_>),
			M_wrap(
				(MusicMIDISettingClipTempoSource) source,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicMIDISetting_;

		using MusicMIDISetting = MusicMIDISetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicMIDISetting_<_>),
			M_wrap(
				(MusicMIDISettingTarget) target,
				(MusicMIDISettingClipTempo) clip_tempo,
			),
		);

		#pragma endregion

		#pragma region audio playback limit setting

		template <typename = None>
		struct AudioPlaybackLimitSettingScope_;

		using AudioPlaybackLimitSettingScope = AudioPlaybackLimitSettingScope_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlaybackLimitSettingScope_<_>),
			M_wrap(
				per_game_object,
				globally,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlaybackLimitSettingWhenPriorityIsEqual_;

		using AudioPlaybackLimitSettingWhenPriorityIsEqual = AudioPlaybackLimitSettingWhenPriorityIsEqual_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlaybackLimitSettingWhenPriorityIsEqual_<_>),
			M_wrap(
				discard_oldest_instance,
				discard_newest_instance,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlaybackLimitSettingWhenLimitIsReached_;

		using AudioPlaybackLimitSettingWhenLimitIsReached = AudioPlaybackLimitSettingWhenLimitIsReached_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlaybackLimitSettingWhenLimitIsReached_<_>),
			M_wrap(
				kill_voice,
				use_virtual_voice_setting,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlaybackLimitSetting_;

		using AudioPlaybackLimitSetting = AudioPlaybackLimitSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlaybackLimitSetting_<_>),
			M_wrap(
				(Boolean) ignore_parent,
				(RegularValue<Integer>) value,
				(AudioPlaybackLimitSettingScope) scope,
				(AudioPlaybackLimitSettingWhenLimitIsReached) when_limit_is_reached,
				(AudioPlaybackLimitSettingWhenPriorityIsEqual) when_priority_is_equal,
			),
		);

		#pragma endregion

		#pragma region audio virtual voice setting

		template <typename = None>
		struct AudioVirtualVoiceSettingBehavior_;

		using AudioVirtualVoiceSettingBehavior = AudioVirtualVoiceSettingBehavior_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioVirtualVoiceSettingBehavior_<_>),
			M_wrap(
				continue_to_play,
				kill_voice,
				send_to_virtual_voice,
				kill_if_finite_else_virtual,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioVirtualVoiceSettingOnReturnToPhysical_;

		using AudioVirtualVoiceSettingOnReturnToPhysical = AudioVirtualVoiceSettingOnReturnToPhysical_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioVirtualVoiceSettingOnReturnToPhysical_<_>),
			M_wrap(
				play_from_beginning,
				play_from_elapsed_time,
				resume,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioVirtualVoiceSetting_;

		using AudioVirtualVoiceSetting = AudioVirtualVoiceSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioVirtualVoiceSetting_<_>),
			M_wrap(
				(AudioVirtualVoiceSettingBehavior) behavior,
				(AudioVirtualVoiceSettingOnReturnToPhysical) on_return_to_physical,
			),
		);

		#pragma endregion

		#pragma region audio playback priority setting

		template <typename = None>
		struct AudioPlaybackPrioritySetting_;

		using AudioPlaybackPrioritySetting = AudioPlaybackPrioritySetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlaybackPrioritySetting_<_>),
			M_wrap(
				(RegularValue<Floating>) value,
				(Boolean) use_distance_factor,
				(RegularValue<Floating>) offset_at_max_distance,
			),
		);

		#pragma endregion

		#pragma region audio time setting

		template <typename = None>
		struct AudioTimeSetting_;

		using AudioTimeSetting = AudioTimeSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioTimeSetting_<_>),
			M_wrap(
				(Floating) time,
				(Floating) offset,
				(Floating) tempo,
				(Tuple<Integer, Integer>) signature,
			),
		);

		#pragma endregion

		#pragma region audio switcher

		template <typename = None>
		struct AudioSwitcherSetting_;

		using AudioSwitcherSetting = AudioSwitcherSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioSwitcherSetting_<_>),
			M_wrap(
				(Boolean) is_state,
				(ID) group,
				(ID) default_item,
			),
		);

		#pragma endregion

		#pragma region audio switcher association

		template <typename = None>
		struct AudioSwitcherAssociationSettingItem_;

		using AudioSwitcherAssociationSettingItem = AudioSwitcherAssociationSettingItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioSwitcherAssociationSettingItem_<_>),
			M_wrap(
				(ID) group,
				(Boolean) is_state,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioSwitcherAssociationSettingMode_;

		using AudioSwitcherAssociationSettingMode = AudioSwitcherAssociationSettingMode_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioSwitcherAssociationSettingMode_<_>),
			M_wrap(
				best_match,
				weighted,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioSwitcherAssociationSettingPath_;

		using AudioSwitcherAssociationSettingPath = AudioSwitcherAssociationSettingPath_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioSwitcherAssociationSettingPath_<_>),
			M_wrap(
				(ID) u1,
				(ID) object,
				(Integer) weight,
				(Integer) probability,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioSwitcherAssociationSetting_;

		using AudioSwitcherAssociationSetting = AudioSwitcherAssociationSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioSwitcherAssociationSetting_<_>),
			M_wrap(
				(List<AudioSwitcherAssociationSettingItem>) item,
				(AudioSwitcherAssociationSettingMode) mode,
				(List<AudioSwitcherAssociationSettingPath>) path,
			),
		);

		#pragma endregion

		#pragma region audio source

		template <typename = None>
		struct AudioSourceType_;

		using AudioSourceType = AudioSourceType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioSourceType_<_>),
			M_wrap(
				internal,
				external,
				external_prefetch,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioSourceSetting_;

		using AudioSourceSetting = AudioSourceSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioSourceSetting_<_>),
			M_wrap(
				(PlugInID) plug_in,
				(AudioSourceType) type,
				(ID) resource,
				(Integer) resource_size,
				(Boolean) non_cachable_stream,
			),
		);

		#pragma endregion

		#pragma region audio play type setting

		template <typename = None>
		struct AudioPlayTypeRandomType_;

		using AudioPlayTypeRandomType = AudioPlayTypeRandomType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlayTypeRandomType_<_>),
			M_wrap(
				standard,
				shuffle,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayTypeRandomSetting_;

		using AudioPlayTypeRandomSetting = AudioPlayTypeRandomSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlayTypeRandomSetting_<_>),
			M_wrap(
				(AudioPlayTypeRandomType) type,
				(Integer) avoid_repeat,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayTypeSequenceAtEndOfPlaylist_;

		using AudioPlayTypeSequenceAtEndOfPlaylist = AudioPlayTypeSequenceAtEndOfPlaylist_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlayTypeSequenceAtEndOfPlaylist_<_>),
			M_wrap(
				restart,
				play_in_reserve_order,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayTypeSequenceSetting_;

		using AudioPlayTypeSequenceSetting = AudioPlayTypeSequenceSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlayTypeSequenceSetting_<_>),
			M_wrap(
				(AudioPlayTypeSequenceAtEndOfPlaylist) at_end_of_playlist,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayTypeSetting_;

		using AudioPlayTypeSetting = AudioPlayTypeSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlayTypeSetting_<_>),
			M_wrap(
				(AudioPlayTypeRandomSetting) random,
				(AudioPlayTypeSequenceSetting) sequence,
			),
		);

		#pragma endregion

		#pragma region audio play mode setting

		template <typename = None>
		struct AudioPlayModeStepSetting_;

		using AudioPlayModeStepSetting = AudioPlayModeStepSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlayModeStepSetting_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayModeContinuousTransitionType_;

		using AudioPlayModeContinuousTransitionType = AudioPlayModeContinuousTransitionType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(AudioPlayModeContinuousTransitionType_<_>),
			M_wrap(
				none,
				xfade_amp,
				xfade_power,
				delay,
				sample_accurate,
				trigger_rate,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayModeContinuousSetting_;

		using AudioPlayModeContinuousSetting = AudioPlayModeContinuousSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlayModeContinuousSetting_<_>),
			M_wrap(
				(Boolean) always_reset_playlist,
				(RandomizedValue<Integer>) loop_count,
				(AudioPlayModeContinuousTransitionType) transition_type,
				(RandomizedValue<Floating>) transition_duration,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayModeSetting_;

		using AudioPlayModeSetting = AudioPlayModeSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPlayModeSetting_<_>),
			M_wrap(
				(AudioPlayModeStepSetting) step,
				(AudioPlayModeContinuousSetting) continuous,
			),
		);

		#pragma endregion

		#pragma region unknown hierarchy

		template <typename = None>
		struct UnknownHierarchy_;

		using UnknownHierarchy = UnknownHierarchy_<>;

		template <typename _> requires (check_version(version, {88, 112, 140}))
		M_record_of_map(
			M_wrap(UnknownHierarchy_<_>),
			M_wrap(
				(Integer) type,
				(ByteList) data,
			),
		);

		#pragma endregion

		#pragma region state group

		template <typename = None>
		struct StateGroupCustomTransition_;

		using StateGroupCustomTransition = StateGroupCustomTransition_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(StateGroupCustomTransition_<_>),
			M_wrap(
				(ID) from,
				(ID) to,
				(Integer) time,
			),
		);

		// ----------------

		template <typename = None>
		struct StateGroup_;

		using StateGroup = StateGroup_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(StateGroup_<_>),
			M_wrap(
				(ID) id,
				(Integer) default_transition,
				(List<StateGroupCustomTransition>) custom_transition,
			),
		);

		#pragma endregion

		#pragma region switch group

		template <typename = None>
		struct SwitchGroupPoint_;

		using SwitchGroupPoint = SwitchGroupPoint_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SwitchGroupPoint_<_>),
			M_wrap(
				(Position2D<Floating, ID>) position,
				(Curve) curve,
			),
		);

		// ----------------

		template <typename = None>
		struct SwitchGroup_;

		using SwitchGroup = SwitchGroup_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SwitchGroup_<_>),
			M_wrap(
				(ID) id,
				(ID) parameter,
				(RealTimeParameterControlXAxisCategory) parameter_category,
				(List<SwitchGroupPoint>) point,
			),
		);

		#pragma endregion

		#pragma region game parameter

		template <typename = None>
		struct GameParameter_;

		using GameParameter = GameParameter_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(GameParameter_<_>),
			M_wrap(
				(ID) id,
				(Floating) range_default,
				(Integer) interpolation_mode,
				(Floating) interpolation_attack,
				(Floating) interpolation_release,
				(Integer) bind_to_built_in_parameter,
			),
		);

		#pragma endregion

		#pragma region plug-in reference

		template <typename = None>
		struct SourcePlugInReferenceU2_;

		using SourcePlugInReferenceU2 = SourcePlugInReferenceU2_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SourcePlugInReferenceU2_<_>),
			M_wrap(
				(Integer) u1,
				(Integer) u2,
				(Integer) u3,
				(Integer) u4,
				(Integer) u5,
				(Integer) u6,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct SourcePlugInReferenceU4_;

		using SourcePlugInReferenceU4 = SourcePlugInReferenceU4_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SourcePlugInReferenceU4_<_>),
			M_wrap(
				(Integer) u1,
				(Integer) u2,
				(Floating) u3,
			),
		);

		// ----------------

		template <typename = None>
		struct SourcePlugInReference_;

		using SourcePlugInReference = SourcePlugInReference_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SourcePlugInReference_<_>),
			M_wrap(
				(ID) id,
				(PlugInID) plug_in,
				(ByteList) expand,
				(List<SourcePlugInReferenceU2>) u2,
				(List<SourcePlugInReferenceU4>) u4,
			),
		);

		// ----------------

		template <typename = None>
		struct UnknownPlugInReference_;

		using UnknownPlugInReference = UnknownPlugInReference_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(UnknownPlugInReference_<_>),
			M_wrap(
				(ID) id,
				(PlugInID) plug_in,
				(ByteList) expand,
				(List<SourcePlugInReferenceU2>) u2,
				(List<SourcePlugInReferenceU4>) u4,
			),
		);

		#pragma endregion

		#pragma region stateful audio property setting

		template <typename = None>
		struct StatefulAudioPropertySetting_;

		using StatefulAudioPropertySetting = StatefulAudioPropertySetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(StatefulAudioPropertySetting_<_>),
			M_wrap(
				(ID) id,
				(Map<AudioPropertyType, Floating>) value,
			),
		);

		#pragma endregion

		#pragma region event action

		template <auto = version> requires (check_version(version, {}))
		struct EventActionProperty_ {

			template <typename = None>
			struct ValueApplyMode_;

			using ValueApplyMode = ValueApplyMode_<>;

			template <typename _> requires (check_version(version, {140}))
			M_enumeration(
				M_wrap(ValueApplyMode_<_>),
				M_wrap(
					absolute,
					relative,
				),
			);

			// ----------------

			template <typename = None>
			struct SeekType_;

			using SeekType = SeekType_<>;

			template <typename _> requires (check_version(version, {140}))
			M_enumeration(
				M_wrap(SeekType_<_>),
				M_wrap(
					time,
					percent,
				),
			);

			// ----------------

			template <typename = None>
			struct PlayAudio_;

			using PlayAudio = PlayAudio_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(PlayAudio_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Floating) probability,
					(ID) sound_bank,
				),
			);

			// ----------------

			template <typename = None>
			struct StopAudio_;

			using StopAudio = StopAudio_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(StopAudio_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			template <typename = None>
			struct PauseAudio_;

			using PauseAudio = PauseAudio_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(PauseAudio_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) include_delayed_resume_action,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			template <typename = None>
			struct ResumeAudio_;

			using ResumeAudio = ResumeAudio_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(ResumeAudio_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) master_resume,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			template <typename = None>
			struct BreakAudio_;

			using BreakAudio = BreakAudio_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(BreakAudio_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct SeekAudio_;

			using SeekAudio = SeekAudio_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SeekAudio_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
					(SeekType) seek_type,
					(RandomizedValue<Floating>) seek_value,
					(Boolean) seek_to_nearest_marker,
				),
			);

			// ----------------

			template <typename = None>
			struct PostEvent_;

			using PostEvent = PostEvent_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(PostEvent_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct SetBusVolume_;

			using SetBusVolume = SetBusVolume_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetBusVolume_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizedValue<Floating>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct SetVoiceVolume_;

			using SetVoiceVolume = SetVoiceVolume_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetVoiceVolume_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizedValue<Floating>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct SetVolumePitch_;

			using SetVolumePitch = SetVolumePitch_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetVolumePitch_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizedValue<Floating>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct SetVolumeLowPassFilter_;

			using SetVolumeLowPassFilter = SetVolumeLowPassFilter_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetVolumeLowPassFilter_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizedValue<Floating>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct SetVolumeHighPassFilter_;

			using SetVolumeHighPassFilter = SetVolumeHighPassFilter_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetVolumeHighPassFilter_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizedValue<Floating>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct SetMute_;

			using SetMute = SetMute_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetMute_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
				),
			);

			// ----------------

			template <typename = None>
			struct SetGameParameter_;

			using SetGameParameter = SetGameParameter_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetGameParameter_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(RandomizedValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizedValue<Floating>) value,
					(Boolean) bypass_game_parameter_interpolation,
				),
			);

			// ----------------

			template <typename = None>
			struct SetStateAvailability_;

			using SetStateAvailability = SetStateAvailability_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetStateAvailability_<_>),
				M_wrap(
					(Boolean) enable,
					(RandomizedValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct ActivateState_;

			using ActivateState = ActivateState_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(ActivateState_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(ID) group,
					(ID) item,
				),
			);

			// ----------------

			template <typename = None>
			struct ActivateSwitch_;

			using ActivateSwitch = ActivateSwitch_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(ActivateSwitch_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
					(ID) group,
					(ID) item,
				),
			);

			// ----------------

			template <typename = None>
			struct ActivateTrigger_;

			using ActivateTrigger = ActivateTrigger_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(ActivateTrigger_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizedValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct SetBypassEffect_;

			using SetBypassEffect = SetBypassEffect_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(SetBypassEffect_<_>),
				M_wrap(
					(Boolean) reset,
					(Boolean) enable,
					(RandomizedValue<Integer>) delay,
					(List<Boolean>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct ReleaseEnvelope_;

			using ReleaseEnvelope = ReleaseEnvelope_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(ReleaseEnvelope_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct ResetPlaylist_;

			using ResetPlaylist = ResetPlaylist_<>;

			template <typename _> requires (check_version(version, {140}))
			M_record_of_map(
				M_wrap(ResetPlaylist_<_>),
				M_wrap(
					(RandomizedValue<Integer>) delay,
					(Integer) u1,
				),
			);

			// ----------------

			template <typename = None>
			struct Type_;

			using Type = Type_<>;

			template <typename _> requires (check_version(version, {140}))
			M_enumeration(
				M_wrap(Type_<_>),
				M_wrap(
					play_audio,
					stop_audio,
					pause_audio,
					resume_audio,
					break_audio,
					seek_audio,
					post_event,
					set_bus_volume,
					set_voice_volume,
					set_voice_pitch,
					set_voice_low_pass_filter,
					set_voice_high_pass_filter,
					set_mute,
					set_game_parameter,
					set_state_availability,
					activate_state,
					activate_switch,
					activate_trigger,
					set_bypass_effect,
					release_envelope,
					reset_playlist,
				),
			);

			// ----------------

			using Item = decltype([] {
				if constexpr (check_version(version, {140})) {
					using Type = EnumerableVariant<
						Type,
						PlayAudio,
						StopAudio,
						PauseAudio,
						ResumeAudio,
						BreakAudio,
						SeekAudio,
						PostEvent,
						SetBusVolume,
						SetVoiceVolume,
						SetVolumePitch,
						SetVolumeLowPassFilter,
						SetVolumeHighPassFilter,
						SetMute,
						SetGameParameter,
						SetStateAvailability,
						ActivateState,
						ActivateSwitch,
						ActivateTrigger,
						SetBypassEffect,
						ReleaseEnvelope,
						ResetPlaylist
					>;
					return declare<Type>();
				}
			}());

		};

		using EventActionProperty = EventActionProperty_<>;

		// ----------------

		template <typename = None>
		struct EventActionException_;

		using EventActionException = EventActionException_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(EventActionException_<_>),
			M_wrap(
				(ID) id,
				(Boolean) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct EventAction_;

		using EventAction = EventAction_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(EventAction_<_>),
			M_wrap(
				(ID) id,
				(ID) target,
				(List<EventActionException>) exception,
				(Boolean) globally,
				(Integer) u1,
				(typename EventActionProperty::Item) property,
			),
		);

		#pragma endregion

		#pragma region event

		template <typename = None>
		struct Event_;

		using Event = Event_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(Event_<_>),
			M_wrap(
				(ID) id,
				(List<ID>) child,
			),
		);

		#pragma endregion

		#pragma region dialogue event

		template <typename = None>
		struct DialogueEvent_;

		using DialogueEvent = DialogueEvent_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(DialogueEvent_<_>),
			M_wrap(
				(ID) id,
				(Integer) probability,
				(AudioSwitcherAssociationSetting) switcher_association,
			),
		);

		#pragma endregion

		#pragma region attenuation

		template <typename = None>
		struct AttenuationCurve_;

		using AttenuationCurve = AttenuationCurve_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AttenuationCurve_<_>),
			M_wrap(
				(Integer) u1,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct AttenuationApplySetting_;

		using AttenuationApplySetting = AttenuationApplySetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AttenuationApplySetting_<_>),
			M_wrap(
				(Optional<Integer>) output_bus_volume,
				(Optional<Integer>) game_defined_auxiliary_send_volume,
				(Optional<Integer>) user_defined_auxiliary_send_volume,
				(Optional<Integer>) low_pass_filter,
				(Optional<Integer>) high_pass_filter,
				(Optional<Integer>) spread,
				(Optional<Integer>) focus,
			),
		);

		// ----------------

		template <typename = None>
		struct AttenuationCone_;

		using AttenuationCone = AttenuationCone_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AttenuationCone_<_>),
			M_wrap(
				(Floating) inner_angle,
				(Floating) outer_angle,
				(Floating) max_value,
				(Floating) low_pass_filter,
				(Floating) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct Attenuation_;

		using Attenuation = Attenuation_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(Attenuation_<_>),
			M_wrap(
				(ID) id,
				(AttenuationApplySetting) apply,
				(List<AttenuationCurve>) curve,
				(Integer) max_distance,
				(Boolean) height_spread,
				(Optional<AttenuationCone>) cone,
				(AttenuationRealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		#pragma endregion

		#pragma region modulator

		template <typename = None>
		struct ModulatorScope_;

		using ModulatorScope = ModulatorScope_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(ModulatorScope_<_>),
			M_wrap(
				voice,
				note_or_event,
				game_object,
				global,
			),
		);

		// ----------------

		template <typename = None>
		struct ModulatorTriggerOn_;

		using ModulatorTriggerOn = ModulatorTriggerOn_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(ModulatorTriggerOn_<_>),
			M_wrap(
				play,
				note_off,
			),
		);

		// ----------------

		template <typename = None>
		struct ModulatorWaveform_;

		using ModulatorWaveform = ModulatorWaveform_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(ModulatorWaveform_<_>),
			M_wrap(
				sine,
				triangle,
				square,
				saw_up,
				saw_down,
				random,
			),
		);

		// ----------------

		template <typename = None>
		struct LowFrequencyOscillatorModulator_;

		using LowFrequencyOscillatorModulator = LowFrequencyOscillatorModulator_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(LowFrequencyOscillatorModulator_<_>),
			M_wrap(
				(ID) id,
				(RandomizedValue<Floating>) depth,
				(RandomizedValue<Floating>) frequency,
				(ModulatorWaveform) waveform,
				(RandomizedValue<Floating>) smoothing,
				(RandomizedValue<Floating>) pulse_width_modulation,
				(RandomizedValue<Floating>) attack,
				(RandomizedValue<Floating>) initial_phase_offset,
				(ModulatorScope) scope,
				(ModulatorRealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		// ----------------

		template <typename = None>
		struct EnvelopeModulator_;

		using EnvelopeModulator = EnvelopeModulator_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(EnvelopeModulator_<_>),
			M_wrap(
				(ID) id,
				(RandomizedValue<Floating>) attack_time,
				(RandomizedValue<Floating>) attack_curve,
				(RandomizedValue<Floating>) decay_time,
				(RandomizedValue<Floating>) sustain_level,
				(RandomizedValue<Floating>) release_time,
				(ModulatorScope) scope,
				(ModulatorTriggerOn) trigger_on,
				(RandomizedValue<Floating>) sustain_time,
				(Boolean) stop_playback_after_release,
				(ModulatorRealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		// ----------------

		template <typename = None>
		struct TimeModulator_;

		using TimeModulator = TimeModulator_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(TimeModulator_<_>),
			M_wrap(
				(ID) id,
				(RandomizedValue<Floating>) initial_delay,
				(RegularValue<Floating>) duration,
				(RandomizedValue<Integer>) loop_count,
				(RandomizedValue<Floating>) playback_rate,
				(ModulatorScope) scope,
				(ModulatorTriggerOn) trigger_on,
				(Boolean) stop_playback_at_end,
				(ModulatorRealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		#pragma endregion

		#pragma region audio device

		template <typename = None>
		struct AudioDeviceSystemExpandSetting_;

		using AudioDeviceSystemExpandSetting = AudioDeviceSystemExpandSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioDeviceSystemExpandSetting_<_>),
			M_wrap(
				(Boolean) allow_3d_audio,
				(ID) main_mix_configuration_for_binauralization,
				(ID) main_mix_configuration_for_home_theater,
				(Boolean) allow_system_audio_object,
				(Integer) minimum_system_audio_object_required,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioDeviceExpandSetting_;

		using AudioDeviceExpandSetting = AudioDeviceExpandSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioDeviceExpandSetting_<_>),
			M_wrap(
				(Optional<AudioDeviceSystemExpandSetting>) system,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioDevice_;

		using AudioDevice = AudioDevice_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioDevice_<_>),
			M_wrap(
				(ID) id,
				(PlugInID) plug_in,
				(AudioDeviceExpandSetting) expand,
				(List<SourcePlugInReferenceU2>) u2,
				(List<SourcePlugInReferenceU4>) u4,
				(AudioEffectSetting) effect,
			),
		);

		#pragma endregion

		#pragma region audio bus

		template <typename = None>
		struct AudioBusMuteForBackgroundMusic_;

		using AudioBusMuteForBackgroundMusic = AudioBusMuteForBackgroundMusic_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioBusMuteForBackgroundMusic_<_>),
			M_wrap(
				(Boolean) enable,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioBusConfiguration_;

		using AudioBusConfiguration = AudioBusConfiguration_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioBusConfiguration_<_>),
			M_wrap(
				(Integer) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioBus_;

		using AudioBus = AudioBus_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) audio_device,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusVoiceVolumeGainSetting) voice_volume_gain,
				(BusBusSetting) bus,
				(BusOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(BusAutoDuckingSetting) auto_ducking,
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
			),
		);

		// ----------------

		template <typename = None>
		struct AuxiliaryAudioBus_;

		using AuxiliaryAudioBus = AuxiliaryAudioBus_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) audio_device,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusVoiceVolumeGainSetting) voice_volume_gain,
				(BusBusSetting) bus,
				(BusOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(BusAutoDuckingSetting) auto_ducking,
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
			),
		);

		#pragma endregion

		#pragma region sound

		template <typename = None>
		struct SoundPlaybackSetting_;

		using SoundPlaybackSetting = SoundPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundPlaybackSetting_<_>),
			M_wrap(
				(RandomizedValue<Floating>) initial_delay,
				(RandomizedValue<Integer>) loop_count,
			),
		);

		// ----------------

		template <typename = None>
		struct Sound_;

		using Sound = Sound_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(Sound_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundPlaylistContainerScope_;

		using SoundPlaylistContainerScope = SoundPlaylistContainerScope_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(SoundPlaylistContainerScope_<_>),
			M_wrap(
				game_object,
				global,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundPlaylistContainerPlaylistItem_;

		using SoundPlaylistContainerPlaylistItem = SoundPlaylistContainerPlaylistItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundPlaylistContainerPlaylistItem_<_>),
			M_wrap(
				(ID) item,
				(Integer) weight,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundPlaylistContainerPlaybackSetting_;

		using SoundPlaylistContainerPlaybackSetting = SoundPlaylistContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundPlaylistContainerPlaybackSetting_<_>),
			M_wrap(
				(RandomizedValue<Floating>) initial_delay,
				(SoundPlaylistContainerScope) scope,
				(AudioPlayType) type,
				(AudioPlayTypeSetting) type_setting,
				(AudioPlayMode) mode,
				(AudioPlayModeSetting) mode_setting,
				(List<SoundPlaylistContainerPlaylistItem>) playlist,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundPlaylistContainer_;

		using SoundPlaylistContainer = SoundPlaylistContainer_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundPlaylistContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundSwitchContainerObjectAttributeItem_;

		using SoundSwitchContainerObjectAttributeItem = SoundSwitchContainerObjectAttributeItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundSwitchContainerObjectAttributeItem_<_>),
			M_wrap(
				(ID) id,
				(Boolean) play_first_only,
				(Boolean) continue_to_play_across_switch,
				(Integer) u1,
				(Integer) fade_out_time,
				(Integer) fade_in_time,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundSwitchContainerObjectAssignItem_;

		using SoundSwitchContainerObjectAssignItem = SoundSwitchContainerObjectAssignItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundSwitchContainerObjectAssignItem_<_>),
			M_wrap(
				(ID) item,
				(List<ID>) object,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundSwitchContainerPlaybackSetting_;

		using SoundSwitchContainerPlaybackSetting = SoundSwitchContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundSwitchContainerPlaybackSetting_<_>),
			M_wrap(
				(RandomizedValue<Floating>) initial_delay,
				(AudioPlayMode) mode,
				(AudioSwitcherSetting) switcher,
				(List<SoundSwitchContainerObjectAttributeItem>) object_attribute,
				(List<SoundSwitchContainerObjectAssignItem>) object_assign,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundSwitchContainer_;

		using SoundSwitchContainer = SoundSwitchContainer_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundSwitchContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundBlendContainerTrackItem_;

		using SoundBlendContainerTrackItem = SoundBlendContainerTrackItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundBlendContainerTrackItem_<_>),
			M_wrap(
				(ID) id,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(ID) cross_fade,
				(RealTimeParameterControlXAxisCategory) cross_fade_category,
				(Integer) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundBlendContainerPlaybackSetting_;

		using SoundBlendContainerPlaybackSetting = SoundBlendContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundBlendContainerPlaybackSetting_<_>),
			M_wrap(
				(RandomizedValue<Floating>) initial_delay,
				(AudioPlayMode) mode,
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundBlendContainer_;

		using SoundBlendContainer = SoundBlendContainer_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundBlendContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct ActorMixerPlaybackSetting_;

		using ActorMixerPlaybackSetting = ActorMixerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(ActorMixerPlaybackSetting_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct ActorMixer_;

		using ActorMixer = ActorMixer_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(ActorMixer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		#pragma endregion

		#pragma region music

		template <typename = None>
		struct MusicTrackTrackType_;

		using MusicTrackTrackType = MusicTrackTrackType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(MusicTrackTrackType_<_>),
			M_wrap(
				normal,
				random_step,
				sequence_step,
				switcher,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackClipCurveItemType_;

		using MusicTrackClipCurveItemType = MusicTrackClipCurveItemType_<>;

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(MusicTrackClipCurveItemType_<_>),
			M_wrap(
				voice_volume,
				voice_low_pass_filter,
				voice_high_pass_filter,
				clip_fade_in,
				clip_fade_out,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackClipCurveItem_;

		using MusicTrackClipCurveItem = MusicTrackClipCurveItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackClipCurveItem_<_>),
			M_wrap(
				(Integer) index,
				(MusicTrackClipCurveItemType) type,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackClipItem_;

		using MusicTrackClipItem = MusicTrackClipItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackClipItem_<_>),
			M_wrap(
				(Integer) u1,
				(ID) source,
				(ID) event,
				(Floating) offset,
				(Floating) begin,
				(Floating) end,
				(Floating) duration,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackClip_;

		using MusicTrackClip = MusicTrackClip_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackClip_<_>),
			M_wrap(
				(Integer) u1,
				(List<MusicTrackClipItem>) item,
				(List<MusicTrackClipCurveItem>) curve,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackPlaybackSetting_;

		using MusicTrackPlaybackSetting = MusicTrackPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackPlaybackSetting_<_>),
			M_wrap(
				(MusicTrackClip) clip,
				(MusicTrackTrackType) type,
				(AudioSwitcherSetting) switcher,
				(MusicTrackTransitionSetting) transition,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackStream_;

		using MusicTrackStream = MusicTrackStream_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackStream_<_>),
			M_wrap(
				(Integer) look_ahead_time,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrack_;

		using MusicTrack = MusicTrack_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrack_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<AudioSourceSetting>) source,
				(MusicTrackPlaybackSetting) playback_setting,
				(MusicTrackStream) stream,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSegmentCueItem_;

		using MusicSegmentCueItem = MusicSegmentCueItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicSegmentCueItem_<_>),
			M_wrap(
				(ID) name,
				(Floating) time,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSegmentCue_;

		using MusicSegmentCue = MusicSegmentCue_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicSegmentCue_<_>),
			M_wrap(
				(List<MusicSegmentCueItem>) item,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSegmentPlaybackSetting_;

		using MusicSegmentPlaybackSetting = MusicSegmentPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicSegmentPlaybackSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) speed,
				(Floating) duration,
				(MusicSegmentCue) cue,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSegment_;

		using MusicSegment = MusicSegment_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicSegment_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicPlaylistContainerPlaylistItem_;

		using MusicPlaylistContainerPlaylistItem = MusicPlaylistContainerPlaylistItem_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainerPlaylistItem_<_>),
			M_wrap(
				(ID) u1,
				(Boolean) group,
				(Integer) child_count,
				(ID) item,
				(Optional<AudioPlayType>) play_type,
				(Optional<AudioPlayMode>) play_mode,
				(AudioPlayTypeRandomSetting) random_setting,
				(Integer) weight,
				(Integer) loop_count,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicPlaylistContainerPlaybackSetting_;

		using MusicPlaylistContainerPlaybackSetting = MusicPlaylistContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainerPlaybackSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) speed,
				(List<MusicPlaylistContainerPlaylistItem>) playlist,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicPlaylistContainer_;

		using MusicPlaylistContainer = MusicPlaylistContainer_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSwitchContainerPlaybackSetting_;

		using MusicSwitchContainerPlaybackSetting = MusicSwitchContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicSwitchContainerPlaybackSetting_<_>),
			M_wrap(
				(RegularValue<Floating>) speed,
				(Boolean) continue_playing_on_switch_change,
				(AudioSwitcherAssociationSetting) switcher_association,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSwitchContainer_;

		using MusicSwitchContainer = MusicSwitchContainer_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicSwitchContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(AudioRealTimeParameterControlSetting) real_time_parameter_control,
				(AudioStateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_metadata,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		#pragma endregion

		#pragma region hierarchy

		template <typename = None>
		struct HierarchyType_;

		using HierarchyType = HierarchyType_<>;

		template <typename _> requires (check_version(version, {88, 112}))
		M_enumeration(
			M_wrap(HierarchyType_<_>),
			M_wrap(
				unknown,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(HierarchyType_<_>),
			M_wrap(
				unknown,
				source_plug_in_reference,
				unknown_plug_in_reference,
				stateful_audio_property_setting,
				event_action,
				event,
				dialogue_event,
				attenuation,
				low_frequency_oscillator_modulator,
				envelope_modulator,
				time_modulator,
				audio_device,
				audio_bus,
				auxiliary_audio_bus,
				sound,
				sound_playlist_container,
				sound_switch_container,
				sound_blend_container,
				actor_mixer,
				music_track,
				music_segment,
				music_playlist_container,
				music_switch_container,
			),
		);

		// ----------------

		using Hierarchy = decltype([] {
			if constexpr (check_version(version, {88, 112})) {
				using Type = EnumerableVariant<
					HierarchyType,
					UnknownHierarchy
				>;
				return declare<Type>();
			}
			if constexpr (check_version(version, {140})) {
				using Type = EnumerableVariant<
					HierarchyType,
					UnknownHierarchy,
					SourcePlugInReference,
					UnknownPlugInReference,
					StatefulAudioPropertySetting,
					EventAction,
					Event,
					DialogueEvent,
					Attenuation,
					LowFrequencyOscillatorModulator,
					EnvelopeModulator,
					TimeModulator,
					AudioDevice,
					AudioBus,
					AuxiliaryAudioBus,
					Sound,
					SoundPlaylistContainer,
					SoundSwitchContainer,
					SoundBlendContainer,
					ActorMixer,
					MusicTrack,
					MusicSegment,
					MusicPlaylistContainer,
					MusicSwitchContainer
				>;
				return declare<Type>();
			}
		}());

		#pragma endregion

		#pragma region game synchronization

		template <typename = None>
		struct GameSynchronizationU1_;

		using GameSynchronizationU1 = GameSynchronizationU1_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(GameSynchronizationU1_<_>),
			M_wrap(
				(ID) id,
				(Floating) u1,
				(Floating) u2,
				(Floating) u3,
				(Floating) u4,
				(Floating) u5,
				(Floating) u6,
			),
		);

		// ----------------

		template <typename = None>
		struct GameSynchronization_;

		using GameSynchronization = GameSynchronization_<>;

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(GameSynchronization_<_>),
			M_wrap(
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(GameSynchronization_<_>),
			M_wrap(
				(List<StateGroup>) state_group,
				(List<SwitchGroup>) switch_group,
				(List<GameParameter>) game_parameter,
				(List<GameSynchronizationU1>) u1,
			),
		);

		#pragma endregion

		#pragma region setting

		template <typename = None>
		struct PlugIn_;

		using PlugIn = PlugIn_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(PlugIn_<_>),
			M_wrap(
				(PlugInID) id,
				(String) name,
			),
		);

		// ----------------

		template <typename = None>
		struct ObstructionSetting_;

		using ObstructionSetting = ObstructionSetting_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(ObstructionSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(Integer) u1,
				(List<Point2D>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct ObstructionSettingBundle_;

		using ObstructionSettingBundle = ObstructionSettingBundle_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(ObstructionSettingBundle_<_>),
			M_wrap(
				(ObstructionSetting) volume,
				(ObstructionSetting) low_pass_filter,
				(ObstructionSetting) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct Setting_;

		using Setting = Setting_<>;

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(Setting_<_>),
			M_wrap(
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(Setting_<_>),
			M_wrap(
				(String) platform,
				(Floating) volume_threshold,
				(Integer) maximum_voice_instance,
				(List<PlugIn>) plug_in,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
			),
		);

		#pragma endregion

		#pragma region reference

		template <typename = None>
		struct Reference_;

		using Reference = Reference_<>;

		template <typename _> requires (check_version(version, {88, 112, 140}))
		M_record_of_map(
			M_wrap(Reference_<_>),
			M_wrap(
				(ID) id,
				(String) name,
			),
		);

		#pragma endregion

		#pragma region sound bank

		template <typename = None>
		struct SoundBank_;

		using SoundBank = SoundBank_<>;

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(SoundBank_<_>),
			M_wrap(
				(ID) id,
				(ID) language,
				(ByteList) header_expand,
				(List<Reference>) reference,
				(List<ID>) embedded_audio,
				(Optional<Setting>) setting,
				(Optional<GameSynchronization>) game_synchronization,
				(List<Hierarchy>) hierarchy,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(SoundBank_<_>),
			M_wrap(
				(ID) id,
				(ID) language,
				(ByteList) header_expand,
				(List<Reference>) reference,
				(List<ID>) embedded_audio,
				(Optional<Setting>) setting,
				(Optional<GameSynchronization>) game_synchronization,
				(List<Hierarchy>) hierarchy,
			),
		);

		#pragma endregion

	};

}
