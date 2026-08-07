module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.particle_effect.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.particle_effect.version;

export namespace Twinning::Kernel::Tool::Popcap::ParticleEffect {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Definition {

		#pragma region value

		M_nested_template_declaration(Color);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_list(
			M_nested_template_definition_name(Color),
			M_wrap(
				(Integer) red,
				(Integer) green,
				(Integer) blue,
			),
		);

		// ----------------

		M_nested_template_declaration(Value1Point);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Value1Point),
			M_wrap(
				(Floater) time,
				(Floater) value,
				(Tuple<Position2<Floater>, Position2<Floater>>) control_value,
			),
		);

		// ----------------

		M_nested_template_declaration(Value1);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Value1),
			M_wrap(
				(Boolean) control,
				(List<Value1Point>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(Value2Point);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Value2Point),
			M_wrap(
				(Integer) time,
				(Position2<Floater>) value,
				(Tuple<Position2<Floater>, Position2<Floater>>) control_value,
			),
		);

		// ----------------

		M_nested_template_declaration(Value2);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Value2),
			M_wrap(
				(Floater) unknown_1,
				(Floater) unknown_2,
				(Boolean) control,
				(List<Value2Point>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(ColorPoint);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(ColorPoint),
			M_wrap(
				(Floater) time,
				(Color) value,
			),
		);

		// ----------------

		M_nested_template_declaration(AlphaPoint);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(AlphaPoint),
			M_wrap(
				(Floater) time,
				(Integer) value,
			),
		);

		#pragma endregion

		#pragma region texture

		M_nested_template_declaration(Texture);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Texture),
			M_wrap(
				(String) name,
				(String) path,
				(Integer) cell,
				(Integer) row,
				(Boolean) padded,
			),
		);

		#pragma endregion

		#pragma region emitter

		M_nested_template_declaration(EmitterParticle);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(EmitterParticle),
			M_wrap(
				(String) name,
				(Integer) texture,
				(Boolean) instance,
				(Boolean) single,
				(Boolean) flip_horizontal,
				(Boolean) flip_vertical,
				(Boolean) lock_aspect,
				(Integer) animation_speed,
				(Boolean) animation_start_on_random_frame,
				(Boolean) use_emitter_angle_and_range,
				(Boolean) attach_to_emitter,
				(Floater) attach_value,
				(Position2<Floater>) reference_point_offset,
				(Integer) angle_value,
				(Integer) angle_range,
				(Integer) angle_offset,
				(Boolean) angle_random_align,
				(Integer) angle_align_offset,
				(Boolean) angle_align_to_motion,
				(Boolean) angle_keep_aligned_to_motion,
				(Integer) repeat_color,
				(Boolean) random_gradient_color,
				(Integer) number_of_each_color,
				(Boolean) use_key_color_only,
				(Boolean) use_next_color_key,
				(Boolean) get_color_from_layer,
				(Boolean) update_color_from_layer,
				(List<ColorPoint>) color,
				(Integer) repeat_alpha,
				(Boolean) preserve_color,
				(Boolean) link_transparency_to_color,
				(Boolean) get_transparency_from_layer,
				(Boolean) update_transparency_from_layer,
				(List<AlphaPoint>) alpha,
				(List<Value1>) value,
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Floater) unknown_4,
				(Integer) unknown_5,
				(Integer) unknown_6,
				(Integer) unknown_7,
				(Integer) unknown_8,
				(Integer) unknown_9,
				(Integer) unknown_10,
				(Integer) unknown_11,
				(Integer) unknown_12,
				(Integer) unknown_13,
				(Integer) unknown_14,
				(Integer) unknown_15,
				(Integer) unknown_16,
				(Integer) unknown_17,
				(Integer) unknown_18,
				(Integer) unknown_19,
				(Integer) unknown_20,
				(Value1) unknown_21,
			),
		);

		// ----------------

		M_nested_template_declaration(Emitter);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Emitter),
			M_wrap(
				(String) name,
				(Boolean) keep_in_order,
				(Boolean) oldest_in_front,
				(List<Value1>) value,
				(List<EmitterParticle>) particle,
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		#pragma endregion

		#pragma region layer

		M_nested_template_declaration(LayerEmitter);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerEmitter),
			M_wrap(
				(String) name,
				(Integer) type,
				(Integer) geom,
				(Boolean) geom_4_if_2,
				(Boolean) is_super,
				(Integer) preload_frame,
				(Boolean) emit_in,
				(Boolean) emit_out,
				(Tuple<Integer, Integer>) emit_at_point,
				(Color) tint_color,
				(Boolean) mask,
				(String) mask_name,
				(List<String>) mask_path,
				(Boolean) invert_mask,
				(Value2) position,
				(List<Value2>) point,
				(List<Value1>) value,
				(List<Integer>) free,
				(Floater) unknown_1,
				(Floater) unknown_2,
				(Floater) unknown_3,
				(Floater) unknown_4,
				(Floater) unknown_5,
				(Floater) unknown_6,
				(Floater) unknown_7,
				(Floater) unknown_8,
				(Floater) unknown_9,
				(Floater) unknown_10,
				(Floater) unknown_11,
				(Floater) unknown_12,
				(Integer) unknown_13,
				(Integer) unknown_14,
				(Integer) unknown_15,
				(Floater) unknown_16,
				(Floater) unknown_17,
				(Integer) unknown_18,
				(Integer) unknown_19,
				(Integer) unknown_20,
				(Integer) unknown_21,
				(Integer) unknown_22,
				(Integer) unknown_23,
				(Integer) unknown_24,
				(Integer) unknown_25,
				(Floater) unknown_26,
				(Floater) unknown_27,
			),
		);

		// ----------------

		M_nested_template_declaration(LayerDeflector);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerDeflector),
			M_wrap(
				(String) name,
				(Integer) bounce,
				(Integer) hit,
				(Integer) thickness,
				(Boolean) visible,
				(Value2) position,
				(Value1) active,
				(Value1) angle,
				(List<Value2>) point,
			),
		);

		// ----------------

		M_nested_template_declaration(LayerBlocker);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerBlocker),
			M_wrap(
				(String) name,
				(Value2) position,
				(Value1) active,
				(Value1) angle,
				(List<Value2>) point,
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		// ----------------

		M_nested_template_declaration(LayerForce);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerForce),
			M_wrap(
				(String) name,
				(Boolean) visible,
				(Value2) position,
				(Value1) active,
				(Value1) strength,
				(Value1) width,
				(Value1) height,
				(Value1) angle,
				(Value1) direction,
				(Value1) unknown_1,
			),
		);

		// ----------------

		M_nested_template_declaration(Layer);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Layer),
			M_wrap(
				(String) name,
				(List<LayerEmitter>) emitter,
				(List<LayerDeflector>) deflector,
				(List<LayerBlocker>) blocker,
				(List<LayerForce>) force,
				(Value2) offset,
				(Value1) angle,
				(String) unknown_1,
				(List<String>) unknown_2,
				(List<Integer>) unknown_3,
				(List<Integer>) unknown_4,
				(List<Integer>) unknown_5,
			),
		);

		#pragma endregion

		#pragma region effect

		M_nested_template_declaration(Effect);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Effect),
			M_wrap(
				(String) note,
				(Size2<Integer>) size,
				(Integer) frame_rate,
				(Tuple<Integer, Integer>) frame_range,
				(Color) background_color,
				(ByteList) startup_state,
				(List<Texture>) texture,
				(List<Emitter>) emitter,
				(List<Layer>) layer,
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
				(Integer) unknown_6,
				(Integer) unknown_7,
				(Integer) unknown_8,
				(Integer) unknown_9,
				(Integer) unknown_10,
				(String) unknown_11,
				(Integer) unknown_12,
				(Integer) unknown_13,
				(Integer) unknown_14,
			),
		);

		#pragma endregion

	};

}
