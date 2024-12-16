module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.serialization.json.read;
import twinning.kernel.utility;
import twinning.kernel.tool.data.serialization.json.common;

export namespace Twinning::Kernel::Tool::Data::Serialization::JSON {

	struct Read :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_value (
			ICharacterStreamView & data,
			Value &                value,
			OCharacterStreamView & buffer
		) -> Void {
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
						}
						else {
							throw SyntaxException{data.position().value, mss("invalid null value"_sf())};
						}
						break;
					}
					case 'f' : {
						if (data.read_of() == 'a'_c && data.read_of() == 'l'_c && data.read_of() == 's'_c && data.read_of() == 'e'_c) {
							value.set_boolean(k_false);
						}
						else {
							throw SyntaxException{data.position().value, mss("invalid false value"_sf())};
						}
						break;
					}
					case 't' : {
						if (data.read_of() == 'r'_c && data.read_of() == 'u'_c && data.read_of() == 'e'_c) {
							value.set_boolean(k_true);
						}
						else {
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
						buffer.backward_to_begin();
						StringParser::read_escape_utf8_string_until(data, buffer, '"'_c);
						data.forward();
						value.get_string() = String{buffer.stream_view()};
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
									process_value(data, item_list.back(), buffer);
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
									buffer.backward_to_begin();
									StringParser::read_escape_utf8_string_until(data, buffer, '"'_c);
									data.forward();
									item_list.back().key = String{buffer.stream_view()};
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
									process_value(data, item_list.back().value, buffer);
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

		inline static auto process_whole (
			ICharacterStreamView & data,
			Value &                value,
			OCharacterStreamView & buffer
		) -> Void {
			process_value(data, value, buffer);
			return;
		}

		// ----------------

		inline static auto process (
			ICharacterStreamView & data_,
			Value &                value,
			OCharacterStreamView & buffer
		) -> Void {
			M_use_zps_of(data);
			restruct(value);
			return process_whole(data, value, buffer);
		}

	};

}
