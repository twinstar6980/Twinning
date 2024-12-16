module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.tool.popcap.reflection_object_notation.common;
import twinning.kernel.tool.common.protocol_buffer_variable_length_integer;

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

		using typename Common::RTIDTypeIdentifierEnumeration;

		using typename Common::RTIDTypeIdentifier;

		using typename Common::RTIDFormat;

		using Common::analysis_rtid;

		using Common::k_binary_blob_format;

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
			Boolean const &                                   enable_rtid
		) -> Void {
			auto is_rtid = k_false;
			if (enable_rtid) {
				if (auto rtid_type = analysis_rtid(value); rtid_type.has()) {
					is_rtid = k_true;
					data.write(TypeIdentifier{TypeIdentifier::Value::string_rtid});
					data.write(rtid_type.get());
					switch (rtid_type.get().value) {
						case RTIDTypeIdentifier::Value::null : {
							break;
						}
						case RTIDTypeIdentifier::Value::uid : {
							auto content = value.sub("RTID("_sl, value.size() - "RTID()"_sl);
							auto at_position = Range::find_index(content, '@'_c).get();
							auto sheet = content.tail(content.size() - (at_position + "@"_sl));
							auto uid = content.head(at_position);
							auto uid_first = IntegerU32{};
							auto uid_middle = IntegerU32{};
							auto uid_last = IntegerU32{};
							// TODO : do not use scanf ?
							auto scan_result = std::sscanf(cast_pointer<char>(make_null_terminated_string(uid).begin()).value, "%u.%u.%x", &uid_first.value, &uid_middle.value, &uid_last.value);
							assert_test(scan_result == 3);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(sheet.size()));
							data.write(sheet);
							ProtocolBufferVariableLengthInteger::encode_u32(data, uid_middle);
							ProtocolBufferVariableLengthInteger::encode_u32(data, uid_first);
							data.write(uid_last);
							break;
						}
						case RTIDTypeIdentifier::Value::alias : {
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
					}
				}
			}
			if (!is_rtid) {
				process_value(data, value, native_string_index);
			}
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::Array const &                               value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_rtid
		) -> Void {
			data.write(TypeIdentifier{TypeIdentifier::Value::array_begin});
			data.write_constant(TypeIdentifier{TypeIdentifier::Value::array_size});
			ProtocolBufferVariableLengthInteger::encode_u32(data, cbox<IntegerU32>(value.size()));
			for (auto & element : value) {
				process_value(data, element, native_string_index, enable_rtid);
			}
			data.write(TypeIdentifier{TypeIdentifier::Value::array_end});
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::Object const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_rtid
		) -> Void {
			data.write(TypeIdentifier{TypeIdentifier::Value::object_begin});
			for (auto & element : value) {
				process_value(data, element.key, native_string_index);
				process_value(data, element.value, native_string_index, enable_rtid);
			}
			data.write(TypeIdentifier{TypeIdentifier::Value::object_end});
			return;
		}

		inline static auto process_value (
			OByteStreamView &                                 data,
			JSON::Value const &                               value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_rtid
		) -> Void {
			switch (value.type().value) {
				case JSON::ValueType::Constant::null().value : {
					assert_fail(R"(value.type() == /* non-null */)");
					break;
				}
				case JSON::ValueType::Constant::boolean().value : {
					process_value(data, value.get_boolean());
					break;
				}
				case JSON::ValueType::Constant::number().value : {
					process_value(data, value.get_number());
					break;
				}
				case JSON::ValueType::Constant::string().value : {
					process_value(data, value.get_string(), native_string_index, enable_rtid);
					break;
				}
				case JSON::ValueType::Constant::array().value : {
					process_value(data, value.get_array(), native_string_index, enable_rtid);
					break;
				}
				case JSON::ValueType::Constant::object().value : {
					process_value(data, value.get_object(), native_string_index, enable_rtid);
					break;
				}
			}
			return;
		}

		// ----------------

		inline static auto process_whole (
			OByteStreamView &   data,
			JSON::Value const & definition,
			Boolean const &     enable_string_index,
			Boolean const &     enable_rtid
		) -> Void {
			data.write_constant(k_magic_identifier);
			auto version_data = OByteStreamView{data.forward_view(bs_static_size<VersionNumber>())};
			data.backward(bs_static_size<TypeIdentifier>());
			auto native_string_index = Optional<std::unordered_map<CStringView, Size>>{};
			if (enable_string_index) {
				native_string_index.set();
			}
			process_value(data, definition.get_object(), native_string_index, enable_rtid);
			data.write_constant(k_done_identifier);
			version_data.write_constant(cbox<VersionNumber>(version.number));
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &   data_,
			JSON::Value const & definition,
			Boolean const &     enable_string_index,
			Boolean const &     enable_rtid
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition, enable_string_index, enable_rtid);
		}

	};

}
