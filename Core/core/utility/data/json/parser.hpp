#pragma once

// TODO : refactor

#include "core/utility/data/json/value.hpp"
#include "core/utility/misc/character_series/stream.hpp"
#include "core/utility/string/parser.hpp"
#include "core/utility/file_system/file_system.hpp"
#include <list>

namespace TwinKleS::Core::JSON {

	inline namespace CommonOfWrite {

		namespace Detail {

		}

	}

	namespace Write {

		namespace Detail {

			#pragma region using

			using namespace CommonOfWrite::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				OCharacterStreamView & stream,
				Value const &          value,
				Boolean const &        disable_trailing_comma,
				Boolean const &        disable_array_wrap_line,
				Size const &           indent_level = k_none_size
			) -> Void {
				switch (value.type()) {
					case ValueType::null : {
						StringParser::write_null(stream, value.get_null());
						break;
					}
					case ValueType::boolean : {
						StringParser::write_boolean(stream, value.get_boolean());
						break;
					}
					case ValueType::number : {
						StringParser::write_number(stream, value.get_number(), k_true);
						break;
					}
					case ValueType::string : {
						stream.write('"'_c);
						StringParser::write_escape_string(stream, as_lvalue(ICharacterStreamView{value.get_string()}), '"'_c);
						stream.write('"'_c);
						break;
					}
					case ValueType::array : {
						auto & array = value.get_array();
						stream.write('['_c);
						if (!array.empty()) {
							auto write_space =
								[&] (
							) -> auto {
								stream.write(!disable_array_wrap_line ? ('\n'_c) : (' '_c));
								return;
							};
							auto write_indent =
								[&] (
								Boolean const & is_inner
							) -> auto {
								if (!disable_array_wrap_line) {
									StringParser::write_character_repeat(stream, '\t'_c, is_inner ? (indent_level + 1_sz) : (indent_level));
								}
								return;
							};
							auto write_element =
								[&] (
								Array::Element const & element
							) -> auto {
								write_indent(k_true);
								process(stream, element, disable_trailing_comma, disable_array_wrap_line, indent_level + 1_sz);
								return;
							};
							write_space();
							for (auto & element : array.head(array.size() - 1_sz)) {
								write_element(element);
								stream.write(','_c);
								write_space();
							}
							{
								write_element(array.last());
								if (!disable_trailing_comma) {
									stream.write(','_c);
								}
								write_space();
							}
							write_indent(k_false);
						}
						stream.write(']'_c);
						break;
					}
					case ValueType::object : {
						auto & object = value.get_object();
						stream.write('{'_c);
						if (!object.empty()) {
							auto write_space =
								[&] (
							) -> auto {
								stream.write('\n'_c);
								return;
							};
							auto write_indent =
								[&] (
								Boolean const & is_inner
							) -> auto {
								StringParser::write_character_repeat(stream, '\t'_c, is_inner ? (indent_level + 1_sz) : (indent_level));
								return;
							};
							auto write_member =
								[&] (
								Object::Element const & member
							) -> auto {
								write_indent(k_true);
								stream.write('"'_c);
								StringParser::write_escape_string(stream, as_lvalue(ICharacterStreamView{member.key}), '"'_c);
								stream.write('"'_c);
								stream.write(':'_c);
								stream.write(' '_c);
								process(stream, member.value, disable_trailing_comma, disable_array_wrap_line, indent_level + 1_sz);
								return;
							};
							write_space();
							for (auto & element : object.head(object.size() - 1_sz)) {
								write_member(element);
								stream.write(','_c);
								write_space();
							}
							{
								write_member(object.last());
								if (!disable_trailing_comma) {
									stream.write(','_c);
								}
								write_space();
							}
							write_indent(k_false);
						}
						stream.write('}'_c);
						break;
					}
				}
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Read {

		namespace Detail {

			#pragma region using

			using namespace CommonOfWrite::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				ICharacterStreamView & stream,
				Value &                value
			) -> Void {
				// TODO : static
				thread_local auto buffer = CharacterArray{0x10000_sz};
				thread_local auto buffer_stream = OCharacterStreamView{buffer};
				try {
					while (k_true) {
						switch (auto character = stream.read_of(); character.value) {
							default : {
								throw ExceptionMessage{{"syntax error : invalid character {:02X}h"_sf(character)}};
								break;
							}
							case ' ' :
							case '\t' :
							case '\n' :
							case '\r' : {
								continue;
								break;
							}
							case '/' : {
								StringParser::read_comment_after_first_mark(stream);
								continue;
								break;
							}
							case 'n' : {
								if (stream.read_of() == 'u'_c && stream.read_of() == 'l'_c && stream.read_of() == 'l'_c) {
									value.set_null();
								} else {
									throw ExceptionMessage{{"syntax error : not a null value"_sf()}};
								}
								break;
							}
							case 'f' : {
								if (stream.read_of() == 'a'_c && stream.read_of() == 'l'_c && stream.read_of() == 's'_c && stream.read_of() == 'e'_c) {
									value.set_boolean(k_false);
								} else {
									throw ExceptionMessage{{"syntax error : not a false value"_sf()}};
								}
								break;
							}
							case 't' : {
								if (stream.read_of() == 'r'_c && stream.read_of() == 'u'_c && stream.read_of() == 'e'_c) {
									value.set_boolean(k_true);
								} else {
									throw ExceptionMessage{{"syntax error : not a true value"_sf()}};
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
								stream.backward();
								value.set_number();
								StringParser::read_number(stream, value.get_number());
								break;
							}
							case '\"' : {
								value.set_string();
								buffer_stream.backward_to_begin();
								StringParser::read_escape_string(stream, buffer_stream, '"'_c);
								stream.forward();
								value.get_string() = String{buffer_stream.stream_view()};
								break;
							}
							case '[' : {
								auto & array = value.set_array();
								auto   value_list = std::list<Array::Element>{};
								auto   array_size = k_none_size;
								auto   has_comma = k_false;
								while (k_true) {
									switch (stream.read_of().value) {
										default : {
											if (!has_comma && array_size != k_none_size) {
												throw ExceptionMessage{{"syntax error : need comma between array's element"_sf()}};
											}
											stream.backward();
											value_list.emplace_back();
											process(stream, value_list.back());
											++array_size;
											has_comma = k_false;
											break;
										}
										case ']' : {
											goto CLOSE_LOOP_ARRAY;
											break;
										}
										case ',' : {
											if (has_comma) {
												throw ExceptionMessage{{"syntax error : too many comma on array"_sf()}};
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
											StringParser::read_comment_after_first_mark(stream);
											break;
										}
									}
								}
							CLOSE_LOOP_ARRAY:
								array.assign(
									value_list,
									[] (auto & element) -> auto&& {
										return as_moveable(element);
									}
								);
								value_list.clear();
								break;
							}
							case '{' : {
								auto & object = value.set_object();
								auto   value_list = std::list<Object::Element>{};
								auto   object_size = k_none_size;
								auto   has_comma = k_false;
								while (k_true) {
									switch (stream.read_of().value) {
										default : {
											if (!has_comma && object_size != k_none_size) {
												throw ExceptionMessage{{"syntax error : need comma between object's member"_sf()}};
											}
											stream.backward();
											value_list.emplace_back();
											if (stream.read_of() != '\"'_c) {
												throw ExceptionMessage{{"syntax error : key must be string"_sf()}};
											}
											buffer_stream.backward_to_begin();
											StringParser::read_escape_string(stream, buffer_stream, '"'_c);
											stream.forward();
											value_list.back().key = String{buffer_stream.stream_view()};
											while (k_true) {
												switch (stream.read_of().value) {
													default : {
														throw ExceptionMessage{{"syntax error : key's next non-space character must be ':'"_sf()}};
														break;
													}
													case ':' : {
														goto CLOSE_LOOP_MEMBER;
														break;
													}
													case ' ' :
													case '\t' :
													case '\n' :
													case '\r' : {
														break;
													}
												}
											}
										CLOSE_LOOP_MEMBER:
											process(stream, value_list.back().value);
											++object_size;
											has_comma = k_false;
											break;
										}
										case '}' : {
											goto TCLOSE_LOOP_OBJEC;
											break;
										}
										case ',' : {
											if (has_comma) {
												throw ExceptionMessage{{"syntax error : too many comma on object"_sf()}};
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
											StringParser::read_comment_after_first_mark(stream);
											break;
										}
									}
								}
							TCLOSE_LOOP_OBJEC:
								object.assign(
									value_list,
									[] (auto & element) -> auto&& {
										return as_moveable(element);
									}
								);
								value_list.clear();
								break;
							}
						}
						break;
					}
					return;
				} catch (ExceptionMessage & exception_message) {
					throw BaseException{
						{
							"$ stream.position() : {:X}h"_sf(stream.position()),
						},
						as_moveable(exception_message)
					};
				}
			}

			#pragma endregion

			#pragma endregion

		}

		using Detail::process;

	}

}
