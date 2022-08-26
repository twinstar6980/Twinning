#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rton/version.hpp"
#include "core/tool/popcap/rton/structure.hpp"
#include "core/tool/common/protocol_buffer_variable_length_integer.hpp"
#include <unordered_map>

namespace TwinKleS::Core::Tool::PopCap::RTON {

	inline namespace CommonOfEncode {

		namespace Detail {

		}

	}

	namespace Encode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region declaration

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &                                 rton,
				JSON::Value const &                               json,
				Optional<std::unordered_map<CStringView, Size>> & string_index,
				Boolean const &                                   enable_rtid
			) -> Void;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &     rton,
				JSON::Boolean const & json
			) -> Void {
				rton.write(json ? (Structure::TypeIdentifier::boolean_true) : (Structure::TypeIdentifier::boolean_false));
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &    rton,
				JSON::Number const & json
			) -> Void {
				if (json.is_integer()) {
					rton.write(Structure::TypeIdentifier::integer_variable_length_signed_64);
					ProtocolBufferVariableLengthInteger::encode_s64(rton, json.get_integer());
				} else {
					rton.write(Structure::TypeIdentifier::floating_64);
					rton.write(json.get_floating());
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &                                 rton,
				JSON::String const &                              json,
				Optional<std::unordered_map<CStringView, Size>> & string_index
			) -> Void {
				if (string_index) {
					if (auto indexed_string = string_index.get().find(json); indexed_string != string_index.get().end()) {
						rton.write(Structure::TypeIdentifier::string_native_indexed);
						ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>((*indexed_string).second));
					} else {
						string_index.get()[json] = mbw<Size>(string_index.get().size());
						rton.write(Structure::TypeIdentifier::string_native_indexing);
						ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(json.size()));
						rton.write(json);
					}
				} else {
					rton.write(Structure::TypeIdentifier::string_native);
					ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(json.size()));
					rton.write(json);
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &                                 rton,
				JSON::String const &                              json,
				Optional<std::unordered_map<CStringView, Size>> & string_index,
				Boolean const &                                   enable_rtid
			) -> Void {
				if (enable_rtid) {
					if (auto rtid_type = Structure::analysis_rtid(json)) {
						rton.write(Structure::TypeIdentifier::string_rtid);
						rton.write(rtid_type.get());
						switch (rtid_type.get()) {
							case Structure::RTIDTypeIdentifier::null : {
								break;
							}
							case Structure::RTIDTypeIdentifier::uid : {
								auto content = json.sub("RTID("_sl, json.size() - "RTID()"_sl);
								auto at_position = Range::find_index(content, '@'_c).get();
								auto sheet = content.tail(content.size() - (at_position + "@"_sl));
								auto uid = content.head(at_position);
								auto uid_first = IntegerU32{};
								auto uid_middle = IntegerU32{};
								auto uid_last = IntegerU32{};
								// TODO : do not use scanf ?
								auto scan_result = std::sscanf(cast_pointer<char>(make_null_terminated_string(uid).begin()).value, "%u.%u.%x", &uid_first.value, &uid_middle.value, &uid_last.value);
								assert_condition(scan_result == 3);
								ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(sheet.size()));
								ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(sheet.size()));
								rton.write(sheet);
								ProtocolBufferVariableLengthInteger::encode_u32(rton, uid_middle);
								ProtocolBufferVariableLengthInteger::encode_u32(rton, uid_first);
								rton.write(uid_last);
								break;
							}
							case Structure::RTIDTypeIdentifier::alias : {
								auto content = json.sub("RTID("_sl, json.size() - "RTID()"_sl);
								auto at_position = Range::find_index(content, '@'_c).get();
								auto sheet = content.tail(content.size() - (at_position + "@"_sl));
								auto alias = content.head(at_position);
								ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(sheet.size()));
								ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(sheet.size()));
								rton.write(sheet);
								ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(alias.size()));
								ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(alias.size()));
								rton.write(alias);
								break;
							}
						}
					} else {
						process_unit<version>(rton, json, string_index);
					}
				} else {
					process_unit<version>(rton, json, string_index);
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &                                 rton,
				JSON::Array const &                               json,
				Optional<std::unordered_map<CStringView, Size>> & string_index,
				Boolean const &                                   enable_rtid
			) -> Void {
				rton.write(Structure::TypeIdentifier::array_begin);
				rton.write(Structure::TypeIdentifier::array_size);
				ProtocolBufferVariableLengthInteger::encode_u32(rton, cbw<IntegerU32>(json.size()));
				for (auto & element : json) {
					process_unit<version>(rton, element, string_index, enable_rtid);
				}
				rton.write(Structure::TypeIdentifier::array_end);
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &                                 rton,
				JSON::Object const &                              json,
				Optional<std::unordered_map<CStringView, Size>> & string_index,
				Boolean const &                                   enable_rtid
			) -> Void {
				rton.write(Structure::TypeIdentifier::object_begin);
				for (auto & element : json) {
					process_unit<version>(rton, element.key, string_index);
					process_unit<version>(rton, element.value, string_index, enable_rtid);
				}
				rton.write(Structure::TypeIdentifier::object_end);
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				OByteStreamView &                                 rton,
				JSON::Value const &                               json,
				Optional<std::unordered_map<CStringView, Size>> & string_index,
				Boolean const &                                   enable_rtid
			) -> Void {
				switch (json.type()) {
					case JSON::ValueType::null : {
						assert_failed(R"(json.type() == /* non-null */)");
						break;
					}
					case JSON::ValueType::boolean : {
						process_unit<version>(rton, json.get_boolean());
						break;
					}
					case JSON::ValueType::number : {
						process_unit<version>(rton, json.get_number());
						break;
					}
					case JSON::ValueType::string : {
						process_unit<version>(rton, json.get_string(), string_index, enable_rtid);
						break;
					}
					case JSON::ValueType::array : {
						process_unit<version>(rton, json.get_array(), string_index, enable_rtid);
						break;
					}
					case JSON::ValueType::object : {
						process_unit<version>(rton, json.get_object(), string_index, enable_rtid);
						break;
					}
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_whole (
				OByteStreamView &   rton,
				JSON::Value const & json,
				Boolean const &     enable_string_index,
				Boolean const &     enable_rtid
			) -> Void {
				rton.write(Structure::k_magic_identifier);
				auto version_data = OByteStreamView{rton.forward_view(bs_static_size<Structure::VersionNumber>())};
				rton.backward(bs_static_size<Structure::TypeIdentifier>());
				auto string_index = Optional<std::unordered_map<CStringView, Size>>{};
				if (enable_string_index) {
					string_index.set();
				}
				process_unit<version>(rton, json.get_object(), string_index, enable_rtid);
				rton.write(Structure::k_done_identifier);
				version_data.write(cbw<Structure::VersionNumber>(version.number));
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				OByteStreamView &   rton,
				JSON::Value const & json,
				Boolean const &     enable_string_index,
				Boolean const &     enable_rtid
			) -> Void {
				process_whole<version>(rton, json, enable_string_index, enable_rtid);
				return;
			}

			inline auto process (
				OByteStreamView &   rton,
				JSON::Value const & json,
				Boolean const &     enable_string_index,
				Boolean const &     enable_rtid,
				Version const &     version
			) -> Void {
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(rton, json, enable_string_index, enable_rtid);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Decode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_unit (
				IByteStreamView &                 rton,
				JSON::Value &                     json,
				List<CStringView> &               native_string_index,
				List<CStringView> &               unicode_string_index,
				Structure::TypeIdentifier const & type_identifier
			) -> Void {
				switch (type_identifier) {
					default : {
						assert_failed(R"(type_identifier == /* valid */)");
						break;
					}
					case Structure::TypeIdentifier::boolean_false : {
						json.set_boolean(k_false);
						break;
					}
					case Structure::TypeIdentifier::boolean_true : {
						json.set_boolean(k_true);
						break;
					}
					case Structure::TypeIdentifier::integer_signed_8 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerS8>()));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_8_zero : {
						json.set_number(cbw<Integer>(0_is8));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_8 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerU8>()));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_8_zero : {
						json.set_number(cbw<Integer>(0_iu8));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_16 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerS16>()));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_16_zero : {
						json.set_number(cbw<Integer>(0_is16));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_16 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerU16>()));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_16_zero : {
						json.set_number(cbw<Integer>(0_iu16));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_32 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerS32>()));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_32_zero : {
						json.set_number(cbw<Integer>(0_is32));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_32 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerU32>()));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_32_zero : {
						json.set_number(cbw<Integer>(0_iu32));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_64 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerS64>()));
						break;
					}
					case Structure::TypeIdentifier::integer_signed_64_zero : {
						json.set_number(cbw<Integer>(0_is64));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_64 : {
						json.set_number(cbw<Integer>(rton.read_of<IntegerU64>()));
						break;
					}
					case Structure::TypeIdentifier::integer_unsigned_64_zero : {
						json.set_number(cbw<Integer>(0_iu64));
						break;
					}
					case Structure::TypeIdentifier::floating_32 : {
						json.set_number(cbw<Floating>(rton.read_of<Floating32>()));
						break;
					}
					case Structure::TypeIdentifier::floating_32_zero : {
						json.set_number(cbw<Floating>(0.0_f32));
						break;
					}
					case Structure::TypeIdentifier::floating_64 : {
						json.set_number(cbw<Floating>(rton.read_of<Floating64>()));
						break;
					}
					case Structure::TypeIdentifier::floating_64_zero : {
						json.set_number(cbw<Floating>(0.0_f64));
						break;
					}
					case Structure::TypeIdentifier::integer_variable_length_unsigned_32 :
					case Structure::TypeIdentifier::integer_variable_length_unsigned_32_equivalent : {
						json.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_u32(rton)));
						break;
					}
					case Structure::TypeIdentifier::integer_variable_length_signed_32 : {
						json.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_s32(rton)));
						break;
					}
					case Structure::TypeIdentifier::integer_variable_length_unsigned_64 :
					case Structure::TypeIdentifier::integer_variable_length_unsigned_64_equivalent : {
						json.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_u64(rton)));
						break;
					}
					case Structure::TypeIdentifier::integer_variable_length_signed_64 : {
						json.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_s64(rton)));
						break;
					}
					case Structure::TypeIdentifier::string_native : {
						auto length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						json.set_string(from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(length))));
						break;
					}
					case Structure::TypeIdentifier::string_native_indexing : {
						auto length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						json.set_string(from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(length))));
						native_string_index.append(json.get_string());
						break;
					}
					case Structure::TypeIdentifier::string_native_indexed : {
						auto index = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						json.set_string(native_string_index[index]);
						break;
					}
					case Structure::TypeIdentifier::string_unicode : {
						auto length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						auto size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						json.set_string(from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(size))));
						break;
					}
					case Structure::TypeIdentifier::string_unicode_indexing : {
						auto length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						auto size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						json.set_string(from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(size))));
						unicode_string_index.append(json.get_string());
						break;
					}
					case Structure::TypeIdentifier::string_unicode_indexed : {
						auto index = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						json.set_string(unicode_string_index[index]);
						break;
					}
					case Structure::TypeIdentifier::string_rtid : {
						switch (rton.read_of<Structure::RTIDTypeIdentifier>()) {
							default : {
								assert_failed(R"(rton.read_of<Structure::RTIDTypeIdentifier>() == /* valid */)");
								break;
							}
							case Structure::RTIDTypeIdentifier::null : {
								json.set_string(Structure::RTIDFormat::null());
								break;
							}
							case Structure::RTIDTypeIdentifier::uid : {
								auto sheet_length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
								auto sheet_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
								auto sheet = from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(sheet_size)));
								auto uid_middle = ProtocolBufferVariableLengthInteger::decode_u32(rton);
								auto uid_first = ProtocolBufferVariableLengthInteger::decode_u32(rton);
								auto uid_last = rton.read_of<IntegerU32>();
								// TODO : unknown type of uid 's value , define them be 'u32'
								json.set_string(Structure::RTIDFormat::uid(uid_first, uid_middle, uid_last, sheet));
								break;
							}
							case Structure::RTIDTypeIdentifier::alias : {
								auto sheet_length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
								auto sheet_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
								auto sheet = from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(sheet_size)));
								auto alias_length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
								auto alias_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
								auto alias = from_byte_view<Character, BasicStringView>(rton.forward_view(bs_static_size<BasicString<Character>>(alias_size)));
								json.set_string(Structure::RTIDFormat::alias(alias, sheet));
								break;
							}
						}
						break;
					}
					case Structure::TypeIdentifier::string_rtid_null : {
						json.set_string(Structure::RTIDFormat::null());
						break;
					}
					case Structure::TypeIdentifier::array_begin : {
						auto & array = json.set_array();
						assert_condition(rton.read_of<Structure::TypeIdentifier>() == Structure::TypeIdentifier::array_size);
						auto array_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(rton));
						array.allocate(array_size);
						while (k_true) {
							auto value_type_identifier = rton.read_of<Structure::TypeIdentifier>();
							if (value_type_identifier == Structure::TypeIdentifier::array_end) {
								break;
							}
							array.append();
							process_unit<version>(rton, array.last(), native_string_index, unicode_string_index, value_type_identifier);
						}
						assert_condition(array.size() == array_size);
						break;
					}
					case Structure::TypeIdentifier::object_begin : {
						json.set_object();
						auto & object = json.get_object();
						auto   member_list = std::list<JSON::Object::Element>{};
						while (k_true) {
							auto key_type_identifier = rton.read_of<Structure::TypeIdentifier>();
							if (key_type_identifier == Structure::TypeIdentifier::object_end) {
								break;
							}
							member_list.emplace_back();
							auto member_key = JSON::Value{};
							process_unit<version>(rton, member_key, native_string_index, unicode_string_index, key_type_identifier);
							member_list.back().key = as_moveable(member_key.get_string());
							auto value_type_identifier = rton.read_of<Structure::TypeIdentifier>();
							process_unit<version>(rton, member_list.back().value, native_string_index, unicode_string_index, value_type_identifier);
						}
						object.assign(
							member_list,
							[] (auto & element) -> auto&& {
								return as_moveable(element);
							}
						);
						break;
					}
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_whole (
				IByteStreamView & rton,
				JSON::Value &     json
			) -> Void {
				assert_condition(rton.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				assert_condition(rton.read_of<Structure::VersionNumber>() == cbw<Structure::VersionNumber>(version.number));
				json.set_null();
				auto native_string_upper_bound = k_none_size;
				auto unicode_string_upper_bound = k_none_size;
				for (auto & element : rton.reserve_view()) {
					switch (self_cast<Structure::TypeIdentifier>(element)) {
							[[likely]]
						default : {
							break;
						}
						case Structure::TypeIdentifier::string_native_indexing : {
							++native_string_upper_bound;
							break;
						}
						case Structure::TypeIdentifier::string_unicode_indexing : {
							++unicode_string_upper_bound;
							break;
						}
					}
				}
				auto native_string_index_list = List<CStringView>{native_string_upper_bound};
				auto unicode_string_index_list = List<CStringView>{unicode_string_upper_bound};
				process_unit<version>(rton, json, native_string_index_list, unicode_string_index_list, Structure::TypeIdentifier::object_begin);
				assert_condition(rton.read_of<Structure::DoneIdentifier>() == Structure::k_done_identifier);
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				IByteStreamView & rton,
				JSON::Value &     json
			) -> Void {
				process_whole<version>(rton, json);
				return;
			}

			inline auto process (
				IByteStreamView & rton,
				JSON::Value &     json,
				Version const &   version
			) -> Void {
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(rton, json);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
