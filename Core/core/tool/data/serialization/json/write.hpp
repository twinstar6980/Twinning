#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/data/serialization/json/common.hpp"
#include <list>

namespace TwinStar::Core::Tool::Data::Serialization::JSON {

	struct Write :
		Common {

		using Common = Common;

		// ----------------

		static auto process_value (
			OCharacterStreamView & data,
			Value const &          value,
			Boolean const &        disable_trailing_comma,
			Boolean const &        disable_array_wrap_line,
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
					auto write_space =
						[&] (
					) -> auto {
						data.write(!disable_array_wrap_line ? ('\n'_c) : (' '_c));
						return;
					};
					auto write_indent =
						[&] (
						Boolean const & is_inner
					) -> auto {
						if (!disable_array_wrap_line) {
							StringParser::write_character_repeat(data, '\t'_c, is_inner ? (indent_level + 1_sz) : (indent_level));
						}
						return;
					};
					if (!array.empty()) {
						auto write_element =
							[&] (
							Array::Element const & element
						) -> auto {
							write_indent(k_true);
							process_value(data, element, disable_trailing_comma, disable_array_wrap_line, indent_level + 1_sz);
							return;
						};
						write_space();
						for (auto & element : array.head(array.size() - 1_sz)) {
							write_element(element);
							data.write(','_c);
							write_space();
						}
						{
							write_element(array.last());
							if (!disable_trailing_comma && !disable_array_wrap_line) {
								data.write(','_c);
							}
						}
					}
					write_space();
					write_indent(k_false);
					data.write(']'_c);
					break;
				}
				case ValueType::Constant::object().value : {
					auto & object = value.get_object();
					data.write('{'_c);
					auto write_space =
						[&] (
					) -> auto {
						data.write('\n'_c);
						return;
					};
					auto write_indent =
						[&] (
						Boolean const & is_inner
					) -> auto {
						StringParser::write_character_repeat(data, '\t'_c, is_inner ? (indent_level + 1_sz) : (indent_level));
						return;
					};
					if (!object.empty()) {
						auto write_member =
							[&] (
							Object::Element const & member
						) -> auto {
							write_indent(k_true);
							data.write('"'_c);
							StringParser::write_escape_utf8_string_until(data, as_lvalue(ICharacterStreamView{member.key}), '"'_c);
							data.write('"'_c);
							data.write(':'_c);
							data.write(' '_c);
							process_value(data, member.value, disable_trailing_comma, disable_array_wrap_line, indent_level + 1_sz);
							return;
						};
						write_space();
						for (auto & element : object.head(object.size() - 1_sz)) {
							write_member(element);
							data.write(','_c);
							write_space();
						}
						{
							write_member(object.last());
							if (!disable_trailing_comma) {
								data.write(','_c);
							}
						}
					}
					write_space();
					write_indent(k_false);
					data.write('}'_c);
					break;
				}
			}
			return;
		}

		static auto process_whole (
			OCharacterStreamView & data,
			Value const &          value,
			Boolean const &        disable_trailing_comma,
			Boolean const &        disable_array_wrap_line
		) -> Void {
			process_value(data, value, disable_trailing_comma, disable_array_wrap_line, k_begin_index);
			return;
		}

		// ----------------

		static auto process (
			OCharacterStreamView & data_,
			Value const &          value,
			Boolean const &        disable_trailing_comma,
			Boolean const &        disable_array_wrap_line
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, value, disable_trailing_comma, disable_array_wrap_line);
		}

	};

}
