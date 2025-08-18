module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.tool.popcap.reflection_object_notation.common;
import twinning.kernel.tool.common.protocol_buffer_variable_length_integer;
import twinning.kernel.third.mscharconv;

export namespace Twinning::Kernel::Tool::PopCap::ReflectionObjectNotation {

	template <auto version> requires (check_version(version, {}, {}))
	struct Encode :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::DoneIdentifier;

		using Common::k_done_identifier;

		using typename Common::VersionNumber;

		using typename Common::TypeIdentifierEnumeration;

		using typename Common::TypeIdentifier;

		using typename Common::ReferenceTypeIdentifierEnumeration;

		using typename Common::ReferenceTypeIdentifier;

		using Common::k_reference_expression_format_of_null;

		using Common::k_reference_expression_format_of_uid;

		using Common::k_reference_expression_format_of_alias;

		using Common::analysis_reference;

		using Common::k_binary_blob_expression_format;

		// ----------------

		inline static auto process_value (
			OByteStreamView &     data,
			JSON::Boolean const & value
		) -> Void {
			data.write(TypeIdentifier{!value ? (TypeIdentifier::Value::boolean_false) : (TypeIdentifier::Value::boolean_true)});
			return;
		}

		inline static auto process_value (
			OByteStreamView &    data,
			JSON::Number const & value
		) -> Void {
			if (value.is_integer()) {
				data.write(TypeIdentifier{TypeIdentifier::Value::integer_variable_length_signed_64});
				ProtocolBufferVariableLengthInteger::encode_s64(data, up_cast<IntegerS64>(value.get_integer()));
			}
			else {
				data.write(TypeIdentifier{TypeIdentifier::Value::floater_signed_64});
				data.write(up_cast<FloaterS64>(value.get_floater()));
			}
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::String const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index
		) -> Void {
			if (!native_string_index.has()) {
				data.write(TypeIdentifier{TypeIdentifier::Value::string_native});
				if constexpr (check_version(version, {}, {false})) {
					ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(StringParser::compute_utf8_string_length(value)));
					StringParser::write_eascii_string(self_cast<OCharacterStreamView>(data), value, as_lvalue(Size{}));
				}
				if constexpr (check_version(version, {}, {true})) {
					ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(value.size()));
					StringParser::write_utf8_string(self_cast<OCharacterStreamView>(data), value.as_view(), as_lvalue(Size{}));
				}
			}
			else {
				if (auto indexed_string = native_string_index.get().find(value); indexed_string != native_string_index.get().end()) {
					data.write(TypeIdentifier{TypeIdentifier::Value::string_native_indexed});
					ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>((*indexed_string).second));
				}
				else {
					native_string_index.get()[value] = mbox<Size>(native_string_index.get().size());
					data.write(TypeIdentifier{TypeIdentifier::Value::string_native_indexing});
					if constexpr (check_version(version, {}, {false})) {
						ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(StringParser::compute_utf8_string_length(value)));
						StringParser::write_eascii_string(self_cast<OCharacterStreamView>(data), value, as_lvalue(Size{}));
					}
					if constexpr (check_version(version, {}, {true})) {
						ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(value.size()));
						StringParser::write_utf8_string(self_cast<OCharacterStreamView>(data), value.as_view(), as_lvalue(Size{}));
					}
				}
			}
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::String const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_reference
		) -> Void {
			auto is_reference = k_false;
			if (enable_reference) {
				if (auto reference_type = analysis_reference(value); reference_type.has()) {
					is_reference = k_true;
					data.write(TypeIdentifier{TypeIdentifier::Value::reference});
					data.write(reference_type.get());
					switch (reference_type.get().value) {
						case ReferenceTypeIdentifier::Value::null: {
							break;
						}
						case ReferenceTypeIdentifier::Value::uid: {
							auto content = value.sub("RTID("_sl, value.size() - "RTID()"_sl);
							auto at_position = Range::find_index(content, '@'_c).get();
							auto sheet = content.tail(content.size() - (at_position + "@"_sl));
							auto uid = content.head(at_position);
							auto uid_part = split_string<String>(uid, StaticArray<Character, 1_sz>{{'.'_c}});
							assert_test(uid_part.size() == 3_sz);
							assert_test(Range::all_of(uid_part[1_ix], &CharacterType::is_number_dec));
							assert_test(Range::all_of(uid_part[2_ix], &CharacterType::is_number_dec));
							assert_test(Range::all_of(uid_part[3_ix], &CharacterType::is_number_hex));
							auto uid_first = IntegerU32{};
							auto uid_middle = IntegerU32{};
							auto uid_last = IntegerU32{};
							assert_test(Third::mscharconv::from_chars(cast_pointer<char>(uid_part[1_ix].begin()).value, cast_pointer<char>(uid_part[1_ix].end()).value, uid_first.value, 10).ec == std::errc{});
							assert_test(Third::mscharconv::from_chars(cast_pointer<char>(uid_part[2_ix].begin()).value, cast_pointer<char>(uid_part[2_ix].end()).value, uid_middle.value, 10).ec == std::errc{});
							assert_test(Third::mscharconv::from_chars(cast_pointer<char>(uid_part[3_ix].begin()).value, cast_pointer<char>(uid_part[3_ix].end()).value, uid_last.value, 16).ec == std::errc{});
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(sheet.size()));
							data.write(sheet);
							ProtocolBufferVariableLengthInteger::encode_u32(data, uid_middle);
							ProtocolBufferVariableLengthInteger::encode_u32(data, uid_first);
							data.write(uid_last);
							break;
						}
						case ReferenceTypeIdentifier::Value::alias: {
							auto content = value.sub("RTID("_sl, value.size() - "RTID()"_sl);
							auto at_position = Range::find_index(content, '@'_c).get();
							auto sheet = content.tail(content.size() - (at_position + "@"_sl));
							auto alias = content.head(at_position);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(sheet.size()));
							data.write(sheet);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(StringParser::compute_utf8_string_length(alias)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(alias.size()));
							data.write(alias);
							break;
						}
						default: throw UnreachableException{};
					}
				}
			}
			if (!is_reference) {
				process_value(data, value, native_string_index);
			}
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::Array const &                               value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_reference
		) -> Void {
			data.write(TypeIdentifier{TypeIdentifier::Value::array_begin});
			data.write_constant(TypeIdentifier{TypeIdentifier::Value::array_size});
			ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(value.size()));
			for (auto & element : value) {
				process_value(data, element, native_string_index, enable_reference);
			}
			data.write(TypeIdentifier{TypeIdentifier::Value::array_end});
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::Object const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_reference
		) -> Void {
			data.write(TypeIdentifier{TypeIdentifier::Value::object_begin});
			for (auto & element : value) {
				process_value(data, element.key, native_string_index);
				process_value(data, element.value, native_string_index, enable_reference);
			}
			data.write(TypeIdentifier{TypeIdentifier::Value::object_end});
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::Value const &                               value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_reference
		) -> Void {
			switch (value.type().value) {
				case JSON::ValueType::Constant::null().value: {
					assert_fail(R"(value.type() == /* non-null */)");
					break;
				}
				case JSON::ValueType::Constant::boolean().value: {
					process_value(data, value.get_boolean());
					break;
				}
				case JSON::ValueType::Constant::number().value: {
					process_value(data, value.get_number());
					break;
				}
				case JSON::ValueType::Constant::string().value: {
					process_value(data, value.get_string(), native_string_index, enable_reference);
					break;
				}
				case JSON::ValueType::Constant::array().value: {
					process_value(data, value.get_array(), native_string_index, enable_reference);
					break;
				}
				case JSON::ValueType::Constant::object().value: {
					process_value(data, value.get_object(), native_string_index, enable_reference);
					break;
				}
				default: throw UnreachableException{};
			}
			return;
		}

		// ----------------

		inline static auto process_whole (
			OByteStreamView &   data,
			JSON::Value const & definition,
			Boolean const &     enable_string_index,
			Boolean const &     enable_reference
		) -> Void {
			data.write_constant(k_magic_identifier);
			auto version_data = OByteStreamView{data.forward_view(bs_static_size<VersionNumber>())};
			data.backward(bs_static_size<TypeIdentifier>());
			auto native_string_index = Optional<std::unordered_map<CStringView, Size>>{};
			if (enable_string_index) {
				native_string_index.set();
			}
			process_value(data, definition.get_object(), native_string_index, enable_reference);
			data.write_constant(k_done_identifier);
			version_data.write_constant(cbox<VersionNumber>(version.number));
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &   data_,
			JSON::Value const & definition,
			Boolean const &     enable_string_index,
			Boolean const &     enable_reference
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition, enable_string_index, enable_reference);
		}

	};

}
