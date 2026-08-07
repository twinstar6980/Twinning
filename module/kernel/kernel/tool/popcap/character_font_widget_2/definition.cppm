module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.character_font_widget_2.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.character_font_widget_2.version;

export namespace Twinning::Kernel::Tool::Popcap::CharacterFontWidget2 {

	template <auto t_version> requires (check_version(t_version))
	struct Definition {

		#pragma region character item

		M_nested_template_declaration(CharacterItem);

		M_nested_template_definition_check(check_version(t_version))
		M_record_of_map(
			M_nested_template_definition_name(CharacterItem),
			M_wrap(
				(Unicode) index,
				(Unicode) value,
			),
		);

		#pragma endregion

		#pragma region font kerning

		M_nested_template_declaration(FontKerning);

		M_nested_template_definition_check(check_version(t_version))
		M_record_of_map(
			M_nested_template_definition_name(FontKerning),
			M_wrap(
				(Unicode) index,
				(Integer) offset,
			),
		);

		#pragma endregion

		#pragma region font character

		M_nested_template_declaration(FontCharacter);

		M_nested_template_definition_check(check_version(t_version))
		M_record_of_map(
			M_nested_template_definition_name(FontCharacter),
			M_wrap(
				(Unicode) index,
				(Integer) image_rect_x,
				(Integer) image_rect_y,
				(Integer) image_rect_width,
				(Integer) image_rect_height,
				(Integer) image_offset_x,
				(Integer) image_offset_y,
				(Integer) kerning_first,
				(Integer) kerning_count,
				(Integer) width,
				(Integer) order,
			),
		);

		#pragma endregion

		#pragma region font layer

		M_nested_template_declaration(FontLayer);

		M_nested_template_definition_check(check_version(t_version))
		M_record_of_map(
			M_nested_template_definition_name(FontLayer),
			M_wrap(
				(String) name,
				(List<String>) tag_require,
				(List<String>) tag_exclude,
				(List<FontKerning>) kerning,
				(List<FontCharacter>) character,
				(Integer) multiply_red,
				(Integer) multiply_green,
				(Integer) multiply_blue,
				(Integer) multiply_alpha,
				(Integer) add_red,
				(Integer) add_green,
				(Integer) add_blue,
				(Integer) add_alpha,
				(String) image_file,
				(Integer) draw_mode,
				(Integer) offset_x,
				(Integer) offset_y,
				(Integer) spacing,
				(Integer) minimum_point_size,
				(Integer) maximum_point_size,
				(Integer) point_size,
				(Integer) ascent,
				(Integer) ascent_padding,
				(Integer) height,
				(Integer) default_height,
				(Integer) line_spacing_offset,
				(Integer) base_order,
			),
		);

		#pragma endregion

		#pragma region font widget

		M_nested_template_declaration(FontWidget);

		M_nested_template_definition_check(check_version(t_version))
		M_record_of_map(
			M_nested_template_definition_name(FontWidget),
			M_wrap(
				(Integer) ascent,
				(Integer) ascent_padding,
				(Integer) height,
				(Integer) line_sepacing_offset,
				(Boolean) initialized,
				(Integer) default_point_size,
				(List<CharacterItem>) character,
				(List<FontLayer>) layer,
				(String) source_file,
				(String) error_header,
				(Integer) point_size,
				(List<String>) tag,
				(Floater) scale,
				(Boolean) force_scaled_image_white,
				(Boolean) activate_all_layer,
			),
		);

		#pragma endregion

	};

}
