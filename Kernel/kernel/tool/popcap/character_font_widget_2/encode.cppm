module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.character_font_widget_2.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.character_font_widget_2.version;
import twinning.kernel.tool.popcap.character_font_widget_2.definition;
import twinning.kernel.tool.popcap.character_font_widget_2.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::CharacterFontWidget2 {

	template <auto version> requires (check_version(version))
	struct Encode :
		Common<version>,
		CommonByteStreamExchangeForOutput {

		using Common = Common<version>;

		using typename Common::Definition;

		// ----------------

		inline static auto exchange_character_item (
			OByteStreamView &                          data,
			typename Definition::CharacterItem const & value
		) -> Void {
			exchange_unicode_fixed<Character16>(data, value.index);
			exchange_unicode_fixed<Character16>(data, value.value);
			return;
		}

		inline static auto exchange_font_kerning (
			OByteStreamView &                        data,
			typename Definition::FontKerning const & value
		) -> Void {
			exchange_integer_fixed<IntegerU16>(data, value.offset);
			exchange_unicode_fixed<Character16>(data, value.index);
			return;
		}

		inline static auto exchange_font_character (
			OByteStreamView &                          data,
			typename Definition::FontCharacter const & value
		) -> Void {
			exchange_unicode_fixed<Character16>(data, value.index);
			exchange_integer_fixed<IntegerS32>(data, value.image_rect_x);
			exchange_integer_fixed<IntegerS32>(data, value.image_rect_y);
			exchange_integer_fixed<IntegerS32>(data, value.image_rect_width);
			exchange_integer_fixed<IntegerS32>(data, value.image_rect_height);
			exchange_integer_fixed<IntegerS32>(data, value.image_offset_x);
			exchange_integer_fixed<IntegerS32>(data, value.image_offset_y);
			exchange_integer_fixed<IntegerU16>(data, value.kerning_count);
			exchange_integer_fixed<IntegerU16>(data, value.kerning_first);
			exchange_integer_fixed<IntegerS32>(data, value.width);
			exchange_integer_fixed<IntegerS32>(data, value.order);
			return;
		}

		inline static auto exchange_font_layer (
			OByteStreamView &                      data,
			typename Definition::FontLayer const & value
		) -> Void {
			exchange_string_block<IntegerU32>(data, value.name);
			exchange_list(data, value.tag_require, &exchange_size_fixed<IntegerU32>, &exchange_string_block<IntegerU32>);
			exchange_list(data, value.tag_exclude, &exchange_size_fixed<IntegerU32>, &exchange_string_block<IntegerU32>);
			exchange_list(data, value.kerning, &exchange_size_fixed<IntegerU32>, &exchange_font_kerning);
			exchange_list(data, value.character, &exchange_size_fixed<IntegerU32>, &exchange_font_character);
			exchange_integer_fixed<IntegerS32>(data, value.multiply_red);
			exchange_integer_fixed<IntegerS32>(data, value.multiply_green);
			exchange_integer_fixed<IntegerS32>(data, value.multiply_blue);
			exchange_integer_fixed<IntegerS32>(data, value.multiply_alpha);
			exchange_integer_fixed<IntegerS32>(data, value.add_red);
			exchange_integer_fixed<IntegerS32>(data, value.add_green);
			exchange_integer_fixed<IntegerS32>(data, value.add_blue);
			exchange_integer_fixed<IntegerS32>(data, value.add_alpha);
			exchange_string_block<IntegerU32>(data, value.image_file);
			exchange_integer_fixed<IntegerS32>(data, value.draw_mode);
			exchange_integer_fixed<IntegerS32>(data, value.offset_x);
			exchange_integer_fixed<IntegerS32>(data, value.offset_y);
			exchange_integer_fixed<IntegerS32>(data, value.spacing);
			exchange_integer_fixed<IntegerS32>(data, value.minimum_point_size);
			exchange_integer_fixed<IntegerS32>(data, value.maximum_point_size);
			exchange_integer_fixed<IntegerS32>(data, value.point_size);
			exchange_integer_fixed<IntegerS32>(data, value.ascent);
			exchange_integer_fixed<IntegerS32>(data, value.ascent_padding);
			exchange_integer_fixed<IntegerS32>(data, value.height);
			exchange_integer_fixed<IntegerS32>(data, value.default_height);
			exchange_integer_fixed<IntegerS32>(data, value.line_spacing_offset);
			exchange_integer_fixed<IntegerS32>(data, value.base_order);
			return;
		}

		inline static auto exchange_font_widget (
			OByteStreamView &                       data,
			typename Definition::FontWidget const & value
		) -> Void {
			exchange_integer_fixed<IntegerS32>(data, value.ascent);
			exchange_integer_fixed<IntegerS32>(data, value.ascent_padding);
			exchange_integer_fixed<IntegerS32>(data, value.height);
			exchange_integer_fixed<IntegerS32>(data, value.line_sepacing_offset);
			exchange_boolean_fixed<Boolean8>(data, value.initialized);
			exchange_integer_fixed<IntegerS32>(data, value.default_point_size);
			exchange_list(data, value.character, &exchange_size_fixed<IntegerU32>, &exchange_character_item);
			exchange_list(data, value.layer, &exchange_size_fixed<IntegerU32>, &exchange_font_layer);
			exchange_string_block<IntegerU32>(data, value.source_file);
			exchange_string_block<IntegerU32>(data, value.error_header);
			exchange_integer_fixed<IntegerS32>(data, value.point_size);
			exchange_list(data, value.tag, &exchange_size_fixed<IntegerU32>, &exchange_string_block<IntegerU32>);
			exchange_floater_fixed<FloaterS64>(data, value.scale);
			exchange_boolean_fixed<Boolean8>(data, value.force_scaled_image_white);
			exchange_boolean_fixed<Boolean8>(data, value.activate_all_layer);
			return;
		}

		// ----------------

		inline static auto process_whole (
			OByteStreamView &                       data,
			typename Definition::FontWidget const & definition
		) -> Void {
			exchange_font_widget(data, definition);
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &                       data_,
			typename Definition::FontWidget const & definition
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition);
		}

	};

}
