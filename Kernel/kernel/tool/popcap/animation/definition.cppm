module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.animation.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.animation.version;

export namespace Twinning::Kernel::Tool::Popcap::Animation {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Definition {

		#pragma region transform

		M_nested_template_declaration(TranslateTransform);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_list(
			M_nested_template_definition_name(TranslateTransform),
			M_wrap(
				(Floater) x,
				(Floater) y,
			),
		);

		// ----------------

		M_nested_template_declaration(RotateTranslateTransform);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_list(
			M_nested_template_definition_name(RotateTranslateTransform),
			M_wrap(
				(Floater) angle,
				(Floater) x,
				(Floater) y,
			),
		);

		// ----------------

		M_nested_template_declaration(MatrixTranslateTransform);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_list(
			M_nested_template_definition_name(MatrixTranslateTransform),
			M_wrap(
				(Floater) a,
				(Floater) b,
				(Floater) c,
				(Floater) d,
				(Floater) x,
				(Floater) y,
			),
		);

		// ----------------

		using VariantTransform = decltype([] {
			if constexpr (check_version(t_version, {1})) {
				using Type = ListRecordVariant<TranslateTransform, RotateTranslateTransform, MatrixTranslateTransform>;
				return declare<Type>();
			}
		}());

		#pragma endregion

		#pragma region color

		M_nested_template_declaration(Color);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_list(
			M_nested_template_definition_name(Color),
			M_wrap(
				(Floater) red,
				(Floater) green,
				(Floater) blue,
				(Floater) alpha,
			),
		);

		#pragma endregion

		#pragma region rectangle

		M_nested_template_declaration(Rectangle);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Rectangle),
			M_wrap(
				(Position2<Floater>) position,
				(Size2<Floater>) size,
			),
		);

		#pragma endregion

		#pragma region frame

		M_nested_template_declaration(Command);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_list(
			M_nested_template_definition_name(Command),
			M_wrap(
				(String) command,
				(String) argument,
			),
		);

		// ----------------

		M_nested_template_declaration(LayerRemove);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerRemove),
			M_wrap(
				(Integer) index,
			),
		);

		// ----------------

		M_nested_template_declaration(LayerAppend);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerAppend),
			M_wrap(
				(Integer) index,
				(Optional<String>) name,
				(Integer) resource,
				(Boolean) sprite,
				(Boolean) additive,
				(Integer) preload_frame,
				(Floater) time_scale,
			),
		);

		// ----------------

		M_nested_template_declaration(LayerChange);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(LayerChange),
			M_wrap(
				(Integer) index,
				(VariantTransform) transform,
				(Optional<Color>) color,
				(Optional<Integer>) sprite_frame_number,
				(Optional<Rectangle>) source_rectangle,
			),
		);

		// ----------------

		M_nested_template_declaration(Frame);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Frame),
			M_wrap(
				(Optional<String>) label,
				(Boolean) stop,
				(List<Command>) command,
				(List<LayerRemove>) remove,
				(List<LayerAppend>) append,
				(List<LayerChange>) change,
			),
		);

		#pragma endregion

		#pragma region sprite

		M_nested_template_declaration(WorkArea);

		M_nested_template_definition_check(check_version(t_version, {5}))
		M_record_of_list(
			M_nested_template_definition_name(WorkArea),
			M_wrap(
				(Integer) start,
				(Integer) duration,
			),
		);

		// ----------------

		M_nested_template_declaration(Sprite);

		M_nested_template_definition_check(check_version(t_version, {1, 4}))
		M_record_of_map(
			M_nested_template_definition_name(Sprite),
			M_wrap(
				(List<Frame>) frame,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {4, 5}))
		M_record_of_map(
			M_nested_template_definition_name(Sprite),
			M_wrap(
				(String) name,
				(Floater) frame_rate,
				(List<Frame>) frame,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {5}))
		M_record_of_map(
			M_nested_template_definition_name(Sprite),
			M_wrap(
				(String) name,
				(Floater) frame_rate,
				(WorkArea) work_area,
				(List<Frame>) frame,
			),
		);

		#pragma endregion

		#pragma region image

		M_nested_template_declaration(Image);

		M_nested_template_definition_check(check_version(t_version, {1, 2}))
		M_record_of_map(
			M_nested_template_definition_name(Image),
			M_wrap(
				(String) name,
				(RotateTranslateTransform) transform,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {2, 4}))
		M_record_of_map(
			M_nested_template_definition_name(Image),
			M_wrap(
				(String) name,
				(MatrixTranslateTransform) transform,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {4}))
		M_record_of_map(
			M_nested_template_definition_name(Image),
			M_wrap(
				(String) name,
				(Size2<Integer>) size,
				(MatrixTranslateTransform) transform,
			),
		);

		#pragma endregion

		#pragma region animation

		M_nested_template_declaration(Animation);

		M_nested_template_definition_check(check_version(t_version, {1, 4}))
		M_record_of_map(
			M_nested_template_definition_name(Animation),
			M_wrap(
				(Integer) frame_rate,
				(Position2<Floater>) position,
				(Size2<Floater>) size,
				(List<Image>) image,
				(List<Sprite>) sprite,
				(Sprite) main_sprite,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {4}))
		M_record_of_map(
			M_nested_template_definition_name(Animation),
			M_wrap(
				(Integer) frame_rate,
				(Position2<Floater>) position,
				(Size2<Floater>) size,
				(List<Image>) image,
				(List<Sprite>) sprite,
				(Optional<Sprite>) main_sprite,
			),
		);

		#pragma endregion

	};

}
