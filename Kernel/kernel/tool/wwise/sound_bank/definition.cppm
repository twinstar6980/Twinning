module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.wwise.sound_bank.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.wwise.sound_bank.version;

export namespace Twinning::Kernel::Tool::Wwise::SoundBank {

	template <auto version> requires (check_version(version, {}))
	struct Definition {

		#pragma region id

		using ID = Integer;

		#pragma endregion

		#pragma region value

		template <typename Value>
		M_record_of_list(
			M_wrap(RegularValue),
			M_wrap(
				(Value) value,
			),
		);

		template <typename Value>
		M_record_of_list(
			M_wrap(RandomizableValue),
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

		template <typename _> requires (check_version(version, {72}))
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
		struct TimePoint_;

		using TimePoint = TimePoint_<>;

		template <typename _> requires (check_version(version, {72, 140}))
		M_enumeration(
			M_wrap(TimePoint_<_>),
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

		template <typename _> requires (check_version(version, {140}))
		M_enumeration(
			M_wrap(TimePoint_<_>),
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

		template <typename = None>
		struct CoordinateMode_;

		using CoordinateMode = CoordinateMode_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(CoordinateMode_<_>),
			M_wrap(
				linear,
				scaled,
				scaled_3,
			),
		);

		// ----------------

		template <typename = None>
		struct CoordinatePoint_;

		using CoordinatePoint = CoordinatePoint_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(CoordinatePoint_<_>),
			M_wrap(
				(Position2<Floater>) position,
				(Curve) curve,
			),
		);

		// ----------------

		template <typename = None>
		struct CoordinateIDPoint_;

		using CoordinateIDPoint = CoordinateIDPoint_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(CoordinateIDPoint_<_>),
			M_wrap(
				(Position2<Floater, ID>) position,
				(Curve) curve,
			),
		);

		#pragma endregion

		#pragma region property

		template <typename = None>
		struct PropertyCategory_;

		using PropertyCategory = PropertyCategory_<>;

		template <typename _> requires (check_version(version, {72, 145}))
		M_enumeration(
			M_wrap(PropertyCategory_<_>),
			M_wrap(
				unidirectional,
				bidirectional,
				bidirectional_ranged,
				boolean,
			),
		);

		template <typename _> requires (check_version(version, {145}))
		M_enumeration(
			M_wrap(PropertyCategory_<_>),
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

		template <typename = None>
		struct ParameterCategory_;

		using ParameterCategory = ParameterCategory_<>;

		template <typename _> requires (check_version(version, {112}))
		M_enumeration(
			M_wrap(ParameterCategory_<_>),
			M_wrap(
				game_parameter,
				midi_parameter,
				modulator,
			),
		);

		// ----------------

		template <typename = None>
		struct Parameter_;

		using Parameter = Parameter_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_list(
			M_wrap(Parameter_<_>),
			M_wrap(
				(ID) id,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_list(
			M_wrap(Parameter_<_>),
			M_wrap(
				(ID) id,
				(ParameterCategory) category,
			),
		);

		#pragma endregion

		#pragma region real time parameter control setting

		template <typename = None>
		struct RealTimeParameterControlSettingItem_;

		using RealTimeParameterControlSettingItem = RealTimeParameterControlSettingItem_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(RealTimeParameterControlSettingItem_<_>),
			M_wrap(
				(Integer) type,
				(Parameter) parameter,
				(ID) u2,
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(RealTimeParameterControlSettingItem_<_>),
			M_wrap(
				(Integer) type,
				(Parameter) parameter,
				(PropertyCategory) u1,
				(ID) u2,
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct RealTimeParameterControlSetting_;

		using RealTimeParameterControlSetting = RealTimeParameterControlSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(RealTimeParameterControlSetting_<_>),
			M_wrap(
				(List<RealTimeParameterControlSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region state setting

		template <typename = None>
		struct StateSettingAttribute_;

		using StateSettingAttribute = StateSettingAttribute_<>;

		template <typename _> requires (check_version(version, {125, 128}))
		M_record_of_map(
			M_wrap(StateSettingAttribute_<_>),
			M_wrap(
				(Integer) type,
				(PropertyCategory) category,
			),
		);

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(StateSettingAttribute_<_>),
			M_wrap(
				(Integer) type,
				(PropertyCategory) category,
				(Integer) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct StateSettingApplyItem_;

		using StateSettingApplyItem = StateSettingApplyItem_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(StateSettingApplyItem_<_>),
			M_wrap(
				(ID) target,
				(ID) setting,
			),
		);

		// ----------------

		template <typename = None>
		struct StateSettingItem_;

		using StateSettingItem = StateSettingItem_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(StateSettingItem_<_>),
			M_wrap(
				(ID) group,
				(TimePoint) change_occur_at,
				(List<StateSettingApplyItem>) apply,
			),
		);

		// ----------------

		template <typename = None>
		struct StateSetting_;

		using StateSetting = StateSetting_<>;

		template <typename _> requires (check_version(version, {72, 125}))
		M_record_of_map(
			M_wrap(StateSetting_<_>),
			M_wrap(
				(List<StateSettingItem>) item,
			),
		);

		template <typename _> requires (check_version(version, {125}))
		M_record_of_map(
			M_wrap(StateSetting_<_>),
			M_wrap(
				(List<StateSettingAttribute>) attribute,
				(List<StateSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region audio play setting

		template <typename = None>
		struct AudioPlayType_;

		using AudioPlayType = AudioPlayType_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(AudioPlayMode_<_>),
			M_wrap(
				step,
				continuous,
			),
		);

		#pragma endregion

		#pragma region voice setting

		template <typename = None>
		struct BusVoiceSetting_;

		using BusVoiceSetting = BusVoiceSetting_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(BusVoiceSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) pitch,
				(RegularValue<Floater>) low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(BusVoiceSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) pitch,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioVoice_;

		using AudioVoice = AudioVoice_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(AudioVoice_<_>),
			M_wrap(
				(RandomizableValue<Floater>) volume,
				(RandomizableValue<Floater>) pitch,
				(RandomizableValue<Floater>) low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(AudioVoice_<_>),
			M_wrap(
				(RandomizableValue<Floater>) volume,
				(RandomizableValue<Floater>) pitch,
				(RandomizableValue<Floater>) low_pass_filter,
				(RandomizableValue<Floater>) high_pass_filter,
			),
		);

		#pragma endregion

		#pragma region voice volume setting

		template <typename = None>
		struct BusVoiceVolumeGainSetting_;

		using BusVoiceVolumeGainSetting = BusVoiceVolumeGainSetting_<>;

		template <typename _> requires (check_version(version, {125}))
		M_record_of_map(
			M_wrap(BusVoiceVolumeGainSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) make_up,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioVoiceVolumeGainSetting_;

		using AudioVoiceVolumeGainSetting = AudioVoiceVolumeGainSetting_<>;

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(AudioVoiceVolumeGainSetting_<_>),
			M_wrap(
				(Boolean) normalization,
				(RegularValue<Floater>) make_up,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(AudioVoiceVolumeGainSetting_<_>),
			M_wrap(
				(Boolean) normalization,
				(RandomizableValue<Floater>) make_up,
			),
		);

		#pragma endregion

		#pragma region bus setting

		template <typename = None>
		struct BusBusSetting_;

		using BusBusSetting = BusBusSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(BusBusSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) volume,
			),
		);

		// ----------------

		template <typename = None>
		struct BusOutputBusSetting_;

		using BusOutputBusSetting = BusOutputBusSetting_<>;

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(BusOutputBusSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioOutputBusSetting_;

		using AudioOutputBusSetting = AudioOutputBusSetting_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(AudioOutputBusSetting_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(AudioOutputBusSetting_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		#pragma endregion

		#pragma region audio auxiliary send setting

		template <typename = None>
		struct AudioGameDefinedAuxiliarySendSetting_;

		using AudioGameDefinedAuxiliarySendSetting = AudioGameDefinedAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {72, 128}))
		M_record_of_map(
			M_wrap(AudioGameDefinedAuxiliarySendSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(RegularValue<Floater>) volume,
			),
		);

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(AudioGameDefinedAuxiliarySendSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioUserDefinedAuxiliarySendSettingItem_;

		using AudioUserDefinedAuxiliarySendSettingItem = AudioUserDefinedAuxiliarySendSettingItem_<>;

		template <typename _> requires (check_version(version, {72, 128}))
		M_record_of_map(
			M_wrap(AudioUserDefinedAuxiliarySendSettingItem_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floater>) volume,
			),
		);

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(AudioUserDefinedAuxiliarySendSettingItem_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floater>) volume,
				(RegularValue<Floater>) low_pass_filter,
				(RegularValue<Floater>) high_pass_filter,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioUserDefinedAuxiliarySendSetting_;

		using AudioUserDefinedAuxiliarySendSetting = AudioUserDefinedAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioUserDefinedAuxiliarySendSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(AudioUserDefinedAuxiliarySendSettingItem) item_1,
				(AudioUserDefinedAuxiliarySendSettingItem) item_2,
				(AudioUserDefinedAuxiliarySendSettingItem) item_3,
				(AudioUserDefinedAuxiliarySendSettingItem) item_4,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioEarlyReflectionAuxiliarySendSetting_;

		using AudioEarlyReflectionAuxiliarySendSetting = AudioEarlyReflectionAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {135}))
		M_record_of_map(
			M_wrap(AudioEarlyReflectionAuxiliarySendSetting_<_>),
			M_wrap(
				(ID) bus,
				(RegularValue<Floater>) volume,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioAuxiliarySendSetting_;

		using AudioAuxiliarySendSetting = AudioAuxiliarySendSetting_<>;

		template <typename _> requires (check_version(version, {72, 135}))
		M_record_of_map(
			M_wrap(AudioAuxiliarySendSetting_<_>),
			M_wrap(
				(AudioGameDefinedAuxiliarySendSetting) game_defined,
				(AudioUserDefinedAuxiliarySendSetting) user_defined,
			),
		);

		template <typename _> requires (check_version(version, {135}))
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

		template <typename _> requires (check_version(version, {72, 150}))
		M_record_of_map(
			M_wrap(AudioEffectSettingItem_<_>),
			M_wrap(
				(Integer) index,
				(ID) id,
				(Boolean) use_share_set,
				(Boolean) u1,
			),
		);

		template <typename _> requires (check_version(version, {150}))
		M_record_of_map(
			M_wrap(AudioEffectSettingItem_<_>),
			M_wrap(
				(Integer) index,
				(ID) id,
				(Boolean) use_share_set,
				(Boolean) bypass,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioEffectSetting_;

		using AudioEffectSetting = AudioEffectSetting_<>;

		template <typename _> requires (check_version(version, {72, 150}))
		M_record_of_map(
			M_wrap(AudioEffectSetting_<_>),
			M_wrap(
				(Tuple<Boolean, Boolean, Boolean, Boolean, Boolean>) bypass,
				(List<AudioEffectSettingItem>) item,
			),
		);

		template <typename _> requires (check_version(version, {150}))
		M_record_of_map(
			M_wrap(AudioEffectSetting_<_>),
			M_wrap(
				(Boolean) bypass,
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
		struct AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint_;

		using AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint = AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint_<_>),
			M_wrap(
				(Position2<Floater>) position,
				(Integer) duration,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomationPoint_<_>),
			M_wrap(
				(Position3<Floater>) position,
				(Integer) duration,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint_;

		using AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint = AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_list(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint_<_>),
			M_wrap(
				(Integer) begin,
				(Integer) count,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange_;

		using AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange = AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_list(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange_<_>),
			M_wrap(
				(Floater) left_right,
				(Floater) front_back,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_list(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange_<_>),
			M_wrap(
				(Floater) left_right,
				(Floater) front_back,
				(Floater) up_down,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingPositionSourceAutomationPath_;

		using AudioPositioningSettingListenerRoutingPositionSourceAutomationPath = AudioPositioningSettingListenerRoutingPositionSourceAutomationPath_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomationPath_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathPoint) point,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomationPathRandomRange) random_range,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingPositionSourceAutomation_;

		using AudioPositioningSettingListenerRoutingPositionSourceAutomation = AudioPositioningSettingListenerRoutingPositionSourceAutomation_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceAutomation_<_>),
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

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingPositionSourceMode_;

		using AudioPositioningSettingListenerRoutingPositionSourceMode = AudioPositioningSettingListenerRoutingPositionSourceMode_<>;

		template <typename _> requires (check_version(version, {72, 132}))
		M_enumeration(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceMode_<_>),
			M_wrap(
				user_defined,
				game_defined,
			),
		);

		template <typename _> requires (check_version(version, {132}))
		M_enumeration(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSourceMode_<_>),
			M_wrap(
				emitter,
				emitter_with_automation,
				listener_with_automation,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingPositionSource_;

		using AudioPositioningSettingListenerRoutingPositionSource = AudioPositioningSettingListenerRoutingPositionSource_<>;

		template <typename _> requires (check_version(version, {72, 132}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSource_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) update_at_each_frame,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomation) automation,
			),
		);

		template <typename _> requires (check_version(version, {132, 140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSource_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) hold_emitter_position_and_orientation,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomation) automation,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingPositionSource_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRoutingPositionSourceMode) mode,
				(Boolean) hold_listener_orientation,
				(Boolean) hold_emitter_position_and_orientation,
				(Boolean) diffraction_and_transmission,
				(AudioPositioningSettingListenerRoutingPositionSourceAutomation) automation,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingAttenuation_;

		using AudioPositioningSettingListenerRoutingAttenuation = AudioPositioningSettingListenerRoutingAttenuation_<>;

		template <typename _> requires (check_version(version, {72, 134}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingAttenuation_<_>),
			M_wrap(
				(ID) id,
			),
		);

		template <typename _> requires (check_version(version, {134}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRoutingAttenuation_<_>),
			M_wrap(
				(Boolean) enable,
				(ID) id,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRoutingSpatialization_;

		using AudioPositioningSettingListenerRoutingSpatialization = AudioPositioningSettingListenerRoutingSpatialization_<>;

		template <typename _> requires (check_version(version, {128}))
		M_enumeration(
			M_wrap(AudioPositioningSettingListenerRoutingSpatialization_<_>),
			M_wrap(
				none,
				position,
				position_and_orientation,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingListenerRouting_;

		using AudioPositioningSettingListenerRouting = AudioPositioningSettingListenerRouting_<>;

		template <typename _> requires (check_version(version, {72, 128}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRouting_<_>),
			M_wrap(
				(Boolean) spatialization,
				(AudioPositioningSettingListenerRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRoutingPositionSource) position_source,
			),
		);

		template <typename _> requires (check_version(version, {128, 132}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRouting_<_>),
			M_wrap(
				(AudioPositioningSettingListenerRoutingSpatialization) spatialization,
				(AudioPositioningSettingListenerRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRoutingPositionSource) position_source,
			),
		);

		template <typename _> requires (check_version(version, {132}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingListenerRouting_<_>),
			M_wrap(
				(Boolean) enable,
				(AudioPositioningSettingListenerRoutingSpatialization) spatialization,
				(RegularValue<Floater>) speaker_panning_divsion_spatialization_mix,
				(AudioPositioningSettingListenerRoutingAttenuation) attenuation,
				(AudioPositioningSettingListenerRoutingPositionSource) position_source,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingSpeakerPanningMode_;

		using AudioPositioningSettingSpeakerPanningMode = AudioPositioningSettingSpeakerPanningMode_<>;

		template <typename _> requires (check_version(version, {132, 140}))
		M_enumeration(
			M_wrap(AudioPositioningSettingSpeakerPanningMode_<_>),
			M_wrap(
				direct_assignment,
				balance_fade,
			),
		);

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

		template <typename _> requires (check_version(version, {72, 132}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingSpeakerPanning_<_>),
			M_wrap(
				(Boolean) enable,
				(Position2<Floater>) position,
			),
		);

		template <typename _> requires (check_version(version, {132, 140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingSpeakerPanning_<_>),
			M_wrap(
				(AudioPositioningSettingSpeakerPanningMode) mode,
				(Position2<Floater>) position,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioPositioningSettingSpeakerPanning_<_>),
			M_wrap(
				(AudioPositioningSettingSpeakerPanningMode) mode,
				(Position3<Floater>) position,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSettingType_;

		using AudioPositioningSettingType = AudioPositioningSettingType_<>;

		template <typename _> requires (check_version(version, {72, 132}))
		M_enumeration(
			M_wrap(AudioPositioningSettingType_<_>),
			M_wrap(
				two_dimension,
				three_dimension,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPositioningSetting_;

		using AudioPositioningSetting = AudioPositioningSetting_<>;

		template <typename _> requires (check_version(version, {72, 125}))
		M_record_of_map(
			M_wrap(AudioPositioningSetting_<_>),
			M_wrap(
				(AudioPositioningSettingType) type,
				(RegularValue<Floater>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRouting) listener_routing,
			),
		);

		template <typename _> requires (check_version(version, {125, 132}))
		M_record_of_map(
			M_wrap(AudioPositioningSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(AudioPositioningSettingType) type,
				(RegularValue<Floater>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRouting) listener_routing,
			),
		);

		template <typename _> requires (check_version(version, {132}))
		M_record_of_map(
			M_wrap(AudioPositioningSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) center_percent,
				(AudioPositioningSettingSpeakerPanning) speaker_panning,
				(AudioPositioningSettingListenerRouting) listener_routing,
			),
		);

		#pragma endregion

		#pragma region audio motion setting

		template <typename = None>
		struct AudioMotionSetting_;

		using AudioMotionSetting = AudioMotionSetting_<>;

		template <typename _> requires (check_version(version, {72, 128}))
		M_record_of_map(
			M_wrap(AudioMotionSetting_<_>),
			M_wrap(
				(RandomizableValue<Floater>) volume_offset,
				(RandomizableValue<Floater>) low_pass_filter,
			),
		);

		#pragma endregion

		#pragma region audio mixer setting

		template <typename = None>
		struct AudioMixerSetting_;

		using AudioMixerSetting = AudioMixerSetting_<>;

		template <typename _> requires (check_version(version, {112, 150}))
		M_record_of_map(
			M_wrap(AudioMixerSetting_<_>),
			M_wrap(
				(ID) id,
			),
		);

		#pragma endregion

		#pragma region bus auto ducking setting

		template <typename = None>
		struct BusAutomaticDuckingSettingBusTarget_;

		using BusAutomaticDuckingSettingBusTarget = BusAutomaticDuckingSettingBusTarget_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(BusAutomaticDuckingSettingBusTarget_<_>),
			M_wrap(
				voice_volume,
				bus_volume,
			),
		);

		// ----------------

		template <typename = None>
		struct BusAutomaticDuckingSettingBus_;

		using BusAutomaticDuckingSettingBus = BusAutomaticDuckingSettingBus_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(BusAutomaticDuckingSettingBus_<_>),
			M_wrap(
				(ID) id,
				(Floater) volume,
				(Integer) fade_out,
				(Integer) fade_in,
				(Curve) curve,
				(BusAutomaticDuckingSettingBusTarget) target,
			),
		);

		// ----------------

		template <typename = None>
		struct BusAutomaticDuckingSetting_;

		using BusAutomaticDuckingSetting = BusAutomaticDuckingSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(BusAutomaticDuckingSetting_<_>),
			M_wrap(
				(Integer) recovery_time,
				(Floater) maximum_ducking_volume,
				(List<BusAutomaticDuckingSettingBus>) bus,
			),
		);

		#pragma endregion

		#pragma region music transition setting

		template <typename = None>
		struct MusicTransitionSettingJumpMode_;

		using MusicTransitionSettingJumpMode = MusicTransitionSettingJumpMode_<>;

		template <typename _> requires (check_version(version, {134}))
		M_enumeration(
			M_wrap(MusicTransitionSettingJumpMode_<_>),
			M_wrap(
				start,
				specific,
				next,
				last_played,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSettingSynchronizeMode_;

		using MusicTransitionSettingSynchronizeMode = MusicTransitionSettingSynchronizeMode_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(MusicTransitionSettingSynchronizeMode_<_>),
			M_wrap(
				entry_cue,
				random_cue,
				custom_cue,
				same_time_as_playing_segment,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTransitionSettingItemFade_;

		using MusicTransitionSettingItemFade = MusicTransitionSettingItemFade_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 134}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItemDestination_<_>),
			M_wrap(
				(ID) id,
				(MusicTransitionSettingSynchronizeMode) synchronize_to,
				(Boolean) play_pre_entry,
				(Boolean) custom_cue_filter_match_source_cue_name,
				(ID) custom_cue_filter_match_target,
				(MusicTransitionSettingItemFade) fade_in,
			),
		);

		template <typename _> requires (check_version(version, {134}))
		M_record_of_map(
			M_wrap(MusicTransitionSettingItemDestination_<_>),
			M_wrap(
				(ID) id,
				(MusicTransitionSettingJumpMode) jump_to,
				(MusicTransitionSettingSynchronizeMode) synchronize_to,
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(MusicStingerSetting_<_>),
			M_wrap(
				(List<MusicStingerSettingItem>) item,
			),
		);

		#pragma endregion

		#pragma region bus hdr setting

		template <typename = None>
		struct BusHDRSettingDynamicReleaseMode_;

		using BusHDRSettingDynamicReleaseMode = BusHDRSettingDynamicReleaseMode_<>;

		template <typename _> requires (check_version(version, {88}))
		M_enumeration(
			M_wrap(BusHDRSettingDynamicReleaseMode_<_>),
			M_wrap(
				linear,
				exponential,
			),
		);

		// ----------------

		template <typename = None>
		struct BusHDRSettingWindowTopOutputGameParameter_;

		using BusHDRSettingWindowTopOutputGameParameter = BusHDRSettingWindowTopOutputGameParameter_<>;

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(BusHDRSettingWindowTopOutputGameParameter_<_>),
			M_wrap(
				(ID) id,
				(Floater) minimum,
				(Floater) maximum,
			),
		);

		// ----------------

		template <typename = None>
		struct BusHDRSettingDynamic_;

		using BusHDRSettingDynamic = BusHDRSettingDynamic_<>;

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(BusHDRSettingDynamic_<_>),
			M_wrap(
				(Floater) threshold,
				(Floater) ratio,
				(Floater) release_time,
				(BusHDRSettingDynamicReleaseMode) release_mode,
			),
		);

		// ----------------

		template <typename = None>
		struct BusHDRSetting_;

		using BusHDRSetting = BusHDRSetting_<>;

		template <typename _> requires (check_version(version, {88}))
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

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(AudioHDRSettingEnvelopeTracking_<_>),
			M_wrap(
				(Boolean) enable,
				(Floater) active_range,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioHDRSetting_;

		using AudioHDRSetting = AudioHDRSetting_<>;

		template <typename _> requires (check_version(version, {88}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioPlaybackLimitSetting_<_>),
			M_wrap(
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

		template <typename _> requires (check_version(version, {72, 140}))
		M_enumeration(
			M_wrap(AudioVirtualVoiceSettingBehavior_<_>),
			M_wrap(
				continue_to_play,
				kill_voice,
				send_to_virtual_voice,
			),
		);

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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioPlaybackPrioritySetting_<_>),
			M_wrap(
				(RegularValue<Floater>) value,
				(Boolean) use_distance_factor,
				(RegularValue<Floater>) offset_at_maximum_distance,
			),
		);

		#pragma endregion

		#pragma region audio time setting

		template <typename = None>
		struct AudioTimeSettingSignature_;

		using AudioTimeSettingSignature = AudioTimeSettingSignature_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_list(
			M_wrap(AudioTimeSettingSignature_<_>),
			M_wrap(
				(Integer) first,
				(Integer) second,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioTimeSetting_;

		using AudioTimeSetting = AudioTimeSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioTimeSetting_<_>),
			M_wrap(
				(Floater) time,
				(Floater) offset,
				(Floater) tempo,
				(AudioTimeSettingSignature) signature,
			),
		);

		#pragma endregion

		#pragma region audio switcher

		template <typename = None>
		struct AudioSwitcherSetting_;

		using AudioSwitcherSetting = AudioSwitcherSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioSwitcherSetting_<_>),
			M_wrap(
				(Boolean) is_state,
				(ID) group,
				(ID) default_item,
			),
		);

		#pragma endregion

		#pragma region audio association

		template <typename = None>
		struct AudioAssociationSettingMode_;

		using AudioAssociationSettingMode = AudioAssociationSettingMode_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(AudioAssociationSettingMode_<_>),
			M_wrap(
				best_match,
				weighted,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioAssociationSettingArgument_;

		using AudioAssociationSettingArgument = AudioAssociationSettingArgument_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(AudioAssociationSettingArgument_<_>),
			M_wrap(
				(ID) id,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(AudioAssociationSettingArgument_<_>),
			M_wrap(
				(ID) id,
				(Boolean) is_state,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioAssociationSettingPath_;

		using AudioAssociationSettingPath = AudioAssociationSettingPath_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioAssociationSettingPath_<_>),
			M_wrap(
				(ID) u1,
				(ID) object,
				(Integer) weight,
				(Integer) probability,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioAssociationSetting_;

		using AudioAssociationSetting = AudioAssociationSetting_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(AudioAssociationSetting_<_>),
			M_wrap(
				(Integer) probability,
				(AudioAssociationSettingMode) mode,
				(List<AudioAssociationSettingArgument>) argument,
				(List<AudioAssociationSettingPath>) path,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(AudioAssociationSetting_<_>),
			M_wrap(
				(AudioAssociationSettingMode) mode,
				(List<AudioAssociationSettingArgument>) argument,
				(List<AudioAssociationSettingPath>) path,
			),
		);

		#pragma endregion

		#pragma region audio source

		template <typename = None>
		struct AudioSourceType_;

		using AudioSourceType = AudioSourceType_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(AudioSourceType_<_>),
			M_wrap(
				embedded,
				streamed,
				streamed_prefetched,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioSourceSetting_;

		using AudioSourceSetting = AudioSourceSetting_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(AudioSourceSetting_<_>),
			M_wrap(
				(ID) plug_in,
				(AudioSourceType) type,
				(ID) source,
				(ID) resource,
				(Integer) resource_offset,
				(Integer) resource_size,
				(Boolean) is_voice,
			),
		);

		template <typename _> requires (check_version(version, {112, 113}))
		M_record_of_map(
			M_wrap(AudioSourceSetting_<_>),
			M_wrap(
				(ID) plug_in,
				(AudioSourceType) type,
				(ID) source,
				(ID) resource,
				(Integer) resource_offset,
				(Integer) resource_size,
				(Boolean) is_voice,
				(Boolean) non_cachable_stream,
			),
		);

		template <typename _> requires (check_version(version, {113}))
		M_record_of_map(
			M_wrap(AudioSourceSetting_<_>),
			M_wrap(
				(ID) plug_in,
				(AudioSourceType) type,
				(ID) resource,
				(Integer) resource_size,
				(Boolean) is_voice,
				(Boolean) non_cachable_stream,
			),
		);

		#pragma endregion

		#pragma region audio play type setting

		template <typename = None>
		struct AudioPlayTypeRandomType_;

		using AudioPlayTypeRandomType = AudioPlayTypeRandomType_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AudioPlayModeStepSetting_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayModeContinuousTransitionType_;

		using AudioPlayModeContinuousTransitionType = AudioPlayModeContinuousTransitionType_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(AudioPlayModeContinuousSetting_<_>),
			M_wrap(
				(Boolean) always_reset_playlist,
				(RegularValue<Integer>) loop,
				(AudioPlayModeContinuousTransitionType) transition_type,
				(RandomizableValue<Floater>) transition_duration,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(AudioPlayModeContinuousSetting_<_>),
			M_wrap(
				(Boolean) always_reset_playlist,
				(RandomizableValue<Integer>) loop,
				(AudioPlayModeContinuousTransitionType) transition_type,
				(RandomizableValue<Floater>) transition_duration,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioPlayModeSetting_;

		using AudioPlayModeSetting = AudioPlayModeSetting_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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
		struct SwitchGroup_;

		using SwitchGroup = SwitchGroup_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(SwitchGroup_<_>),
			M_wrap(
				(ID) id,
				(Parameter) parameter,
				(List<CoordinateIDPoint>) point,
			),
		);

		#pragma endregion

		#pragma region game parameter

		template <typename = None>
		struct GameParameterBindToBuiltInParameterMode_;

		using GameParameterBindToBuiltInParameterMode = GameParameterBindToBuiltInParameterMode_<>;

		template <typename _> requires (check_version(version, {112, 128}))
		M_enumeration(
			M_wrap(GameParameterBindToBuiltInParameterMode_<_>),
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

		template <typename _> requires (check_version(version, {128}))
		M_enumeration(
			M_wrap(GameParameterBindToBuiltInParameterMode_<_>),
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

		template <typename = None>
		struct GameParameterInterpolationMode_;

		using GameParameterInterpolationMode = GameParameterInterpolationMode_<>;

		template <typename _> requires (check_version(version, {112}))
		M_enumeration(
			M_wrap(GameParameterInterpolationMode_<_>),
			M_wrap(
				none,
				slew_rate,
				filtering_over_time,
			),
		);

		// ----------------

		template <typename = None>
		struct GameParameter_;

		using GameParameter = GameParameter_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(GameParameter_<_>),
			M_wrap(
				(ID) id,
				(Floater) range_default,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(GameParameter_<_>),
			M_wrap(
				(ID) id,
				(Floater) range_default,
				(GameParameterInterpolationMode) interpolation_mode,
				(Floater) interpolation_attack,
				(Floater) interpolation_release,
				(GameParameterBindToBuiltInParameterMode) bind_to_built_in_parameter,
			),
		);

		#pragma endregion

		#pragma region game synchronization u1

		template <typename = None>
		struct GameSynchronizationU1_;

		using GameSynchronizationU1 = GameSynchronizationU1_<>;

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(GameSynchronizationU1_<_>),
			M_wrap(
				(ID) id,
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

		template <typename = None>
		struct StatefulPropertySettingItem_;

		using StatefulPropertySettingItem = StatefulPropertySettingItem_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(StatefulPropertySettingItem_<_>),
			M_wrap(
				(Integer) type,
				(Floater) value,
			),
		);

		// ----------------

		template <typename = None>
		struct StatefulPropertySetting_;

		using StatefulPropertySetting = StatefulPropertySetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(StatefulPropertySetting_<_>),
			M_wrap(
				(ID) id,
				(List<StatefulPropertySettingItem>) value,
			),
		);

		#pragma endregion

		#pragma region event action

		template <auto = version> requires (check_version(version, {}))
		struct EventActionProperty_ {

			template <typename = None>
			struct ValueApplyMode_;

			using ValueApplyMode = ValueApplyMode_<>;

			template <typename _> requires (check_version(version, {72}))
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

			template <typename _> requires (check_version(version, {72}))
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

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(PlayAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Floater) probability,
					(ID) sound_bank,
				),
			);

			// ----------------

			template <typename = None>
			struct StopAudio_;

			using StopAudio = StopAudio_<>;

			template <typename _> requires (check_version(version, {72, 125}))
			M_record_of_map(
				M_wrap(StopAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
				),
			);

			template <typename _> requires (check_version(version, {125}))
			M_record_of_map(
				M_wrap(StopAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) resume_state_transition,
					(Boolean) apply_to_dynamic_sequence,
				),
			);

			// ----------------

			template <typename = None>
			struct PauseAudio_;

			using PauseAudio = PauseAudio_<>;

			template <typename _> requires (check_version(version, {72, 125}))
			M_record_of_map(
				M_wrap(PauseAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) include_delayed_resume_action,
				),
			);

			template <typename _> requires (check_version(version, {125}))
			M_record_of_map(
				M_wrap(PauseAudio_<_>),
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

			template <typename = None>
			struct ResumeAudio_;

			using ResumeAudio = ResumeAudio_<>;

			template <typename _> requires (check_version(version, {72, 125}))
			M_record_of_map(
				M_wrap(ResumeAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(Boolean) master_resume,
				),
			);

			template <typename _> requires (check_version(version, {125}))
			M_record_of_map(
				M_wrap(ResumeAudio_<_>),
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

			template <typename = None>
			struct BreakAudio_;

			using BreakAudio = BreakAudio_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(BreakAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct SeekAudio_;

			using SeekAudio = SeekAudio_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SeekAudio_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(SeekType) seek_type,
					(RandomizableValue<Floater>) seek_value,
					(Boolean) seek_to_nearest_marker,
				),
			);

			// ----------------

			template <typename = None>
			struct PostEvent_;

			using PostEvent = PostEvent_<>;

			template <typename _> requires (check_version(version, {113}))
			M_record_of_map(
				M_wrap(PostEvent_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct SetBusVolume_;

			using SetBusVolume = SetBusVolume_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetBusVolume_<_>),
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

			template <typename = None>
			struct SetVoiceVolume_;

			using SetVoiceVolume = SetVoiceVolume_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetVoiceVolume_<_>),
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

			template <typename = None>
			struct SetVolumePitch_;

			using SetVolumePitch = SetVolumePitch_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetVolumePitch_<_>),
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

			template <typename = None>
			struct SetVolumeLowPassFilter_;

			using SetVolumeLowPassFilter = SetVolumeLowPassFilter_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetVolumeLowPassFilter_<_>),
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

			template <typename = None>
			struct SetVolumeHighPassFilter_;

			using SetVolumeHighPassFilter = SetVolumeHighPassFilter_<>;

			template <typename _> requires (check_version(version, {112}))
			M_record_of_map(
				M_wrap(SetVolumeHighPassFilter_<_>),
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

			template <typename = None>
			struct SetMute_;

			using SetMute = SetMute_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetMute_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
				),
			);

			// ----------------

			template <typename = None>
			struct SetGameParameter_;

			using SetGameParameter = SetGameParameter_<>;

			template <typename _> requires (check_version(version, {72, 112}))
			M_record_of_map(
				M_wrap(SetGameParameter_<_>),
				M_wrap(
					(Boolean) reset,
					(RandomizableValue<Integer>) delay,
					(RandomizableValue<Integer>) fade_time,
					(Curve) fade_curve,
					(ValueApplyMode) apply_mode,
					(RandomizableValue<Floater>) value,
				),
			);

			template <typename _> requires (check_version(version, {112}))
			M_record_of_map(
				M_wrap(SetGameParameter_<_>),
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

			template <typename = None>
			struct SetStateAvailability_;

			using SetStateAvailability = SetStateAvailability_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetStateAvailability_<_>),
				M_wrap(
					(Boolean) enable,
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct ActivateState_;

			using ActivateState = ActivateState_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(ActivateState_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(ID) group,
					(ID) item,
				),
			);

			// ----------------

			template <typename = None>
			struct ActivateSwitch_;

			using ActivateSwitch = ActivateSwitch_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(ActivateSwitch_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
					(ID) group,
					(ID) item,
				),
			);

			// ----------------

			template <typename = None>
			struct ActivateTrigger_;

			using ActivateTrigger = ActivateTrigger_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(ActivateTrigger_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct SetBypassEffect_;

			using SetBypassEffect = SetBypassEffect_<>;

			template <typename _> requires (check_version(version, {72}))
			M_record_of_map(
				M_wrap(SetBypassEffect_<_>),
				M_wrap(
					(Boolean) reset,
					(Boolean) enable,
					(RandomizableValue<Integer>) delay,
					(Tuple<Boolean, Boolean, Boolean, Boolean, Boolean>) value,
				),
			);

			// ----------------

			template <typename = None>
			struct ReleaseEnvelope_;

			using ReleaseEnvelope = ReleaseEnvelope_<>;

			template <typename _> requires (check_version(version, {112}))
			M_record_of_map(
				M_wrap(ReleaseEnvelope_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct ResetPlaylist_;

			using ResetPlaylist = ResetPlaylist_<>;

			template <typename _> requires (check_version(version, {113}))
			M_record_of_map(
				M_wrap(ResetPlaylist_<_>),
				M_wrap(
					(RandomizableValue<Integer>) delay,
				),
			);

			// ----------------

			template <typename = None>
			struct Type_;

			using Type = Type_<>;

			template <typename _> requires (check_version(version, {72, 112}))
			M_enumeration(
				M_wrap(Type_<_>),
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

			template <typename _> requires (check_version(version, {112, 113}))
			M_enumeration(
				M_wrap(Type_<_>),
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

			template <typename _> requires (check_version(version, {113}))
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
				if constexpr (check_version(version, {72, 112})) {
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
				if constexpr (check_version(version, {112, 113})) {
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
				if constexpr (check_version(version, {113})) {
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
		struct EventActionMode_;

		using EventActionMode = EventActionMode_<>;

		template <typename _> requires (check_version(version, {72, 125}))
		M_enumeration(
			M_wrap(EventActionMode_<_>),
			M_wrap(
				none,
				one,
				all,
				all_except,
			),
		);

		template <typename _> requires (check_version(version, {125}))
		M_enumeration(
			M_wrap(EventActionMode_<_>),
			M_wrap(
				none,
				one,
				all,
			),
		);

		// ----------------

		template <typename = None>
		struct EventActionScope_;

		using EventActionScope = EventActionScope_<>;

		template <typename _> requires (check_version(version, {72}))
		M_enumeration(
			M_wrap(EventActionScope_<_>),
			M_wrap(
				global,
				game_object,
			),
		);

		// ----------------

		template <typename = None>
		struct EventActionException_;

		using EventActionException = EventActionException_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(EventAction_<_>),
			M_wrap(
				(ID) id,
				(ID) target,
				(EventActionMode) mode,
				(List<EventActionException>) exception,
				(EventActionScope) scope,
				(Integer) u1,
				(typename EventActionProperty::Item) property,
			),
		);

		#pragma endregion

		#pragma region event

		template <typename = None>
		struct Event_;

		using Event = Event_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(DialogueEvent_<_>),
			M_wrap(
				(ID) id,
				(AudioAssociationSetting) association,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(DialogueEvent_<_>),
			M_wrap(
				(ID) id,
				(Integer) probability,
				(AudioAssociationSetting) association,
			),
		);

		#pragma endregion

		#pragma region attenuation

		template <typename = None>
		struct AttenuationCurve_;

		using AttenuationCurve = AttenuationCurve_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(AttenuationCurve_<_>),
			M_wrap(
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct AttenuationApplySetting_;

		using AttenuationApplySetting = AttenuationApplySetting_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(AttenuationApplySetting_<_>),
			M_wrap(
				(Integer) output_bus_volume,
				(Integer) auxiliary_send_volume,
				(Integer) low_pass_filter,
				(Integer) spread,
			),
		);

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(AttenuationApplySetting_<_>),
			M_wrap(
				(Integer) output_bus_volume,
				(Integer) game_defined_auxiliary_send_volume,
				(Integer) user_defined_auxiliary_send_volume,
				(Integer) low_pass_filter,
				(Integer) spread,
			),
		);

		template <typename _> requires (check_version(version, {112, 145}))
		M_record_of_map(
			M_wrap(AttenuationApplySetting_<_>),
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

		template <typename _> requires (check_version(version, {145}))
		M_record_of_map(
			M_wrap(AttenuationApplySetting_<_>),
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

		template <typename = None>
		struct AttenuationCone_;

		using AttenuationCone = AttenuationCone_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(AttenuationCone_<_>),
			M_wrap(
				(Boolean) enable,
				(Floater) inner_angle,
				(Floater) outer_angle,
				(Floater) maximum_value,
				(Floater) low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(AttenuationCone_<_>),
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

		template <typename = None>
		struct Attenuation_;

		using Attenuation = Attenuation_<>;

		template <typename _> requires (check_version(version, {72, 140}))
		M_record_of_map(
			M_wrap(Attenuation_<_>),
			M_wrap(
				(ID) id,
				(AttenuationApplySetting) apply,
				(List<AttenuationCurve>) curve,
				(AttenuationCone) cone,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(Attenuation_<_>),
			M_wrap(
				(ID) id,
				(AttenuationApplySetting) apply,
				(List<AttenuationCurve>) curve,
				(Boolean) height_spread,
				(AttenuationCone) cone,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		#pragma endregion

		#pragma region modulator

		template <typename = None>
		struct ModulatorScope_;

		using ModulatorScope = ModulatorScope_<>;

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {112, 125}))
		M_enumeration(
			M_wrap(ModulatorWaveform_<_>),
			M_wrap(
				sine,
				triangle,
				square,
				saw_up,
				saw_down,
			),
		);

		template <typename _> requires (check_version(version, {125}))
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

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(LowFrequencyOscillatorModulator_<_>),
			M_wrap(
				(ID) id,
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

		template <typename = None>
		struct EnvelopeModulator_;

		using EnvelopeModulator = EnvelopeModulator_<>;

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(EnvelopeModulator_<_>),
			M_wrap(
				(ID) id,
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

		template <typename = None>
		struct TimeModulator_;

		using TimeModulator = TimeModulator_<>;

		template <typename _> requires (check_version(version, {132}))
		M_record_of_map(
			M_wrap(TimeModulator_<_>),
			M_wrap(
				(ID) id,
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

		template <typename = None>
		struct EffectU1_;

		using EffectU1 = EffectU1_<>;

		template <typename _> requires (check_version(version, {112, 128}))
		M_record_of_map(
			M_wrap(EffectU1_<_>),
			M_wrap(
				(Integer) type,
				(Floater) value,
			),
		);

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(EffectU1_<_>),
			M_wrap(
				(Integer) type,
				(CoordinateMode) mode,
				(Floater) value,
			),
		);

		// ----------------

		template <typename = None>
		struct Effect_;

		using Effect = Effect_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(Effect_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		template <typename _> requires (check_version(version, {112, 128}))
		M_record_of_map(
			M_wrap(Effect_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(List<EffectU1>) u1,
			),
		);

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(Effect_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct Source_;

		using Source = Source_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(Source_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
			),
		);

		template <typename _> requires (check_version(version, {112, 128}))
		M_record_of_map(
			M_wrap(Source_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(List<EffectU1>) u1,
			),
		);

		template <typename _> requires (check_version(version, {128}))
		M_record_of_map(
			M_wrap(Source_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
			),
		);

		// ----------------

		template <typename = None>
		struct AudioDevice_;

		using AudioDevice = AudioDevice_<>;

		template <typename _> requires (check_version(version, {128, 140}))
		M_record_of_map(
			M_wrap(AudioDevice_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioDevice_<_>),
			M_wrap(
				(ID) id,
				(ID) plug_in,
				(ByteList) expand,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(List<EffectU1>) u1,
				(AudioEffectSetting) effect,
			),
		);

		#pragma endregion

		#pragma region audio bus

		template <typename = None>
		struct AudioBusMuteForBackgroundMusic_;

		using AudioBusMuteForBackgroundMusic = AudioBusMuteForBackgroundMusic_<>;

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {88}))
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
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

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(AudioBusConfiguration) bus_configuration,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {112, 125}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {125, 128}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) mixer,
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
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {128, 140}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) audio_device,
				(ID) mixer,
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
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) audio_device,
				(ID) mixer,
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
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		// ----------------

		template <typename = None>
		struct AuxiliaryAudioBus_;

		using AuxiliaryAudioBus = AuxiliaryAudioBus_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
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

		template <typename _> requires (check_version(version, {88, 112}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(AudioBusConfiguration) bus_configuration,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {112, 125}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) mixer,
				(AudioBusConfiguration) bus_configuration,
				(AudioBusMuteForBackgroundMusic) mute_for_background_music,
				(BusVoiceSetting) voice,
				(BusBusSetting) bus,
				(AudioEffectSetting) effect,
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(BusAutomaticDuckingSetting) automatic_ducking,
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_positioning,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {125, 128}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) mixer,
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
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {128, 140}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) audio_device,
				(ID) mixer,
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
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(AuxiliaryAudioBus_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(ID) audio_device,
				(ID) mixer,
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
				(BusHDRSetting) hdr,
				(AudioPlaybackLimitSetting) playback_limit,
				(Boolean) override_playback_limit,
			),
		);

		#pragma endregion

		#pragma region sound

		template <typename = None>
		struct SoundPlaybackSetting_;

		using SoundPlaybackSetting = SoundPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundPlaybackSetting_<_>),
			M_wrap(
				(RandomizableValue<Integer>) loop,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(SoundPlaybackSetting_<_>),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(RandomizableValue<Integer>) loop,
			),
		);

		// ----------------

		template <typename = None>
		struct Sound_;

		using Sound = Sound_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(Sound_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundPlaylistContainerPlaybackSetting_<_>),
			M_wrap(
				(SoundPlaylistContainerScope) scope,
				(AudioPlayType) type,
				(AudioPlayTypeSetting) type_setting,
				(AudioPlayMode) mode,
				(AudioPlayModeSetting) mode_setting,
				(List<SoundPlaylistContainerPlaylistItem>) playlist,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(SoundPlaylistContainerPlaybackSetting_<_>),
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

		template <typename = None>
		struct SoundPlaylistContainer_;

		using SoundPlaylistContainer = SoundPlaylistContainer_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundPlaylistContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundSwitchContainerPlaybackSetting_<_>),
			M_wrap(
				(AudioPlayMode) mode,
				(AudioSwitcherSetting) switcher,
				(List<SoundSwitchContainerObjectAttributeItem>) object_attribute,
				(List<SoundSwitchContainerObjectAssignItem>) object_assign,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(SoundSwitchContainerPlaybackSetting_<_>),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundSwitchContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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
		struct SoundBlendContainerTrackChildItem_;

		using SoundBlendContainerTrackChildItem = SoundBlendContainerTrackChildItem_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(SoundBlendContainerTrackChildItem_<_>),
			M_wrap(
				(ID) id,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundBlendContainerTrackItem_;

		using SoundBlendContainerTrackItem = SoundBlendContainerTrackItem_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(SoundBlendContainerTrackItem_<_>),
			M_wrap(
				(ID) id,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(Parameter) cross_fade,
				(List<SoundBlendContainerTrackChildItem>) child,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundBlendContainerPlaybackSetting_;

		using SoundBlendContainerPlaybackSetting = SoundBlendContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundBlendContainerPlaybackSetting_<_>),
			M_wrap(
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		template <typename _> requires (check_version(version, {88, 120}))
		M_record_of_map(
			M_wrap(SoundBlendContainerPlaybackSetting_<_>),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		template <typename _> requires (check_version(version, {120}))
		M_record_of_map(
			M_wrap(SoundBlendContainerPlaybackSetting_<_>),
			M_wrap(
				(RandomizableValue<Floater>) initial_delay,
				(AudioPlayMode) mode,
				(List<SoundBlendContainerTrackItem>) track,
			),
		);

		// ----------------

		template <typename = None>
		struct SoundBlendContainer_;

		using SoundBlendContainer = SoundBlendContainer_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(SoundBlendContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(ActorMixerPlaybackSetting_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct ActorMixer_;

		using ActorMixer = ActorMixer_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(ActorMixer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(SoundMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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

		template <typename _> requires (check_version(version, {72, 112}))
		M_enumeration(
			M_wrap(MusicTrackTrackType_<_>),
			M_wrap(
				normal,
				random_step,
				sequence_step,
			),
		);

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {72, 112}))
		M_enumeration(
			M_wrap(MusicTrackClipCurveItemType_<_>),
			M_wrap(
				voice_volume,
				voice_low_pass_filter,
				clip_fade_in,
				clip_fade_out,
			),
		);

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(MusicTrackClipCurveItem_<_>),
			M_wrap(
				(Integer) index,
				(MusicTrackClipCurveItemType) type,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackClipItem_;

		using MusicTrackClipItem = MusicTrackClipItem_<>;

		template <typename _> requires (check_version(version, {72, 140}))
		M_record_of_map(
			M_wrap(MusicTrackClipItem_<_>),
			M_wrap(
				(Integer) u1,
				(ID) source,
				(Floater) offset,
				(Floater) begin,
				(Floater) end,
				(Floater) duration,
			),
		);

		template <typename _> requires (check_version(version, {140}))
		M_record_of_map(
			M_wrap(MusicTrackClipItem_<_>),
			M_wrap(
				(Integer) u1,
				(ID) source,
				(ID) event,
				(Floater) offset,
				(Floater) begin,
				(Floater) end,
				(Floater) duration,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicTrackClip_;

		using MusicTrackClip = MusicTrackClip_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(MusicTrackPlaybackSetting_<_>),
			M_wrap(
				(MusicTrackClip) clip,
				(MusicTrackTrackType) type,
			),
		);

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(MusicTrack_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename = None>
		struct MusicSegmentCueItem_;

		using MusicSegmentCueItem = MusicSegmentCueItem_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(MusicSegmentCueItem_<_>),
			M_wrap(
				(ID) name,
				(Floater) time,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSegmentCue_;

		using MusicSegmentCue = MusicSegmentCue_<>;

		template <typename _> requires (check_version(version, {72}))
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

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(MusicSegmentPlaybackSetting_<_>),
			M_wrap(
				(Floater) duration,
				(MusicSegmentCue) cue,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(MusicSegmentPlaybackSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) speed,
				(Floater) duration,
				(MusicSegmentCue) cue,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSegment_;

		using MusicSegment = MusicSegment_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(MusicSegment_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainerPlaylistItem_<_>),
			M_wrap(
				(ID) u1,
				(Boolean) group,
				(Integer) child_count,
				(ID) item,
				(AudioPlayType) play_type,
				(AudioPlayMode) play_mode,
				(AudioPlayTypeRandomSetting) random_setting,
				(Integer) weight,
				(Integer) loop,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicPlaylistContainerPlaybackSetting_;

		using MusicPlaylistContainerPlaybackSetting = MusicPlaylistContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainerPlaybackSetting_<_>),
			M_wrap(
				(List<MusicPlaylistContainerPlaylistItem>) playlist,
			),
		);

		template <typename _> requires (check_version(version, {112}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainerPlaybackSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) speed,
				(List<MusicPlaylistContainerPlaylistItem>) playlist,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicPlaylistContainer_;

		using MusicPlaylistContainer = MusicPlaylistContainer_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(MusicPlaylistContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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
		struct MusicSwitchContainerAssociationItem_;

		using MusicSwitchContainerAssociationItem = MusicSwitchContainerAssociationItem_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(MusicSwitchContainerAssociationItem_<_>),
			M_wrap(
				(ID) item,
				(ID) child,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSwitchContainerPlaybackSetting_;

		using MusicSwitchContainerPlaybackSetting = MusicSwitchContainerPlaybackSetting_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(MusicSwitchContainerPlaybackSetting_<_>),
			M_wrap(
				(Boolean) continue_playing_on_switch_change,
				(AudioSwitcherSetting) switcher,
				(List<MusicSwitchContainerAssociationItem>) association,
			),
		);

		template <typename _> requires (check_version(version, {88}))
		M_record_of_map(
			M_wrap(MusicSwitchContainerPlaybackSetting_<_>),
			M_wrap(
				(RegularValue<Floater>) speed,
				(Boolean) continue_playing_on_switch_change,
				(AudioAssociationSetting) association,
			),
		);

		// ----------------

		template <typename = None>
		struct MusicSwitchContainer_;

		using MusicSwitchContainer = MusicSwitchContainer_<>;

		template <typename _> requires (check_version(version, {72, 88}))
		M_record_of_map(
			M_wrap(MusicSwitchContainer_<_>),
			M_wrap(
				(ID) id,
				(ID) parent,
				(List<ID>) child,
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

		template <typename _> requires (check_version(version, {88, 112}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
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

		template <typename _> requires (check_version(version, {112, 128}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {128, 135}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {135, 140}))
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
				(AudioPositioningSetting) positioning,
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {140, 150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
				(MusicTransitionSetting) transition,
				(MusicStingerSetting) stinger,
				(AudioHDRSetting) hdr,
				(MusicMIDISetting) midi,
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

		template <typename _> requires (check_version(version, {150}))
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
				(RealTimeParameterControlSetting) real_time_parameter_control,
				(StateSetting) state,
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

		template <typename _> requires (check_version(version, {72, 112}))
		M_enumeration(
			M_wrap(HierarchyType_<_>),
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

		template <typename _> requires (check_version(version, {112, 128}))
		M_enumeration(
			M_wrap(HierarchyType_<_>),
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

		template <typename _> requires (check_version(version, {128, 132}))
		M_enumeration(
			M_wrap(HierarchyType_<_>),
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

		template <typename _> requires (check_version(version, {132}))
		M_enumeration(
			M_wrap(HierarchyType_<_>),
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
			if constexpr (check_version(version, {72, 112})) {
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
			if constexpr (check_version(version, {112, 128})) {
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
			if constexpr (check_version(version, {128, 132})) {
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
			if constexpr (check_version(version, {132})) {
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

		template <typename = None>
		struct SoundBankReference_;

		using SoundBankReference = SoundBankReference_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(SoundBankReference_<_>),
			M_wrap(
				(ID) id,
				(String) name,
			),
		);

		// ----------------

		template <typename = None>
		struct PlugInReference_;

		using PlugInReference = PlugInReference_<>;

		template <typename _> requires (check_version(version, {118}))
		M_record_of_map(
			M_wrap(PlugInReference_<_>),
			M_wrap(
				(ID) id,
				(String) library,
			),
		);

		#pragma endregion

		#pragma region game synchronization

		template <typename = None>
		struct GameSynchronization_;

		using GameSynchronization = GameSynchronization_<>;

		template <typename _> requires (check_version(version, {72, 140}))
		M_record_of_map(
			M_wrap(GameSynchronization_<_>),
			M_wrap(
				(List<StateGroup>) state_group,
				(List<SwitchGroup>) switch_group,
				(List<GameParameter>) game_parameter,
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
		struct VoiceFilterBehavior_;

		using VoiceFilterBehavior = VoiceFilterBehavior_<>;

		template <typename _> requires (check_version(version, {145}))
		M_enumeration(
			M_wrap(VoiceFilterBehavior_<_>),
			M_wrap(
				sum_all_value,
				use_highest_value,
			),
		);

		// ----------------

		template <typename = None>
		struct ObstructionSetting_;

		using ObstructionSetting = ObstructionSetting_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(ObstructionSetting_<_>),
			M_wrap(
				(Boolean) enable,
				(CoordinateMode) mode,
				(List<CoordinatePoint>) point,
			),
		);

		// ----------------

		template <typename = None>
		struct ObstructionSettingBundle_;

		using ObstructionSettingBundle = ObstructionSettingBundle_<>;

		template <typename _> requires (check_version(version, {72, 112}))
		M_record_of_map(
			M_wrap(ObstructionSettingBundle_<_>),
			M_wrap(
				(ObstructionSetting) volume,
				(ObstructionSetting) low_pass_filter,
			),
		);

		template <typename _> requires (check_version(version, {112}))
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

		template <typename _> requires (check_version(version, {72, 113}))
		M_record_of_map(
			M_wrap(Setting_<_>),
			M_wrap(
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
			),
		);

		template <typename _> requires (check_version(version, {113, 118}))
		M_record_of_map(
			M_wrap(Setting_<_>),
			M_wrap(
				(String) platform,
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
			),
		);

		template <typename _> requires (check_version(version, {118, 145}))
		M_record_of_map(
			M_wrap(Setting_<_>),
			M_wrap(
				(String) platform,
				(Floater) volume_threshold,
				(Integer) maximum_voice_instance,
				(ObstructionSettingBundle) obstruction,
				(ObstructionSettingBundle) occlusion,
				(List<PlugInReference>) plug_in,
			),
		);

		template <typename _> requires (check_version(version, {145}))
		M_record_of_map(
			M_wrap(Setting_<_>),
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

		template <typename = None>
		struct SoundBank_;

		using SoundBank = SoundBank_<>;

		template <typename _> requires (check_version(version, {72}))
		M_record_of_map(
			M_wrap(SoundBank_<_>),
			M_wrap(
				(ID) id,
				(ID) language,
				(ByteList) header_expand,
				(List<ID>) embedded_media,
				(List<SoundBankReference>) reference,
				(Optional<Setting>) setting,
				(Optional<GameSynchronization>) game_synchronization,
				(List<Hierarchy>) hierarchy,
			),
		);

		#pragma endregion

	};

}
