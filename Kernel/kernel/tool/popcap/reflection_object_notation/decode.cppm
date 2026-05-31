module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.tool.popcap.reflection_object_notation.common;
import twinning.kernel.tool.common.protocol_buffer_variable_length_integer;

export namespace Twinning::Kernel::Tool::Popcap::ReflectionObjectNotation {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Decode :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::DoneMarker;

		using Common::k_done_marker;

		using typename Common::VersionNumber;

		using typename Common::TypeIdentifier;

		using typename Common::ReferenceTypeIdentifier;

		using Common::k_reference_expression_format_of_null;

		using Common::k_reference_expression_format_of_identifier;

		using Common::k_reference_expression_format_of_alias;

		using Common::analysis_reference;

		using Common::k_binary_blob_expression_format;

		// ----------------

		inline static auto process_value(
			InputByteStreamView &      data,
			Notation::Json::Value &    value,
			List<ConstantStringView> & native_string_index,
			List<ConstantStringView> & unicode_string_index,
			TypeIdentifier const &     type_identifier
		) -> Void {
			switch (type_identifier.value) {
				case TypeIdentifier::Constant::boolean_false().value: {
					value.set_boolean(k_false);
					break;
				}
				case TypeIdentifier::Constant::boolean_true().value: {
					value.set_boolean(k_true);
					break;
				}
				case TypeIdentifier::Constant::integer_signed_8().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerS8>()));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_8_zero().value: {
					value.set_number(cast_box<Integer>(0_is8));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_8().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerU8>()));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_8_zero().value: {
					value.set_number(cast_box<Integer>(0_iu8));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_16().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerS16>()));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_16_zero().value: {
					value.set_number(cast_box<Integer>(0_is16));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_16().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerU16>()));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_16_zero().value: {
					value.set_number(cast_box<Integer>(0_iu16));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_32().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerS32>()));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_32_zero().value: {
					value.set_number(cast_box<Integer>(0_is32));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_32().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerU32>()));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_32_zero().value: {
					value.set_number(cast_box<Integer>(0_iu32));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_64().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerS64>()));
					break;
				}
				case TypeIdentifier::Constant::integer_signed_64_zero().value: {
					value.set_number(cast_box<Integer>(0_is64));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_64().value: {
					value.set_number(cast_box<Integer>(data.read_of<IntegerU64>()));
					break;
				}
				case TypeIdentifier::Constant::integer_unsigned_64_zero().value: {
					value.set_number(cast_box<Integer>(0_iu64));
					break;
				}
				case TypeIdentifier::Constant::floater_signed_32().value: {
					value.set_number(cast_box<Floater>(data.read_of<FloaterS32>()));
					break;
				}
				case TypeIdentifier::Constant::floater_signed_32_zero().value: {
					value.set_number(cast_box<Floater>(0.0_fs32));
					break;
				}
				case TypeIdentifier::Constant::floater_signed_64().value: {
					value.set_number(cast_box<Floater>(data.read_of<FloaterS64>()));
					break;
				}
				case TypeIdentifier::Constant::floater_signed_64_zero().value: {
					value.set_number(cast_box<Floater>(0.0_fs64));
					break;
				}
				case TypeIdentifier::Constant::integer_variable_length_unsigned_32().value:
				case TypeIdentifier::Constant::integer_variable_length_unsigned_32_equivalent().value: {
					value.set_number(cast_box<Integer>(ProtocolBufferVariableLengthInteger::decode_u32(data)));
					break;
				}
				case TypeIdentifier::Constant::integer_variable_length_signed_32().value: {
					value.set_number(cast_box<Integer>(ProtocolBufferVariableLengthInteger::decode_s32(data)));
					break;
				}
				case TypeIdentifier::Constant::integer_variable_length_unsigned_64().value:
				case TypeIdentifier::Constant::integer_variable_length_unsigned_64_equivalent().value: {
					value.set_number(cast_box<Integer>(ProtocolBufferVariableLengthInteger::decode_u64(data)));
					break;
				}
				case TypeIdentifier::Constant::integer_variable_length_signed_64().value: {
					value.set_number(cast_box<Integer>(ProtocolBufferVariableLengthInteger::decode_s64(data)));
					break;
				}
				case TypeIdentifier::Constant::string_native().value: {
					auto   size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto & content = value.set_string();
					if constexpr (check_version(t_version, {}, {false})) {
						StringParser::read_eascii_string(unsafe_cast<InputCharacterStreamView>(data), content, size);
					}
					if constexpr (check_version(t_version, {}, {true})) {
						auto content_view = ConstantStringView{};
						StringParser::read_utf8_string_by_size(unsafe_cast<InputCharacterStreamView>(data), content_view, as_left(Size{}), size);
						content = content_view;
					}
					break;
				}
				case TypeIdentifier::Constant::string_native_indexing().value: {
					auto   size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto & content = value.set_string();
					if constexpr (check_version(t_version, {}, {false})) {
						StringParser::read_eascii_string(unsafe_cast<InputCharacterStreamView>(data), content, size);
					}
					if constexpr (check_version(t_version, {}, {true})) {
						auto content_view = ConstantStringView{};
						StringParser::read_utf8_string_by_size(unsafe_cast<InputCharacterStreamView>(data), content_view, as_left(Size{}), size);
						content = content_view;
					}
					native_string_index.append(content);
					break;
				}
				case TypeIdentifier::Constant::string_native_indexed().value: {
					auto index = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					value.set_string(native_string_index[index]);
					break;
				}
				case TypeIdentifier::Constant::string_unicode().value: {
					auto length = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto content = ConstantStringView{};
					StringParser::read_utf8_string(unsafe_cast<InputCharacterStreamView>(data), content, length);
					assert_test(content.size() == size);
					value.set_string(content);
					break;
				}
				case TypeIdentifier::Constant::string_unicode_indexing().value: {
					auto length = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto content = ConstantStringView{};
					StringParser::read_utf8_string(unsafe_cast<InputCharacterStreamView>(data), content, length);
					assert_test(content.size() == size);
					value.set_string(content);
					unicode_string_index.append(content);
					break;
				}
				case TypeIdentifier::Constant::string_unicode_indexed().value: {
					auto index = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					value.set_string(unicode_string_index[index]);
					break;
				}
				case TypeIdentifier::Constant::reference().value: {
					switch (data.read_of<ReferenceTypeIdentifier>().value) {
						case ReferenceTypeIdentifier::Constant::null().value: {
							value.set_string(k_reference_expression_format_of_null());
							break;
						}
						case ReferenceTypeIdentifier::Constant::identifier().value: {
							auto sheet_length = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_content = ConstantStringView{};
							StringParser::read_utf8_string(unsafe_cast<InputCharacterStreamView>(data), sheet_content, sheet_length);
							assert_test(sheet_content.size() == sheet_size);
							// TODO: unknown type of identifier value, define them be 'int-var-u32'
							auto identifier_middle = ProtocolBufferVariableLengthInteger::decode_u32(data);
							auto identifier_first = ProtocolBufferVariableLengthInteger::decode_u32(data);
							auto identifier_last = data.read_of<IntegerU32>();
							value.set_string(k_reference_expression_format_of_identifier(identifier_first, identifier_middle, identifier_last, sheet_content));
							break;
						}
						case ReferenceTypeIdentifier::Constant::alias().value: {
							auto sheet_length = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_content = ConstantStringView{};
							StringParser::read_utf8_string(unsafe_cast<InputCharacterStreamView>(data), sheet_content, sheet_length);
							assert_test(sheet_content.size() == sheet_size);
							auto alias_length = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto alias_size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto alias_content = ConstantStringView{};
							StringParser::read_utf8_string(unsafe_cast<InputCharacterStreamView>(data), alias_content, alias_length);
							assert_test(alias_content.size() == alias_size);
							value.set_string(k_reference_expression_format_of_alias(alias_content, sheet_content));
							break;
						}
						default: {
							assert_fail(R"(data.read_of<ReferenceTypeIdentifier>() == /* valid */)");
							break;
						}
					}
					break;
				}
				case TypeIdentifier::Constant::reference_null().value: {
					value.set_string(k_reference_expression_format_of_null());
					break;
				}
				case TypeIdentifier::Constant::array_begin().value: {
					auto & array = value.set_array();
					data.read_constant(TypeIdentifier::Constant::array_size());
					auto size = cast_box<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					array.allocate(size);
					while (k_true) {
						auto value_type_identifier = data.read_of<TypeIdentifier>();
						if (value_type_identifier == TypeIdentifier::Constant::array_end()) {
							break;
						}
						array.append();
						process_value(data, array.last(), native_string_index, unicode_string_index, value_type_identifier);
					}
					assert_test(array.size() == size);
					break;
				}
				case TypeIdentifier::Constant::object_begin().value: {
					auto & object = value.set_object();
					auto   member_list = std::list<Notation::Json::Object::Element>{};
					while (k_true) {
						auto key_type_identifier = data.read_of<TypeIdentifier>();
						if (key_type_identifier == TypeIdentifier::Constant::object_end()) {
							break;
						}
						member_list.emplace_back();
						auto member_key = Notation::Json::Value{};
						process_value(data, member_key, native_string_index, unicode_string_index, key_type_identifier);
						member_list.back().key = as_moveable(member_key.get_string());
						auto value_type_identifier = data.read_of<TypeIdentifier>();
						process_value(data, member_list.back().value, native_string_index, unicode_string_index, value_type_identifier);
					}
					object.assign(
						member_list,
						[](auto & element) -> auto && {
							return as_moveable(element);
						}
					);
					break;
				}
				default: {
					assert_fail(R"(type_identifier == /* valid */)");
					break;
				}
			}
			return;
		}

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &   data,
			Notation::Json::Value & definition
		) -> Void {
			data.read_constant(k_magic_marker);
			data.read_constant(cast_box<VersionNumber>(t_version.number));
			auto native_string_upper_bound = 0_sz;
			auto unicode_string_upper_bound = 0_sz;
			for (auto & element : data.reserve_view()) {
				switch (unsafe_cast<TypeIdentifier>(element).value) {
					case TypeIdentifier::Constant::string_native_indexing().value: {
						++native_string_upper_bound;
						break;
					}
					case TypeIdentifier::Constant::string_unicode_indexing().value: {
						++unicode_string_upper_bound;
						break;
					}
						[[likely]]
					default: {
						break;
					}
				}
			}
			auto native_string_index_list = List<ConstantStringView>{native_string_upper_bound};
			auto unicode_string_index_list = List<ConstantStringView>{unicode_string_upper_bound};
			process_value(data, definition, native_string_index_list, unicode_string_index_list, TypeIdentifier::Constant::object_begin());
			data.read_constant(k_done_marker);
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &   data_,
			Notation::Json::Value & definition
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_whole(data, definition);
		}

	};

}
