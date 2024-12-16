module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.character_font_widget_2.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.character_font_widget_2.version;

export namespace Twinning::Kernel::Tool::PopCap::CharacterFontWidget2 {

	template <auto version> requires (check_version(version))
	struct Definition {

		#pragma region character item

		template <typename = None>
		struct CharacterItem_;

		using CharacterItem = CharacterItem_<>;

		template <typename _> requires (check_version(version))
		M_record_of_map(
			M_wrap(CharacterItem_<_>),
			M_wrap(
				(Unicode) index,
				(Unicode) value,
			),
		);

		#pragma endregion

		#pragma region font kerning

		template <typename = None>
		struct FontKerning_;

		using FontKerning = FontKerning_<>;

		template <typename _> requires (check_version(version))
		M_record_of_map(
			M_wrap(FontKerning_<_>),
			M_wrap(
				(Unicode) index,
				(Integer) offset,
			),
		);

		#pragma endregion

		#pragma region font character

		template <typename = None>
		struct FontCharacter_;

		using FontCharacter = FontCharacter_<>;

		template <typename _> requires (check_version(version))
		M_record_of_map(
			M_wrap(FontCharacter_<_>),
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

		template <typename = None>
		struct FontLayer_;

		using FontLayer = FontLayer_<>;

		template <typename _> requires (check_version(version))
		M_record_of_map(
			M_wrap(FontLayer_<_>),
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

		template <typename = None>
		struct FontWidget_;

		using FontWidget = FontWidget_<>;

		template <typename _> requires (check_version(version))
		M_record_of_map(
			M_wrap(FontWidget_<_>),
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
