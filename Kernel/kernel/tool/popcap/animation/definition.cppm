module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.animation.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.animation.version;

export namespace Twinning::Kernel::Tool::PopCap::Animation {

	template <auto version> requires (check_version(version, {}))
	struct Definition {

		#pragma region transform

		template <typename = None>
		struct TranslateTransform_;

		using TranslateTransform = TranslateTransform_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_list(
			M_wrap(TranslateTransform_<_>),
			M_wrap(
				(Floater) x,
				(Floater) y,
			),
		);

		// ----------------

		template <typename = None>
		struct RotateTranslateTransform_;

		using RotateTranslateTransform = RotateTranslateTransform_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_list(
			M_wrap(RotateTranslateTransform_<_>),
			M_wrap(
				(Floater) angle,
				(Floater) x,
				(Floater) y,
			),
		);

		// ----------------

		template <typename = None>
		struct MatrixTranslateTransform_;

		using MatrixTranslateTransform = MatrixTranslateTransform_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_list(
			M_wrap(MatrixTranslateTransform_<_>),
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
			if constexpr (check_version(version, {1})) {
				using Type = ListRecordVariant<TranslateTransform, RotateTranslateTransform, MatrixTranslateTransform>;
				return declare<Type>();
			}
		}());

		#pragma endregion

		#pragma region color

		template <typename = None>
		struct Color_;

		using Color = Color_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_list(
			M_wrap(Color_<_>),
			M_wrap(
				(Floater) red,
				(Floater) green,
				(Floater) blue,
				(Floater) alpha,
			),
		);

		#pragma endregion

		#pragma region rectangle

		template <typename = None>
		struct Rectangle_;

		using Rectangle = Rectangle_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(Rectangle_<_>),
			M_wrap(
				(Position2<Floater>) position,
				(Size2<Floater>) size,
			),
		);

		#pragma endregion

		#pragma region frame

		template <typename = None>
		struct Command_;

		using Command = Command_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_list(
			M_wrap(Command_<_>),
			M_wrap(
				(String) command,
				(String) argument,
			),
		);

		// ----------------

		template <typename = None>
		struct LayerRemove_;

		using LayerRemove = LayerRemove_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(LayerRemove_<_>),
			M_wrap(
				(Integer) index,
			),
		);

		// ----------------

		template <typename = None>
		struct LayerAppend_;

		using LayerAppend = LayerAppend_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(LayerAppend_<_>),
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

		template <typename = None>
		struct LayerChange_;

		using LayerChange = LayerChange_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(LayerChange_<_>),
			M_wrap(
				(Integer) index,
				(VariantTransform) transform,
				(Optional<Color>) color,
				(Optional<Integer>) sprite_frame_number,
				(Optional<Rectangle>) source_rectangle,
			),
		);

		// ----------------

		template <typename = None>
		struct Frame_;

		using Frame = Frame_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(Frame_<_>),
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

		template <typename = None>
		struct WorkArea_;

		using WorkArea = WorkArea_<>;

		template <typename _> requires (check_version(version, {5}))
		M_record_of_list(
			M_wrap(WorkArea_<_>),
			M_wrap(
				(Integer) start,
				(Integer) duration,
			),
		);

		// ----------------

		template <typename = None>
		struct Sprite_;

		using Sprite = Sprite_<>;

		template <typename _> requires (check_version(version, {1, 4}))
		M_record_of_map(
			M_wrap(Sprite_<_>),
			M_wrap(
				(List<Frame>) frame,
			),
		);

		template <typename _> requires (check_version(version, {4, 5}))
		M_record_of_map(
			M_wrap(Sprite_<_>),
			M_wrap(
				(String) name,
				(Floater) frame_rate,
				(List<Frame>) frame,
			),
		);

		template <typename _> requires (check_version(version, {5}))
		M_record_of_map(
			M_wrap(Sprite_<_>),
			M_wrap(
				(String) name,
				(Floater) frame_rate,
				(WorkArea) work_area,
				(List<Frame>) frame,
			),
		);

		#pragma endregion

		#pragma region image

		template <typename = None>
		struct Image_;

		using Image = Image_<>;

		template <typename _> requires (check_version(version, {1, 2}))
		M_record_of_map(
			M_wrap(Image_<_>),
			M_wrap(
				(String) name,
				(RotateTranslateTransform) transform,
			),
		);

		template <typename _> requires (check_version(version, {2, 4}))
		M_record_of_map(
			M_wrap(Image_<_>),
			M_wrap(
				(String) name,
				(MatrixTranslateTransform) transform,
			),
		);

		template <typename _> requires (check_version(version, {4}))
		M_record_of_map(
			M_wrap(Image_<_>),
			M_wrap(
				(String) name,
				(Size2<Integer>) size,
				(MatrixTranslateTransform) transform,
			),
		);

		#pragma endregion

		#pragma region animation

		template <typename = None>
		struct Animation_;

		using Animation = Animation_<>;

		template <typename _> requires (check_version(version, {1, 4}))
		M_record_of_map(
			M_wrap(Animation_<_>),
			M_wrap(
				(Integer) frame_rate,
				(Position2<Floater>) position,
				(Size2<Floater>) size,
				(List<Image>) image,
				(List<Sprite>) sprite,
				(Sprite) main_sprite,
			),
		);

		template <typename _> requires (check_version(version, {4}))
		M_record_of_map(
			M_wrap(Animation_<_>),
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
