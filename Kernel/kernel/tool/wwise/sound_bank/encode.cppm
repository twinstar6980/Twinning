module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.wwise.sound_bank.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.wwise.sound_bank.version;
import twinning.kernel.tool.wwise.sound_bank.definition;
import twinning.kernel.tool.wwise.sound_bank.common;
import twinning.kernel.tool.common.byte_stream;
import twinning.kernel.tool.common.wave_structure;

export namespace Twinning::Kernel::Tool::Wwise::SoundBank {

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		Common<version>,
		CommonByteStreamExchangeForOutput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::VersionNumber;

		using typename Common::ChunkSign;

		using typename Common::ChunkSignFlag;

		using Common::k_data_block_padding_size;

		using typename Common::IDWrapper;

		using typename Common::CommonPropertyValue;

		template <typename Type> requires
			AutoConstraint
		using CommonPropertyMap = typename Common::template CommonPropertyMap<Type>;

		using typename Common::EventActionCommonPropertyType;

		using typename Common::ModulatorCommonPropertyType;

		using typename Common::AudioCommonPropertyType;

		template <typename Type> requires
			AutoConstraint
		using EnumerationAttribute = typename Common::template EnumerationAttribute<Type>;

		// ----------------

		template <typename Value> requires
			CategoryConstraint<IsPureInstance<Value>>
			&& (IsEnumerationBox<Value>)
		inline static auto convert_enumeration_index (
			Enumerated &  index_value,
			Value const & value
		) -> Void {
			auto has_case = k_false;
			Generalization::each<typename EnumerationAttribute<Value>::Index>(
				[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) {
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
		inline static auto convert_common_property (
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
				if (cbox<Enumerated>(value) != default_value) {
					auto & element = map.regular.append();
					element.key = type;
					convert_enumeration_index(element.value.template get<1_ix>().template set<Enumerated>(), value);
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
		inline static auto convert_common_property_as_regular (
			CommonPropertyMap<decltype(type)> &                       map,
			typename Definition::template RegularValue<Value> const & value
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
		inline static auto convert_common_property_as_randomizable (
			CommonPropertyMap<decltype(type)> &                            map,
			typename Definition::template RandomizableValue<Value> const & value
		) -> Void {
			if (value.value != EnumerationAttribute<decltype(type)>::Attribute::template Element<static_cast<ZSize>(type.value)>::template Element<3_ixz>::template element<1_ixz>) {
				auto & element = map.regular.append();
				element.key = type;
				element.value.template get<1_ix>().template set<Value>() = value.value;
			}
			if (value.minimum_value != mbox<Value>(0) || value.maximum_value != mbox<Value>(0)) {
				auto & element = map.randomizer.append();
				element.key = type;
				element.value.template get<1_ix>().template set<Value>() = value.minimum_value;
				element.value.template get<2_ix>().template set<Value>() = value.maximum_value;
			}
			return;
		}

		// ----------------

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Definition::BusVoiceSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::voice_volume()>(map, value.volume);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::voice_pitch()>(map, value.pitch);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::voice_low_pass_filter()>(map, value.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property_as_regular<CPTC::voice_high_pass_filter()>(map, value.high_pass_filter);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Definition::AudioVoice const &      value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_randomizable<CPTC::voice_volume()>(map, value.volume);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_randomizable<CPTC::voice_pitch()>(map, value.pitch);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_randomizable<CPTC::voice_low_pass_filter()>(map, value.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property_as_randomizable<CPTC::voice_high_pass_filter()>(map, value.high_pass_filter);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &           map,
			typename Definition::BusVoiceVolumeGainSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {125})) {
				convert_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, value.make_up);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &             map,
			typename Definition::AudioVoiceVolumeGainSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88, 112})) {
				convert_common_property_as_regular<CPTC::voice_volume_make_up_gain()>(map, value.make_up);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property_as_randomizable<CPTC::voice_volume_make_up_gain()>(map, value.make_up);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Definition::BusBusSetting const &   value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::bus_volume()>(map, value.volume);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &     map,
			typename Definition::BusOutputBusSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::output_bus_volume()>(map, value.volume);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, value.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, value.high_pass_filter);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &       map,
			typename Definition::AudioOutputBusSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::output_bus_volume()>(map, value.volume);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::output_bus_low_pass_filter()>(map, value.low_pass_filter);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property_as_regular<CPTC::output_bus_high_pass_filter()>(map, value.high_pass_filter);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &           map,
			typename Definition::AudioAuxiliarySendSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::game_defined_auxiliary_send_volume()>(map, value.game_defined.volume);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::game_defined_auxiliary_send_low_pass_filter()>(map, value.game_defined.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::game_defined_auxiliary_send_high_pass_filter()>(map, value.game_defined.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_0()>(map, value.user_defined.item_1.volume);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_0()>(map, value.user_defined.item_1.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_0()>(map, value.user_defined.item_1.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_1()>(map, value.user_defined.item_2.volume);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_1()>(map, value.user_defined.item_2.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_1()>(map, value.user_defined.item_2.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_2()>(map, value.user_defined.item_3.volume);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_2()>(map, value.user_defined.item_3.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_2()>(map, value.user_defined.item_3.high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_volume_3()>(map, value.user_defined.item_4.volume);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_low_pass_filter_3()>(map, value.user_defined.item_4.low_pass_filter);
			}
			if constexpr (check_version(version, {128})) {
				convert_common_property_as_regular<CPTC::user_defined_auxiliary_send_high_pass_filter_3()>(map, value.user_defined.item_4.high_pass_filter);
			}
			if constexpr (check_version(version, {135})) {
				convert_common_property_as_regular<CPTC::early_reflection_auxiliary_send_volume()>(map, value.early_reflection.volume);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &         map,
			typename Definition::AudioPositioningSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::positioning_center_percent()>(map, value.center_percent);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property<CPTC::positioning_speaker_panning_x()>(map, value.speaker_panning.position.x);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property<CPTC::positioning_speaker_panning_y()>(map, value.speaker_panning.position.y);
			}
			if constexpr (check_version(version, {140})) {
				convert_common_property<CPTC::positioning_speaker_panning_z()>(map, value.speaker_panning.position.z);
			}
			if constexpr (check_version(version, {132})) {
				convert_common_property_as_regular<CPTC::positioning_listener_routing_speaker_panning_division_spatialization_mix()>(map, value.listener_routing.speaker_panning_divsion_spatialization_mix);
			}
			if constexpr (check_version(version, {132})) {
				convert_common_property<CPTC::positioning_listener_routing_attenuation_id()>(map, value.listener_routing.attenuation.id);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Definition::BusHDRSetting const &   value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_threshold()>(map, value.dynamic.threshold);
			}
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_ratio()>(map, value.dynamic.ratio);
			}
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_release_time()>(map, value.dynamic.release_time);
			}
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_window_tap_output_game_parameter_id()>(map, value.window_top_output_game_parameter.id);
			}
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_window_tap_output_game_parameter_minimum()>(map, value.window_top_output_game_parameter.minimum);
			}
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_window_tap_output_game_parameter_maximum()>(map, value.window_top_output_game_parameter.maximum);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> & map,
			typename Definition::AudioHDRSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {88})) {
				convert_common_property<CPTC::hdr_envelope_tracking_active_range()>(map, value.envelope_tracking.active_range);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &  map,
			typename Definition::SoundMIDISetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_event_play_on()>(map, value.event.play_on);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_note_tracking_root_note()>(map, value.note_tracking.root_note);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property_as_regular<CPTC::midi_transformation_transposition()>(map, value.transformation.transposition);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property_as_regular<CPTC::midi_transformation_velocity_offset()>(map, value.transformation.velocity_offset);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_filter_key_range_minimum()>(map, value.filter.key_range_minimum);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_filter_key_range_maximum()>(map, value.filter.key_range_maximum);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_filter_velocity_minimum()>(map, value.filter.velocity_minimum);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_filter_velocity_maximum()>(map, value.filter.velocity_maximum);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_filter_channel()>(map, value.filter.channel);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &  map,
			typename Definition::MusicMIDISetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_target_id()>(map, value.target.id);
			}
			if constexpr (check_version(version, {112})) {
				convert_common_property<CPTC::midi_clip_tempo_source()>(map, value.clip_tempo.source);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &           map,
			typename Definition::AudioPlaybackLimitSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &          map,
			typename Definition::AudioVirtualVoiceSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &              map,
			typename Definition::AudioPlaybackPrioritySetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::playback_priority_value()>(map, value.value);
			}
			if constexpr (check_version(version, {72})) {
				convert_common_property_as_regular<CPTC::playback_priority_offset_at_maximum_distance()>(map, value.offset_at_maximum_distance);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &    map,
			typename Definition::AudioMotionSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {72, 128})) {
				convert_common_property_as_randomizable<CPTC::motion_low_pass_filter()>(map, value.low_pass_filter);
			}
			if constexpr (check_version(version, {72, 128})) {
				convert_common_property_as_randomizable<CPTC::motion_volume_offset()>(map, value.volume_offset);
			}
			return;
		}

		inline static auto load_common_property (
			CommonPropertyMap<AudioCommonPropertyType> &   map,
			typename Definition::AudioMixerSetting const & value
		) -> Void {
			using CPTC = typename AudioCommonPropertyType::Constant;
			if constexpr (check_version(version, {112, 150})) {
				convert_common_property<CPTC::mixer_id()>(map, value.id);
			}
			return;
		}

		// ----------------

		inline static auto exchange_id (
			OByteStreamView &               data,
			typename Definition::ID const & value
		) -> Void {
			data.write(cbox<IntegerU32>(value));
			return;
		}

		template <typename RawValue, auto ignore_reserve = k_false, typename ... Value> requires
			CategoryConstraint<IsPureInstance<RawValue> && IsInstance<Value ...>>
			&& (IsIntegerBox<RawValue>)
			&& (IsSameV<ignore_reserve, Boolean>)
			&& ((IsSame<Value, Boolean> || IsEnumerationBox<Value>) && ...)
		inline static auto exchange_bit_multi (
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
					}
					else if constexpr (IsEnumerationBox<CurrentValue>) {
						auto index_value = Enumerated{};
						convert_enumeration_index(index_value, current_value);
						for (auto & bit_index : SizeRange{EnumerationAttribute<CurrentValue>::size}) {
							bit_set.set(current_index, cbox<Boolean>(clip_bit(index_value, bit_index, 1_sz)));
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

		template <typename RawSizeValue, auto is_zeroed = k_false> requires
			CategoryConstraint<IsPure<RawSizeValue>>
			&& (IsVoid<RawSizeValue> || IsIntegerBox<RawSizeValue>)
			&& (IsSameV<is_zeroed, Boolean>)
		inline static auto exchange_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			if constexpr (IsVoid<RawSizeValue>) {
				StringParser::write_string_until(self_cast<OCharacterStreamView>(data), value.as_view(), CharacterType::k_null);
				self_cast<OCharacterStreamView>(data).write_constant(CharacterType::k_null);
			}
			if constexpr (IsIntegerBox<RawSizeValue>) {
				if constexpr (!is_zeroed) {
					data.write(cbox<RawSizeValue>(value.size()));
					data.write(value);
				}
				else {
					data.write(cbox<RawSizeValue>(value.size() + 1_sz));
					data.write(value);
					self_cast<OCharacterStreamView>(data).write_constant(CharacterType::k_null);
				}
			}
			return;
		}

		template <typename ActualValue> requires
			CategoryConstraint<>
			&& (IsSame<ActualValue, Boolean, Integer, Floater, Enumerated, IDWrapper>)
		inline static auto exchange_common_property_value (
			OByteStreamView &           data,
			CommonPropertyValue const & value
		) -> Void {
			if constexpr (IsSame<ActualValue, Boolean>) {
				exchange_bit_multi<IntegerU32>(data, value.template get<Boolean>());
			}
			if constexpr (IsSame<ActualValue, Integer>) {
				exchange_integer_fixed<IntegerS32>(data, value.template get<Integer>());
			}
			if constexpr (IsSame<ActualValue, Floater>) {
				exchange_floater_fixed<FloaterS32>(data, value.template get<Floater>());
			}
			if constexpr (IsSame<ActualValue, Enumerated>) {
				exchange_enumerated_fixed<IntegerU32>(data, value.template get<Enumerated>());
			}
			if constexpr (IsSame<ActualValue, IDWrapper>) {
				exchange_id(data, value.template get<IDWrapper>().value);
			}
			return;
		}

		// ----------------

		template <typename Type, typename Parser> requires
			CategoryConstraint<IsPureInstance<Type> && IsPureInstance<Parser>>
			&& (IsEnumerationBox<Type>)
			&& (IsGenericCallable<Parser>)
		inline static auto exchange_section_sub (
			OByteStreamView & data,
			Boolean const &   randomizable,
			Parser const &    parser
		) -> Void {
			// NOTE : HERE
			auto map = CommonPropertyMap<Type>{};
			parser(map);
			exchange_list(
				data,
				map.regular.as_list(),
				&exchange_size_fixed<IntegerU8>,
				[] (auto & data, auto & element) {
					auto type = Enumerated{};
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) {
							if (index == static_cast<ZSize>(element.key.value)) {
								type = Attribute::template Element<1_ixz>::template element<1_ixz>;
								has_case = k_true;
							}
						}
					);
					assert_test(has_case);
					exchange_enumerated_fixed<IntegerU8>(data, type);
				},
				[] (auto & data, auto & element) {
					auto has_case = k_false;
					Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
						[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) {
							if (index == static_cast<ZSize>(element.key.value)) {
								exchange_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
								has_case = k_true;
							}
						}
					);
					assert_test(has_case);
				}
			);
			if (randomizable) {
				exchange_list(
					data,
					map.randomizer.as_list(),
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & element) {
						auto type = Enumerated{};
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) {
								if (index == static_cast<ZSize>(element.key.value)) {
									type = Attribute::template Element<1_ixz>::template element<1_ixz>;
									has_case = k_true;
								}
							}
						);
						assert_test(has_case);
						exchange_enumerated_fixed<IntegerU8>(data, type);
					},
					[] (auto & data, auto & element) {
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<Type>::Attribute>(
							[&] <auto index, typename Attribute> (ValuePackage<index>, TypePackage<Attribute>) {
								if (index == static_cast<ZSize>(element.key.value)) {
									exchange_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<1_ix>());
									exchange_common_property_value<typename Attribute::template Element<2_ixz>::template Element<1_ixz>>(data, element.value.template get<2_ix>());
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

		template <typename RawSizeValue> requires
			CategoryConstraint<IsPureInstance<RawSizeValue>>
			&& (IsIntegerBox<RawSizeValue>)
		inline static auto exchange_section_sub (
			OByteStreamView &                     data,
			List<typename Definition::ID> const & value_list
		) -> Void {
			exchange_list(
				data,
				value_list,
				&exchange_size_fixed<RawSizeValue>,
				[] (auto & data, auto & value) {
					exchange_id(data, value);
				}
			);
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                            data,
			typename Definition::RealTimeParameterControlSetting const & real_time_parameter_control_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					real_time_parameter_control_value.item,
					&exchange_size_fixed<IntegerU16>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.parameter.id);
						}
						if constexpr (check_version(version, {112})) {
							exchange_bit_multi<IntegerU8>(data, value.parameter.category);
						}
						if constexpr (check_version(version, {112})) {
							exchange_bit_multi<IntegerU8>(data, value.u1);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_integer_fixed<IntegerU32>(data, value.type);
						}
						if constexpr (check_version(version, {112})) {
							exchange_integer_fixed<IntegerU8>(data, value.type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.u2);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.mode);
						}
						if constexpr (check_version(version, {72})) {
							exchange_list(
								data,
								value.point,
								&exchange_size_fixed<IntegerU16>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_bit_multi<IntegerU32>(data, value.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                         data,
			typename Definition::StateSetting const & state_value
		) -> Void {
			if constexpr (check_version(version, {72, 125})) {
				exchange_list(
					data,
					state_value.item,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72, 125})) {
							exchange_id(data, value.group);
						}
						if constexpr (check_version(version, {72, 125})) {
							exchange_bit_multi<IntegerU8>(data, value.change_occur_at);
						}
						if constexpr (check_version(version, {72, 125})) {
							exchange_list(
								data,
								value.apply,
								&exchange_size_fixed<IntegerU16>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72, 125})) {
										exchange_id(data, value.target);
									}
									if constexpr (check_version(version, {72, 125})) {
										exchange_id(data, value.setting);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {125})) {
				exchange_list(
					data,
					state_value.attribute,
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {125})) {
							exchange_integer_fixed<IntegerU8>(data, value.type);
						}
						if constexpr (check_version(version, {125})) {
							exchange_bit_multi<IntegerU8>(data, value.category);
						}
						if constexpr (check_version(version, {128})) {
							exchange_integer_fixed<IntegerU8>(data, value.u1);
						}
					}
				);
				exchange_list(
					data,
					state_value.item,
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {125})) {
							exchange_id(data, value.group);
						}
						if constexpr (check_version(version, {125})) {
							exchange_bit_multi<IntegerU8>(data, value.change_occur_at);
						}
						if constexpr (check_version(version, {125})) {
							exchange_list(
								data,
								value.apply,
								&exchange_size_fixed<IntegerU8>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {125})) {
										exchange_id(data, value.target);
									}
									if constexpr (check_version(version, {125})) {
										exchange_id(data, value.setting);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                           data,
			List<typename Definition::EffectU1> const & u1_value
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_list(
					data,
					u1_value,
					&exchange_size_fixed<IntegerU16>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {112})) {
							exchange_integer_fixed<IntegerU8>(data, value.type);
						}
						if constexpr (check_version(version, {128})) {
							exchange_bit_multi<IntegerU8>(data, value.mode);
						}
						if constexpr (check_version(version, {112})) {
							exchange_floater_fixed<FloaterS32>(data, value.value);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                        data,
			typename Definition::AudioVoiceVolumeGainSetting const & voice_volume_gain_value,
			typename Definition::AudioHDRSetting const &             hdr_value,
			Boolean const &                                          voice_volume_loudness_normalization_override,
			Boolean const &                                          hdr_envelope_tracking_override
		) -> Void {
			if constexpr (check_version(version, {88, 112})) {
				exchange_bit_multi<IntegerU8>(data, hdr_envelope_tracking_override);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_bit_multi<IntegerU8>(data, voice_volume_loudness_normalization_override);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_bit_multi<IntegerU8>(data, voice_volume_gain_value.normalization);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_bit_multi<IntegerU8>(data, hdr_value.envelope_tracking.enable);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					hdr_envelope_tracking_override,
					voice_volume_loudness_normalization_override,
					voice_volume_gain_value.normalization,
					hdr_value.envelope_tracking.enable
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                  data,
			typename Definition::AudioOutputBusSetting const & output_bus_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_id(data, output_bus_value.bus);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                              data,
			typename Definition::AudioMixerSetting const & mixer_value,
			Boolean const &                                mixer_override
		) -> Void {
			if constexpr (check_version(version, {112, 150})) {
				exchange_bit_multi<IntegerU8>(
					data,
					mixer_override
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                      data,
			typename Definition::AudioAuxiliarySendSetting const & auxiliary_send_value,
			Boolean const &                                        game_defined_auxiliary_send_override,
			Boolean const &                                        user_defined_auxiliary_send_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, game_defined_auxiliary_send_override);
				exchange_bit_multi<IntegerU8>(data, auxiliary_send_value.game_defined.enable);
				exchange_bit_multi<IntegerU8>(data, user_defined_auxiliary_send_override);
				exchange_bit_multi<IntegerU8>(data, auxiliary_send_value.user_defined.enable);
			}
			if constexpr (check_version(version, {112, 135})) {
				exchange_bit_multi<IntegerU8>(
					data,
					game_defined_auxiliary_send_override,
					auxiliary_send_value.game_defined.enable,
					user_defined_auxiliary_send_override,
					auxiliary_send_value.user_defined.enable
				);
			}
			if constexpr (check_version(version, {72, 135})) {
				if (auxiliary_send_value.user_defined.enable) {
					if constexpr (check_version(version, {72, 135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_1.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_2.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_3.bus);
					}
					if constexpr (check_version(version, {72, 135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_4.bus);
					}
				}
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                      data,
			typename Definition::AudioAuxiliarySendSetting const & auxiliary_send_value,
			Boolean const &                                        game_defined_auxiliary_send_override,
			Boolean const &                                        user_defined_auxiliary_send_override,
			Boolean const &                                        early_reflection_auxiliary_send_override
		) -> Void {
			if constexpr (check_version(version, {135})) {
				exchange_bit_multi<IntegerU8>(
					data,
					game_defined_auxiliary_send_override,
					auxiliary_send_value.game_defined.enable,
					user_defined_auxiliary_send_override,
					auxiliary_send_value.user_defined.enable,
					early_reflection_auxiliary_send_override
				);
			}
			if constexpr (check_version(version, {135})) {
				if (auxiliary_send_value.user_defined.enable) {
					if constexpr (check_version(version, {135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_1.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_2.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_3.bus);
					}
					if constexpr (check_version(version, {135})) {
						exchange_id(data, auxiliary_send_value.user_defined.item_4.bus);
					}
				}
			}
			if constexpr (check_version(version, {135})) {
				exchange_id(data, auxiliary_send_value.early_reflection.bus);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                    data,
			typename Definition::AudioPositioningSetting const & positioning_value,
			Boolean const &                                      positioning_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, positioning_override);
				if (positioning_override) {
					// NOTE : HERE
					auto b1 = Boolean{};
					auto b2 = Boolean{};
					auto b3 = Boolean{};
					if (positioning_value.type == Definition::AudioPositioningSettingType::Constant::two_dimension()) {
						if constexpr (check_version(version, {72, 88})) {
							b2 = k_false;
						}
						if constexpr (check_version(version, {88, 112})) {
							b3 = k_true;
						}
						b1 = positioning_value.speaker_panning.enable;
					}
					if (positioning_value.type == Definition::AudioPositioningSettingType::Constant::three_dimension()) {
						if constexpr (check_version(version, {72, 88})) {
							b2 = k_true;
						}
						if constexpr (check_version(version, {88, 112})) {
							b3 = k_false;
						}
						b1 = positioning_value.listener_routing.position_source.mode == Definition::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::game_defined();
					}
					if constexpr (check_version(version, {88, 112})) {
						exchange_bit_multi<IntegerU8>(data, b3);
					}
					if constexpr (check_version(version, {72, 112})) {
						exchange_bit_multi<IntegerU8>(data, positioning_value.type);
					}
					if constexpr (check_version(version, {72, 88})) {
						exchange_bit_multi<IntegerU8>(
							data,
							b1,
							b2
						);
					}
					if constexpr (check_version(version, {88, 112})) {
						exchange_bit_multi<IntegerU8>(
							data,
							b1
						);
					}
					if (positioning_value.type == Definition::AudioPositioningSettingType::Constant::two_dimension()) {
					}
					if (positioning_value.type == Definition::AudioPositioningSettingType::Constant::three_dimension()) {
						exchange_raw_constant(data, 0_iu8);
						exchange_raw_constant(data, 0_iu8);
						exchange_raw_constant(data, 0_iu8);
						exchange_id(data, positioning_value.listener_routing.attenuation.id);
						exchange_bit_multi<IntegerU8>(data, positioning_value.listener_routing.spatialization);
						if (positioning_value.listener_routing.position_source.mode == Definition::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::game_defined()) {
							exchange_bit_multi<IntegerU8>(data, positioning_value.listener_routing.position_source.update_at_each_frame);
						}
						if (positioning_value.listener_routing.position_source.mode == Definition::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) {
							exchange_bit_multi<IntegerU8>(
								data,
								positioning_value.listener_routing.position_source.automation.play_type,
								positioning_value.listener_routing.position_source.automation.play_mode,
								positioning_value.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
							exchange_raw_constant(data, 0_iu8);
							exchange_raw_constant(data, 0_iu8);
							exchange_raw_constant(data, 0_iu8);
							exchange_bit_multi<IntegerU8>(data, positioning_value.listener_routing.position_source.automation.loop);
							exchange_integer_fixed<IntegerU32>(data, positioning_value.listener_routing.position_source.automation.transition_time);
							exchange_bit_multi<IntegerU8>(data, positioning_value.listener_routing.position_source.hold_listener_orientation);
							exchange_list(
								data,
								positioning_value.listener_routing.position_source.automation.point,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									exchange_floater_fixed<FloaterS32>(data, value.position.x);
									exchange_raw_constant(data, 0_iu32);
									exchange_floater_fixed<FloaterS32>(data, value.position.y);
									exchange_integer_fixed<IntegerU32>(data, value.duration);
								}
							);
							exchange_list(
								data,
								positioning_value.listener_routing.position_source.automation.path,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									exchange_integer_fixed<IntegerU32>(data, value.point.begin);
									exchange_integer_fixed<IntegerU32>(data, value.point.count);
								},
								[] (auto & data, auto & value) {
									exchange_floater_fixed<FloaterS32>(data, value.random_range.left_right);
									exchange_floater_fixed<FloaterS32>(data, value.random_range.front_back);
								}
							);
						}
					}
				}
			}
			if constexpr (check_version(version, {112, 132})) {
				auto b2 = Boolean{};
				if constexpr (check_version(version, {112, 125})) {
					exchange_bit_multi<IntegerU8>(
						data,
						positioning_override,
						b2,
						// TODO
						positioning_value.speaker_panning.enable,
						positioning_value.type,
						positioning_value.listener_routing.spatialization,
						positioning_value.listener_routing.position_source.automation.loop,
						positioning_value.listener_routing.position_source.update_at_each_frame,
						positioning_value.listener_routing.position_source.hold_listener_orientation
					);
				}
				if constexpr (check_version(version, {125, 132})) {
					exchange_bit_multi<IntegerU8>(
						data,
						positioning_override,
						positioning_value.enable,
						b2,
						// TODO
						positioning_value.speaker_panning.enable,
						positioning_value.type
					);
				}
				if (positioning_value.type == Definition::AudioPositioningSettingType::Constant::three_dimension()) {
					if constexpr (check_version(version, {112, 125})) {
						exchange_bit_multi<IntegerU8>(
							data,
							positioning_value.listener_routing.position_source.mode
						);
					}
					if constexpr (check_version(version, {125, 132})) {
						exchange_bit_multi<IntegerU8>(
							data,
							positioning_value.listener_routing.spatialization,
							positioning_value.listener_routing.position_source.automation.loop,
							positioning_value.listener_routing.position_source.update_at_each_frame,
							positioning_value.listener_routing.position_source.hold_listener_orientation,
							positioning_value.listener_routing.position_source.mode
						);
					}
					if constexpr (check_version(version, {112, 132})) {
						exchange_id(data, positioning_value.listener_routing.attenuation.id);
					}
					if (positioning_value.listener_routing.position_source.mode == Definition::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::user_defined()) {
						if constexpr (check_version(version, {112, 132})) {
							exchange_bit_multi<IntegerU8>(
								data,
								positioning_value.listener_routing.position_source.automation.play_type,
								positioning_value.listener_routing.position_source.automation.play_mode,
								positioning_value.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_integer_fixed<IntegerU32>(data, positioning_value.listener_routing.position_source.automation.transition_time);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_list(
								data,
								positioning_value.listener_routing.position_source.automation.point,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.z);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_integer_fixed<IntegerU32>(data, value.duration);
									}
								}
							);
						}
						if constexpr (check_version(version, {112, 132})) {
							exchange_list(
								data,
								positioning_value.listener_routing.position_source.automation.path,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_integer_fixed<IntegerU32>(data, value.point.begin);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_integer_fixed<IntegerU32>(data, value.point.count);
									}
								},
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {112, 132})) {
										exchange_floater_fixed<FloaterS32>(data, value.random_range.left_right);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_floater_fixed<FloaterS32>(data, value.random_range.front_back);
									}
									if constexpr (check_version(version, {112, 132})) {
										exchange_floater_fixed<FloaterS32>(data, value.random_range.up_down);
									}
								}
							);
						}
					}
				}
			}
			if constexpr (check_version(version, {132})) {
				if constexpr (check_version(version, {132})) {
					exchange_bit_multi<IntegerU8>(
						data,
						positioning_override,
						positioning_value.listener_routing.enable,
						positioning_value.speaker_panning.mode,
						k_false,
						positioning_value.listener_routing.position_source.mode,
						k_false
					);
				}
				if (positioning_value.listener_routing.enable) {
					if constexpr (check_version(version, {132, 134})) {
						exchange_bit_multi<IntegerU8>(
							data,
							positioning_value.listener_routing.spatialization,
							positioning_value.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_value.listener_routing.position_source.hold_listener_orientation,
							positioning_value.listener_routing.position_source.automation.loop
						);
					}
					if constexpr (check_version(version, {134, 140})) {
						exchange_bit_multi<IntegerU8>(
							data,
							positioning_value.listener_routing.spatialization,
							positioning_value.listener_routing.attenuation.enable,
							positioning_value.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_value.listener_routing.position_source.hold_listener_orientation,
							positioning_value.listener_routing.position_source.automation.loop
						);
					}
					if constexpr (check_version(version, {140})) {
						exchange_bit_multi<IntegerU8>(
							data,
							positioning_value.listener_routing.spatialization,
							positioning_value.listener_routing.attenuation.enable,
							positioning_value.listener_routing.position_source.hold_emitter_position_and_orientation,
							positioning_value.listener_routing.position_source.hold_listener_orientation,
							positioning_value.listener_routing.position_source.automation.loop,
							positioning_value.listener_routing.position_source.diffraction_and_transmission
						);
					}
					if (positioning_value.listener_routing.position_source.mode != Definition::AudioPositioningSettingListenerRoutingPositionSourceMode::Constant::emitter()) {
						if constexpr (check_version(version, {132})) {
							exchange_bit_multi<IntegerU8>(
								data,
								positioning_value.listener_routing.position_source.automation.play_type,
								positioning_value.listener_routing.position_source.automation.play_mode,
								positioning_value.listener_routing.position_source.automation.pick_new_path_when_sound_start
							);
						}
						if constexpr (check_version(version, {132})) {
							exchange_integer_fixed<IntegerU32>(data, positioning_value.listener_routing.position_source.automation.transition_time);
						}
						if constexpr (check_version(version, {132})) {
							exchange_list(
								data,
								positioning_value.listener_routing.position_source.automation.point,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {132})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {132})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.z);
									}
									if constexpr (check_version(version, {132})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {132})) {
										exchange_integer_fixed<IntegerU32>(data, value.duration);
									}
								}
							);
						}
						if constexpr (check_version(version, {132})) {
							exchange_list(
								data,
								positioning_value.listener_routing.position_source.automation.path,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {132})) {
										exchange_integer_fixed<IntegerU32>(data, value.point.begin);
									}
									if constexpr (check_version(version, {132})) {
										exchange_integer_fixed<IntegerU32>(data, value.point.count);
									}
								},
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {132})) {
										exchange_floater_fixed<FloaterS32>(data, value.random_range.left_right);
									}
									if constexpr (check_version(version, {132})) {
										exchange_floater_fixed<FloaterS32>(data, value.random_range.front_back);
									}
									if constexpr (check_version(version, {132})) {
										exchange_floater_fixed<FloaterS32>(data, value.random_range.up_down);
									}
								}
							);
						}
					}
				}
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                             data,
			typename Definition::MusicMIDISetting const & midi_value,
			Boolean const &                               midi_target_override,
			Boolean const &                               midi_clip_tempo_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					k_false,
					midi_clip_tempo_override,
					midi_target_override
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                         data,
			typename Definition::SoundMIDISetting const &             midi_value,
			typename Definition::AudioPlaybackPrioritySetting const & playback_priority_value,
			Boolean const &                                           midi_event_override,
			Boolean const &                                           midi_note_tracking_override,
			Boolean const &                                           playback_priority_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					playback_priority_override,
					playback_priority_value.use_distance_factor,
					midi_event_override,
					midi_note_tracking_override,
					midi_value.note_tracking.enable,
					midi_value.event.break_on_note_off
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                         data,
			typename Definition::AudioPlaybackPrioritySetting const & playback_priority_value,
			Boolean const &                                           playback_priority_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_priority_override);
				exchange_bit_multi<IntegerU8>(data, playback_priority_value.use_distance_factor);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					playback_priority_override,
					playback_priority_value.use_distance_factor
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                      data,
			typename Definition::AudioPlaybackLimitSetting const & playback_limit_value,
			Boolean const &                                        playback_limit_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_value.when_priority_is_equal);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_value.when_limit_is_reached);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_integer_fixed<IntegerU16>(data, playback_limit_value.value.value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_override);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                           data,
			typename Definition::AudioPlaybackLimitSetting const &      playback_limit_value,
			typename Definition::AudioBusMuteForBackgroundMusic const & mute_for_background_music_value,
			Boolean const &                                             playback_limit_override
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					playback_limit_value.when_priority_is_equal,
					playback_limit_value.when_limit_is_reached,
					playback_limit_override,
					mute_for_background_music_value.enable
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_integer_fixed<IntegerU16>(data, playback_limit_value.value.value);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                      data,
			typename Definition::AudioPlaybackLimitSetting const & playback_limit_value,
			typename Definition::AudioVirtualVoiceSetting const &  virtual_voice_value,
			Boolean const &                                        playback_limit_override,
			Boolean const &                                        virtual_voice_override
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, virtual_voice_value.on_return_to_physical);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_value.when_priority_is_equal);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_value.when_limit_is_reached);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_integer_fixed<IntegerU16>(data, playback_limit_value.value.value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_value.scope);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, virtual_voice_value.behavior);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, playback_limit_override);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, virtual_voice_override);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					playback_limit_value.when_priority_is_equal,
					playback_limit_value.when_limit_is_reached,
					playback_limit_value.scope,
					playback_limit_override,
					virtual_voice_override
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, virtual_voice_value.on_return_to_physical);
			}
			if constexpr (check_version(version, {112})) {
				exchange_integer_fixed<IntegerU16>(data, playback_limit_value.value.value);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, virtual_voice_value.behavior);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                               data,
			typename Definition::AudioEffectSetting const & effect_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					effect_value.item,
					[&] (auto & data, auto & value) {
						exchange_size_fixed<IntegerU8>(data, value);
						if constexpr (check_version(version, {72, 150})) {
							if (value > 0_sz) {
								exchange_bit_multi<IntegerU8>(
									data,
									effect_value.bypass.template get<1_ix>(),
									effect_value.bypass.template get<2_ix>(),
									effect_value.bypass.template get<3_ix>(),
									effect_value.bypass.template get<4_ix>(),
									effect_value.bypass.template get<5_ix>()
								);
							}
						}
						if constexpr (check_version(version, {150})) {
							if (value > 0_sz) {
								exchange_bit_multi<IntegerU8>(data, effect_value.bypass);
							}
						}
					},
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU8>(data, value.index);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {72, 150})) {
							// TODO : in typical, render = 1 -> mode = 0 & u2 = 1, render = 0 -> mode = 1 & u2 = 0
							// TODO : if render, mode value will be changed ?
							exchange_bit_multi<IntegerU8>(data, value.use_share_set);
						}
						if constexpr (check_version(version, {72, 150})) {
							exchange_bit_multi<IntegerU8>(data, value.u1);
						}
						if constexpr (check_version(version, {150})) {
							exchange_bit_multi<IntegerU8>(data, value.bypass, value.use_share_set);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                               data,
			typename Definition::AudioEffectSetting const & effect_value,
			Boolean const &                                 effect_override
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, effect_override);
			}
			exchange_section_sub(data, effect_value);
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                 data,
			typename Definition::AudioMetadataSetting const & metadata_value
		) -> Void {
			if constexpr (check_version(version, {140})) {
				exchange_list(
					data,
					metadata_value.item,
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {140})) {
							exchange_integer_fixed<IntegerU8>(data, value.index);
						}
						if constexpr (check_version(version, {140})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {140})) {
							exchange_bit_multi<IntegerU8>(data, value.use_share_set);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                 data,
			typename Definition::AudioMetadataSetting const & metadata_value,
			Boolean const &                                   metadata_override
		) -> Void {
			if constexpr (check_version(version, {140})) {
				exchange_bit_multi<IntegerU8>(data, metadata_override);
			}
			exchange_section_sub(data, metadata_value);
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                               data,
			typename Definition::AudioSourceSetting const & value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.plug_in);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU32>(data, value.type);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, value.type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.resource);
			}
			if constexpr (check_version(version, {72, 113})) {
				exchange_id(data, value.source);
			}
			if constexpr (check_version(version, {72, 113})) {
				if (value.type != Definition::AudioSourceType::Constant::streamed()) {
					exchange_integer_fixed<IntegerU32>(data, value.resource_offset);
				}
			}
			if constexpr (check_version(version, {72, 112})) {
				if (value.type != Definition::AudioSourceType::Constant::streamed()) {
					if constexpr (check_version(version, {72})) {
						exchange_integer_fixed<IntegerU32>(data, value.resource_size);
					}
				}
			}
			if constexpr (check_version(version, {112})) {
				exchange_integer_fixed<IntegerU32>(data, value.resource_size);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, value.is_voice);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					value.is_voice,
					k_false,
					k_false,
					value.non_cachable_stream
				);
			}
			if constexpr (check_version(version, {72})) {
				// TODO
				if ((value.plug_in & 0x0000FFFF_i) >= 0x0002_i) {
					exchange_raw_constant(data, 0_iu32);
				}
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                     data,
			List<typename Definition::AudioSourceSetting> const & value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						exchange_section_sub(data, value);
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                       data,
			typename Definition::BusAutomaticDuckingSetting const & automatic_ducking_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU32>(data, automatic_ducking_value.recovery_time);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, automatic_ducking_value.maximum_ducking_volume);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					automatic_ducking_value.bus,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS32>(data, value.volume);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.fade_out);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.fade_in);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.curve);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.target);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                  data,
			typename Definition::AudioBusConfiguration const & bus_configuration_value
		) -> Void {
			if constexpr (check_version(version, {88})) {
				exchange_integer_fixed<IntegerU32>(data, bus_configuration_value.u1);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                          data,
			typename Definition::BusHDRSetting const & hdr_value
		) -> Void {
			if constexpr (check_version(version, {88, 112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					hdr_value.enable
				);
				exchange_bit_multi<IntegerU8>(
					data,
					hdr_value.dynamic.release_mode
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					hdr_value.enable,
					hdr_value.dynamic.release_mode
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                             data,
			typename Definition::AudioTimeSetting const & time_setting_value,
			Boolean const &                               time_setting_override
		) -> Void {
			if constexpr (check_version(version, {72})) {
				// TODO : test frequency mode-preset time and offset
				// NOTE : EXPLAIN - time = 960000 * signature / tempo, then with frequency mode-preset
				exchange_floater_fixed<FloaterS64>(data, time_setting_value.time);
			}
			if constexpr (check_version(version, {72})) {
				// NOTE : EXPLAIN - 0 if mode.no, millisecond if mode.custom, else by mode-preset
				exchange_floater_fixed<FloaterS64>(data, time_setting_value.offset);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, time_setting_value.tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU8>(data, time_setting_value.signature.first);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU8>(data, time_setting_value.signature.second);
			}
			if constexpr (check_version(version, {72, 140})) {
				auto b2 = Boolean{};
				auto b3 = Boolean{};
				auto b4 = Boolean{};
				auto b5 = Boolean{};
				auto b6 = Boolean{};
				auto b7 = Boolean{};
				auto b8 = Boolean{};
				exchange_bit_multi<IntegerU8>(data, time_setting_override, b2, b3, b4, b5, b6, b7, b8);
				assert_test(b2 == b3 && b3 == b4 && b4 == b5 && b5 == b6 && b6 == b7 && b7 == b8);
			}
			if constexpr (check_version(version, {140})) {
				exchange_bit_multi<IntegerU8>(data, time_setting_override);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                           data,
			typename Definition::MusicTrackClip const & clip_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					clip_value.item,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.source);
						}
						if constexpr (check_version(version, {140})) {
							exchange_id(data, value.event);
						}
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS64>(data, value.offset);
						}
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS64>(data, value.begin);
						}
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS64>(data, value.end);
						}
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS64>(data, value.duration);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				if (!clip_value.item.empty()) {
					exchange_integer_fixed<IntegerU32>(data, clip_value.u1);
				}
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					clip_value.curve,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.index);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU32>(data, value.type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_list(
								data,
								value.point,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_bit_multi<IntegerU32>(data, value.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                data,
			typename Definition::MusicStingerSetting const & stinger_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					stinger_value.item,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.trigger);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.segment_to_play);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU32>(data, value.play_at);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.cue_name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.do_not_play_this_stinger_again_for);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU32>(data, value.allow_playing_stinger_in_next_segment);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                           data,
			typename Definition::MusicTransitionSettingItemFade const & fade_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU32>(data, fade_value.time);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU32>(data, fade_value.curve);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerS32>(data, fade_value.offset);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                   data,
			typename Definition::MusicTransitionSetting const & transition_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					transition_value.item,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {88})) {
							exchange_raw_constant(data, 1_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.source.id);
						}
						if constexpr (check_version(version, {88})) {
							exchange_raw_constant(data, 1_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.destination.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_section_sub(data, value.source.fade_out);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU32>(data, value.source.exit_source_at);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.source.exit_source_at_custom_cue_match);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_bit_multi<IntegerU8, k_true>(data, value.source.play_post_exit);
						}
						if constexpr (check_version(version, {140})) {
							exchange_bit_multi<IntegerU8>(data, value.source.play_post_exit);
						}
						if constexpr (check_version(version, {72})) {
							exchange_section_sub(data, value.destination.fade_in);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.destination.custom_cue_filter_match_target);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.u1);
						}
						if constexpr (check_version(version, {134})) {
							exchange_bit_multi<IntegerU16>(data, value.destination.jump_to);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU16>(data, value.destination.synchronize_to);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_bit_multi<IntegerU8, k_true>(data, value.destination.play_pre_entry);
						}
						if constexpr (check_version(version, {140})) {
							exchange_bit_multi<IntegerU8>(data, value.destination.play_pre_entry);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.destination.custom_cue_filter_match_source_cue_name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.segment.enable);
						}
						if constexpr (check_version(version, {72})) {
							auto has_segment_data = Boolean{};
							if constexpr (check_version(version, {72, 88})) {
								has_segment_data = k_true;
							}
							if constexpr (check_version(version, {88})) {
								has_segment_data = value.segment.enable;
							}
							if (has_segment_data) {
								if constexpr (check_version(version, {72})) {
									exchange_id(data, value.segment.id);
								}
								if constexpr (check_version(version, {72})) {
									exchange_section_sub(data, value.segment.fade_in);
								}
								if constexpr (check_version(version, {72})) {
									exchange_section_sub(data, value.segment.fade_out);
								}
								if constexpr (check_version(version, {72, 140})) {
									exchange_bit_multi<IntegerU8, k_true>(data, value.segment.play_pre_entry);
								}
								if constexpr (check_version(version, {140})) {
									exchange_bit_multi<IntegerU8>(data, value.segment.play_pre_entry);
								}
								if constexpr (check_version(version, {72, 140})) {
									exchange_bit_multi<IntegerU8, k_true>(data, value.segment.play_post_exit);
								}
								if constexpr (check_version(version, {140})) {
									exchange_bit_multi<IntegerU8>(data, value.segment.play_pre_entry);
								}
							}
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                        data,
			typename Definition::MusicTrackTransitionSetting const & transition_value
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_raw_constant(data, 1_iu32);
			}
			if constexpr (check_version(version, {112})) {
				exchange_id(data, transition_value.switcher);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, transition_value.source.fade_out);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU32>(data, transition_value.source.exit_source_at);
			}
			if constexpr (check_version(version, {112})) {
				exchange_id(data, transition_value.source.exit_source_at_custom_cue_match);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, transition_value.destination.fade_in);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                 data,
			typename Definition::AudioSwitcherSetting const & switcher_value
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU32>(data, switcher_value.is_state);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, switcher_value.is_state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, switcher_value.group);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, switcher_value.default_item);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                    data,
			typename Definition::AudioAssociationSetting const & association_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					association_value.argument,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.id);
						}
					},
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {88})) {
							exchange_bit_multi<IntegerU8>(data, value.is_state);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					association_value.path,
					[&] (auto & data, auto & value) {
						// NOTE : HERE
						exchange_size_fixed<IntegerU32>(data, value * 12_sz);
						if constexpr (check_version(version, {72, 88})) {
							exchange_integer_fixed<IntegerU8>(data, association_value.probability);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(
								data,
								association_value.mode
							);
						}
					},
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.object);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU16>(data, value.weight);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU16>(data, value.probability);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                        data,
			typename Definition::SoundPlaylistContainerScope const & scope_value,
			typename Definition::AudioPlayType const &               play_type_value,
			typename Definition::AudioPlayTypeSetting const &        play_type_setting_value,
			typename Definition::AudioPlayMode const &               play_mode_value,
			typename Definition::AudioPlayModeSetting const &        play_mode_setting_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerS16>(data, play_mode_setting_value.continuous.loop.value);
			}
			if constexpr (check_version(version, {88})) {
				exchange_integer_fixed<IntegerS16>(data, play_mode_setting_value.continuous.loop.minimum_value);
			}
			if constexpr (check_version(version, {88})) {
				exchange_integer_fixed<IntegerS16>(data, play_mode_setting_value.continuous.loop.maximum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, play_mode_setting_value.continuous.transition_duration.value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, play_mode_setting_value.continuous.transition_duration.minimum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, play_mode_setting_value.continuous.transition_duration.maximum_value);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU16>(data, play_type_setting_value.random.avoid_repeat);
			}
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, play_mode_setting_value.continuous.transition_type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, play_type_setting_value.random.type);
			}
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, play_type_value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, k_false);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, play_mode_setting_value.continuous.always_reset_playlist);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, play_type_setting_value.sequence.at_end_of_playlist);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, play_mode_value);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU8>(data, scope_value);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(
					data,
					k_false,
					play_mode_setting_value.continuous.always_reset_playlist,
					play_type_setting_value.sequence.at_end_of_playlist,
					play_mode_value,
					scope_value
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                                     data,
			List<typename Definition::SoundPlaylistContainerPlaylistItem> const & playlist_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					playlist_value,
					&exchange_size_fixed<IntegerU16>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.weight);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                                          data,
			List<typename Definition::SoundSwitchContainerObjectAttributeItem> const & object_attribute_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					object_attribute_value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_bit_multi<IntegerU8>(data, value.play_first_only);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_bit_multi<IntegerU8>(data, value.continue_to_play_across_switch);
						}
						if constexpr (check_version(version, {112})) {
							exchange_bit_multi<IntegerU8>(data, value.play_first_only, value.continue_to_play_across_switch);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_integer_fixed<IntegerU32>(data, value.u1);
						}
						if constexpr (check_version(version, {112})) {
							exchange_integer_fixed<IntegerU8>(data, value.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.fade_out_time);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.fade_in_time);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                                       data,
			List<typename Definition::SoundSwitchContainerObjectAssignItem> const & assigned_object_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					assigned_object_value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_list(
								data,
								value.object,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72})) {
										exchange_id(data, value);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                               data,
			List<typename Definition::SoundBlendContainerTrackItem> const & track_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					track_value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_section_sub(data, value.real_time_parameter_control);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.cross_fade.id);
						}
						if constexpr (check_version(version, {112})) {
							exchange_bit_multi<IntegerU8>(data, value.cross_fade.category);
						}
						if constexpr (check_version(version, {72})) {
							exchange_list(
								data,
								value.child,
								&exchange_size_fixed<IntegerU32>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72})) {
										exchange_id(data, value.id);
									}
									if constexpr (check_version(version, {72})) {
										exchange_list(
											data,
											value.point,
											&exchange_size_fixed<IntegerU32>,
											[] (auto & data, auto & value) {
												if constexpr (check_version(version, {72})) {
													exchange_floater_fixed<FloaterS32>(data, value.position.x);
												}
												if constexpr (check_version(version, {72})) {
													exchange_floater_fixed<FloaterS32>(data, value.position.y);
												}
												if constexpr (check_version(version, {72})) {
													exchange_bit_multi<IntegerU32>(data, value.curve);
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

		inline static auto exchange_section_sub (
			OByteStreamView &                                data,
			typename Definition::MusicTrackTrackType const & track_type_value
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				exchange_bit_multi<IntegerU32>(data, track_type_value);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                        data,
			typename Definition::MusicTrackTrackType const &         track_type_value,
			typename Definition::AudioSwitcherSetting const &        switcher_value,
			typename Definition::MusicTrackTransitionSetting const & transition_value
		) -> Void {
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, track_type_value);
			}
			if constexpr (check_version(version, {112})) {
				if (track_type_value == Definition::MusicTrackTrackType::Constant::switcher()) {
					if constexpr (check_version(version, {112})) {
						exchange_section_sub(data, switcher_value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_section_sub(data, transition_value);
					}
				}
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                             data,
			typename Definition::MusicTrackStream const & stream_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU16>(data, stream_value.look_ahead_time);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                            data,
			typename Definition::MusicSegmentCue const & cue_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					cue_value.item,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.name);
						}
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS64>(data, value.time);
						}
						if constexpr (check_version(version, {72, 140})) {
							exchange_raw_constant(data, 0_iu32);
						}
						if constexpr (check_version(version, {140})) {
							exchange_raw_constant(data, 0_iu8);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                                     data,
			List<typename Definition::MusicPlaylistContainerPlaylistItem> const & playlist_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					playlist_value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.item);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.u1);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.child_count);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU32, k_true>(data, value.play_mode, value.play_type);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU16>(data, value.loop);
						}
						if constexpr (check_version(version, {112})) {
							exchange_raw_constant(data, 0_iu32);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.weight);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU16>(data, value.random_setting.avoid_repeat);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.group); // TODO : maybe
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.random_setting.type);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                                                      data,
			List<typename Definition::MusicSwitchContainerAssociationItem> const & association_value
		) -> Void {
			if constexpr (check_version(version, {72, 88})) {
				exchange_list(
					data,
					association_value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72, 88})) {
							exchange_id(data, value.item);
						}
						if constexpr (check_version(version, {72, 88})) {
							exchange_id(data, value.child);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section_sub (
			OByteStreamView &                          data,
			typename Definition::AudioPlayMode const & play_mode_value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, play_mode_value);
			}
			return;
		}

		// ----------------

		inline static auto exchange_section (
			OByteStreamView &                       data,
			typename Definition::StateGroup const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU32>(data, value.default_transition);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					value.custom_transition,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.from);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.to);
						}
						if constexpr (check_version(version, {72})) {
							exchange_integer_fixed<IntegerU32>(data, value.time);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                        data,
			typename Definition::SwitchGroup const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parameter.id);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, value.parameter.category);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					value.point,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_floater_fixed<FloaterS32>(data, value.position.x);
						}
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.position.y);
						}
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU32>(data, value.curve);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                          data,
			typename Definition::GameParameter const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, value.range_default);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU32>(data, value.interpolation_mode);
			}
			if constexpr (check_version(version, {112})) {
				exchange_floater_fixed<FloaterS32>(data, value.interpolation_attack);
			}
			if constexpr (check_version(version, {112})) {
				exchange_floater_fixed<FloaterS32>(data, value.interpolation_release);
			}
			if constexpr (check_version(version, {112})) {
				exchange_bit_multi<IntegerU8>(data, value.bind_to_built_in_parameter);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                  data,
			typename Definition::GameSynchronizationU1 const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {140})) {
				exchange_floater_fixed<FloaterS32>(data, value.u1);
			}
			if constexpr (check_version(version, {140})) {
				exchange_floater_fixed<FloaterS32>(data, value.u2);
			}
			if constexpr (check_version(version, {140})) {
				exchange_floater_fixed<FloaterS32>(data, value.u3);
			}
			if constexpr (check_version(version, {140})) {
				exchange_floater_fixed<FloaterS32>(data, value.u4);
			}
			if constexpr (check_version(version, {140})) {
				exchange_floater_fixed<FloaterS32>(data, value.u5);
			}
			if constexpr (check_version(version, {140})) {
				exchange_floater_fixed<FloaterS32>(data, value.u6);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                    data,
			typename Definition::StatefulPropertySetting const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72, 128})) {
				exchange_list(
					data,
					value.value,
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72, 128})) {
							exchange_integer_fixed<IntegerU8>(data, value.type);
						}
					},
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72, 128})) {
							exchange_floater_fixed<FloaterS32>(data, value.value);
						}
					}
				);
			}
			if constexpr (check_version(version, {128})) {
				exchange_list(
					data,
					value.value,
					&exchange_size_fixed<IntegerU16>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {128})) {
							exchange_integer_fixed<IntegerU16>(data, value.type);
						}
					},
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {128})) {
							exchange_floater_fixed<FloaterS32>(data, value.value);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                        data,
			typename Definition::EventAction const & value
		) -> Void {
			exchange_id(data, value.id);
			// NOTE : HERE
			auto type = Enumerated{};
			auto type_data_begin = Size{};
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, value.scope, value.mode);
			}
			if constexpr (check_version(version, {72})) {
				type_data_begin = data.position();
				data.forward_view(bs_static_size<IntegerU8>());
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.target);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU8>(data, value.u1);
			}
			auto exchange_section_sub_of_exception_list = [&] (
			) -> Void {
				if constexpr (check_version(version, {72, 125})) {
					exchange_list(
						data,
						value.exception,
						&exchange_size_fixed<IntegerU32>,
						[] (auto & data, auto & value) {
							if constexpr (check_version(version, {72, 125})) {
								exchange_id(data, value.id);
							}
							if constexpr (check_version(version, {72, 125})) {
								exchange_bit_multi<IntegerU8>(data, value.u1);
							}
						}
					);
				}
				if constexpr (check_version(version, {125})) {
					exchange_list(
						data,
						value.exception,
						&exchange_size_fixed<IntegerU8>,
						[] (auto & data, auto & value) {
							if constexpr (check_version(version, {125})) {
								exchange_id(data, value.id);
							}
							if constexpr (check_version(version, {125})) {
								exchange_bit_multi<IntegerU8>(data, value.u1);
							}
						}
					);
				}
				return;
			};
			auto has_case = k_false;
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::play_audio()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::play_audio()>();
					type = 4_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property<CPTC::probability()>(common_property, property_value.probability);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_id(data, property_value.sound_bank);
					}
					if constexpr (check_version(version, {145})) {
						exchange_raw_constant(data, 0_iu32);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::stop_audio()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::stop_audio()>();
					type = 1_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {125})) {
						exchange_bit_multi<IntegerU8>(
							data,
							k_false,
							property_value.resume_state_transition,
							property_value.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::pause_audio()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::pause_audio()>();
					type = 2_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72, 125})) {
						exchange_bit_multi<IntegerU8>(
							data,
							property_value.include_delayed_resume_action
						);
					}
					if constexpr (check_version(version, {125})) {
						exchange_bit_multi<IntegerU8>(
							data,
							property_value.include_delayed_resume_action,
							property_value.resume_state_transition,
							property_value.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::resume_audio()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::resume_audio()>();
					type = 3_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72, 125})) {
						exchange_bit_multi<IntegerU8>(
							data,
							property_value.master_resume
						);
					}
					if constexpr (check_version(version, {125})) {
						exchange_bit_multi<IntegerU8>(
							data,
							property_value.master_resume,
							property_value.resume_state_transition,
							property_value.apply_to_dynamic_sequence
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::break_audio()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::break_audio()>();
					type = 28_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::seek_audio()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::seek_audio()>();
					type = 30_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.seek_type);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.seek_value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.seek_value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.seek_value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(
							data,
							property_value.seek_to_nearest_marker
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {113})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::post_event()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::post_event()>();
					type = 33_e;
					if constexpr (check_version(version, {113})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {113})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_voice_pitch()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_voice_pitch()>();
					type = !property_value.reset ? (8_e) : (9_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_voice_volume()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_voice_volume()>();
					type = !property_value.reset ? (10_e) : (11_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_bus_volume()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_bus_volume()>();
					type = !property_value.reset ? (12_e) : (13_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_voice_low_pass_filter()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_voice_low_pass_filter()>();
					type = !property_value.reset ? (14_e) : (15_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {112})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_voice_high_pass_filter()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_voice_high_pass_filter()>();
					type = !property_value.reset ? (32_e) : (48_e);
					if constexpr (check_version(version, {112})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {112})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {112})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {112})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {112})) {
						exchange_bit_multi<IntegerU8>(data, property_value.apply_mode);
					}
					if constexpr (check_version(version, {112})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.minimum_value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.maximum_value);
					}
					if constexpr (check_version(version, {112})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_mute()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_mute()>();
					type = !property_value.reset ? (6_e) : (7_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_game_parameter()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_game_parameter()>();
					type = !property_value.reset ? (19_e) : (20_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::fade_time()>(common_property, property_value.fade_time);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.fade_curve);
					}
					if constexpr (check_version(version, {112})) {
						exchange_bit_multi<IntegerU8>(data, property_value.bypass_game_parameter_interpolation);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.apply_mode);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.minimum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, property_value.value.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_state_availability()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_state_availability()>();
					type = !property_value.enable ? (17_e) : (16_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::activate_state()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::activate_state()>();
					type = 18_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_id(data, property_value.group);
					}
					if constexpr (check_version(version, {72})) {
						exchange_id(data, property_value.item);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::activate_switch()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::activate_switch()>();
					type = 25_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_id(data, property_value.group);
					}
					if constexpr (check_version(version, {72})) {
						exchange_id(data, property_value.item);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::activate_trigger()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::activate_trigger()>();
					type = 29_e;
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::set_bypass_effect()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::set_bypass_effect()>();
					type = !property_value.reset ? (26_e) : (27_e);
					if constexpr (check_version(version, {72})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {72})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(data, property_value.enable);
					}
					if constexpr (check_version(version, {72})) {
						exchange_bit_multi<IntegerU8>(
							data,
							property_value.value.template get<1_ix>(),
							property_value.value.template get<2_ix>(),
							property_value.value.template get<3_ix>(),
							property_value.value.template get<4_ix>(),
							property_value.value.template get<5_ix>(),
							as_constant(property_value.reset),
							as_constant(property_value.reset),
							as_constant(property_value.reset)
						);
					}
					if constexpr (check_version(version, {72})) {
						exchange_section_sub_of_exception_list();
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {112})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::release_envelope()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::release_envelope()>();
					type = 31_e;
					if constexpr (check_version(version, {112})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {112})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					has_case = k_true;
				}
			}
			if constexpr (check_version(version, {113})) {
				if (value.property.type() == Definition::EventActionProperty::Type::Constant::reset_playlist()) {
					auto & property_value = value.property.template get_of_type<Definition::EventActionProperty::Type::Constant::reset_playlist()>();
					type = 34_e;
					if constexpr (check_version(version, {113})) {
						exchange_section_sub<EventActionCommonPropertyType>(
							data,
							k_true,
							[&] (auto & common_property) {
								using CPTC = typename EventActionCommonPropertyType::Constant;
								if constexpr (check_version(version, {113})) {
									convert_common_property_as_randomizable<CPTC::delay()>(common_property, property_value.delay);
								}
							}
						);
					}
					if constexpr (check_version(version, {113})) {
						exchange_raw_constant(data, 4_iu8); // TODO : maybe fade curve ?
					}
					if constexpr (check_version(version, {113, 115})) {
						exchange_raw_constant(data, 0_iu32);
					}
					if constexpr (check_version(version, {115})) {
						exchange_raw_constant(data, 0_iu8);
					}
					has_case = k_true;
				}
			}
			assert_test(has_case);
			if constexpr (check_version(version, {72})) {
				exchange_enumerated_fixed<IntegerU8>(as_lvalue(OByteStreamView{data.sub_view(type_data_begin, bs_static_size<IntegerU8>())}), type);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                  data,
			typename Definition::Event const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72, 125})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {125})) {
				exchange_section_sub<IntegerU8>(data, value.child);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                          data,
			typename Definition::DialogueEvent const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {88})) {
				exchange_integer_fixed<IntegerU8>(data, value.probability);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.association);
			}
			if constexpr (check_version(version, {120})) {
				exchange_raw_constant(data, 0_iu16);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                        data,
			typename Definition::Attenuation const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {140})) {
				exchange_bit_multi<IntegerU8>(data, value.height_spread);
			}
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, value.cone.enable);
				if (value.cone.enable) {
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, value.cone.inner_angle);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, value.cone.outer_angle);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, value.cone.maximum_value);
					}
					if constexpr (check_version(version, {72})) {
						exchange_floater_fixed<FloaterS32>(data, value.cone.low_pass_filter);
					}
					if constexpr (check_version(version, {112})) {
						exchange_floater_fixed<FloaterS32>(data, value.cone.high_pass_filter);
					}
				}
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_integer_fixed<IntegerU8>(data, value.apply.output_bus_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.spread);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_integer_fixed<IntegerU8>(data, value.apply.output_bus_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.game_defined_auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.user_defined_auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.spread);
			}
			if constexpr (check_version(version, {112, 145})) {
				exchange_integer_fixed<IntegerU8>(data, value.apply.output_bus_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.game_defined_auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.user_defined_auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.high_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.spread);
				exchange_integer_fixed<IntegerU8>(data, value.apply.focus);
			}
			if constexpr (check_version(version, {145})) {
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_output_bus_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_game_defined_auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_user_defined_auxiliary_send_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_high_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_spread);
				exchange_integer_fixed<IntegerU8>(data, value.apply.distance_focus);
				exchange_integer_fixed<IntegerU8>(data, value.apply.obstruction_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.obstruction_low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.obstruction_high_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.occlusion_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.occlusion_low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.occlusion_high_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.diffraction_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.diffraction_low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.diffraction_high_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.transmission_volume);
				exchange_integer_fixed<IntegerU8>(data, value.apply.transmission_low_pass_filter);
				exchange_integer_fixed<IntegerU8>(data, value.apply.transmission_high_pass_filter);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					value.curve,
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_bit_multi<IntegerU8>(data, value.mode);
						}
						if constexpr (check_version(version, {72})) {
							exchange_list(
								data,
								value.point,
								&exchange_size_fixed<IntegerU16>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {72})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {72})) {
										exchange_bit_multi<IntegerU32>(data, value.curve);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                            data,
			typename Definition::LowFrequencyOscillatorModulator const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {112})) {
				exchange_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::depth()>(common_property, value.depth);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::frequency()>(common_property, value.frequency);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property<CPTC::waveform()>(common_property, value.waveform);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::smoothing()>(common_property, value.smoothing);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::pulse_width_modulation()>(common_property, value.pulse_width_modulation);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::attack()>(common_property, value.attack);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::initial_phase_offset()>(common_property, value.initial_phase_offset);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property<CPTC::scope()>(common_property, value.scope);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                              data,
			typename Definition::EnvelopeModulator const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {112})) {
				exchange_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::attack_time()>(common_property, value.attack_time);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::attack_curve()>(common_property, value.attack_curve);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::decay_time()>(common_property, value.decay_time);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::sustain_level()>(common_property, value.sustain_level);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::release_time()>(common_property, value.release_time);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property<CPTC::scope()>(common_property, value.scope);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property<CPTC::trigger_on()>(common_property, value.trigger_on);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_randomizable<CPTC::sustain_time()>(common_property, value.sustain_time);
						}
						if constexpr (check_version(version, {112})) {
							convert_common_property<CPTC::stop_playback()>(common_property, value.stop_playback_after_release);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                          data,
			typename Definition::TimeModulator const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {132})) {
				exchange_section_sub<ModulatorCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename ModulatorCommonPropertyType::Constant;
						if constexpr (check_version(version, {132})) {
							convert_common_property_as_randomizable<CPTC::initial_delay()>(common_property, value.initial_delay);
						}
						if constexpr (check_version(version, {132})) {
							convert_common_property_as_regular<CPTC::duration()>(common_property, value.duration);
						}
						if constexpr (check_version(version, {132})) {
							convert_common_property_as_randomizable<CPTC::loop()>(common_property, value.loop);
						}
						if constexpr (check_version(version, {132})) {
							convert_common_property_as_randomizable<CPTC::playback_rate()>(common_property, value.playback_rate);
						}
						if constexpr (check_version(version, {132})) {
							convert_common_property<CPTC::scope()>(common_property, value.scope);
						}
						if constexpr (check_version(version, {132})) {
							convert_common_property<CPTC::trigger_on()>(common_property, value.trigger_on);
						}
						if constexpr (check_version(version, {132})) {
							convert_common_property<CPTC::stop_playback()>(common_property, value.stop_playback_at_end);
						}
					}
				);
			}
			if constexpr (check_version(version, {132})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                   data,
			typename Definition::Effect const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.plug_in);
			}
			if constexpr (check_version(version, {72})) {
				exchange_data(data, value.expand, &exchange_size_fixed<IntegerU32>);
			}
			if constexpr (check_version(version, {72})) {
				exchange_raw_constant(data, 0_iu8);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {125, 128})) {
				exchange_raw_constant(data, 0_iu16);
			}
			if constexpr (check_version(version, {128})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.u1);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                   data,
			typename Definition::Source const & value
		) -> Void {
			return exchange_section(data, self_cast<typename Definition::Effect>(value));
		}

		inline static auto exchange_section (
			OByteStreamView &                        data,
			typename Definition::AudioDevice const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {128})) {
				exchange_id(data, value.plug_in);
			}
			if constexpr (check_version(version, {128})) {
				exchange_data(data, value.expand, &exchange_size_fixed<IntegerU32>);
			}
			if constexpr (check_version(version, {128})) {
				exchange_raw_constant(data, 0_iu8);
			}
			if constexpr (check_version(version, {128})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {128})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {128})) {
				exchange_section_sub(data, value.u1);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.effect);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                     data,
			typename Definition::AudioBus const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {128})) {
				if (value.parent == 0_i) {
					if constexpr (check_version(version, {128})) {
						exchange_id(data, value.audio_device);
					}
				}
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_false,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {125})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.bus);
						}
						if constexpr (check_version(version, {128})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {125})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
					}
				);
			}
			if constexpr (check_version(version, {88, 112})) {
				exchange_bit_multi<IntegerU8>(data, value.override_positioning);
				exchange_bit_multi<IntegerU8>(data, value.positioning.speaker_panning.enable);
			}
			if constexpr (check_version(version, {112, 125})) {
				exchange_bit_multi<IntegerU8>(data, value.override_positioning, value.positioning.speaker_panning.enable);
			}
			if constexpr (check_version(version, {125})) {
				// NOTE : HERE
				auto override_positioning = Boolean{k_true};
				exchange_section_sub(data, value.positioning, override_positioning);
			}
			if constexpr (check_version(version, {125, 135})) {
				// NOTE : HERE
				auto override_game_defined_auxiliary_send = Boolean{k_true};
				auto override_user_defined_auxiliary_send = Boolean{k_true};
				exchange_section_sub(data, value.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				// NOTE : HERE
				auto override_game_defined_auxiliary_send = Boolean{k_true};
				auto override_user_defined_auxiliary_send = Boolean{k_true};
				auto override_early_reflection_auxiliary_send = Boolean{k_true};
				exchange_section_sub(data, value.auxiliary_send, override_game_defined_auxiliary_send, override_user_defined_auxiliary_send, override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_limit, value.override_playback_limit);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.playback_limit, value.mute_for_background_music, value.override_playback_limit);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.bus_configuration);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.hdr);
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_raw_constant(data, 63_iu32);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.automatic_ducking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_id(data, value.mixer);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_raw_constant(data, 0_iu16);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                              data,
			typename Definition::AuxiliaryAudioBus const & value
		) -> Void {
			return exchange_section(data, self_cast<typename Definition::AudioBus>(value));
		}

		inline static auto exchange_section (
			OByteStreamView &                  data,
			typename Definition::Sound const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.source);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.playback_priority, value.override_midi_event, value.override_midi_note_tracking, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							convert_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, value.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							convert_common_property_as_randomizable<CPTC::playback_loop()>(common_property, value.playback_setting.loop);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                   data,
			typename Definition::SoundPlaylistContainer const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.playback_priority, value.override_midi_event, value.override_midi_note_tracking, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							convert_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, value.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.scope, value.playback_setting.type, value.playback_setting.type_setting, value.playback_setting.mode, value.playback_setting.mode_setting);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.playlist);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                 data,
			typename Definition::SoundSwitchContainer const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.playback_priority, value.override_midi_event, value.override_midi_note_tracking, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							convert_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, value.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.switcher);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.mode);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.object_assign);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.object_attribute);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                data,
			typename Definition::SoundBlendContainer const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.playback_priority, value.override_midi_event, value.override_midi_note_tracking, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {88})) {
							convert_common_property_as_randomizable<CPTC::playback_initial_delay()>(common_property, value.playback_setting.initial_delay);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.track);
			}
			if constexpr (check_version(version, {120})) {
				exchange_section_sub(data, value.playback_setting.mode);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                       data,
			typename Definition::ActorMixer const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.playback_priority, value.override_midi_event, value.override_midi_note_tracking, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                       data,
			typename Definition::MusicTrack const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.override_midi_target, value.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.source);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.clip);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72, 112})) {
				exchange_section_sub(data, value.playback_setting.type);
			}
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.playback_setting.type, value.playback_setting.switcher, value.playback_setting.transition);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.stream);
			}
			if constexpr (check_version(version, {72})) {
				exchange_raw_constant(data, 0_iu16);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                         data,
			typename Definition::MusicSegment const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.override_midi_target, value.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_regular<CPTC::playback_speed()>(common_property, value.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.time_setting, value.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.stinger);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS64>(data, value.playback_setting.duration);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.cue);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                   data,
			typename Definition::MusicPlaylistContainer const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.override_midi_target, value.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_regular<CPTC::playback_speed()>(common_property, value.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.time_setting, value.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.stinger);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.transition);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.playlist);
			}
			return;
		}

		inline static auto exchange_section (
			OByteStreamView &                                 data,
			typename Definition::MusicSwitchContainer const & value
		) -> Void {
			exchange_id(data, value.id);
			if constexpr (check_version(version, {112})) {
				exchange_section_sub(data, value.midi, value.override_midi_target, value.override_midi_clip_tempo);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.effect, value.override_effect);
			}
			if constexpr (check_version(version, {140})) {
				exchange_section_sub(data, value.metadata, value.override_metadata);
			}
			if constexpr (check_version(version, {112, 150})) {
				exchange_section_sub(data, value.mixer, value.override_mixer);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.output_bus);
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.parent);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_priority, value.override_playback_priority);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<AudioCommonPropertyType>(
					data,
					k_true,
					[&] (auto & common_property) {
						using CPTC = typename AudioCommonPropertyType::Constant;
						if constexpr (check_version(version, {112})) {
							convert_common_property_as_regular<CPTC::playback_speed()>(common_property, value.playback_setting.speed);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.voice);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.voice_volume_gain);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.output_bus);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.auxiliary_send);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.positioning);
						}
						if constexpr (check_version(version, {88})) {
							load_common_property(common_property, value.hdr);
						}
						if constexpr (check_version(version, {112})) {
							load_common_property(common_property, value.midi);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_limit);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.virtual_voice);
						}
						if constexpr (check_version(version, {72})) {
							load_common_property(common_property, value.playback_priority);
						}
						if constexpr (check_version(version, {72, 128})) {
							load_common_property(common_property, value.motion);
						}
						if constexpr (check_version(version, {112, 150})) {
							load_common_property(common_property, value.mixer);
						}
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.positioning, value.override_positioning);
			}
			if constexpr (check_version(version, {72, 135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send);
			}
			if constexpr (check_version(version, {135})) {
				exchange_section_sub(data, value.auxiliary_send, value.override_game_defined_auxiliary_send, value.override_user_defined_auxiliary_send, value.override_early_reflection_auxiliary_send);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_limit, value.virtual_voice, value.override_playback_limit, value.override_virtual_voice);
			}
			if constexpr (check_version(version, {88})) {
				exchange_section_sub(data, value.voice_volume_gain, value.hdr, value.override_voice_volume_loudness_normalization, value.override_hdr_envelope_tracking);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.state);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.real_time_parameter_control);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub<IntegerU32>(data, value.child);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.time_setting, value.override_time_setting);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.stinger);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.transition);
			}
			if constexpr (check_version(version, {72, 88})) {
				exchange_section_sub(data, value.playback_setting.switcher);
			}
			if constexpr (check_version(version, {72})) {
				exchange_bit_multi<IntegerU8>(data, value.playback_setting.continue_playing_on_switch_change);
			}
			if constexpr (check_version(version, {72})) {
				exchange_section_sub(data, value.playback_setting.association);
			}
			return;
		}

		// ----------------

		inline static auto exchange_chunk_bkhd (
			OByteStreamView &                      data,
			typename Definition::SoundBank const & value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				data.write_constant(cbox<VersionNumber>(version.number));
			}
			if constexpr (check_version(version, {72})) {
				exchange_id(data, value.id);
			}
			if constexpr (check_version(version, {72, 125})) {
				exchange_integer_fixed<IntegerU32>(data, value.language);
			}
			if constexpr (check_version(version, {125})) {
				exchange_id(data, value.language);
			}
			if constexpr (check_version(version, {72})) {
				// NOTE : HERE
				data.write(value.header_expand);
			}
			return;
		}

		inline static auto exchange_chunk_didx_data (
			OByteStreamView &                     didx_data,
			OByteStreamView &                     data_data,
			List<typename Definition::ID> const & value,
			Path const &                          embedded_media_directory,
			Size const &                          data_begin_position
		) -> Void {
			if constexpr (check_version(version, {72})) {
				// NOTE : HERE
				exchange_list_element(
					didx_data,
					value,
					[&] (auto & data, auto & value) {
						auto data_offset = Size{};
						auto data_size = Size{};
						if (value == 0_i) {
							data_offset = 1_ix;
							data_size = 0_sz;
						}
						else {
							data_data.write_space(k_null_byte, compute_padding_size(data_begin_position + data_data.position(), k_data_block_padding_size));
							data_offset = data_data.position();
							data_size = Storage::read_stream_file(embedded_media_directory / "{}.wem"_sf(value), data_data);
						}
						exchange_id(data, value);
						exchange_size_fixed<IntegerU32>(data, data_offset);
						exchange_size_fixed<IntegerU32>(data, data_size);
					}
				);
			}
			return;
		}

		inline static auto exchange_chunk_init (
			OByteStreamView &                    data,
			typename Definition::Setting const & value
		) -> Void {
			if constexpr (check_version(version, {118})) {
				exchange_list(
					data,
					value.plug_in,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {118})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {118, 140})) {
							exchange_string<IntegerU32, k_true>(data, value.library);
						}
						if constexpr (check_version(version, {140})) {
							exchange_string<Void>(data, value.library);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_chunk_stmg (
			OByteStreamView &                                data,
			typename Definition::Setting const &             value,
			typename Definition::GameSynchronization const & game_synchronization_value
		) -> Void {
			if constexpr (check_version(version, {145})) {
				exchange_bit_multi<IntegerU16>(data, value.voice_filter_behavior);
			}
			if constexpr (check_version(version, {72})) {
				exchange_floater_fixed<FloaterS32>(data, value.volume_threshold);
			}
			if constexpr (check_version(version, {72})) {
				exchange_integer_fixed<IntegerU16>(data, value.maximum_voice_instance);
			}
			if constexpr (check_version(version, {128})) {
				exchange_raw_constant(data, 50_iu16);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					game_synchronization_value.state_group,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						exchange_section(data, value);
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					game_synchronization_value.switch_group,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						exchange_section(data, value);
					}
				);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					game_synchronization_value.game_parameter,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						exchange_section(data, value);
					}
				);
			}
			if constexpr (check_version(version, {120, 125})) {
				exchange_raw_constant(data, 0_iu32);
				exchange_raw_constant(data, 0_iu32);
			}
			if constexpr (check_version(version, {125, 140})) {
				exchange_raw_constant(data, 0_iu32);
			}
			if constexpr (check_version(version, {140})) {
				exchange_list(
					data,
					game_synchronization_value.u1,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						exchange_section(data, value);
					}
				);
			}
			return;
		}

		inline static auto exchange_chunk_hirc (
			OByteStreamView &                            data,
			List<typename Definition::Hierarchy> const & value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						// NOTE : HERE
						auto type = Enumerated{};
						auto type_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU8>())};
						auto size_data = OByteStreamView{data.forward_view(bs_static_size<IntegerU32>())};
						auto item_data = OByteStreamView{data.reserve_view()};
						auto has_case = k_false;
						Generalization::each<typename EnumerationAttribute<typename Definition::HierarchyType>::Index>(
							[&] <auto index, auto value_index> (ValuePackage<index>, ValuePackage<value_index>) {
								constexpr auto variant_type = mbox<typename Definition::HierarchyType>(index);
								if constexpr (variant_type != Definition::HierarchyType::Constant::unknown()) {
									if (static_cast<ZSize>(value.index().value) == index) {
										type = cbox<Enumerated>(value_index);
										exchange_section(item_data, value.template get_of_type<variant_type>());
										has_case = k_true;
									}
								}
							}
						);
						if (!has_case) {
							auto & unknown_value = value.template get_of_type<Definition::HierarchyType::Constant::unknown()>();
							type = cbox<Enumerated>(unknown_value.type);
							item_data.write(unknown_value.data);
						}
						exchange_enumerated_fixed<IntegerU8>(type_data, type);
						exchange_integer_fixed<IntegerU32>(size_data, cbox<Integer>(item_data.position()));
						data.forward(item_data.position());
						return;
					}
				);
			}
			return;
		}

		inline static auto exchange_chunk_stid (
			OByteStreamView &                                     data,
			List<typename Definition::SoundBankReference> const & value
		) -> Void {
			if constexpr (check_version(version, {72})) {
				exchange_raw_constant(data, 1_iu32);
			}
			if constexpr (check_version(version, {72})) {
				exchange_list(
					data,
					value,
					&exchange_size_fixed<IntegerU32>,
					[] (auto & data, auto & value) {
						if constexpr (check_version(version, {72})) {
							exchange_id(data, value.id);
						}
						if constexpr (check_version(version, {72})) {
							exchange_string<IntegerU8>(data, value.name);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_chunk_envs (
			OByteStreamView &                    data,
			typename Definition::Setting const & value
		) -> Void {
			if constexpr (check_version(version, {72, 112})) {
				// NOTE : HERE
				exchange_list_element(
					data,
					as_lvalue(
						make_list<CWrapperView<typename Definition::ObstructionSetting>>(
							value.obstruction.volume,
							value.obstruction.low_pass_filter,
							value.occlusion.volume,
							value.occlusion.low_pass_filter
						)
					),
					[] (auto & data, auto & value_view) {
						auto & value = value_view.get();
						if constexpr (check_version(version, {72, 112})) {
							exchange_bit_multi<IntegerU8>(data, value.enable);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_bit_multi<IntegerU8>(data, value.mode);
						}
						if constexpr (check_version(version, {72, 112})) {
							exchange_list(
								data,
								value.point,
								&exchange_size_fixed<IntegerU16>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {72, 112})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {72, 112})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {72, 112})) {
										exchange_bit_multi<IntegerU32>(data, value.curve);
									}
								}
							);
						}
					}
				);
			}
			if constexpr (check_version(version, {112})) {
				// NOTE : HERE
				exchange_list_element(
					data,
					as_lvalue(
						make_list<CWrapperView<typename Definition::ObstructionSetting>>(
							value.obstruction.volume,
							value.obstruction.low_pass_filter,
							value.obstruction.high_pass_filter,
							value.occlusion.volume,
							value.occlusion.low_pass_filter,
							value.occlusion.high_pass_filter
						)
					),
					[] (auto & data, auto & value_view) {
						auto & value = value_view.get();
						if constexpr (check_version(version, {112})) {
							exchange_bit_multi<IntegerU8>(data, value.enable);
						}
						if constexpr (check_version(version, {112})) {
							exchange_bit_multi<IntegerU8>(data, value.mode);
						}
						if constexpr (check_version(version, {112})) {
							exchange_list(
								data,
								value.point,
								&exchange_size_fixed<IntegerU16>,
								[] (auto & data, auto & value) {
									if constexpr (check_version(version, {112})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.x);
									}
									if constexpr (check_version(version, {112})) {
										exchange_floater_fixed<FloaterS32>(data, value.position.y);
									}
									if constexpr (check_version(version, {112})) {
										exchange_bit_multi<IntegerU32>(data, value.curve);
									}
								}
							);
						}
					}
				);
			}
			return;
		}

		inline static auto exchange_chunk_plat (
			OByteStreamView &                    data,
			typename Definition::Setting const & value
		) -> Void {
			if constexpr (check_version(version, {113, 118})) {
				exchange_string<IntegerU32>(data, value.platform);
			}
			if constexpr (check_version(version, {118, 140})) {
				exchange_string<IntegerU32, k_true>(data, value.platform);
			}
			if constexpr (check_version(version, {140})) {
				exchange_string<Void>(data, value.platform);
			}
			return;
		}

		// ----------------

		inline static auto exchange_sound_bank (
			OByteStreamView &                      data,
			typename Definition::SoundBank const & value,
			Path const &                           embedded_media_directory
		) -> Void {
			if constexpr (check_version(version, {72})) {
				{
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_bkhd(chunk, value);
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::bkhd,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				constexpr auto didx_item_structure_size = bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>() + bs_static_size<IntegerU32>();
				if (!value.embedded_media.empty()) {
					auto didx_sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto didx_chunk = OByteStreamView{data.forward_view(didx_item_structure_size * value.embedded_media.size())};
					auto data_sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto data_chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_didx_data(didx_chunk, data_chunk, value.embedded_media, embedded_media_directory, data.position());
					didx_sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::didx,
							.size = cbox<IntegerU32>(didx_chunk.position()),
						}
					);
					data_sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::data,
							.size = cbox<IntegerU32>(data_chunk.position()),
						}
					);
					data.forward(data_chunk.position());
				}
			}
			if constexpr (check_version(version, {118})) {
				if (value.setting.has()) {
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_init(chunk, value.setting.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::init,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.setting.has()) {
					assert_test(value.game_synchronization.has());
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_stmg(chunk, value.setting.get(), value.game_synchronization.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::stmg,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (!value.hierarchy.empty()) {
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_hirc(chunk, value.hierarchy);
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::hirc,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (!value.reference.empty()) {
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_stid(chunk, value.reference);
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::stid,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {72})) {
				if (value.setting.has()) {
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_envs(chunk, value.setting.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::envs,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			if constexpr (check_version(version, {113})) {
				if (value.setting.has()) {
					auto sign_data = OByteStreamView{data.forward_view(bs_static_size<ChunkSign>())};
					auto chunk = OByteStreamView{data.reserve_view()};
					exchange_chunk_plat(chunk, value.setting.get());
					sign_data.write(
						ChunkSign{
							.id = ChunkSignFlag::plat,
							.size = cbox<IntegerU32>(chunk.position()),
						}
					);
					data.forward(chunk.position());
				}
			}
			return;
		}

		// ----------------

		inline static auto process_whole (
			OByteStreamView &                      data,
			typename Definition::SoundBank const & definition,
			Path const &                           embedded_media_directory
		) -> Void {
			exchange_sound_bank(data, definition, embedded_media_directory);
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &                      data_,
			typename Definition::SoundBank const & definition,
			Path const &                           embedded_media_directory
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition, embedded_media_directory);
		}

	};

}
