module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.tool.popcap.reflection_object_notation.common;
import twinning.kernel.tool.common.protocol_buffer_variable_length_integer;

export namespace Twinning::Kernel::Tool::PopCap::ReflectionObjectNotation {

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::DoneIdentifier;

		using Common::k_done_identifier;

		using typename Common::VersionNumber;

		using typename Common::TypeIdentifierEnumeration;

		using typename Common::TypeIdentifier;

		using typename Common::RTIDTypeIdentifierEnumeration;

		using typename Common::RTIDTypeIdentifier;

		using typename Common::RTIDFormat;

		using Common::analysis_rtid;

		using Common::k_binary_blob_format;

		// ----------------

		inline static auto process_value (
			IByteStreamView &      data,
			JSON::Value &          value,
			List<CStringView> &    native_string_index,
			List<CStringView> &    unicode_string_index,
			TypeIdentifier const & type_identifier
		) -> Void {
			switch (type_identifier.value) {
				case TypeIdentifier::Value::boolean_false : {
					value.set_boolean(k_false);
					break;
				}
				case TypeIdentifier::Value::boolean_true : {
					value.set_boolean(k_true);
					break;
				}
				case TypeIdentifier::Value::integer_signed_8 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerS8>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_8_zero : {
					value.set_number(cbox<Integer>(0_is8));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_8 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerU8>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_8_zero : {
					value.set_number(cbox<Integer>(0_iu8));
					break;
				}
				case TypeIdentifier::Value::integer_signed_16 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerS16>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_16_zero : {
					value.set_number(cbox<Integer>(0_is16));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_16 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerU16>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_16_zero : {
					value.set_number(cbox<Integer>(0_iu16));
					break;
				}
				case TypeIdentifier::Value::integer_signed_32 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerS32>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_32_zero : {
					value.set_number(cbox<Integer>(0_is32));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_32 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerU32>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_32_zero : {
					value.set_number(cbox<Integer>(0_iu32));
					break;
				}
				case TypeIdentifier::Value::integer_signed_64 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerS64>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_64_zero : {
					value.set_number(cbox<Integer>(0_is64));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_64 : {
					value.set_number(cbox<Integer>(data.read_of<IntegerU64>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_64_zero : {
					value.set_number(cbox<Integer>(0_iu64));
					break;
				}
				case TypeIdentifier::Value::floater_signed_32 : {
					value.set_number(cbox<Floater>(data.read_of<FloaterS32>()));
					break;
				}
				case TypeIdentifier::Value::floater_signed_32_zero : {
					value.set_number(cbox<Floater>(0.0_fs32));
					break;
				}
				case TypeIdentifier::Value::floater_signed_64 : {
					value.set_number(cbox<Floater>(data.read_of<FloaterS64>()));
					break;
				}
				case TypeIdentifier::Value::floater_signed_64_zero : {
					value.set_number(cbox<Floater>(0.0_fs64));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_unsigned_32 :
				case TypeIdentifier::Value::integer_variable_length_unsigned_32_equivalent : {
					value.set_number(cbox<Integer>(ProtocolBufferVariableLengthInteger::decode_u32(data)));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_signed_32 : {
					value.set_number(cbox<Integer>(ProtocolBufferVariableLengthInteger::decode_s32(data)));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_unsigned_64 :
				case TypeIdentifier::Value::integer_variable_length_unsigned_64_equivalent : {
					value.set_number(cbox<Integer>(ProtocolBufferVariableLengthInteger::decode_u64(data)));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_signed_64 : {
					value.set_number(cbox<Integer>(ProtocolBufferVariableLengthInteger::decode_s64(data)));
					break;
				}
				case TypeIdentifier::Value::string_native : {
					auto   size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto & content = value.set_string();
					if constexpr (check_version(version, {}, {false})) {
						StringParser::read_eascii_string(self_cast<ICharacterStreamView>(data), content, size);
					}
					if constexpr (check_version(version, {}, {true})) {
						auto content_view = CStringView{};
						StringParser::read_utf8_string_by_size(self_cast<ICharacterStreamView>(data), content_view, as_lvalue(Size{}), size);
						content = content_view;
					}
					break;
				}
				case TypeIdentifier::Value::string_native_indexing : {
					auto   size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto & content = value.set_string();
					if constexpr (check_version(version, {}, {false})) {
						StringParser::read_eascii_string(self_cast<ICharacterStreamView>(data), content, size);
					}
					if constexpr (check_version(version, {}, {true})) {
						auto content_view = CStringView{};
						StringParser::read_utf8_string_by_size(self_cast<ICharacterStreamView>(data), content_view, as_lvalue(Size{}), size);
						content = content_view;
					}
					native_string_index.append(content);
					break;
				}
				case TypeIdentifier::Value::string_native_indexed : {
					auto index = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					value.set_string(native_string_index[index]);
					break;
				}
				case TypeIdentifier::Value::string_unicode : {
					auto length = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto content = CStringView{};
					StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), content, length);
					assert_test(content.size() == size);
					value.set_string(content);
					break;
				}
				case TypeIdentifier::Value::string_unicode_indexing : {
					auto length = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto content = CStringView{};
					StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), content, length);
					assert_test(content.size() == size);
					value.set_string(content);
					unicode_string_index.append(content);
					break;
				}
				case TypeIdentifier::Value::string_unicode_indexed : {
					auto index = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					value.set_string(unicode_string_index[index]);
					break;
				}
				case TypeIdentifier::Value::string_rtid : {
					switch (data.read_of<RTIDTypeIdentifier>().value) {
						case RTIDTypeIdentifier::Value::null : {
							value.set_string(RTIDFormat::null());
							break;
						}
						case RTIDTypeIdentifier::Value::uid : {
							auto sheet_length = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_content = CStringView{};
							StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), sheet_content, sheet_length);
							assert_test(sheet_content.size() == sheet_size);
							auto uid_middle = ProtocolBufferVariableLengthInteger::decode_u32(data);
							auto uid_first = ProtocolBufferVariableLengthInteger::decode_u32(data);
							auto uid_last = data.read_of<IntegerU32>();
							// TODO : unknown type of uid 's value , define them be 'var-int-u32'
							value.set_string(RTIDFormat::uid(uid_first, uid_middle, uid_last, sheet_content));
							break;
						}
						case RTIDTypeIdentifier::Value::alias : {
							auto sheet_length = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_content = CStringView{};
							StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), sheet_content, sheet_length);
							assert_test(sheet_content.size() == sheet_size);
							auto alias_length = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto alias_size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto alias_content = CStringView{};
							StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), alias_content, alias_length);
							assert_test(alias_content.size() == alias_size);
							value.set_string(RTIDFormat::alias(alias_content, sheet_content));
							break;
						}
						default : {
							assert_fail(R"(data.read_of<RTIDTypeIdentifier>() == /* valid */)");
						}
					}
					break;
				}
				case TypeIdentifier::Value::string_rtid_null : {
					value.set_string(RTIDFormat::null());
					break;
				}
				case TypeIdentifier::Value::array_begin : {
					auto & array = value.set_array();
					data.read_constant(TypeIdentifier{TypeIdentifier::Value::array_size});
					auto size = cbox<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					array.allocate(size);
					while (k_true) {
						auto value_type_identifier = data.read_of<TypeIdentifier>();
						if (value_type_identifier.value == TypeIdentifier::Value::array_end) {
							break;
						}
						array.append();
						process_value(data, array.last(), native_string_index, unicode_string_index, value_type_identifier);
					}
					assert_test(array.size() == size);
					break;
				}
				case TypeIdentifier::Value::object_begin : {
					auto & object = value.set_object();
					auto   member_list = std::list<JSON::Object::Element>{};
					while (k_true) {
						auto key_type_identifier = data.read_of<TypeIdentifier>();
						if (key_type_identifier.value == TypeIdentifier::Value::object_end) {
							break;
						}
						member_list.emplace_back();
						auto member_key = JSON::Value{};
						process_value(data, member_key, native_string_index, unicode_string_index, key_type_identifier);
						member_list.back().key = as_moveable(member_key.get_string());
						auto value_type_identifier = data.read_of<TypeIdentifier>();
						process_value(data, member_list.back().value, native_string_index, unicode_string_index, value_type_identifier);
					}
					object.assign(
						member_list,
						[] (auto & element) -> auto && {
							return as_moveable(element);
						}
					);
					break;
				}
				default : {
					assert_fail(R"(type_identifier == /* valid */)");
				}
			}
			return;
		}

		// ----------------

		inline static auto process_whole (
			IByteStreamView & data,
			JSON::Value &     definition
		) -> Void {
			data.read_constant(k_magic_identifier);
			data.read_constant(cbox<VersionNumber>(version.number));
			auto native_string_upper_bound = k_none_size;
			auto unicode_string_upper_bound = k_none_size;
			for (auto & element : data.reserve_view()) {
				switch (self_cast<TypeIdentifier>(element).value) {
					case TypeIdentifier::Value::string_native_indexing : {
						++native_string_upper_bound;
						break;
					}
					case TypeIdentifier::Value::string_unicode_indexing : {
						++unicode_string_upper_bound;
						break;
					}
						[[likely]]
					default : {
					}
				}
			}
			auto native_string_index_list = List<CStringView>{native_string_upper_bound};
			auto unicode_string_index_list = List<CStringView>{unicode_string_upper_bound};
			process_value(data, definition, native_string_index_list, unicode_string_index_list, TypeIdentifier{TypeIdentifier::Value::object_begin});
			data.read_constant(k_done_identifier);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & data_,
			JSON::Value &     definition
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_whole(data, definition);
		}

	};

}
