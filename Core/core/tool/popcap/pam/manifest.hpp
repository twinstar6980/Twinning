#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/pam/version.hpp"

namespace TwinKleS::Core::Tool::PopCap::PAM::Manifest {

	template <auto t_version>
	struct TranslateTransform;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct TranslateTransform<t_version> {
		Floating x;
		Floating y;
	};

	// ----------------

	template <auto t_version>
	struct RotateTranslateTransform;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct RotateTranslateTransform<t_version> {
		Floating angle;
		Floating x;
		Floating y;
	};

	// ----------------

	template <auto t_version>
	struct MatrixTranslateTransform;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct MatrixTranslateTransform<t_version> {
		Floating a;
		Floating b;
		Floating c;
		Floating d;
		Floating x;
		Floating y;
	};

	// ----------------

	template <auto t_version>
	class VariantTransform;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	class VariantTransform<t_version> :
		public Variant<TranslateTransform<t_version>, RotateTranslateTransform<t_version>, MatrixTranslateTransform<t_version>> {
	};

	// ----------------

	template <auto t_version>
	struct Color;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct Color<t_version> {
		Floating red;
		Floating green;
		Floating blue;
		Floating alpha;
	};

	// ----------------

	template <auto t_version>
	struct WorkArea;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {5, 6}))
	struct WorkArea<t_version> {
		Integer start;
		Integer duration;
	};

	// ----------------

	template <auto t_version>
	struct Command;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct Command<t_version> {
		String command;
		String argument;
	};

	// ----------------

	template <auto t_version>
	struct Rectangle;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct Rectangle<t_version> {
		Position2D<Floating> position;
		Size2D<Floating>     size;
	};

	// ----------------

	template <auto t_version>
	struct LayerRemove;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct LayerRemove<t_version> {
		Integer index;
	};

	// ----------------

	template <auto t_version>
	struct LayerAppend;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct LayerAppend<t_version> {
		Integer          index;
		Optional<String> name;
		Integer          resource;
		Boolean          sprite;
		Boolean          additive;
		Integer          preload_frame;
		Floating         time_scale;
	};

	// ----------------

	template <auto t_version>
	struct LayerChange;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct LayerChange<t_version> {
		Integer                        index;
		VariantTransform<t_version>    transform;
		Optional<Color<t_version>>     color;
		Optional<Integer>              sprite_frame_number;
		Optional<Rectangle<t_version>> source_rectangle;
	};

	// ----------------

	template <auto t_version>
	struct Frame;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3, 4, 5, 6}))
	struct Frame<t_version> {
		Optional<String>             label;
		Boolean                      stop;
		List<Command<t_version>>     command;
		List<LayerRemove<t_version>> remove;
		List<LayerAppend<t_version>> append;
		List<LayerChange<t_version>> change;
	};

	// ----------------

	template <auto t_version>
	struct Sprite;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3}))
	struct Sprite<t_version> {
		List<Frame<t_version>> frame;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4}))
	struct Sprite<t_version> {
		String                 name;
		Floating               frame_rate;
		List<Frame<t_version>> frame;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {5, 6}))
	struct Sprite<t_version> {
		String                 name;
		Floating               frame_rate;
		WorkArea<t_version>    work_area;
		List<Frame<t_version>> frame;
	};

	// ----------------

	template <auto t_version>
	struct Image;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1}))
	struct Image<t_version> {
		String                              name;
		RotateTranslateTransform<t_version> transform;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {2, 3}))
	struct Image<t_version> {
		String                              name;
		MatrixTranslateTransform<t_version> transform;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4, 5, 6}))
	struct Image<t_version> {
		String                              name;
		Size2D<Integer>                     size;
		MatrixTranslateTransform<t_version> transform;
	};

	// ----------------

	template <auto t_version>
	struct Animation;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {1, 2, 3}))
	struct Animation<t_version> {
		Integer                 frame_rate;
		Position2D<Floating>    position;
		Size2D<Floating>        size;
		List<Image<t_version>>  image;
		List<Sprite<t_version>> sprite;
		Sprite<t_version>       main_sprite;
	};

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {4, 5, 6}))
	struct Animation<t_version> {
		Integer                     frame_rate;
		Position2D<Floating>        position;
		Size2D<Floating>            size;
		List<Image<t_version>>      image;
		List<Sprite<t_version>>     sprite;
		Optional<Sprite<t_version>> main_sprite;
	};

	// ----------------

	using AnimationVariant = VariantOfVersion<VersionEnum, Animation>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_array_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::TranslateTransform<t_version>),
		M_wrap(x, y),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_array_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::RotateTranslateTransform<t_version>),
		M_wrap(angle, x, y),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_array_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::MatrixTranslateTransform<t_version>),
		M_wrap(a, b, c, d, x, y),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
	struct ValueAdapter<Tool::PopCap::PAM::Manifest::VariantTransform<t_version>> {

		using This = Value;
		using That = Tool::PopCap::PAM::Manifest::VariantTransform<t_version>;

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			using namespace Tool::PopCap::PAM::Manifest;
			thix.set_array();
			auto & thix_array = thix.get_array();
			if (that.template is<TranslateTransform<t_version>>()) {
				auto & that_value = that.template get<TranslateTransform<t_version>>();
				thix_array.allocate(2_sz);
				thix_array(1_ix).from(that_value.x);
				thix_array(2_ix).from(that_value.y);
			} else if (that.template is<RotateTranslateTransform<t_version>>()) {
				auto & that_value = that.template get<RotateTranslateTransform<t_version>>();
				thix_array.allocate(3_sz);
				thix_array(1_ix).from(that_value.angle);
				thix_array(2_ix).from(that_value.x);
				thix_array(3_ix).from(that_value.y);
			} else if (that.template is<MatrixTranslateTransform<t_version>>()) {
				auto & that_value = that.template get<MatrixTranslateTransform<t_version>>();
				thix_array.allocate(6_sz);
				thix_array(1_ix).from(that_value.a);
				thix_array(2_ix).from(that_value.b);
				thix_array(3_ix).from(that_value.c);
				thix_array(4_ix).from(that_value.d);
				thix_array(5_ix).from(that_value.x);
				thix_array(6_ix).from(that_value.y);
			} else {
				throw NeverException{};
			}
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			using namespace Tool::PopCap::PAM::Manifest;
			auto & thix_array = thix.get_array();
			if (thix_array.size() == 2_sz) {
				that.template set<TranslateTransform<t_version>>();
				auto & that_value = that.template get<TranslateTransform<t_version>>();
				thix_array[1_ix].to(that_value.x);
				thix_array[2_ix].to(that_value.y);
			} else if (thix_array.size() == 3_sz) {
				that.template set<RotateTranslateTransform<t_version>>();
				auto & that_value = that.template get<RotateTranslateTransform<t_version>>();
				thix_array[1_ix].to(that_value.angle);
				thix_array[2_ix].to(that_value.x);
				thix_array[3_ix].to(that_value.y);
			} else if (thix_array.size() == 6_sz) {
				that.template set<MatrixTranslateTransform<t_version>>();
				auto & that_value = that.template get<MatrixTranslateTransform<t_version>>();
				thix_array[1_ix].to(that_value.a);
				thix_array[2_ix].to(that_value.b);
				thix_array[3_ix].to(that_value.c);
				thix_array[4_ix].to(that_value.d);
				thix_array[5_ix].to(that_value.x);
				thix_array[6_ix].to(that_value.y);
			} else {
				assert_failed(R"(thix_array.size() == /* valid */)");
			}
			return;
		}

	};

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_array_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Color<t_version>),
		M_wrap(red, green, blue, alpha),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_array_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Command<t_version>),
		M_wrap(command, argument),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Rectangle<t_version>),
		M_wrap(position, size),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::LayerRemove<t_version>),
		M_wrap(index),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::LayerAppend<t_version>),
		M_wrap(index, name, resource, sprite, additive, preload_frame, time_scale),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::LayerChange<t_version>),
		M_wrap(index, transform, color, sprite_frame_number, source_rectangle),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3, 4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Frame<t_version>),
		M_wrap(label, stop, command, remove, append, change),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {5, 6}))
	M_json_value_adapter_for_aggregate_as_array_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::WorkArea<t_version>),
		M_wrap(start, duration),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Sprite<t_version>),
		M_wrap(frame),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {4}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Sprite<t_version>),
		M_wrap(name, frame_rate, frame),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Sprite<t_version>),
		M_wrap(name, frame_rate, work_area, frame),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Image<t_version>),
		M_wrap(name, transform),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {2, 3}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Image<t_version>),
		M_wrap(name, transform),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Image<t_version>),
		M_wrap(name, size, transform),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {1, 2, 3}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Animation<t_version>),
		M_wrap(frame_rate, position, size, image, sprite, main_sprite),
	);

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::PopCap::PAM::check_version(t_version, {4, 5, 6}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::PopCap::PAM::Manifest::Animation<t_version>),
		M_wrap(frame_rate, position, size, image, sprite, main_sprite),
	);

}
