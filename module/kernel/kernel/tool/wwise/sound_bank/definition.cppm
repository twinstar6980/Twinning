module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.wwise.sound_bank.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.wwise.sound_bank.version;

export namespace Twinning::Kernel::Tool::Wwise::SoundBank {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Definition {

		#pragma region identifier

		using Identifier = Integer;

		#pragma endregion

		#pragma region value

		template <typename TValue>
		M_record_of_list(
			M_wrap(RegularValue),
			M_wrap(
				(TValue) value,
			),
		);

		template <typename TValue>
		M_record_of_list(
			M_wrap(RandomizableValue),
			M_wrap(
				(TValue) value,
				(TValue) minimum_value,
				(TValue) maximum_value,
			),
		);

		#pragma endregion

		#pragma region common

		M_nested_template_declaration(Curve);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(Curve),
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

		M_nested_template_declaration(TimePoint);

		M_nested_template_definition_check(check_version(t_version, {72, 140}))
		M_enumeration(
			M_nested_template_definition_name(TimePoint),
			M_wrap(
				immediate,
				next_grid,
				next_bar,
				next_beat,
				next_cue,
				custom_cue,
				entry_cue,
				exit_cue,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_enumeration(
			M_nested_template_definition_name(TimePoint),
			M_wrap(
				immediate,
				next_grid,
				next_bar,
				next_beat,
				next_cue,
				custom_cue,
				entry_cue,
				exit_cue,
				last_exit_position,
			),
		);

		#pragma endregion

		#pragma region coordinate

		M_nested_template_declaration(CoordinateMode);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(CoordinateMode),
			M_wrap(
				linear,
				scaled,
				scaled_3,
			),
		);

		// ----------------

		M_nested_template_declaration(CoordinatePoint);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(CoordinatePoint),
			M_wrap(
				(Position2<Floater>) position,
				(Curve) curve,
			),
		);

		// ----------------

		M_nested_template_declaration(CoordinateIdentifierPoint);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(CoordinateIdentifierPoint),
			M_wrap(
				(Position2<Floater, Identifier>) position,
				(Curve) curve,
			),
		);

		#pragma endregion

		#pragma region property

		M_nested_template_declaration(PropertyCategory);

		M_nested_template_definition_check(check_version(t_version, {72, 145}))
		M_enumeration(
			M_nested_template_definition_name(PropertyCategory),
			M_wrap(
				unidirectional,
				bidirectional,
				bidirectional_ranged,
				boolean,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {145}))
		M_enumeration(
			M_nested_template_definition_name(PropertyCategory),
			M_wrap(
				unidirectional,
				bidirectional,
				bidirectional_ranged,
				boolean,
				unknown_6,
			),
		);

		#pragma endregion

		#pragma region parameter

		M_nested_template_declaration(ParameterCategory);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(ParameterCategory),
			M_wrap(
				game_parameter,
				midi_parameter,
				modulator,
			),
		);

		// ----------------

		M_nested_template_declaration(Parameter);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_list(
			M_nested_template_definition_name(Parameter),
			M_wrap(
				(Identifier) identifier,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_list(
			M_nested_template_definition_name(Parameter),
			M_wrap(
				(Identifier) identifier,
				(ParameterCategory) category,
			),
		);

		#pragma endregion

		#pragma region real time parameter control setting

		M_nested_template_declaration(RealTimeParameterControlSettingItem);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(RealTimeParameterControlSettingItem),
			M_wrap(
				(Integer) type,
				(Parameter) parameter,
				(Identifier) u2,
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(RealTimeParameterControlSettingItem),
			M_wrap(
				(Integer) type,
				(Parameter) parameter,
				(PropertyCategory) u1,
				(Identifier) u2,
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(RealTimeParameterControlSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(RealTimeParameterControlSetting),
			M_wrap(
				(List<RealTimeParameterControlSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region state setting

		M_nested_template_declaration(StateSettingAttribute);

		M_nested_template_definition_check(check_version(t_version, {125, 128}))
		M_record_of_map(
			M_nested_template_definition_name(StateSettingAttribute),
			M_wrap(
				(Integer) type,
				(PropertyCategory) category,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(StateSettingAttribute),
			M_wrap(
				(Integer) type,
				(PropertyCategory) category,
				(Integer) u1,
			),
		);

		// ----------------

		M_nested_template_declaration(StateSettingApplyItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(StateSettingApplyItem),
			M_wrap(
				(Identifier) target,
				(Identifier) setting,
			),
		);

		// ----------------

		M_nested_template_declaration(StateSettingItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(StateSettingItem),
			M_wrap(
				(Identifier) group,
				(TimePoint) change_occur_at,
				(List<StateSettingApplyItem>) apply,
			),
		);

		// ----------------

		M_nested_template_declaration(StateSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 125}))
		M_record_of_map(
			M_nested_template_definition_name(StateSetting),
			M_wrap(
				(List<StateSettingItem>) item,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {125}))
		M_record_of_map(
			M_nested_template_definition_name(StateSetting),
			M_wrap(
				(List<StateSettingAttribute>) attribute,
				(List<StateSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio play setting

		M_nested_template_declaration(AudioPlayType);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlayType),
			M_wrap(
				sequence,
				random,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayMode);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlayMode),
			M_wrap(
				step,
				continuous,
			),
		);

		#pragma endregion

		#pragma region voice setting

		M_nested_template_declaration(BusVoiceSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(BusVoiceSetting),
			M_wrap(
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) pitch,
				(RegularValue<Floater>) low_pass_filter,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(BusVoiceSetting),
			M_wrap(
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) pitch,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioVoice);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioVoice),
			M_wrap(
				(RandomizableValue<Floater>) volume,
				(RandomizableValue<Floater>) pitch,
				(RandomizableValue<Floater>) low_pass_filter,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioVoice),
			M_wrap(
				(RandomizableValue<Floater>) volume,
				(RandomizableValue<Floater>) pitch,
				(RandomizableValue<Floater>) low_pass_filter,
				(RandomizableValue<Floater>) high_pass_filter,
			),
		);

		#pragma endregion

		#pragma region voice volume setting

		M_nested_template_declaration(BusVoiceVolumeGainSetting);

		M_nested_template_definition_check(check_version(t_version, {125}))
		M_record_of_map(
			M_nested_template_definition_name(BusVoiceVolumeGainSetting),
			M_wrap(
				(RegularValue<Floater>) make_up,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioVoiceVolumeGainSetting);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioVoiceVolumeGainSetting),
			M_wrap(
				(Boolean) normalization,
				(RegularValue<Floater>) make_up,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioVoiceVolumeGainSetting),
			M_wrap(
				(Boolean) normalization,
				(RandomizableValue<Floater>) make_up,
			),
		);

		#pragma endregion

		#pragma region bus setting

		M_nested_template_declaration(BusBusSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(BusBusSetting),
			M_wrap(
				(RegularValue<Floater>) volume,
			),
		);

		// ----------------

		M_nested_template_declaration(BusOutputBusSetting);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(BusOutputBusSetting),
			M_wrap(
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioOutputBusSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioOutputBusSetting),
			M_wrap(
				(Identifier) bus,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioOutputBusSetting),
			M_wrap(
				(Identifier) bus,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		#pragma endregion

		#pragma region audio auxiliary send setting

		M_nested_template_declaration(AudioGameDefinedAuxiliarySendSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioGameDefinedAuxiliarySendSetting),
			M_wrap(
				(Boolean) enable,
				(RegularValue<Floater>) volume,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioGameDefinedAuxiliarySendSetting),
			M_wrap(
				(Boolean) enable,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioUserDefinedAuxiliarySendSettingItem);

		M_nested_template_definition_check(check_version(t_version, {72, 128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioUserDefinedAuxiliarySendSettingItem),
			M_wrap(
				(Identifier) bus,
				(RegularValue<Floater>) volume,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioUserDefinedAuxiliarySendSettingItem),
			M_wrap(
				(Identifier) bus,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioUserDefinedAuxiliarySendSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioUserDefinedAuxiliarySendSetting),
			M_wrap(
				(Boolean) enable,
				(AudioUserDefinedAuxiliarySendSettingItem) item_1,
				(AudioUserDefinedAuxiliarySendSettingItem) item_2,
				(AudioUserDefinedAuxiliarySendSettingItem) item_3,
				(AudioUserDefinedAuxiliarySendSettingItem) item_4,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioEarlyReflectionAuxiliarySendSetting);

		M_nested_template_definition_check(check_version(t_version, {135}))
		M_record_of_map(
			M_nested_template_definition_name(AudioEarlyReflectionAuxiliarySendSetting),
			M_wrap(
				(Identifier) bus,
				(RegularValue<Floater>) volume,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioAuxiliarySendSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 135}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAuxiliarySendSetting),
			M_wrap(
				(AudioGameDefinedAuxiliarySendSetting) game_defined,
				(AudioUserDefinedAuxiliarySendSetting) user_defined,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAuxiliarySendSetting),
			M_wrap(
				(AudioGameDefinedAuxiliarySendSetting) game_defined,
				(AudioUserDefinedAuxiliarySendSetting) user_defined,
				(AudioEarlyReflectionAuxiliarySendSetting) early_reflection,
			),
		);

		#pragma endregion

		#pragma region audio effect setting

		M_nested_template_declaration(AudioEffectSettingItem);

		M_nested_template_definition_check(check_version(t_version, {72, 150}))
		M_record_of_map(
			M_nested_template_definition_name(AudioEffectSettingItem),
			M_wrap(
				(Integer) index,
				(Identifier) identifier,
				(Boolean) use_share_set,
				(Boolean) u1,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(AudioEffectSettingItem),
			M_wrap(
				(Integer) index,
				(Identifier) identifier,
				(Boolean) use_share_set,
				(Boolean) bypass,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioEffectSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 150}))
		M_record_of_map(
			M_nested_template_definition_name(AudioEffectSetting),
			M_wrap(
				(Tuple<Boolean, Boolean, Boolean, Boolean, Boolean>) bypass,
				(List<AudioEffectSettingItem>) item,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(AudioEffectSetting),
			M_wrap(
				(Boolean) bypass,
				(List<AudioEffectSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio metadata setting

		M_nested_template_declaration(AudioMetadataSettingItem);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioMetadataSettingItem),
			M_wrap(
				(Integer) index,
				(Identifier) identifier,
				(Boolean) use_share_set,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioMetadataSetting);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioMetadataSetting),
			M_wrap(
				(List<AudioMetadataSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio positioning setting

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint),
			M_wrap(
				(Position2<Floater>) position,
				(Integer) duration,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint),
			M_wrap(
				(Position3<Floater>) position,
				(Integer) duration,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_list(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint),
			M_wrap(
				(Integer) begin,
				(Integer) count,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_list(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange),
			M_wrap(
				(Floater) left_right,
				(Floater) front_back,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_list(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange),
			M_wrap(
				(Floater) left_right,
				(Floater) front_back,
				(Floater) up_down,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSourceAutomationPath);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomationPath),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint) point,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange) random_range,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSourceAutomation);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceAutomation),
			M_wrap(
				(AudioPlayType) play_type,
				(AudioPlayMode) play_mode,
				(Boolean) pick_new_path_when_sound_start,
				(Boolean) loop,
				(Integer) transition_time,
				(List<AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint>) point,
				(List<AudioPositioningSettingListenerRoutingPositionSourceAutomationPath>) path,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSourceMode);

		M_nested_template_definition_check(check_version(t_version, {72, 132}))
		M_enumeration(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceMode),
			M_wrap(
				user_defined,
				game_defined,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {132}))
		M_enumeration(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSourceMode),
			M_wrap(
				emitter,
				emitter_with_automation,
				listener_with_automation,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingPositionSource);

		M_nested_template_definition_check(check_version(t_version, {72, 132}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSource),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) update_at_each_frame,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomation) automation,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {132, 140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSource),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) hold_emitter_position_and_orientation,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomation) automation,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingPositionSource),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) hold_emitter_position_and_orientation,
				(Boolean) diffraction_and_transmission,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomation) automation,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingAttenuation);

		M_nested_template_definition_check(check_version(t_version, {72, 134}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingAttenuation),
			M_wrap(
				(Identifier) identifier,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {134}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingAttenuation),
			M_wrap(
				(Boolean) enable,
				(Identifier) identifier,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRoutingSpatialization);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_enumeration(
			M_nested_template_definition_name(AudioPositioningSettingListenerRoutingSpatialization),
			M_wrap(
				none,
				position,
				position_and_orientation,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingListenerRouting);

		M_nested_template_definition_check(check_version(t_version, {72, 128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRouting),
			M_wrap(
				(Boolean) spatialization,
				(AudioPositioningSettingListenerRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRoutingPositionSource) position_source,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 132}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRouting),
			M_wrap(
				(AudioPositioningSettingListenerRoutingSpatialization) spatialization,
				(AudioPositioningSettingListenerRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRoutingPositionSource) position_source,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {132}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingListenerRouting),
			M_wrap(
				(Boolean) enable,
				(AudioPositioningSettingListenerRoutingSpatialization) spatialization,
				(RegularValue<Floater>) speaker_panning_divsion_spatialization_mix,
				(AudioPositioningSettingListenerRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRoutingPositionSource) position_source,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingSpeakerPanningMode);

		M_nested_template_definition_check(check_version(t_version, {132, 140}))
		M_enumeration(
			M_nested_template_definition_name(AudioPositioningSettingSpeakerPanningMode),
			M_wrap(
				direct_assignment,
				balance_fade,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_enumeration(
			M_nested_template_definition_name(AudioPositioningSettingSpeakerPanningMode),
			M_wrap(
				direct_assignment,
				balance_fade,
				steering,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingSpeakerPanning);

		M_nested_template_definition_check(check_version(t_version, {72, 132}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingSpeakerPanning),
			M_wrap(
				(Boolean) enable,
				(Position2<Floater>) position,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {132, 140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingSpeakerPanning),
			M_wrap(
				(AudioPositioningSettingSpeakerPanningMode) mode,
				(Position2<Floater>) position,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSettingSpeakerPanning),
			M_wrap(
				(AudioPositioningSettingSpeakerPanningMode) mode,
				(Position3<Floater>) position,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSettingType);

		M_nested_template_definition_check(check_version(t_version, {72, 132}))
		M_enumeration(
			M_nested_template_definition_name(AudioPositioningSettingType),
			M_wrap(
				two_dimension,
				three_dimension,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPositioningSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 125}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSetting),
			M_wrap(
				(AudioPositioningSettingType) type,
				(RegularValue<Floater>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRouting) listener_routing,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {125, 132}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSetting),
			M_wrap(
				(Boolean) enable,
				(AudioPositioningSettingType) type,
				(RegularValue<Floater>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRouting) listener_routing,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {132}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPositioningSetting),
			M_wrap(
				(RegularValue<Floater>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRouting) listener_routing,
			),
		);

		#pragma endregion

		#pragma region audio motion setting

		M_nested_template_declaration(AudioMotionSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioMotionSetting),
			M_wrap(
				(RandomizableValue<Floater>) volume_offset,
				(RandomizableValue<Floater>) low_pass_filter,
			),
		);

		#pragma endregion

		#pragma region audio mixer setting

		M_nested_template_declaration(AudioMixerSetting);

		M_nested_template_definition_check(check_version(t_version, {112, 150}))
		M_record_of_map(
			M_nested_template_definition_name(AudioMixerSetting),
			M_wrap(
				(Identifier) identifier,
			),
		);

		#pragma endregion

		#pragma region bus auto ducking setting

		M_nested_template_declaration(BusAutomaticDuckingSettingBusTarget);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(BusAutomaticDuckingSettingBusTarget),
			M_wrap(
				voice_volume,
				bus_volume,
			),
		);

		// ----------------

		M_nested_template_declaration(BusAutomaticDuckingSettingBus);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(BusAutomaticDuckingSettingBus),
			M_wrap(
				(Identifier) identifier,
				(Floater) volume,
				(Integer) fade_out,
				(Integer) fade_in,
				(Curve) curve,
				(BusAutomaticDuckingSettingBusTarget) target,
			),
		);

		// ----------------

		M_nested_template_declaration(BusAutomaticDuckingSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(BusAutomaticDuckingSetting),
			M_wrap(
				(Integer) recovery_time,
				(Floater) maximum_ducking_volume,
				(List<BusAutomaticDuckingSettingBus>) bus,
			),
		);

		#pragma endregion

		#pragma region music transition setting

		M_nested_template_declaration(MusicTransitionSettingJumpMode);

		M_nested_template_definition_check(check_version(t_version, {134}))
		M_enumeration(
			M_nested_template_definition_name(MusicTransitionSettingJumpMode),
			M_wrap(
				start,
				specific,
				next,
				last_played,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSettingSynchronizeMode);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(MusicTransitionSettingSynchronizeMode),
			M_wrap(
				entry_cue,
				random_cue,
				custom_cue,
				same_time_as_playing_segment,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSettingItemFade);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSettingItemFade),
			M_wrap(
				(Integer) time,
				(Integer) curve,
				(Integer) offset,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSettingItemSource);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSettingItemSource),
			M_wrap(
				(Identifier) identifier,
				(TimePoint) exit_source_at,
				(Identifier) exit_source_at_custom_cue_match,
				(Boolean) play_post_exit,
				(MusicTransitionSettingItemFade) fade_out,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSettingItemDestination);

		M_nested_template_definition_check(check_version(t_version, {72, 134}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSettingItemDestination),
			M_wrap(
				(Identifier) identifier,
				(MusicTransitionSettingSynchronizeMode) synchronize_to,
				(Boolean) play_pre_entry,
				(Boolean) custom_cue_filter_match_source_cue_name,
				(Identifier) custom_cue_filter_match_target,
				(MusicTransitionSettingItemFade) fade_in,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {134}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSettingItemDestination),
			M_wrap(
				(Identifier) identifier,
				(MusicTransitionSettingJumpMode) jump_to,
				(MusicTransitionSettingSynchronizeMode) synchronize_to,
				(Boolean) play_pre_entry,
				(Boolean) custom_cue_filter_match_source_cue_name,
				(Identifier) custom_cue_filter_match_target,
				(MusicTransitionSettingItemFade) fade_in,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSettingItemSegment);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSettingItemSegment),
			M_wrap(
				(Boolean) enable,
				(Identifier) identifier,
				(Boolean) play_pre_entry,
				(MusicTransitionSettingItemFade) fade_in,
				(Boolean) play_post_exit,
				(MusicTransitionSettingItemFade) fade_out,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSettingItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSettingItem),
			M_wrap(
				(Identifier) u1,
				(MusicTransitionSettingItemSource) source,
				(MusicTransitionSettingItemDestination) destination,
				(MusicTransitionSettingItemSegment) segment,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTransitionSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTransitionSetting),
			M_wrap(
				(List<MusicTransitionSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region music track transition setting

		M_nested_template_declaration(MusicTrackTransitionSettingItemSource);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackTransitionSettingItemSource),
			M_wrap(
				(TimePoint) exit_source_at,
				(Identifier) exit_source_at_custom_cue_match,
				(MusicTransitionSettingItemFade) fade_out,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackTransitionSettingItemDestination);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackTransitionSettingItemDestination),
			M_wrap(
				(MusicTransitionSettingItemFade) fade_in,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackTransitionSetting);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackTransitionSetting),
			M_wrap(
				(Identifier) switcher,
				(MusicTrackTransitionSettingItemSource) source,
				(MusicTrackTransitionSettingItemDestination) destination,
			),
		);

		#pragma endregion

		#pragma region music stinger setting

		M_nested_template_declaration(MusicStingerSettingItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicStingerSettingItem),
			M_wrap(
				(Identifier) trigger,
				(Identifier) segment_to_play,
				(TimePoint) play_at,
				(Identifier) cue_name,
				(Integer) do_not_play_this_stinger_again_for,
				(Boolean) allow_playing_stinger_in_next_segment,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicStingerSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicStingerSetting),
			M_wrap(
				(List<MusicStingerSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region bus hdr setting

		M_nested_template_declaration(BusHdrSettingDynamicReleaseMode);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_enumeration(
			M_nested_template_definition_name(BusHdrSettingDynamicReleaseMode),
			M_wrap(
				linear,
				exponential,
			),
		);

		// ----------------

		M_nested_template_declaration(BusHdrSettingWindowTopOutputGameParameter);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(BusHdrSettingWindowTopOutputGameParameter),
			M_wrap(
				(Identifier) identifier,
				(Floater) minimum,
				(Floater) maximum,
			),
		);

		// ----------------

		M_nested_template_declaration(BusHdrSettingDynamic);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(BusHdrSettingDynamic),
			M_wrap(
				(Floater) threshold,
				(Floater) ratio,
				(Floater) release_time,
				(BusHdrSettingDynamicReleaseMode) release_mode,
			),
		);

		// ----------------

		M_nested_template_declaration(BusHdrSetting);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(BusHdrSetting),
			M_wrap(
				(Boolean) enable,
				(Boolean) u1,
				(BusHdrSettingDynamic) dynamic,
				(BusHdrSettingWindowTopOutputGameParameter) window_top_output_game_parameter,
			),
		);

		#pragma endregion

		#pragma region audio hdr setting

		M_nested_template_declaration(AudioHdrSettingEnvelopeTracking);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioHdrSettingEnvelopeTracking),
			M_wrap(
				(Boolean) enable,
				(Floater) active_range,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioHdrSetting);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioHdrSetting),
			M_wrap(
				(AudioHdrSettingEnvelopeTracking) envelope_tracking,
			),
		);

		#pragma endregion

		#pragma region sound midi setting

		M_nested_template_declaration(SoundMidiSettingEventPlayOn);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(SoundMidiSettingEventPlayOn),
			M_wrap(
				note_on,
				note_off,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundMidiSettingEvent);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundMidiSettingEvent),
			M_wrap(
				(SoundMidiSettingEventPlayOn) play_on,
				(Boolean) break_on_note_off,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundMidiSettingNoteTracking);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundMidiSettingNoteTracking),
			M_wrap(
				(Boolean) enable,
				(Integer) root_note,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundMidiSettingTransformation);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundMidiSettingTransformation),
			M_wrap(
				(RegularValue<Integer>) transposition,
				(RegularValue<Integer>) velocity_offset,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundMidiSettingFilter);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundMidiSettingFilter),
			M_wrap(
				(Integer) key_range_minimum,
				(Integer) key_range_maximum,
				(Integer) velocity_minimum,
				(Integer) velocity_maximum,
				(Integer) channel,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundMidiSetting);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundMidiSetting),
			M_wrap(
				(SoundMidiSettingEvent) event,
				(SoundMidiSettingNoteTracking) note_tracking,
				(SoundMidiSettingTransformation) transformation,
				(SoundMidiSettingFilter) filter,
			),
		);

		#pragma endregion

		#pragma region music midi setting

		M_nested_template_declaration(MusicMidiSettingTarget);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicMidiSettingTarget),
			M_wrap(
				(Identifier) identifier,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicMidiSettingClipTempoSource);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(MusicMidiSettingClipTempoSource),
			M_wrap(
				hierarchy,
				file,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicMidiSettingClipTempo);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicMidiSettingClipTempo),
			M_wrap(
				(MusicMidiSettingClipTempoSource) source,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicMidiSetting);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicMidiSetting),
			M_wrap(
				(MusicMidiSettingTarget) target,
				(MusicMidiSettingClipTempo) clip_tempo,
			),
		);

		#pragma endregion

		#pragma region audio playback limit setting

		M_nested_template_declaration(AudioPlaybackLimitSettingScope);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlaybackLimitSettingScope),
			M_wrap(
				per_game_object,
				globally,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlaybackLimitSettingWhenPriorityIsEqual);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlaybackLimitSettingWhenPriorityIsEqual),
			M_wrap(
				discard_oldest_instance,
				discard_newest_instance,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlaybackLimitSettingWhenLimitIsReached);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlaybackLimitSettingWhenLimitIsReached),
			M_wrap(
				kill_voice,
				use_virtual_voice_setting,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlaybackLimitSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlaybackLimitSetting),
			M_wrap(
				(RegularValue<Integer>) value,
				(AudioPlaybackLimitSettingScope) scope,
				(AudioPlaybackLimitSettingWhenLimitIsReached) when_limit_is_reached,
				(AudioPlaybackLimitSettingWhenPriorityIsEqual) when_priority_is_equal,
			),
		);

		#pragma endregion

		#pragma region audio virtual voice setting

		M_nested_template_declaration(AudioVirtualVoiceSettingBehavior);

		M_nested_template_definition_check(check_version(t_version, {72, 140}))
		M_enumeration(
			M_nested_template_definition_name(AudioVirtualVoiceSettingBehavior),
			M_wrap(
				continue_to_play,
				kill_voice,
				send_to_virtual_voice,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_enumeration(
			M_nested_template_definition_name(AudioVirtualVoiceSettingBehavior),
			M_wrap(
				continue_to_play,
				kill_voice,
				send_to_virtual_voice,
				kill_if_finite_else_virtual,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioVirtualVoiceSettingOnReturnToPhysical);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioVirtualVoiceSettingOnReturnToPhysical),
			M_wrap(
				play_from_beginning,
				play_from_elapsed_time,
				resume,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioVirtualVoiceSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioVirtualVoiceSetting),
			M_wrap(
				(AudioVirtualVoiceSettingBehavior) behavior,
				(AudioVirtualVoiceSettingOnReturnToPhysical) on_return_to_physical,
			),
		);

		#pragma endregion

		#pragma region audio playback priority setting

		M_nested_template_declaration(AudioPlaybackPrioritySetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlaybackPrioritySetting),
			M_wrap(
				(RegularValue<Floater>) value,
				(Boolean) use_distance_factor,
				(RegularValue<Floater>) offset_at_maximum_distance,
			),
		);

		#pragma endregion

		#pragma region audio time setting

		M_nested_template_declaration(AudioTimeSettingSignature);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_list(
			M_nested_template_definition_name(AudioTimeSettingSignature),
			M_wrap(
				(Integer) first,
				(Integer) second,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioTimeSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioTimeSetting),
			M_wrap(
				(Floater) time,
				(Floater) offset,
				(Floater) tempo,
				(AudioTimeSettingSignature) signature,
			),
		);

		#pragma endregion

		#pragma region audio switcher

		M_nested_template_declaration(AudioSwitcherSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioSwitcherSetting),
			M_wrap(
				(Boolean) is_state,
				(Identifier) group,
				(Identifier) default_item,
			),
		);

		#pragma endregion

		#pragma region audio association

		M_nested_template_declaration(AudioAssociationSettingMode);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioAssociationSettingMode),
			M_wrap(
				best_match,
				weighted,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioAssociationSettingArgument);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAssociationSettingArgument),
			M_wrap(
				(Identifier) identifier,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAssociationSettingArgument),
			M_wrap(
				(Identifier) identifier,
				(Boolean) is_state,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioAssociationSettingPath);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAssociationSettingPath),
			M_wrap(
				(Identifier) u1,
				(Identifier) object,
				(Integer) weight,
				(Integer) probability,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioAssociationSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAssociationSetting),
			M_wrap(
				(Integer) probability,
				(AudioAssociationSettingMode) mode,
				(List<AudioAssociationSettingArgument>) argument,
				(List<AudioAssociationSettingPath>) path,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioAssociationSetting),
			M_wrap(
				(AudioAssociationSettingMode) mode,
				(List<AudioAssociationSettingArgument>) argument,
				(List<AudioAssociationSettingPath>) path,
			),
		);

		#pragma endregion

		#pragma region audio source

		M_nested_template_declaration(AudioSourceType);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioSourceType),
			M_wrap(
				embedded,
				streamed,
				streamed_prefetched,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioSourceSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioSourceSetting),
			M_wrap(
				(Identifier) plug_in,
				(AudioSourceType) type,
				(Identifier) source,
				(Identifier) resource,
				(Integer) resource_offset,
				(Integer) resource_size,
				(Boolean) is_voice,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 113}))
		M_record_of_map(
			M_nested_template_definition_name(AudioSourceSetting),
			M_wrap(
				(Identifier) plug_in,
				(AudioSourceType) type,
				(Identifier) source,
				(Identifier) resource,
				(Integer) resource_offset,
				(Integer) resource_size,
				(Boolean) is_voice,
				(Boolean) non_cachable_stream,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {113}))
		M_record_of_map(
			M_nested_template_definition_name(AudioSourceSetting),
			M_wrap(
				(Identifier) plug_in,
				(AudioSourceType) type,
				(Identifier) resource,
				(Integer) resource_size,
				(Boolean) is_voice,
				(Boolean) non_cachable_stream,
			),
		);

		#pragma endregion

		#pragma region audio play type setting

		M_nested_template_declaration(AudioPlayTypeRandomType);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlayTypeRandomType),
			M_wrap(
				standard,
				shuffle,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayTypeRandomSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayTypeRandomSetting),
			M_wrap(
				(AudioPlayTypeRandomType) type,
				(Integer) avoid_repeat,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayTypeSequenceAtEndOfPlaylist);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlayTypeSequenceAtEndOfPlaylist),
			M_wrap(
				restart,
				play_in_reserve_order,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayTypeSequenceSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayTypeSequenceSetting),
			M_wrap(
				(AudioPlayTypeSequenceAtEndOfPlaylist) at_end_of_playlist,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayTypeSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayTypeSetting),
			M_wrap(
				(AudioPlayTypeRandomSetting) random,
				(AudioPlayTypeSequenceSetting) sequence,
			),
		);

		#pragma endregion

		#pragma region audio play mode setting

		M_nested_template_declaration(AudioPlayModeStepSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayModeStepSetting),
			M_wrap(
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayModeContinuousTransitionType);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(AudioPlayModeContinuousTransitionType),
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

		M_nested_template_declaration(AudioPlayModeContinuousSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayModeContinuousSetting),
			M_wrap(
				(Boolean) always_reset_playlist,
				(RegularValue<Integer>) loop,
				(AudioPlayModeContinuousTransitionType) transition_type,
				(RandomizableValue<Floater>) transition_duration,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayModeContinuousSetting),
			M_wrap(
				(Boolean) always_reset_playlist,
				(RandomizableValue<Integer>) loop,
				(AudioPlayModeContinuousTransitionType) transition_type,
				(RandomizableValue<Floater>) transition_duration,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioPlayModeSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AudioPlayModeSetting),
			M_wrap(
				(AudioPlayModeStepSetting) step,
				(AudioPlayModeContinuousSetting) continuous,
			),
		);

		#pragma endregion

		#pragma region unknown hierarchy

		M_nested_template_declaration(UnknownHierarchy);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(UnknownHierarchy),
			M_wrap(
				(Integer) type,
				(ByteList) data,
			),
		);

		#pragma endregion

		#pragma region state group

		M_nested_template_declaration(StateGroupCustomTransition);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(StateGroupCustomTransition),
			M_wrap(
				(Identifier) from,
				(Identifier) to,
				(Integer) time,
			),
		);

		// ----------------

		M_nested_template_declaration(StateGroup);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(StateGroup),
			M_wrap(
				(Identifier) identifier,
				(Integer) default_transition,
				(List<StateGroupCustomTransition>) custom_transition,
			),
		);

		#pragma endregion

		#pragma region switch group

		M_nested_template_declaration(SwitchGroup);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SwitchGroup),
			M_wrap(
				(Identifier) identifier,
				(Parameter) parameter,
				(List<CoordinateIdentifierPoint>) point,
			),
		);

		#pragma endregion

		#pragma region game parameter

		M_nested_template_declaration(GameParameterBindToBuiltInParameterMode);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_enumeration(
			M_nested_template_definition_name(GameParameterBindToBuiltInParameterMode),
			M_wrap(
				none,
				distance,
				azimuth,
				elevation,
				object_to_listener_angle,
				obstruction,
				occlusion,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_enumeration(
			M_nested_template_definition_name(GameParameterBindToBuiltInParameterMode),
			M_wrap(
				none,
				distance,
				azimuth,
				elevation,
				emitter_cone,
				obstruction,
				occlusion,
				listener_cone,
				diffraction,
			),
		);

		// ----------------

		M_nested_template_declaration(GameParameterInterpolationMode);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(GameParameterInterpolationMode),
			M_wrap(
				none,
				slew_rate,
				filtering_over_time,
			),
		);

		// ----------------

		M_nested_template_declaration(GameParameter);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(GameParameter),
			M_wrap(
				(Identifier) identifier,
				(Floater) range_default,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(GameParameter),
			M_wrap(
				(Identifier) identifier,
				(Floater) range_default,
				(GameParameterInterpolationMode) interpolation_mode,
				(Floater) interpolation_attack,
				(Floater) interpolation_release,
				(GameParameterBindToBuiltInParameterMode) bind_to_built_in_parameter,
			),
		);

		#pragma endregion

		#pragma region game synchronization u1

		M_nested_template_declaration(GameSynchronizationU1);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(GameSynchronizationU1),
			M_wrap(
				(Identifier) identifier,
				(Floater) u1,
				(Floater) u2,
				(Floater) u3,
				(Floater) u4,
				(Floater) u5,
				(Floater) u6,
			),
		);

		#pragma endregion

		#pragma region stateful property setting

		M_nested_template_declaration(StatefulPropertySettingItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(StatefulPropertySettingItem),
			M_wrap(
				(Integer) type,
				(Floater) value,
			),
		);

		// ----------------

		M_nested_template_declaration(StatefulPropertySetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(StatefulPropertySetting),
			M_wrap(
				(Identifier) identifier,
				(List<StatefulPropertySettingItem>) value,
			),
		);

		#pragma endregion

		#pragma region event action

		M_nested_template_declaration(EventActionProperty);

		M_nested_template_definition(EventActionProperty) {

			M_nested_template_declaration(ValueApplyMode);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_enumeration(
				M_nested_template_definition_name(ValueApplyMode),
				M_wrap(
					absolute,
					relative,
				),
			);

			// ----------------

			M_nested_template_declaration(SeekType);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_enumeration(
				M_nested_template_definition_name(SeekType),
				M_wrap(
					time,
					percent,
				),
			);

			// ----------------

			M_nested_template_declaration(PlayAudio);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(PlayAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Floater) probability,
					(Identifier) sound_bank,
				),
			);

			// ----------------

			M_nested_template_declaration(StopAudio);

			M_nested_template_definition_check(check_version(t_version, {72, 125}))
			M_record_of_map(
				M_nested_template_definition_name(StopAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
				),
			);

			M_nested_template_definition_check(check_version(t_version, {125}))
			M_record_of_map(
				M_nested_template_definition_name(StopAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			M_nested_template_declaration(PauseAudio);

			M_nested_template_definition_check(check_version(t_version, {72, 125}))
			M_record_of_map(
				M_nested_template_definition_name(PauseAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) include_delayed_resume_action,
				),
			);

			M_nested_template_definition_check(check_version(t_version, {125}))
			M_record_of_map(
				M_nested_template_definition_name(PauseAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) include_delayed_resume_action,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			M_nested_template_declaration(ResumeAudio);

			M_nested_template_definition_check(check_version(t_version, {72, 125}))
			M_record_of_map(
				M_nested_template_definition_name(ResumeAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) master_resume,
				),
			);

			M_nested_template_definition_check(check_version(t_version, {125}))
			M_record_of_map(
				M_nested_template_definition_name(ResumeAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) master_resume,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			M_nested_template_declaration(BreakAudio);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(BreakAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			M_nested_template_declaration(SeekAudio);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SeekAudio),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(SeekType) seek_type,
					(RandomizableValue<Floater>) seek_value,
					(Boolean) seek_to_nearest_marker,
				),
			);

			// ----------------

			M_nested_template_declaration(PostEvent);

			M_nested_template_definition_check(check_version(t_version, {113}))
			M_record_of_map(
				M_nested_template_definition_name(PostEvent),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			M_nested_template_declaration(SetBusVolume);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetBusVolume),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			// ----------------

			M_nested_template_declaration(SetVoiceVolume);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetVoiceVolume),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			// ----------------

			M_nested_template_declaration(SetVolumePitch);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetVolumePitch),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			// ----------------

			M_nested_template_declaration(SetVolumeLowPassFilter);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetVolumeLowPassFilter),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			// ----------------

			M_nested_template_declaration(SetVolumeHighPassFilter);

			M_nested_template_definition_check(check_version(t_version, {112}))
			M_record_of_map(
				M_nested_template_definition_name(SetVolumeHighPassFilter),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			// ----------------

			M_nested_template_declaration(SetMute);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetMute),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
				),
			);

			// ----------------

			M_nested_template_declaration(SetGameParameter);

			M_nested_template_definition_check(check_version(t_version, {72, 112}))
			M_record_of_map(
				M_nested_template_definition_name(SetGameParameter),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			M_nested_template_definition_check(check_version(t_version, {112}))
			M_record_of_map(
				M_nested_template_definition_name(SetGameParameter),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
					(Boolean) bypass_game_parameter_interpolation,
				),
			);

			// ----------------

			M_nested_template_declaration(SetStateAvailability);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetStateAvailability),
				M_wrap(
					(Boolean) enable,
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			M_nested_template_declaration(ActivateState);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(ActivateState),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(Identifier) group,
					(Identifier) item,
				),
			);

			// ----------------

			M_nested_template_declaration(ActivateSwitch);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(ActivateSwitch),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(Identifier) group,
					(Identifier) item,
				),
			);

			// ----------------

			M_nested_template_declaration(ActivateTrigger);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(ActivateTrigger),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			M_nested_template_declaration(SetBypassEffect);

			M_nested_template_definition_check(check_version(t_version, {72}))
			M_record_of_map(
				M_nested_template_definition_name(SetBypassEffect),
				M_wrap(
					(Boolean) reset,
					(Boolean) enable,
					(RandomizableValue<Integer>) delay,
					(Tuple<Boolean, Boolean, Boolean, Boolean, Boolean>) value,
				),
			);

			// ----------------

			M_nested_template_declaration(ReleaseEnvelope);

			M_nested_template_definition_check(check_version(t_version, {112}))
			M_record_of_map(
				M_nested_template_definition_name(ReleaseEnvelope),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			M_nested_template_declaration(ResetPlaylist);

			M_nested_template_definition_check(check_version(t_version, {113}))
			M_record_of_map(
				M_nested_template_definition_name(ResetPlaylist),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			M_nested_template_declaration(Type);

			M_nested_template_definition_check(check_version(t_version, {72, 112}))
			M_enumeration(
				M_nested_template_definition_name(Type),
				M_wrap(
					play_audio,
					stop_audio,
					pause_audio,
					resume_audio,
					break_audio,
					seek_audio,
					set_bus_volume,
					set_voice_volume,
					set_voice_pitch,
					set_voice_low_pass_filter,
					set_mute,
					set_game_parameter,
					set_state_availability,
					activate_state,
					activate_switch,
					activate_trigger,
					set_bypass_effect,
				),
			);

			M_nested_template_definition_check(check_version(t_version, {112, 113}))
			M_enumeration(
				M_nested_template_definition_name(Type),
				M_wrap(
					play_audio,
					stop_audio,
					pause_audio,
					resume_audio,
					break_audio,
					seek_audio,
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
				),
			);

			M_nested_template_definition_check(check_version(t_version, {113}))
			M_enumeration(
				M_nested_template_definition_name(Type),
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
				if constexpr (check_version(t_version, {72, 112})) {
					using Type = EnumerableVariant<
						Type,
						PlayAudio,
						StopAudio,
						PauseAudio,
						ResumeAudio,
						BreakAudio,
						SeekAudio,
						SetBusVolume,
						SetVoiceVolume,
						SetVolumePitch,
						SetVolumeLowPassFilter,
						SetMute,
						SetGameParameter,
						SetStateAvailability,
						ActivateState,
						ActivateSwitch,
						ActivateTrigger,
						SetBypassEffect
					>;
					return declare<Type>();
				}
				if constexpr (check_version(t_version, {112, 113})) {
					using Type = EnumerableVariant<
						Type,
						PlayAudio,
						StopAudio,
						PauseAudio,
						ResumeAudio,
						BreakAudio,
						SeekAudio,
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
						ReleaseEnvelope
					>;
					return declare<Type>();
				}
				if constexpr (check_version(t_version, {113})) {
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

		// ----------------

		M_nested_template_declaration(EventActionMode);

		M_nested_template_definition_check(check_version(t_version, {72, 125}))
		M_enumeration(
			M_nested_template_definition_name(EventActionMode),
			M_wrap(
				none,
				one,
				all,
				all_except,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {125}))
		M_enumeration(
			M_nested_template_definition_name(EventActionMode),
			M_wrap(
				none,
				one,
				all,
			),
		);

		// ----------------

		M_nested_template_declaration(EventActionScope);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(EventActionScope),
			M_wrap(
				global,
				game_object,
			),
		);

		// ----------------

		M_nested_template_declaration(EventActionException);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(EventActionException),
			M_wrap(
				(Identifier) identifier,
				(Boolean) u1,
			),
		);

		// ----------------

		M_nested_template_declaration(EventAction);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(EventAction),
			M_wrap(
				(Identifier) identifier,
				(Identifier) target,
				(EventActionMode) mode,
				(List<EventActionException>) exception,
				(EventActionScope) scope,
				(Integer) u1,
				(typename EventActionProperty::Item) property,
			),
		);

		#pragma endregion

		#pragma region event

		M_nested_template_declaration(Event);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(Event),
			M_wrap(
				(Identifier) identifier,
				(List<Identifier>) child,
			),
		);

		#pragma endregion

		#pragma region dialogue event

		M_nested_template_declaration(DialogueEvent);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(DialogueEvent),
			M_wrap(
				(Identifier) identifier,
				(AudioAssociationSetting) association,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(DialogueEvent),
			M_wrap(
				(Identifier) identifier,
				(Integer) probability,
				(AudioAssociationSetting) association,
			),
		);

		#pragma endregion

		#pragma region attenuation

		M_nested_template_declaration(AttenuationCurve);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationCurve),
			M_wrap(
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(AttenuationApplySetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationApplySetting),
			M_wrap(
				(Integer) output_bus_volume,
				(Integer) auxiliary_send_volume,
				(Integer) low_pass_filter,
				(Integer) spread,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationApplySetting),
			M_wrap(
				(Integer) output_bus_volume,
				(Integer) game_defined_auxiliary_send_volume,
				(Integer) user_defined_auxiliary_send_volume,
				(Integer) low_pass_filter,
				(Integer) spread,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 145}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationApplySetting),
			M_wrap(
				(Integer) output_bus_volume,
				(Integer) game_defined_auxiliary_send_volume,
				(Integer) user_defined_auxiliary_send_volume,
				(Integer) low_pass_filter,
				(Integer) high_pass_filter,
				(Integer) spread,
				(Integer) focus,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {145}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationApplySetting),
			M_wrap(
				(Integer) distance_output_bus_volume,
				(Integer) distance_game_defined_auxiliary_send_volume,
				(Integer) distance_user_defined_auxiliary_send_volume,
				(Integer) distance_low_pass_filter,
				(Integer) distance_high_pass_filter,
				(Integer) distance_spread,
				(Integer) distance_focus,
				(Integer) obstruction_volume,
				(Integer) obstruction_low_pass_filter,
				(Integer) obstruction_high_pass_filter,
				(Integer) occlusion_volume,
				(Integer) occlusion_low_pass_filter,
				(Integer) occlusion_high_pass_filter,
				(Integer) diffraction_volume,
				(Integer) diffraction_low_pass_filter,
				(Integer) diffraction_high_pass_filter,
				(Integer) transmission_volume,
				(Integer) transmission_low_pass_filter,
				(Integer) transmission_high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(AttenuationCone);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationCone),
			M_wrap(
				(Boolean) enable,
				(Floater) inner_angle,
				(Floater) outer_angle,
				(Floater) maximum_value,
				(Floater) low_pass_filter,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(AttenuationCone),
			M_wrap(
				(Boolean) enable,
				(Floater) inner_angle,
				(Floater) outer_angle,
				(Floater) maximum_value,
				(Floater) low_pass_filter,
				(Floater) high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(Attenuation);

		M_nested_template_definition_check(check_version(t_version, {72, 140}))
		M_record_of_map(
			M_nested_template_definition_name(Attenuation),
			M_wrap(
				(Identifier) identifier,
				(AttenuationApplySetting) apply,
				(List<AttenuationCurve>) curve,
				(AttenuationCone) cone,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(Attenuation),
			M_wrap(
				(Identifier) identifier,
				(AttenuationApplySetting) apply,
				(List<AttenuationCurve>) curve,
				(Boolean) height_spread,
				(AttenuationCone) cone,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		#pragma endregion

		#pragma region modulator

		M_nested_template_declaration(ModulatorScope);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(ModulatorScope),
			M_wrap(
				voice,
				note_or_event,
				game_object,
				global,
			),
		);

		// ----------------

		M_nested_template_declaration(ModulatorTriggerOn);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(ModulatorTriggerOn),
			M_wrap(
				play,
				note_off,
			),
		);

		// ----------------

		M_nested_template_declaration(ModulatorWaveform);

		M_nested_template_definition_check(check_version(t_version, {112, 125}))
		M_enumeration(
			M_nested_template_definition_name(ModulatorWaveform),
			M_wrap(
				sine,
				triangle,
				square,
				saw_up,
				saw_down,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {125}))
		M_enumeration(
			M_nested_template_definition_name(ModulatorWaveform),
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

		M_nested_template_declaration(LowFrequencyOscillatorModulator);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(LowFrequencyOscillatorModulator),
			M_wrap(
				(Identifier) identifier,
				(RandomizableValue<Floater>) depth,
				(RandomizableValue<Floater>) frequency,
				(ModulatorWaveform) waveform,
				(RandomizableValue<Floater>) smoothing,
				(RandomizableValue<Floater>) pulse_width_modulation,
				(RandomizableValue<Floater>) attack,
				(RandomizableValue<Floater>) initial_phase_offset,
				(ModulatorScope) scope,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		// ----------------

		M_nested_template_declaration(EnvelopeModulator);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(EnvelopeModulator),
			M_wrap(
				(Identifier) identifier,
				(RandomizableValue<Floater>) attack_time,
				(RandomizableValue<Floater>) attack_curve,
				(RandomizableValue<Floater>) decay_time,
				(RandomizableValue<Floater>) sustain_level,
				(RandomizableValue<Floater>) release_time,
				(ModulatorScope) scope,
				(ModulatorTriggerOn) trigger_on,
				(RandomizableValue<Floater>) sustain_time,
				(Boolean) stop_playback_after_release,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		// ----------------

		M_nested_template_declaration(TimeModulator);

		M_nested_template_definition_check(check_version(t_version, {132}))
		M_record_of_map(
			M_nested_template_definition_name(TimeModulator),
			M_wrap(
				(Identifier) identifier,
				(RandomizableValue<Floater>) initial_delay,
				(RegularValue<Floater>) duration,
				(RandomizableValue<Integer>) loop,
				(RandomizableValue<Floater>) playback_rate,
				(ModulatorScope) scope,
				(ModulatorTriggerOn) trigger_on,
				(Boolean) stop_playback_at_end,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		#pragma endregion

		#pragma region effect

		M_nested_template_declaration(EffectU1);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(EffectU1),
			M_wrap(
				(Integer) type,
				(Floater) value,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(EffectU1),
			M_wrap(
				(Integer) type,
				(CoordinateMode) mode,
				(Floater) value,
			),
		);

		// ----------------

		M_nested_template_declaration(Effect);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(Effect),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(Effect),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(List<EffectU1>) u1,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(Effect),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
			),
		);

		// ----------------

		M_nested_template_declaration(Source);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(Source),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(Source),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(List<EffectU1>) u1,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128}))
		M_record_of_map(
			M_nested_template_definition_name(Source),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioDevice);

		M_nested_template_definition_check(check_version(t_version, {128, 140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioDevice),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioDevice),
			M_wrap(
				(Identifier) identifier,
				(Identifier) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
				(AudioEffectSetting) effect,
			),
		);

		#pragma endregion

		#pragma region audio bus

		M_nested_template_declaration(AudioBusMuteForBackgroundMusic);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBusMuteForBackgroundMusic),
			M_wrap(
				(Boolean) enable,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioBusConfiguration);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBusConfiguration),
			M_wrap(
				(Integer) u1,
			),
		);

		// ----------------

		M_nested_template_declaration(AudioBus);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioBusConfiguration) bus_configuration,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 125}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {125, 128}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusVoiceVolumeGainSetting) voice_volume_gain,
				(BusBusSetting) bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) audio_device,
				(Identifier) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusVoiceVolumeGainSetting) voice_volume_gain,
				(BusBusSetting) bus,
				(BusOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) audio_device,
				(Identifier) mixer,
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		// ----------------

		M_nested_template_declaration(AuxiliaryAudioBus);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(AuxiliaryAudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(AuxiliaryAudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioBusConfiguration) bus_configuration,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 125}))
		M_record_of_map(
			M_nested_template_definition_name(AuxiliaryAudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {125, 128}))
		M_record_of_map(
			M_nested_template_definition_name(AuxiliaryAudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusVoiceVolumeGainSetting) voice_volume_gain,
				(BusBusSetting) bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 140}))
		M_record_of_map(
			M_nested_template_definition_name(AuxiliaryAudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) audio_device,
				(Identifier) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusVoiceVolumeGainSetting) voice_volume_gain,
				(BusBusSetting) bus,
				(BusOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(AuxiliaryAudioBus),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(Identifier) audio_device,
				(Identifier) mixer,
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		#pragma endregion

		#pragma region sound

		M_nested_template_declaration(SoundPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaybackSetting),
			M_wrap(
				(RandomizableValue<Integer>) loop,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaybackSetting),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(RandomizableValue<Integer>) loop,
			),
		);

		// ----------------

		M_nested_template_declaration(Sound);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(Sound),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(AudioSourceSetting) source,
				(SoundPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
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

		M_nested_template_declaration(SoundPlaylistContainerScope);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_enumeration(
			M_nested_template_definition_name(SoundPlaylistContainerScope),
			M_wrap(
				game_object,
				global,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundPlaylistContainerPlaylistItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainerPlaylistItem),
			M_wrap(
				(Identifier) item,
				(Integer) weight,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundPlaylistContainerPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainerPlaybackSetting),
			M_wrap(
				(SoundPlaylistContainerScope) scope,
				(AudioPlayType) type,
				(AudioPlayTypeSetting) type_setting,
				(AudioPlayMode) mode,
				(AudioPlayModeSetting) mode_setting,
				(List<SoundPlaylistContainerPlaylistItem>) playlist,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainerPlaybackSetting),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(SoundPlaylistContainerScope) scope,
				(AudioPlayType) type,
				(AudioPlayTypeSetting) type_setting,
				(AudioPlayMode) mode,
				(AudioPlayModeSetting) mode_setting,
				(List<SoundPlaylistContainerPlaylistItem>) playlist,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundPlaylistContainer);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(SoundPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundPlaylistContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
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

		M_nested_template_declaration(SoundSwitchContainerObjectAttributeItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainerObjectAttributeItem),
			M_wrap(
				(Identifier) identifier,
				(Boolean) play_first_only,
				(Boolean) continue_to_play_across_switch,
				(Integer) u1,
				(Integer) fade_out_time,
				(Integer) fade_in_time,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundSwitchContainerObjectAssignItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainerObjectAssignItem),
			M_wrap(
				(Identifier) item,
				(List<Identifier>) object,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundSwitchContainerPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainerPlaybackSetting),
			M_wrap(
				(AudioPlayMode) mode,
				(AudioSwitcherSetting) switcher,
				(List<SoundSwitchContainerObjectAttributeItem>) object_attribute,
				(List<SoundSwitchContainerObjectAssignItem>) object_assign,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainerPlaybackSetting),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(AudioPlayMode) mode,
				(AudioSwitcherSetting) switcher,
				(List<SoundSwitchContainerObjectAttributeItem>) object_attribute,
				(List<SoundSwitchContainerObjectAssignItem>) object_assign,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundSwitchContainer);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(SoundSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundSwitchContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
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

		M_nested_template_declaration(SoundBlendContainerTrackChildItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainerTrackChildItem),
			M_wrap(
				(Identifier) identifier,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundBlendContainerTrackItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainerTrackItem),
			M_wrap(
				(Identifier) identifier,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(Parameter) cross_fade,
				(List<SoundBlendContainerTrackChildItem>) child,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundBlendContainerPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainerPlaybackSetting),
			M_wrap(
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 120}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainerPlaybackSetting),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {120}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainerPlaybackSetting),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(AudioPlayMode) mode,
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		// ----------------

		M_nested_template_declaration(SoundBlendContainer);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBlendContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(SoundBlendContainerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
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

		M_nested_template_declaration(ActorMixerPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixerPlaybackSetting),
			M_wrap(
			),
		);

		// ----------------

		M_nested_template_declaration(ActorMixer);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_event,
				(Boolean) override_midi_note_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(ActorMixer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(ActorMixerPlaybackSetting) playback_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(SoundMidiSetting) midi,
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

		M_nested_template_declaration(MusicTrackTrackType);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_enumeration(
			M_nested_template_definition_name(MusicTrackTrackType),
			M_wrap(
				normal,
				random_step,
				sequence_step,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(MusicTrackTrackType),
			M_wrap(
				normal,
				random_step,
				sequence_step,
				switcher,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackClipCurveItemType);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_enumeration(
			M_nested_template_definition_name(MusicTrackClipCurveItemType),
			M_wrap(
				voice_volume,
				voice_low_pass_filter,
				clip_fade_in,
				clip_fade_out,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_enumeration(
			M_nested_template_definition_name(MusicTrackClipCurveItemType),
			M_wrap(
				voice_volume,
				voice_low_pass_filter,
				voice_high_pass_filter,
				clip_fade_in,
				clip_fade_out,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackClipCurveItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackClipCurveItem),
			M_wrap(
				(Integer) index,
				(MusicTrackClipCurveItemType) type,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackClipItem);

		M_nested_template_definition_check(check_version(t_version, {72, 140}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackClipItem),
			M_wrap(
				(Integer) u1,
				(Identifier) source,
				(Floater) offset,
				(Floater) begin,
				(Floater) end,
				(Floater) duration,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackClipItem),
			M_wrap(
				(Integer) u1,
				(Identifier) source,
				(Identifier) event,
				(Floater) offset,
				(Floater) begin,
				(Floater) end,
				(Floater) duration,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackClip);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackClip),
			M_wrap(
				(Integer) u1,
				(List<MusicTrackClipItem>) item,
				(List<MusicTrackClipCurveItem>) curve,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackPlaybackSetting),
			M_wrap(
				(MusicTrackClip) clip,
				(MusicTrackTrackType) type,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackPlaybackSetting),
			M_wrap(
				(MusicTrackClip) clip,
				(MusicTrackTrackType) type,
				(AudioSwitcherSetting) switcher,
				(MusicTrackTransitionSetting) transition,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrackStream);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrackStream),
			M_wrap(
				(Integer) look_ahead_time,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicTrack);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<AudioSourceSetting>) source,
				(MusicTrackPlaybackSetting) playback_setting,
				(MusicTrackStream) stream,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<AudioSourceSetting>) source,
				(MusicTrackPlaybackSetting) playback_setting,
				(MusicTrackStream) stream,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<AudioSourceSetting>) source,
				(MusicTrackPlaybackSetting) playback_setting,
				(MusicTrackStream) stream,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<AudioSourceSetting>) source,
				(MusicTrackPlaybackSetting) playback_setting,
				(MusicTrackStream) stream,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<AudioSourceSetting>) source,
				(MusicTrackPlaybackSetting) playback_setting,
				(MusicTrackStream) stream,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicTrack),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
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
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicSegmentCueItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegmentCueItem),
			M_wrap(
				(Identifier) name,
				(Floater) time,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicSegmentCue);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegmentCue),
			M_wrap(
				(List<MusicSegmentCueItem>) item,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicSegmentPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegmentPlaybackSetting),
			M_wrap(
				(Floater) duration,
				(MusicSegmentCue) cue,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegmentPlaybackSetting),
			M_wrap(
				(RegularValue<Floater>) speed,
				(Floater) duration,
				(MusicSegmentCue) cue,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicSegment);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_time_setting,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSegment),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSegmentPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
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

		M_nested_template_declaration(MusicPlaylistContainerPlaylistItem);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainerPlaylistItem),
			M_wrap(
				(Identifier) u1,
				(Boolean) group,
				(Integer) child_count,
				(Identifier) item,
				(AudioPlayType) play_type,
				(AudioPlayMode) play_mode,
				(AudioPlayTypeRandomSetting) random_setting,
				(Integer) weight,
				(Integer) loop,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicPlaylistContainerPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainerPlaybackSetting),
			M_wrap(
				(List<MusicPlaylistContainerPlaylistItem>) playlist,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainerPlaybackSetting),
			M_wrap(
				(RegularValue<Floater>) speed,
				(List<MusicPlaylistContainerPlaylistItem>) playlist,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicPlaylistContainer);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_time_setting,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicPlaylistContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicPlaylistContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
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

		M_nested_template_declaration(MusicSwitchContainerAssociationItem);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainerAssociationItem),
			M_wrap(
				(Identifier) item,
				(Identifier) child,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicSwitchContainerPlaybackSetting);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainerPlaybackSetting),
			M_wrap(
				(Boolean) continue_playing_on_switch_change,
				(AudioSwitcherSetting) switcher,
				(List<MusicSwitchContainerAssociationItem>) association,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainerPlaybackSetting),
			M_wrap(
				(RegularValue<Floater>) speed,
				(Boolean) continue_playing_on_switch_change,
				(AudioAssociationSetting) association,
			),
		);

		// ----------------

		M_nested_template_declaration(MusicSwitchContainer);

		M_nested_template_definition_check(check_version(t_version, {72, 88}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_time_setting,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {88, 112}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMotionSetting) motion,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {128, 135}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {135, 140}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
				(Boolean) override_time_setting,
				(Boolean) override_voice_volume_loudness_normalization,
				(Boolean) override_game_defined_auxiliary_send,
				(Boolean) override_user_defined_auxiliary_send,
				(Boolean) override_early_reflection_auxiliary_send,
				(Boolean) override_effect,
				(Boolean) override_positioning,
				(Boolean) override_hdr_envelope_tracking,
				(Boolean) override_midi_target,
				(Boolean) override_midi_clip_tempo,
				(Boolean) override_playback_limit,
				(Boolean) override_virtual_voice,
				(Boolean) override_playback_priority,
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140, 150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
				(AudioPlaybackLimitSetting) playback_limit,
				(AudioVirtualVoiceSetting) virtual_voice,
				(AudioPlaybackPrioritySetting) playback_priority,
				(AudioMixerSetting) mixer,
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
				(Boolean) override_mixer,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {150}))
		M_record_of_map(
			M_nested_template_definition_name(MusicSwitchContainer),
			M_wrap(
				(Identifier) identifier,
				(Identifier) parent,
				(List<Identifier>) child,
				(MusicSwitchContainerPlaybackSetting) playback_setting,
				(AudioTimeSetting) time_setting,
				(AudioVoice) voice,
				(AudioVoiceVolumeGainSetting) voice_volume_gain,
				(AudioOutputBusSetting) output_bus,
				(AudioAuxiliarySendSetting) auxiliary_send,
				(AudioEffectSetting) effect,
				(AudioMetadataSetting) metadata,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHdrSetting) hdr,
				(MusicMidiSetting) midi,
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

		M_nested_template_declaration(HierarchyType);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_enumeration(
			M_nested_template_definition_name(HierarchyType),
			M_wrap(
				unknown,
				stateful_property_setting,
				event_action,
				event,
				dialogue_event,
				attenuation,
				effect,
				source,
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

		M_nested_template_definition_check(check_version(t_version, {112, 128}))
		M_enumeration(
			M_nested_template_definition_name(HierarchyType),
			M_wrap(
				unknown,
				stateful_property_setting,
				event_action,
				event,
				dialogue_event,
				attenuation,
				low_frequency_oscillator_modulator,
				envelope_modulator,
				effect,
				source,
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

		M_nested_template_definition_check(check_version(t_version, {128, 132}))
		M_enumeration(
			M_nested_template_definition_name(HierarchyType),
			M_wrap(
				unknown,
				stateful_property_setting,
				event_action,
				event,
				dialogue_event,
				attenuation,
				low_frequency_oscillator_modulator,
				envelope_modulator,
				effect,
				source,
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

		M_nested_template_definition_check(check_version(t_version, {132}))
		M_enumeration(
			M_nested_template_definition_name(HierarchyType),
			M_wrap(
				unknown,
				stateful_property_setting,
				event_action,
				event,
				dialogue_event,
				attenuation,
				low_frequency_oscillator_modulator,
				envelope_modulator,
				time_modulator,
				effect,
				source,
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
			if constexpr (check_version(t_version, {72, 112})) {
				using Type = EnumerableVariant<
					HierarchyType,
					UnknownHierarchy,
					StatefulPropertySetting,
					EventAction,
					Event,
					DialogueEvent,
					Attenuation,
					Effect,
					Source,
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
			if constexpr (check_version(t_version, {112, 128})) {
				using Type = EnumerableVariant<
					HierarchyType,
					UnknownHierarchy,
					StatefulPropertySetting,
					EventAction,
					Event,
					DialogueEvent,
					Attenuation,
					LowFrequencyOscillatorModulator,
					EnvelopeModulator,
					Effect,
					Source,
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
			if constexpr (check_version(t_version, {128, 132})) {
				using Type = EnumerableVariant<
					HierarchyType,
					UnknownHierarchy,
					StatefulPropertySetting,
					EventAction,
					Event,
					DialogueEvent,
					Attenuation,
					LowFrequencyOscillatorModulator,
					EnvelopeModulator,
					Effect,
					Source,
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
			if constexpr (check_version(t_version, {132})) {
				using Type = EnumerableVariant<
					HierarchyType,
					UnknownHierarchy,
					StatefulPropertySetting,
					EventAction,
					Event,
					DialogueEvent,
					Attenuation,
					LowFrequencyOscillatorModulator,
					EnvelopeModulator,
					TimeModulator,
					Effect,
					Source,
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

		#pragma region reference

		M_nested_template_declaration(SoundBankReference);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBankReference),
			M_wrap(
				(Identifier) identifier,
				(String) name,
			),
		);

		// ----------------

		M_nested_template_declaration(PlugInReference);

		M_nested_template_definition_check(check_version(t_version, {118}))
		M_record_of_map(
			M_nested_template_definition_name(PlugInReference),
			M_wrap(
				(Identifier) identifier,
				(String) library,
			),
		);

		#pragma endregion

		#pragma region game synchronization

		M_nested_template_declaration(GameSynchronization);

		M_nested_template_definition_check(check_version(t_version, {72, 140}))
		M_record_of_map(
			M_nested_template_definition_name(GameSynchronization),
			M_wrap(
				(List<StateGroup>) state_group,
				(List<SwitchGroup>) switch_group,
				(List<GameParameter>) game_parameter,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {140}))
		M_record_of_map(
			M_nested_template_definition_name(GameSynchronization),
			M_wrap(
				(List<StateGroup>) state_group,
				(List<SwitchGroup>) switch_group,
				(List<GameParameter>) game_parameter,
				(List<GameSynchronizationU1>) u1,
			),
		);

		#pragma endregion

		#pragma region setting

		M_nested_template_declaration(VoiceFilterBehavior);

		M_nested_template_definition_check(check_version(t_version, {145}))
		M_enumeration(
			M_nested_template_definition_name(VoiceFilterBehavior),
			M_wrap(
				sum_all_value,
				use_highest_value,
			),
		);

		// ----------------

		M_nested_template_declaration(ObstructionSetting);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(ObstructionSetting),
			M_wrap(
				(Boolean) enable,
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(ObstructionSettingBundle);

		M_nested_template_definition_check(check_version(t_version, {72, 112}))
		M_record_of_map(
			M_nested_template_definition_name(ObstructionSettingBundle),
			M_wrap(
				(ObstructionSetting) volume,
				(ObstructionSetting) low_pass_filter,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {112}))
		M_record_of_map(
			M_nested_template_definition_name(ObstructionSettingBundle),
			M_wrap(
				(ObstructionSetting) volume,
				(ObstructionSetting) low_pass_filter,
				(ObstructionSetting) high_pass_filter,
			),
		);

		// ----------------

		M_nested_template_declaration(Setting);

		M_nested_template_definition_check(check_version(t_version, {72, 113}))
		M_record_of_map(
			M_nested_template_definition_name(Setting),
			M_wrap(
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {113, 118}))
		M_record_of_map(
			M_nested_template_definition_name(Setting),
			M_wrap(
				(String) platform,
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {118, 145}))
		M_record_of_map(
			M_nested_template_definition_name(Setting),
			M_wrap(
				(String) platform,
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
				(List<PlugInReference>) plug_in,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {145}))
		M_record_of_map(
			M_nested_template_definition_name(Setting),
			M_wrap(
				(String) platform,
				(VoiceFilterBehavior) voice_filter_behavior,
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
				(List<PlugInReference>) plug_in,
			),
		);

		#pragma endregion

		#pragma region sound bank

		M_nested_template_declaration(SoundBank);

		M_nested_template_definition_check(check_version(t_version, {72}))
		M_record_of_map(
			M_nested_template_definition_name(SoundBank),
			M_wrap(
				(Identifier) identifier,
				(Identifier) language,
				(ByteList) header_expand,
				(List<Identifier>) embedded_media,
				(List<SoundBankReference>) reference,
				(Optional<Setting>) setting,
				(Optional<GameSynchronization>) game_synchronization,
				(List<Hierarchy>) hierarchy,
			),
		);

		#pragma endregion

	};

}
