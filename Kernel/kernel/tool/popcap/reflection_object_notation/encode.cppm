module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.tool.popcap.reflection_object_notation.common;
import twinning.kernel.tool.common.protocol_buffer_variable_length_integer;

export namespace Twinning::Kernel::Tool::Popcap::ReflectionObjectNotation {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Encode :
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
			OutputByteStreamView &          data,
			Notation::Json::Boolean const & value
		) -> Void {
			data.write(!value ? (TypeIdentifier::Constant::boolean_false()) : (TypeIdentifier::Constant::boolean_true()));
			return;
		}

		inline static auto process_value(
			OutputByteStreamView &         data,
			Notation::Json::Number const & value
		) -> Void {
			if (value.is_integer()) {
				data.write(TypeIdentifier::Constant::integer_variable_length_signed_64());
				ProtocolBufferVariableLengthInteger::encode_s64(data, up_cast<IntegerS64>(value.get_integer()));
			}
			else {
				data.write(TypeIdentifier::Constant::floater_signed_64());
				data.write(up_cast<FloaterS64>(value.get_floater()));
			}
			return;
		}

		inline static auto process_value(
			OutputByteStreamView &                                   data,
			Notation::Json::String const &                           value,
			Optional<std::unordered_map<ConstantStringView, Size>> & native_string_index
		) -> Void {
			if (!native_string_index.has()) {
				data.write(TypeIdentifier::Constant::string_native());
				if constexpr (check_version(t_version, {}, {false})) {
					ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(StringParser::compute_utf8_string_length(value)));
					StringParser::write_eascii_string(unsafe_cast<OutputCharacterStreamView>(data), value, as_left(Size{}));
				}
				if constexpr (check_version(t_version, {}, {true})) {
					ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(value.size()));
					StringParser::write_utf8_string(unsafe_cast<OutputCharacterStreamView>(data), value.as_view(), as_left(Size{}));
				}
			}
			else {
				auto indexed_string = native_string_index.get().find(value);
				if (indexed_string != native_string_index.get().end()) {
					data.write(TypeIdentifier::Constant::string_native_indexed());
					ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>((*indexed_string).second));
				}
				else {
					native_string_index.get()[value] = make_box<Size>(native_string_index.get().size());
					data.write(TypeIdentifier::Constant::string_native_indexing());
					if constexpr (check_version(t_version, {}, {false})) {
						ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(StringParser::compute_utf8_string_length(value)));
						StringParser::write_eascii_string(unsafe_cast<OutputCharacterStreamView>(data), value, as_left(Size{}));
					}
					if constexpr (check_version(t_version, {}, {true})) {
						ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(value.size()));
						StringParser::write_utf8_string(unsafe_cast<OutputCharacterStreamView>(data), value.as_view(), as_left(Size{}));
					}
				}
			}
			return;
		}

		inline static auto process_value(
			OutputByteStreamView &                                   data,
			Notation::Json::String const &                           value,
			Optional<std::unordered_map<ConstantStringView, Size>> & native_string_index,
			Boolean const &                                          enable_reference
		) -> Void {
			auto is_reference = k_false;
			if (enable_reference) {
				auto reference_type = analysis_reference(value);
				if (reference_type.has()) {
					is_reference = k_true;
					data.write(TypeIdentifier::Constant::reference());
					data.write(reference_type.get());
					switch (reference_type.get().value) {
						case ReferenceTypeIdentifier::Constant::null().value: {
							break;
						}
						case ReferenceTypeIdentifier::Constant::identifier().value: {
							auto content = value.sub("RTID("_sl, value.size() - "RTID()"_sl);
							auto at_position = Range::find_index(content, '@'_c).get();
							auto sheet = content.tail(content.size() - (at_position + "@"_sl));
							auto identifier = content.head(at_position);
							auto identifier_stream = InputCharacterStreamView{identifier};
							auto identifier_first = Integer{};
							auto identifier_middle = Integer{};
							auto identifier_last = Integer{};
							StringParser::read_number(identifier_stream, identifier_first);
							identifier_stream.read_constant('.'_c);
							StringParser::read_number(identifier_stream, identifier_middle);
							identifier_stream.read_constant('.'_c);
							StringParser::read_number_hexadecimal(identifier_stream, identifier_last);
							assert_test(identifier_stream.full());
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(sheet.size()));
							data.write(sheet);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(identifier_middle));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(identifier_first));
							data.write(cast_box<IntegerU32>(identifier_last));
							break;
						}
						case ReferenceTypeIdentifier::Constant::alias().value: {
							auto content = value.sub("RTID("_sl, value.size() - "RTID()"_sl);
							auto at_position = Range::find_index(content, '@'_c).get();
							auto sheet = content.tail(content.size() - (at_position + "@"_sl));
							auto alias = content.head(at_position);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(sheet.size()));
							data.write(sheet);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(StringParser::compute_utf8_string_length(alias)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(alias.size()));
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

		inline static auto process_value(
			OutputByteStreamView &                                   data,
			Notation::Json::Array const &                            value,
			Optional<std::unordered_map<ConstantStringView, Size>> & native_string_index,
			Boolean const &                                          enable_reference
		) -> Void {
			data.write(TypeIdentifier::Constant::array_begin());
			data.write_constant(TypeIdentifier::Constant::array_size());
			ProtocolBufferVariableLengthInteger::encode_u32(data, cast_box<IntegerU32>(value.size()));
			for (auto & element : value) {
				process_value(data, element, native_string_index, enable_reference);
			}
			data.write(TypeIdentifier::Constant::array_end());
			return;
		}

		inline static auto process_value(
			OutputByteStreamView &                                   data,
			Notation::Json::Object const &                           value,
			Optional<std::unordered_map<ConstantStringView, Size>> & native_string_index,
			Boolean const &                                          enable_reference
		) -> Void {
			data.write(TypeIdentifier::Constant::object_begin());
			for (auto & element : value) {
				process_value(data, element.key, native_string_index);
				process_value(data, element.value, native_string_index, enable_reference);
			}
			data.write(TypeIdentifier::Constant::object_end());
			return;
		}

		inline static auto process_value(
			OutputByteStreamView &                                   data,
			Notation::Json::Value const &                            value,
			Optional<std::unordered_map<ConstantStringView, Size>> & native_string_index,
			Boolean const &                                          enable_reference
		) -> Void {
			switch (value.type().value) {
				case Notation::Json::ValueType::Constant::null().value: {
					assert_fail(R"(value.type() == /* non-null */)");
					break;
				}
				case Notation::Json::ValueType::Constant::boolean().value: {
					process_value(data, value.get_boolean());
					break;
				}
				case Notation::Json::ValueType::Constant::number().value: {
					process_value(data, value.get_number());
					break;
				}
				case Notation::Json::ValueType::Constant::string().value: {
					process_value(data, value.get_string(), native_string_index, enable_reference);
					break;
				}
				case Notation::Json::ValueType::Constant::array().value: {
					process_value(data, value.get_array(), native_string_index, enable_reference);
					break;
				}
				case Notation::Json::ValueType::Constant::object().value: {
					process_value(data, value.get_object(), native_string_index, enable_reference);
					break;
				}
				default: throw UnreachableException{};
			}
			return;
		}

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView &        data,
			Notation::Json::Value const & definition,
			Boolean const &               enable_string_index,
			Boolean const &               enable_reference
		) -> Void {
			data.write_constant(k_magic_marker);
			auto version_data = OutputByteStreamView{data.forward_view(bs_static_size<VersionNumber>())};
			data.backward(bs_static_size<TypeIdentifier>());
			auto native_string_index = Optional<std::unordered_map<ConstantStringView, Size>>{};
			if (enable_string_index) {
				native_string_index.set();
			}
			process_value(data, definition.get_object(), native_string_index, enable_reference);
			data.write_constant(k_done_marker);
			version_data.write_constant(cast_box<VersionNumber>(t_version.number));
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView &        data_,
			Notation::Json::Value const & definition,
			Boolean const &               enable_string_index,
			Boolean const &               enable_reference
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition, enable_string_index, enable_reference);
		}

	};

}
