#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/character_font_widget_2/version.hpp"
#include "core/tool/popcap/character_font_widget_2/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::CharacterFontWidget2 {

	template <auto version> requires (check_version(version))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

	};

	template <auto version> requires (check_version(version))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		// ----------------

		static auto exchange_unit_character_16 (
			OByteStreamView & data,
			Unicode const &   value
		) -> Void {
			data.write(cbw<Character16>(value));
			return;
		}

		static auto exchange_unit_boolean_8 (
			OByteStreamView & data,
			Boolean const &   value
		) -> Void {
			data.write(cbw<Boolean8>(value));
			return;
		}

		static auto exchange_unit_integer_32 (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerS32>(value));
			return;
		}

		static auto exchange_unit_integer_16 (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerS16>(value));
			return;
		}

		static auto exchange_unit_floating_64 (
			OByteStreamView & data,
			Floating const &  value
		) -> Void {
			data.write(cbw<FloatingS64>(value));
			return;
		}

		static auto exchange_unit_string_32 (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			data.write(self_cast<StringBlock32>(value));
			return;
		}

		static auto exchange_unit_hash_128 (
			OByteStreamView & data,
			ByteList const &  value
		) -> Void {
			assert_test(value.size() == 16_sz);
			data.write(value);
			return;
		}

		template <typename Element, typename Parser> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<Parser>>
			&& (IsGenericCallable<Parser>)
		static auto exchange_unit_list_32 (
			OByteStreamView &     data,
			List<Element> const & value,
			Parser const &        parser
		) -> Void {
			data.write(cbw<IntegerU32>(value.size()));
			for (auto & element : value) {
				parser(data, element);
			}
			return;
		}

		// ----------------

		static auto process_character_item (
			OByteStreamView &                        character_item_data,
			typename Manifest::CharacterItem const & character_item_manifest
		) -> Void {
			exchange_unit_character_16(character_item_data, character_item_manifest.index);
			exchange_unit_character_16(character_item_data, character_item_manifest.value);
			return;
		}

		static auto process_font_kerning (
			OByteStreamView &                      font_kerning_data,
			typename Manifest::FontKerning const & font_kerning_manifest
		) -> Void {
			exchange_unit_integer_16(font_kerning_data, font_kerning_manifest.offset);
			exchange_unit_character_16(font_kerning_data, font_kerning_manifest.index);
			return;
		}

		static auto process_font_character (
			OByteStreamView &                        font_character_data,
			typename Manifest::FontCharacter const & font_character_manifest
		) -> Void {
			exchange_unit_character_16(font_character_data, font_character_manifest.index);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_x);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_y);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_width);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_height);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_offset_x);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_offset_y);
			exchange_unit_integer_16(font_character_data, font_character_manifest.kerning_count);
			exchange_unit_integer_16(font_character_data, font_character_manifest.kerning_first);
			exchange_unit_integer_32(font_character_data, font_character_manifest.width);
			exchange_unit_integer_32(font_character_data, font_character_manifest.order);
			return;
		}

		static auto process_font_layer (
			OByteStreamView &                    font_layer_data,
			typename Manifest::FontLayer const & font_layer_manifest
		) -> Void {
			exchange_unit_string_32(font_layer_data, font_layer_manifest.name);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.tag_require,
				[] (auto & data, auto & manifest) {
					exchange_unit_string_32(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.tag_exclude,
				[] (auto & data, auto & manifest) {
					exchange_unit_string_32(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.kerning,
				[] (auto & data, auto & manifest) {
					process_font_kerning(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.character,
				[] (auto & data, auto & manifest) {
					process_font_character(data, manifest);
				}
			);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_red);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_green);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_blue);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_alpha);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_red);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_green);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_blue);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_alpha);
			exchange_unit_string_32(font_layer_data, font_layer_manifest.image_file);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.draw_mode);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.offset_x);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.offset_y);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.spacing);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.minimum_point_size);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.maximum_point_size);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.point_size);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.ascent);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.ascent_padding);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.height);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.default_height);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.line_spacing_offset);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.base_order);
			return;
		}

		static auto process_font_widget (
			OByteStreamView &                     font_widget_data,
			typename Manifest::FontWidget const & font_widget_manifest
		) -> Void {
			exchange_unit_hash_128(font_widget_data, font_widget_manifest.source_hash);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.ascent);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.ascent_padding);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.height);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.line_sepacing_offset);
			exchange_unit_boolean_8(font_widget_data, font_widget_manifest.initialized);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.default_point_size);
			exchange_unit_list_32(
				font_widget_data,
				font_widget_manifest.character,
				[] (auto & data, auto & manifest) {
					process_character_item(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_widget_data,
				font_widget_manifest.layer,
				[] (auto & data, auto & manifest) {
					process_font_layer(data, manifest);
				}
			);
			exchange_unit_string_32(font_widget_data, font_widget_manifest.source_file);
			exchange_unit_string_32(font_widget_data, font_widget_manifest.error_header);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.point_size);
			exchange_unit_list_32(
				font_widget_data,
				font_widget_manifest.tag,
				[] (auto & data, auto & manifest) {
					exchange_unit_string_32(data, manifest);
				}
			);
			exchange_unit_floating_64(font_widget_data, font_widget_manifest.scale);
			exchange_unit_boolean_8(font_widget_data, font_widget_manifest.force_scaled_image_white);
			exchange_unit_boolean_8(font_widget_data, font_widget_manifest.activate_all_layer);
			return;
		}

	public:

		static auto do_process_font_widget (
			OByteStreamView &                     font_widget_data_,
			typename Manifest::FontWidget const & font_widget_manifest
		) -> Void {
			M_use_zps_of(font_widget_data);
			return process_font_widget(font_widget_data, font_widget_manifest);
		}

	};

	template <auto version> requires (check_version(version))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		// ----------------

		static auto exchange_unit_character_16 (
			IByteStreamView & data,
			Unicode &         value
		) -> Void {
			value = cbw<Unicode>(data.read_of<Character16>());
			return;
		}

		static auto exchange_unit_boolean_8 (
			IByteStreamView & data,
			Boolean &         value
		) -> Void {
			value = cbw<Boolean>(data.read_of<Boolean8>());
			return;
		}

		static auto exchange_unit_integer_32 (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerS32>());
			return;
		}

		static auto exchange_unit_integer_16 (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerS16>());
			return;
		}

		static auto exchange_unit_floating_64 (
			IByteStreamView & data,
			Floating &        value
		) -> Void {
			value = cbw<Floating>(data.read_of<FloatingS64>());
			return;
		}

		static auto exchange_unit_string_32 (
			IByteStreamView & data,
			String &          value
		) -> Void {
			data.read(self_cast<StringBlock32>(value));
			return;
		}

		static auto exchange_unit_hash_128 (
			IByteStreamView & data,
			ByteList &        value
		) -> Void {
			data.read(value, 16_sz);
			return;
		}

		template <typename Element, typename Parser> requires
			CategoryConstraint<IsPureInstance<Element> && IsPureInstance<Parser>>
			&& (IsGenericCallable<Parser>)
		static auto exchange_unit_list_32 (
			IByteStreamView & data,
			List<Element> &   value,
			Parser const &    parser
		) -> Void {
			value.allocate_full(cbw<Size>(data.read_of<IntegerU32>()));
			for (auto & element : value) {
				parser(data, element);
			}
			return;
		}

		// ----------------

		static auto process_character_item (
			IByteStreamView &                  character_item_data,
			typename Manifest::CharacterItem & character_item_manifest
		) -> Void {
			exchange_unit_character_16(character_item_data, character_item_manifest.index);
			exchange_unit_character_16(character_item_data, character_item_manifest.value);
			return;
		}

		static auto process_font_kerning (
			IByteStreamView &                font_kerning_data,
			typename Manifest::FontKerning & font_kerning_manifest
		) -> Void {
			exchange_unit_integer_16(font_kerning_data, font_kerning_manifest.offset);
			exchange_unit_character_16(font_kerning_data, font_kerning_manifest.index);
			return;
		}

		static auto process_font_character (
			IByteStreamView &                  font_character_data,
			typename Manifest::FontCharacter & font_character_manifest
		) -> Void {
			exchange_unit_character_16(font_character_data, font_character_manifest.index);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_x);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_y);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_width);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_rect_height);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_offset_x);
			exchange_unit_integer_32(font_character_data, font_character_manifest.image_offset_y);
			exchange_unit_integer_16(font_character_data, font_character_manifest.kerning_count);
			exchange_unit_integer_16(font_character_data, font_character_manifest.kerning_first);
			exchange_unit_integer_32(font_character_data, font_character_manifest.width);
			exchange_unit_integer_32(font_character_data, font_character_manifest.order);
			return;
		}

		static auto process_font_layer (
			IByteStreamView &              font_layer_data,
			typename Manifest::FontLayer & font_layer_manifest
		) -> Void {
			exchange_unit_string_32(font_layer_data, font_layer_manifest.name);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.tag_require,
				[] (auto & data, auto & manifest) {
					exchange_unit_string_32(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.tag_exclude,
				[] (auto & data, auto & manifest) {
					exchange_unit_string_32(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.kerning,
				[] (auto & data, auto & manifest) {
					process_font_kerning(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_layer_data,
				font_layer_manifest.character,
				[] (auto & data, auto & manifest) {
					process_font_character(data, manifest);
				}
			);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_red);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_green);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_blue);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.multiply_alpha);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_red);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_green);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_blue);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.add_alpha);
			exchange_unit_string_32(font_layer_data, font_layer_manifest.image_file);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.draw_mode);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.offset_x);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.offset_y);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.spacing);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.minimum_point_size);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.maximum_point_size);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.point_size);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.ascent);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.ascent_padding);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.height);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.default_height);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.line_spacing_offset);
			exchange_unit_integer_32(font_layer_data, font_layer_manifest.base_order);
			return;
		}

		static auto process_font_widget (
			IByteStreamView &               font_widget_data,
			typename Manifest::FontWidget & font_widget_manifest
		) -> Void {
			exchange_unit_hash_128(font_widget_data, font_widget_manifest.source_hash);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.ascent);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.ascent_padding);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.height);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.line_sepacing_offset);
			exchange_unit_boolean_8(font_widget_data, font_widget_manifest.initialized);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.default_point_size);
			exchange_unit_list_32(
				font_widget_data,
				font_widget_manifest.character,
				[] (auto & data, auto & manifest) {
					process_character_item(data, manifest);
				}
			);
			exchange_unit_list_32(
				font_widget_data,
				font_widget_manifest.layer,
				[] (auto & data, auto & manifest) {
					process_font_layer(data, manifest);
				}
			);
			exchange_unit_string_32(font_widget_data, font_widget_manifest.source_file);
			exchange_unit_string_32(font_widget_data, font_widget_manifest.error_header);
			exchange_unit_integer_32(font_widget_data, font_widget_manifest.point_size);
			exchange_unit_list_32(
				font_widget_data,
				font_widget_manifest.tag,
				[] (auto & data, auto & manifest) {
					exchange_unit_string_32(data, manifest);
				}
			);
			exchange_unit_floating_64(font_widget_data, font_widget_manifest.scale);
			exchange_unit_boolean_8(font_widget_data, font_widget_manifest.force_scaled_image_white);
			exchange_unit_boolean_8(font_widget_data, font_widget_manifest.activate_all_layer);
			return;
		}

	public:

		static auto do_process_font_widget (
			IByteStreamView &               font_widget_data_,
			typename Manifest::FontWidget & font_widget_manifest
		) -> Void {
			M_use_zps_of(font_widget_data);
			restruct(font_widget_manifest);
			return process_font_widget(font_widget_data, font_widget_manifest);
		}

	};

}
