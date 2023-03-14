#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/animation/version.hpp"

namespace TwinStar::Core::Tool::PopCap::Animation {

	template <auto version> requires (check_version(version, {}))
	struct Manifest {

		#pragma region transform

		template <typename = None>
		struct TranslateTransform_;

		using TranslateTransform = TranslateTransform_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
		M_record_of_list(
			M_wrap(TranslateTransform_<_>),
			M_wrap(
				(Floating) x,
				(Floating) y,
			),
		);

		// ----------------

		template <typename = None>
		struct RotateTranslateTransform_;

		using RotateTranslateTransform = RotateTranslateTransform_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
		M_record_of_list(
			M_wrap(RotateTranslateTransform_<_>),
			M_wrap(
				(Floating) angle,
				(Floating) x,
				(Floating) y,
			),
		);

		// ----------------

		template <typename = None>
		struct MatrixTranslateTransform_;

		using MatrixTranslateTransform = MatrixTranslateTransform_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
		M_record_of_list(
			M_wrap(MatrixTranslateTransform_<_>),
			M_wrap(
				(Floating) a,
				(Floating) b,
				(Floating) c,
				(Floating) d,
				(Floating) x,
				(Floating) y,
			),
		);

		// ----------------

		using VariantTransform = decltype([] {
			if constexpr (check_version(version, {1, 2, 3, 4, 5, 6})) {
				using Type = ListRecordVariant<TranslateTransform, RotateTranslateTransform, MatrixTranslateTransform>;
				return declare<Type>();
			}
		}());

		#pragma endregion

		#pragma region color

		template <typename = None>
		struct Color_;

		using Color = Color_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
		M_record_of_list(
			M_wrap(Color_<_>),
			M_wrap(
				(Floating) red,
				(Floating) green,
				(Floating) blue,
				(Floating) alpha,
			),
		);

		#pragma endregion

		#pragma region rectangle

		template <typename = None>
		struct Rectangle_;

		using Rectangle = Rectangle_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
		M_record_of_map(
			M_wrap(Rectangle_<_>),
			M_wrap(
				(Position2D<Floating>) position,
				(Size2D<Floating>) size,
			),
		);

		#pragma endregion

		#pragma region frame

		template <typename = None>
		struct Command_;

		using Command = Command_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
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

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
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

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
		M_record_of_map(
			M_wrap(LayerAppend_<_>),
			M_wrap(
				(Integer) index,
				(Optional<String>) name,
				(Integer) resource,
				(Boolean) sprite,
				(Boolean) additive,
				(Integer) preload_frame,
				(Floating) time_scale,
			),
		);

		// ----------------

		template <typename = None>
		struct LayerChange_;

		using LayerChange = LayerChange_<>;

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
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

		template <typename _> requires (check_version(version, {1, 2, 3, 4, 5, 6}))
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

		template <typename _> requires (check_version(version, {5, 6}))
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

		template <typename _> requires (check_version(version, {1, 2, 3}))
		M_record_of_map(
			M_wrap(Sprite_<_>),
			M_wrap(
				(List<Frame>) frame,
			),
		);

		template <typename _> requires (check_version(version, {4}))
		M_record_of_map(
			M_wrap(Sprite_<_>),
			M_wrap(
				(String) name,
				(Floating) frame_rate,
				(List<Frame>) frame,
			),
		);

		template <typename _> requires (check_version(version, {5, 6}))
		M_record_of_map(
			M_wrap(Sprite_<_>),
			M_wrap(
				(String) name,
				(Floating) frame_rate,
				(WorkArea) work_area,
				(List<Frame>) frame,
			),
		);

		#pragma endregion

		#pragma region image

		template <typename = None>
		struct Image_;

		using Image = Image_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(Image_<_>),
			M_wrap(
				(String) name,
				(RotateTranslateTransform) transform,
			),
		);

		template <typename _> requires (check_version(version, {2, 3}))
		M_record_of_map(
			M_wrap(Image_<_>),
			M_wrap(
				(String) name,
				(MatrixTranslateTransform) transform,
			),
		);

		template <typename _> requires (check_version(version, {4, 5, 6}))
		M_record_of_map(
			M_wrap(Image_<_>),
			M_wrap(
				(String) name,
				(Size2D<Integer>) size,
				(MatrixTranslateTransform) transform,
			),
		);

		#pragma endregion

		#pragma region animation

		template <typename = None>
		struct Animation_;

		using Animation = Animation_<>;

		template <typename _> requires (check_version(version, {1, 2, 3}))
		M_record_of_map(
			M_wrap(Animation_<_>),
			M_wrap(
				(Integer) frame_rate,
				(Position2D<Floating>) position,
				(Size2D<Floating>) size,
				(List<Image>) image,
				(List<Sprite>) sprite,
				(Sprite) main_sprite,
			),
		);

		template <typename _> requires (check_version(version, {4, 5, 6}))
		M_record_of_map(
			M_wrap(Animation_<_>),
			M_wrap(
				(Integer) frame_rate,
				(Position2D<Floating>) position,
				(Size2D<Floating>) size,
				(List<Image>) image,
				(List<Sprite>) sprite,
				(Optional<Sprite>) main_sprite,
			),
		);

		#pragma endregion

	};

}
