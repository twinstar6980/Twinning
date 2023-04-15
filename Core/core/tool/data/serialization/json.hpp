#pragma once

#include "core/utility/utility.hpp"
#include <list>

namespace TwinStar::Core::Tool::Data::Serialization::JSON {

	using Core::JSON::ValueType;

	using Core::JSON::Value;

	using Core::JSON::Array;

	using Core::JSON::Object;

	// ----------------

	struct WriteCommon {

	protected:

	};

	struct Write :
		WriteCommon {

	protected:

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

	public:

		static auto do_process_whole (
			OCharacterStreamView & data_,
			Value const &          value,
			Boolean const &        disable_trailing_comma,
			Boolean const &        disable_array_wrap_line
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, value, disable_trailing_comma, disable_array_wrap_line);
		}

	};

	struct Read :
		WriteCommon {

	protected:

		static auto process_value (
			ICharacterStreamView & data,
			Value &                value
		) -> Void {
			// TODO : static
			thread_local auto buffer = CharacterArray{0x10000_sz};
			thread_local auto buffer_stream = OCharacterStreamView{buffer};
			while (k_true) {
				switch (auto character = data.read_of(); character.value) {
					case ' ' :
					case '\t' :
					case '\n' :
					case '\r' : {
						continue;
						break;
					}
					case '/' : {
						StringParser::read_comment_after_first_mark(data);
						continue;
						break;
					}
					case 'n' : {
						if (data.read_of() == 'u'_c && data.read_of() == 'l'_c && data.read_of() == 'l'_c) {
							value.set_null();
						} else {
							throw SyntaxException{data.position().value, mss("invalid null value"_sf())};
						}
						break;
					}
					case 'f' : {
						if (data.read_of() == 'a'_c && data.read_of() == 'l'_c && data.read_of() == 's'_c && data.read_of() == 'e'_c) {
							value.set_boolean(k_false);
						} else {
							throw SyntaxException{data.position().value, mss("invalid false value"_sf())};
						}
						break;
					}
					case 't' : {
						if (data.read_of() == 'r'_c && data.read_of() == 'u'_c && data.read_of() == 'e'_c) {
							value.set_boolean(k_true);
						} else {
							throw SyntaxException{data.position().value, mss("invalid true value"_sf())};
						}
						break;
					}
					case '-' :
					case '+' :
					case '0' :
					case '1' :
					case '2' :
					case '3' :
					case '4' :
					case '5' :
					case '6' :
					case '7' :
					case '8' :
					case '9' : {
						data.backward();
						value.set_number();
						StringParser::read_number(data, value.get_number());
						break;
					}
					case '\"' : {
						value.set_string();
						buffer_stream.backward_to_begin();
						StringParser::read_escape_utf8_string_until(data, buffer_stream, '"'_c);
						data.forward();
						value.get_string() = String{buffer_stream.stream_view()};
						break;
					}
					case '[' : {
						auto & array = value.set_array();
						auto   item_list = std::list<Array::Element>{};
						auto   has_comma = k_false;
						for (auto need_more_item = k_true; need_more_item;) {
							switch (data.read_of().value) {
								case ']' : {
									if (has_comma && item_list.empty()) {
										throw SyntaxException{data.position().value, mss("invalid comma on empty array"_sf())};
									}
									need_more_item = k_false;
									break;
								}
								case ',' : {
									if (has_comma) {
										throw SyntaxException{data.position().value, mss("too many comma on array"_sf())};
									}
									has_comma = k_true;
									break;
								}
								case ' ' :
								case '\t' :
								case '\n' :
								case '\r' : {
									break;
								}
								case '/' : {
									StringParser::read_comment_after_first_mark(data);
									break;
								}
								default : {
									if (has_comma && item_list.empty()) {
										throw SyntaxException{data.position().value, mss("invalid comma before array's first element"_sf())};
									}
									if (!has_comma && !item_list.empty()) {
										throw SyntaxException{data.position().value, mss("need comma between array's element"_sf())};
									}
									data.backward();
									item_list.emplace_back();
									process_value(data, item_list.back());
									has_comma = k_false;
								}
							}
						}
						array.assign(
							item_list,
							[] (auto & element) -> auto && {
								return as_moveable(element);
							}
						);
						item_list.clear();
						break;
					}
					case '{' : {
						auto & object = value.set_object();
						auto   item_list = std::list<Object::Element>{};
						auto   has_comma = k_false;
						for (auto need_more_item = k_true; need_more_item;) {
							switch (data.read_of().value) {
								case '}' : {
									if (has_comma && item_list.empty()) {
										throw SyntaxException{data.position().value, mss("invalid comma on empty object"_sf())};
									}
									need_more_item = k_false;
									break;
								}
								case ',' : {
									if (has_comma) {
										throw SyntaxException{data.position().value, mss("too many comma on object"_sf())};
									}
									has_comma = k_true;
									break;
								}
								case ' ' :
								case '\t' :
								case '\n' :
								case '\r' : {
									break;
								}
								case '/' : {
									StringParser::read_comment_after_first_mark(data);
									break;
								}
								default : {
									if (has_comma && item_list.empty()) {
										throw SyntaxException{data.position().value, mss("invalid comma before object's first member"_sf())};
									}
									if (!has_comma && !item_list.empty()) {
										throw SyntaxException{data.position().value, mss("need comma between object's member"_sf())};
									}
									data.backward();
									item_list.emplace_back();
									if (data.read_of() != '\"'_c) {
										throw SyntaxException{data.position().value, mss("key must be string"_sf())};
									}
									buffer_stream.backward_to_begin();
									StringParser::read_escape_utf8_string_until(data, buffer_stream, '"'_c);
									data.forward();
									item_list.back().key = String{buffer_stream.stream_view()};
									for (auto need_more_space = k_true; need_more_space;) {
										switch (data.read_of().value) {
											case ':' : {
												need_more_space = k_false;
												break;
											}
											case ' ' :
											case '\t' :
											case '\n' :
											case '\r' : {
												break;
											}
											default : {
												throw SyntaxException{data.position().value, mss("key's next non-space character must be ':'"_sf())};
											}
										}
									}
									process_value(data, item_list.back().value);
									has_comma = k_false;
								}
							}
						}
						object.assign(
							item_list,
							[] (auto & element) -> auto && {
								return as_moveable(element);
							}
						);
						item_list.clear();
						break;
					}
					default : {
						throw SyntaxException{data.position().value, mss("invalid character {:02X}h"_sf(character))};
					}
				}
				break;
			}
			return;
		}

		static auto process_whole (
			ICharacterStreamView & data,
			Value &                value
		) -> Void {
			process_value(data, value);
			return;
		}

	public:

		static auto do_process_whole (
			ICharacterStreamView & data_,
			Value &                value
		) -> Void {
			M_use_zps_of(data);
			restruct(value);
			return process_whole(data, value);
		}

	};

}
