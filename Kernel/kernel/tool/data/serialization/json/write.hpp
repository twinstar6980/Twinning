#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/serialization/json/common.hpp"
#include <list>

namespace TwinStar::Kernel::Tool::Data::Serialization::JSON {

	struct Write :
		Common {

		using Common = Common;

		// ----------------

		static auto process_value (
			OCharacterStreamView & data,
			Value const &          value,
			Boolean const &        disable_array_trailing_comma,
			Boolean const &        disable_array_line_breaking,
			Boolean const &        disable_object_trailing_comma,
			Boolean const &        disable_object_line_breaking,
			Size const &           indent_level
		) -> Void {
			switch (value.type().value) {
				case ValueType::Constant::null().value : {
					StringParser::write_null(data, value.get_null());
					break;
				}
				case ValueType::Constant::boolean().value : {
					StringParser::write_boolean(data, value.get_boolean());
					break;
				}
				case ValueType::Constant::number().value : {
					StringParser::write_number(data, value.get_number(), k_true);
					break;
				}
				case ValueType::Constant::string().value : {
					data.write('"'_c);
					StringParser::write_escape_utf8_string_until(data, as_lvalue(ICharacterStreamView{value.get_string()}), '"'_c);
					data.write('"'_c);
					break;
				}
				case ValueType::Constant::array().value : {
					auto & array = value.get_array();
					data.write('['_c);
					auto write_line =
						[&] (
						Boolean const & is_inner
					) -> auto {
						if (!disable_array_line_breaking) {
							data.write('\n'_c);
							StringParser::write_character_repeat(data, '\t'_c, indent_level + (is_inner ? (1_sz) : (0_sz)));
						}
						else {
							data.write(' '_c);
						}
						return;
					};
					if (!array.empty()) {
						auto write_element =
							[&] (
							Array::Element const & element
						) -> auto {
							write_line(k_true);
							process_value(data, element, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, indent_level + (!disable_array_line_breaking ? (1_sz) : (0_sz)));
							return;
						};
						for (auto & element : array.head(array.size() - 1_sz)) {
							write_element(element);
							data.write(','_c);
						}
						{
							write_element(array.last());
							if (!disable_array_trailing_comma) {
								data.write(','_c);
							}
						}
					}
					write_line(k_false);
					data.write(']'_c);
					break;
				}
				case ValueType::Constant::object().value : {
					auto & object = value.get_object();
					data.write('{'_c);
					auto write_line =
						[&] (
						Boolean const & is_inner
					) -> auto {
						if (!disable_object_line_breaking) {
							data.write('\n'_c);
							StringParser::write_character_repeat(data, '\t'_c, indent_level + (is_inner ? (1_sz) : (0_sz)));
						}
						else {
							data.write(' '_c);
						}
						return;
					};
					if (!object.empty()) {
						auto write_member =
							[&] (
							Object::Element const & member
						) -> auto {
							write_line(k_true);
							data.write('"'_c);
							StringParser::write_escape_utf8_string_until(data, as_lvalue(ICharacterStreamView{member.key}), '"'_c);
							data.write('"'_c);
							data.write(':'_c);
							data.write(' '_c);
							process_value(data, member.value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, indent_level + (!disable_object_line_breaking ? (1_sz) : (0_sz)));
							return;
						};
						for (auto & element : object.head(object.size() - 1_sz)) {
							write_member(element);
							data.write(','_c);
						}
						{
							write_member(object.last());
							if (!disable_object_trailing_comma) {
								data.write(','_c);
							}
						}
					}
					write_line(k_false);
					data.write('}'_c);
					break;
				}
			}
			return;
		}

		static auto process_whole (
			OCharacterStreamView & data,
			Value const &          value,
			Boolean const &        disable_array_trailing_comma,
			Boolean const &        disable_array_line_breaking,
			Boolean const &        disable_object_trailing_comma,
			Boolean const &        disable_object_line_breaking
		) -> Void {
			process_value(data, value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, k_begin_index);
			return;
		}

		// ----------------

		static auto process (
			OCharacterStreamView & data_,
			Value const &          value,
			Boolean const &        disable_array_trailing_comma,
			Boolean const &        disable_array_line_breaking,
			Boolean const &        disable_object_trailing_comma,
			Boolean const &        disable_object_line_breaking
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking);
		}

	};

}
