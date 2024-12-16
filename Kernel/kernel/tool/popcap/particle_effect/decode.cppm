module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.particle_effect.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.particle_effect.version;
import twinning.kernel.tool.popcap.particle_effect.definition;
import twinning.kernel.tool.popcap.particle_effect.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::ParticleEffect {

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		Common<version>,
		CommonByteStreamExchangeForInput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		// ----------------

		inline static auto exchange_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			exchange_string_block<IntegerU8>(data, value);
			return;
		}

		template <typename Element, typename ElementParser> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<ElementParser>>
			&& (IsGenericCallable<ElementParser>)
		inline static auto exchange_list (
			IByteStreamView &     data,
			List<Element> &       value,
			ElementParser const & element_parser,
			Size const &          size,
			Size const &          actual_size
		) -> Void {
			value.allocate_full(actual_size);
			for (auto & element : value.head(size)) {
				element_parser(data, element);
			}
			return;
		}

		template <typename Element, typename ElementParser> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<ElementParser>>
			&& (IsGenericCallable<ElementParser>)
		inline static auto exchange_list (
			IByteStreamView &     data,
			List<Element> &       value,
			ElementParser const & element_parser,
			Size const &          size
		) -> Void {
			exchange_list(data, value, element_parser, size, size);
			return;
		}

		template <typename Element, typename ElementParser> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<ElementParser>>
			&& (IsGenericCallable<ElementParser>)
		inline static auto exchange_list (
			IByteStreamView &     data,
			List<Element> &       value,
			ElementParser const & element_parser
		) -> Void {
			auto size = Size{};
			exchange_size_fixed<IntegerU16>(data, size);
			exchange_list(data, value, element_parser, size);
			return;
		}

		// ----------------

		inline static auto process_value_1 (
			IByteStreamView &             data,
			typename Definition::Value1 & value
		) -> Void {
			auto count = Size{};
			auto flag = BitSet<5_sz>{};
			auto initial_time = Optional<Floater>{};
			auto initial_value = Optional<Floater>{};
			auto count_with_flag = Integer{};
			exchange_integer_fixed<IntegerU8>(data, count_with_flag);
			count = cbox<Size>(count_with_flag & 0b111_i);
			if (count == 0b111_sz) {
				exchange_size_fixed<IntegerS16>(data, count);
			}
			flag.from_integer(cbox<IntegerU8>(count_with_flag >> 3_sz));
			value.control = count > 1_sz && flag.get(1_ix);
			if (!flag.get(2_ix)) {
				initial_time.reset();
			}
			else if (flag.get(2_ix)) {
				initial_time.set(0.0_f);
			}
			if (!flag.get(3_ix) && !flag.get(4_ix)) {
				initial_value.reset();
			}
			else if (flag.get(3_ix) && !flag.get(4_ix)) {
				initial_value.set(0.0_f);
			}
			else if (!flag.get(3_ix) && flag.get(4_ix)) {
				initial_value.set(1.0_f);
			}
			else if (flag.get(3_ix) && flag.get(4_ix)) {
				initial_value.set(2.0_f);
			}
			assert_test(!flag.get(5_ix));
			auto first = k_true;
			exchange_list(
				data,
				value.point,
				[&control = value.control, &initial_time, &initial_value, &first] (auto & data, auto & value) {
					if (!first || !initial_time.has()) {
						exchange_floater_fixed<FloaterS32>(data, value.time);
					}
					if (!first || !initial_value.has()) {
						exchange_floater_fixed<FloaterS32>(data, value.value);
					}
					if (control) {
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<1_ix>().x);
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<1_ix>().y);
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<2_ix>().x);
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<2_ix>().y);
					}
					first = k_false;
				},
				count
			);
			if (initial_time.has()) {
				value.point[1_ix].time = initial_time.get();
			}
			if (initial_value.has()) {
				value.point[1_ix].value = initial_value.get();
			}
			return;
		}

		inline static auto process_value_2 (
			IByteStreamView &             data,
			typename Definition::Value2 & value
		) -> Void {
			auto count = Size{};
			exchange_size_fixed<IntegerU16>(data, count);
			if (count > 1_sz) {
				exchange_boolean_fixed<Boolean8>(data, value.control);
			}
			else {
				value.control = k_false;
			}
			exchange_list(
				data,
				value.point,
				[&control = value.control] (auto & data, auto & value) {
					exchange_integer_fixed<IntegerS32>(data, value.time);
					exchange_floater_fixed<FloaterS32>(data, value.value.x);
					exchange_floater_fixed<FloaterS32>(data, value.value.y);
					if (control) {
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<1_ix>().x);
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<1_ix>().y);
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<2_ix>().x);
						exchange_floater_fixed<FloaterS32>(data, value.control_value.template get<2_ix>().y);
					}
				},
				count
			);
			return;
		}

		inline static auto process_value_2_simple (
			IByteStreamView &             data,
			typename Definition::Value2 & value
		) -> Void {
			exchange_floater_fixed<FloaterS32>(data, value.unknown_1);
			exchange_floater_fixed<FloaterS32>(data, value.unknown_2);
			exchange_list(
				data,
				value.point,
				[] (auto & data, auto & value) {
					exchange_integer_fixed<IntegerS32>(data, value.time);
					exchange_floater_fixed<FloaterS32>(data, value.value.x);
					exchange_floater_fixed<FloaterS32>(data, value.value.y);
				}
			);
			return;
		}

		// ----------------

		inline static auto process_texture (
			IByteStreamView &              data,
			typename Definition::Texture & value
		) -> Void {
			exchange_string(data, value.name);
			exchange_integer_fixed<IntegerS16>(data, value.cell);
			exchange_integer_fixed<IntegerS16>(data, value.row);
			exchange_boolean_fixed<Boolean8>(data, value.padded);
			exchange_string(data, value.path);
			return;
		}

		inline static auto process_emitter (
			IByteStreamView &              data,
			typename Definition::Emitter & value
		) -> Void {
			exchange_integer_fixed<IntegerS32>(data, value.unknown_1);
			exchange_string(data, value.name);
			exchange_boolean_fixed<Boolean8>(data, value.keep_in_order);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_2);
			exchange_boolean_fixed<Boolean8>(data, value.oldest_in_front);
			exchange_list(
				data,
				value.particle,
				[] (auto & data, auto & value) {
					exchange_integer_fixed<IntegerS32>(data, value.unknown_1);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_2);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_3);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_4);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_5);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_6);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_7);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_8);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_9);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_10);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_11);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_12);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_13);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_14);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_15);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_16);
					exchange_boolean_fixed<Boolean8>(data, value.instance);
					exchange_boolean_fixed<Boolean8>(data, value.single);
					exchange_boolean_fixed<Boolean8>(data, value.preserve_color);
					exchange_boolean_fixed<Boolean8>(data, value.attach_to_emitter);
					exchange_floater_fixed<FloaterS32>(data, value.attach_value);
					exchange_boolean_fixed<Boolean8>(data, value.flip_horizontal);
					exchange_boolean_fixed<Boolean8>(data, value.flip_vertical);
					exchange_boolean_fixed<Boolean8>(data, value.animation_start_on_random_frame);
					exchange_integer_fixed<IntegerS32>(data, value.repeat_color);
					exchange_integer_fixed<IntegerS32>(data, value.repeat_alpha);
					exchange_boolean_fixed<Boolean8>(data, value.link_transparency_to_color);
					exchange_string(data, value.name);
					exchange_boolean_fixed<Boolean8>(data, value.angle_align_to_motion);
					exchange_boolean_fixed<Boolean8>(data, value.angle_random_align);
					exchange_boolean_fixed<Boolean8>(data, value.angle_keep_aligned_to_motion);
					exchange_integer_fixed<IntegerS32>(data, value.angle_value);
					exchange_integer_fixed<IntegerS32>(data, value.angle_align_offset);
					exchange_integer_fixed<IntegerS32>(data, value.animation_speed);
					exchange_boolean_fixed<Boolean8>(data, value.random_gradient_color);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_17);
					exchange_integer_fixed<IntegerS32>(data, value.texture);
					exchange_list(
						data,
						value.color,
						[] (auto & data, auto & value) {
							exchange_integer_fixed<IntegerU8>(data, value.value.red);
							exchange_integer_fixed<IntegerU8>(data, value.value.green);
							exchange_integer_fixed<IntegerU8>(data, value.value.blue);
							exchange_floater_fixed<FloaterS32>(data, value.time);
						}
					);
					exchange_list(
						data,
						value.alpha,
						[] (auto & data, auto & value) {
							exchange_integer_fixed<IntegerU8>(data, value.value);
							exchange_floater_fixed<FloaterS32>(data, value.time);
						}
					);
					exchange_list(data, value.value, &process_value_1, 23_sz, 28_sz);
					exchange_floater_fixed<FloaterS32>(data, value.reference_point_offset.x);
					exchange_floater_fixed<FloaterS32>(data, value.reference_point_offset.y);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_18);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_19);
					exchange_boolean_fixed<Boolean8>(data, value.lock_aspect);
					process_value_1(data, value.value[26_ix]);
					process_value_1(data, value.value[27_ix]);
					process_value_1(data, value.value[28_ix]);
					exchange_integer_fixed<IntegerS32>(data, value.angle_range);
					exchange_integer_fixed<IntegerS32>(data, value.angle_offset);
					exchange_boolean_fixed<Boolean8>(data, value.get_color_from_layer);
					exchange_boolean_fixed<Boolean8>(data, value.update_color_from_layer);
					exchange_boolean_fixed<Boolean8>(data, value.use_emitter_angle_and_range);
					process_value_1(data, value.value[24_ix]);
					process_value_1(data, value.value[25_ix]);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_20);
					process_value_1(data, value.unknown_21);
					exchange_boolean_fixed<Boolean8>(data, value.use_key_color_only);
					exchange_boolean_fixed<Boolean8>(data, value.update_transparency_from_layer);
					exchange_boolean_fixed<Boolean8>(data, value.use_next_color_key);
					exchange_integer_fixed<IntegerS32>(data, value.number_of_each_color);
					exchange_boolean_fixed<Boolean8>(data, value.get_transparency_from_layer);
				}
			);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_3);
			exchange_list(data, value.value, &process_value_1, 42_sz);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_4);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_5);
			return;
		}

		inline static auto process_layer (
			IByteStreamView &            data,
			typename Definition::Layer & value
		) -> Void {
			exchange_string(data, value.name);
			exchange_list(
				data,
				value.emitter,
				[] (auto & data, auto & value) {
					exchange_floater_fixed<FloaterS32>(data, value.unknown_1);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_2);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_3);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_4);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_5);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_6);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_7);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_8);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_9);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_10);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_11);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_12);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_13);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_14);
					exchange_integer_fixed<IntegerS32>(data, value.preload_frame);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_15);
					exchange_string(data, value.name);
					exchange_integer_fixed<IntegerS32>(data, value.geom);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_16);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_17);
					exchange_boolean_fixed<Boolean8>(data, value.geom_4_if_2);
					exchange_boolean_fixed<Boolean8>(data, value.emit_in);
					exchange_boolean_fixed<Boolean8>(data, value.emit_out);
					exchange_integer_fixed<IntegerU32>(data, value.tint_color.red);
					exchange_integer_fixed<IntegerU32>(data, value.tint_color.green);
					exchange_integer_fixed<IntegerU32>(data, value.tint_color.blue);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_18);
					exchange_integer_fixed<IntegerS32>(data, value.emit_at_point.template get<1_ix>());
					exchange_integer_fixed<IntegerS32>(data, value.type);
					process_value_2(data, value.position);
					exchange_list(data, value.point, &process_value_2_simple);
					exchange_list(data, value.value, &process_value_1, 17_sz, 19_sz);
					exchange_integer_fixed<IntegerS32>(data, value.emit_at_point.template get<2_ix>());
					exchange_integer_fixed<IntegerS32>(data, value.unknown_19);
					process_value_1(data, value.value[18_ix]);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_20);
					process_value_1(data, value.value[19_ix]);
					exchange_list(data, value.mask_path, &exchange_string);
					exchange_boolean_fixed<Boolean8>(data, value.mask);
					exchange_string(data, value.mask_name);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_21);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_22);
					exchange_boolean_fixed<Boolean8>(data, value.invert_mask);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_23);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_24);
					exchange_boolean_fixed<Boolean8>(data, value.is_super);
					exchange_list(data, value.free, &exchange_integer_fixed<IntegerS16>);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_25);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_26);
					exchange_floater_fixed<FloaterS32>(data, value.unknown_27);
				}
			);
			exchange_list(
				data,
				value.deflector,
				[] (auto & data, auto & value) {
					exchange_string(data, value.name);
					exchange_integer_fixed<IntegerS32>(data, value.bounce);
					exchange_integer_fixed<IntegerS32>(data, value.hit);
					exchange_integer_fixed<IntegerS32>(data, value.thickness);
					exchange_boolean_fixed<Boolean8>(data, value.visible);
					process_value_2(data, value.position);
					exchange_list(data, value.point, &process_value_2_simple);
					process_value_1(data, value.active);
					process_value_1(data, value.angle);
				}
			);
			exchange_list(
				data,
				value.blocker,
				[] (auto & data, auto & value) {
					exchange_string(data, value.name);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_1);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_2);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_3);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_4);
					exchange_integer_fixed<IntegerS32>(data, value.unknown_5);
					process_value_2(data, value.position);
					exchange_list(data, value.point, &process_value_2_simple);
					process_value_1(data, value.active);
					process_value_1(data, value.angle);
				}
			);
			process_value_2(data, value.offset);
			process_value_1(data, value.angle);
			exchange_string(data, value.unknown_1);
			exchange_list(data, value.unknown_3, &exchange_integer_fixed<IntegerU8>, 32_sz);
			exchange_list(data, value.unknown_2, &exchange_string);
			exchange_list(data, value.unknown_4, &exchange_integer_fixed<IntegerU8>, 36_sz);
			exchange_list(
				data,
				value.force,
				[] (auto & data, auto & value) {
					exchange_string(data, value.name);
					exchange_boolean_fixed<Boolean8>(data, value.visible);
					process_value_2(data, value.position);
					process_value_1(data, value.active);
					process_value_1(data, value.unknown_1);
					process_value_1(data, value.strength);
					process_value_1(data, value.width);
					process_value_1(data, value.height);
					process_value_1(data, value.angle);
					process_value_1(data, value.direction);
				}
			);
			exchange_list(data, value.unknown_5, &exchange_integer_fixed<IntegerU8>, 28_sz);
			return;
		}

		inline static auto process_effect (
			IByteStreamView &             data,
			typename Definition::Effect & value
		) -> Void {
			exchange_string(data, value.note);
			exchange_list(data, value.texture, &process_texture);
			exchange_list(data, value.emitter, &process_emitter);
			exchange_list(data, value.layer, &process_layer);
			exchange_integer_fixed<IntegerU32>(data, value.background_color.red);
			exchange_integer_fixed<IntegerU32>(data, value.background_color.green);
			exchange_integer_fixed<IntegerU32>(data, value.background_color.blue);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_1);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_2);
			exchange_integer_fixed<IntegerS16>(data, value.frame_rate);
			exchange_integer_fixed<IntegerS16>(data, value.unknown_3);
			exchange_integer_fixed<IntegerS16>(data, value.unknown_4);
			exchange_integer_fixed<IntegerS16>(data, value.unknown_5);
			exchange_integer_fixed<IntegerS32>(data, value.size.width);
			exchange_integer_fixed<IntegerS32>(data, value.size.height);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_6);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_7);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_8);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_9);
			exchange_integer_fixed<IntegerS32>(data, value.unknown_10);
			exchange_integer_fixed<IntegerS32>(data, value.frame_range.template get<1_ix>());
			exchange_integer_fixed<IntegerS32>(data, value.frame_range.template get<2_ix>());
			exchange_string(data, value.unknown_11);
			exchange_integer_fixed<IntegerU8>(data, value.unknown_12);
			exchange_integer_fixed<IntegerS16>(data, value.unknown_13);
			exchange_integer_fixed<IntegerS16>(data, value.unknown_14);
			exchange_data(data, value.startup_state, &exchange_size_fixed<IntegerU32>);
			return;
		}

		// ----------------

		inline static auto process_whole (
			IByteStreamView &             data,
			typename Definition::Effect & definition
		) -> Void {
			data.read_constant(k_magic_identifier);
			data.read_constant(cbox<VersionNumber>(version.number));
			process_effect(data, definition);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &             data_,
			typename Definition::Effect & definition
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_whole(data, definition);
		}

	};

}
