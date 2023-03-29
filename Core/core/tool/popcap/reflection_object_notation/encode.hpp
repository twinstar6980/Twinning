#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/reflection_object_notation/version.hpp"
#include "core/tool/common/protocol_buffer_variable_length_integer.hpp"
#include <unordered_map>

namespace TwinStar::Core::Tool::PopCap::ReflectionObjectNotation {

	template <auto version> requires (check_version(version, {}, {}))
	struct EncodeCommon {

	protected:

		using MagicIdentifier = StaticByteArray<4_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{'R'_b, 'T'_b, 'O'_b, 'N'_b}};

		// ----------------

		using DoneIdentifier = StaticByteArray<4_sz>;

		inline static constexpr auto k_done_identifier = DoneIdentifier{{'D'_b, 'O'_b, 'N'_b, 'E'_b}};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		struct TypeIdentifierEnumeration {
			enum class Type : ZByte {
				// boolean
				boolean_false = 0x00,
				boolean_true  = 0x01,
				// integer
				integer_signed_8         = 0x08,
				integer_signed_8_zero    = 0x09,
				integer_unsigned_8       = 0x0A,
				integer_unsigned_8_zero  = 0x0B,
				integer_signed_16        = 0x10,
				integer_signed_16_zero   = 0x11,
				integer_unsigned_16      = 0x12,
				integer_unsigned_16_zero = 0x13,
				integer_signed_32        = 0x20,
				integer_signed_32_zero   = 0x21,
				integer_unsigned_32      = 0x26,
				integer_unsigned_32_zero = 0x27,
				integer_signed_64        = 0x40,
				integer_signed_64_zero   = 0x41,
				integer_unsigned_64      = 0x46,
				integer_unsigned_64_zero = 0x47,
				// floating
				floating_signed_32      = 0x22,
				floating_signed_32_zero = 0x23,
				floating_signed_64      = 0x42,
				floating_signed_64_zero = 0x43,
				// variable length integer
				integer_variable_length_unsigned_32            = 0x24,
				integer_variable_length_signed_32              = 0x25,
				integer_variable_length_unsigned_32_equivalent = 0x28,
				integer_variable_length_unsigned_64            = 0x44,
				integer_variable_length_signed_64              = 0x45,
				integer_variable_length_unsigned_64_equivalent = 0x48,
				// native string
				string_native          = 0x81,
				string_native_indexing = 0x90,
				string_native_indexed  = 0x91,
				// unicode string
				string_unicode          = 0x82,
				string_unicode_indexing = 0x92,
				string_unicode_indexed  = 0x93,
				// rtid string
				string_rtid      = 0x83,
				string_rtid_null = 0x84,
				// array
				array_begin = 0x86,
				array_size  = 0xFD,
				array_end   = 0xFE,
				// object
				object_begin = 0x85,
				object_end   = 0xFF,
				// TODO : never appeared in known rton file
				_string_literal_star         = 0x02,
				_string_binary_blob          = 0x87,
				_string_native_x1            = 0xB0,
				_string_native_x2            = 0xB1,
				_string_unicode_x1           = 0xB2,
				_string_unicode_x2           = 0xB3,
				_string_native_or_unicode_x1 = 0xB4,
				_string_native_or_unicode_x2 = 0xB5,
				_string_native_or_unicode_x3 = 0xB6,
				_string_native_or_unicode_x4 = 0xB7,
				_object_begin_x1             = 0xB8,
				_array_begin_x1              = 0xB9,
				_string_native_x3            = 0xBA,
				_string_binary_blob_x1       = 0xBB,
				_boolean_x1                  = 0xBC,
			};
		};

		using TypeIdentifier = Enumeration<typename TypeIdentifierEnumeration::Type>;

		// ----------------

		struct RTIDTypeIdentifierEnumeration {
			enum class Type : ZByte {
				null  = 0x00,
				uid   = 0x02,
				alias = 0x03,
			};
		};

		using RTIDTypeIdentifier = Enumeration<typename RTIDTypeIdentifierEnumeration::Type>;

		struct RTIDFormat {
			inline static constexpr auto null = StringFormatter{"RTID(0)"_sf};
			inline static constexpr auto uid = StringFormatter{"RTID({:d}.{:d}.{:08x}@{:s})"_sf};
			inline static constexpr auto alias = StringFormatter{"RTID({:s}@{:s})"_sf};
		};

		static auto analysis_rtid (
			CStringView const & string
		) -> Optional<RTIDTypeIdentifier> {
			if (!(string.size() > "RTID()"_sl && string.head("RTID("_sl) == "RTID("_sv && string.tail(")"_sl) == ")"_sv)) {
				return k_null_optional;
			}
			auto content = string.sub("RTID("_sl, string.size() - "RTID()"_sl);
			if (content == "0"_sv) {
				return make_optional_of(RTIDTypeIdentifier{RTIDTypeIdentifier::Value::null});
			}
			auto at_position = Range::find_index(content, '@'_c);
			if (!at_position) {
				return k_null_optional;
			}
			// TODO : should test the content is number or not ?
			if (Range::count(content.head(at_position.get()), '.'_c) == 2_sz) {
				return make_optional_of(RTIDTypeIdentifier{RTIDTypeIdentifier::Value::uid});
			} else {
				return make_optional_of(RTIDTypeIdentifier{RTIDTypeIdentifier::Value::alias});
			}
		}

		// ----------------

		inline static constexpr auto k_binary_blob_format = StringFormatter{R"($BINARY("{:s}", {:d}))"_sf};

	};

	template <auto version> requires (check_version(version, {}, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

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

		static auto process_unit (
			OByteStreamView &     data,
			JSON::Boolean const & value
		) -> Void {
			data.write(TypeIdentifier{!value ? (TypeIdentifier::Value::boolean_false) : (TypeIdentifier::Value::boolean_true)});
			return;
		}

		static auto process_unit (
			OByteStreamView &    data,
			JSON::Number const & value
		) -> Void {
			if (value.is_integer()) {
				data.write(TypeIdentifier{TypeIdentifier::Value::integer_variable_length_signed_64});
				ProtocolBufferVariableLengthInteger::encode_s64(data, up_cast<IntegerS64>(value.get_integer()));
			} else {
				data.write(TypeIdentifier{TypeIdentifier::Value::floating_signed_64});
				data.write(up_cast<FloatingS64>(value.get_floating()));
			}
			return;
		}

		static auto process_unit (
			OByteStreamView &                                 data,
			JSON::String const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index
		) -> Void {
			if (!native_string_index) {
				data.write(TypeIdentifier{TypeIdentifier::Value::string_native});
				if constexpr (check_version(version, {}, {false})) {
					ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(StringParser::compute_utf8_string_length(value)));
					StringParser::write_eascii_string(self_cast<OCharacterStreamView>(data), value, as_lvalue(Size{}));
				}
				if constexpr (check_version(version, {}, {true})) {
					ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(value.size()));
					StringParser::write_utf8_string(self_cast<OCharacterStreamView>(data), value.as_view(), as_lvalue(Size{}));
				}
			} else {
				if (auto indexed_string = native_string_index.get().find(value); indexed_string != native_string_index.get().end()) {
					data.write(TypeIdentifier{TypeIdentifier::Value::string_native_indexed});
					ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>((*indexed_string).second));
				} else {
					native_string_index.get()[value] = mbw<Size>(native_string_index.get().size());
					data.write(TypeIdentifier{TypeIdentifier::Value::string_native_indexing});
					if constexpr (check_version(version, {}, {false})) {
						ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(StringParser::compute_utf8_string_length(value)));
						StringParser::write_eascii_string(self_cast<OCharacterStreamView>(data), value, as_lvalue(Size{}));
					}
					if constexpr (check_version(version, {}, {true})) {
						ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(value.size()));
						StringParser::write_utf8_string(self_cast<OCharacterStreamView>(data), value.as_view(), as_lvalue(Size{}));
					}
				}
			}
			return;
		}

		static auto process_unit (
			OByteStreamView &                                 data,
			JSON::String const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_rtid
		) -> Void {
			auto is_rtid = k_false;
			if (enable_rtid) {
				if (auto rtid_type = analysis_rtid(value)) {
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
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(sheet.size()));
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
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(StringParser::compute_utf8_string_length(sheet)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(sheet.size()));
							data.write(sheet);
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(StringParser::compute_utf8_string_length(alias)));
							ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(alias.size()));
							data.write(alias);
							break;
						}
					}
				}
			}
			if (!is_rtid) {
				process_unit(data, value, native_string_index);
			}
			return;
		}

		static auto process_unit (
			OByteStreamView &                                 data,
			JSON::Array const &                               value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_rtid
		) -> Void {
			data.write(TypeIdentifier{TypeIdentifier::Value::array_begin});
			data.write_constant(TypeIdentifier{TypeIdentifier::Value::array_size});
			ProtocolBufferVariableLengthInteger::encode_u32(data, cbw<IntegerU32>(value.size()));
			for (auto & element : value) {
				process_unit(data, element, native_string_index, enable_rtid);
			}
			data.write(TypeIdentifier{TypeIdentifier::Value::array_end});
			return;
		}

		static auto process_unit (
			OByteStreamView &                                 data,
			JSON::Object const &                              value,
			Optional<std::unordered_map<CStringView, Size>> & native_string_index,
			Boolean const &                                   enable_rtid
		) -> Void {
			data.write(TypeIdentifier{TypeIdentifier::Value::object_begin});
			for (auto & element : value) {
				process_unit(data, element.key, native_string_index);
				process_unit(data, element.value, native_string_index, enable_rtid);
			}
			data.write(TypeIdentifier{TypeIdentifier::Value::object_end});
			return;
		}

		static auto process_unit (
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
					process_unit(data, value.get_boolean());
					break;
				}
				case JSON::ValueType::Constant::number().value : {
					process_unit(data, value.get_number());
					break;
				}
				case JSON::ValueType::Constant::string().value : {
					process_unit(data, value.get_string(), native_string_index, enable_rtid);
					break;
				}
				case JSON::ValueType::Constant::array().value : {
					process_unit(data, value.get_array(), native_string_index, enable_rtid);
					break;
				}
				case JSON::ValueType::Constant::object().value : {
					process_unit(data, value.get_object(), native_string_index, enable_rtid);
					break;
				}
			}
			return;
		}

		static auto process_whole (
			OByteStreamView &   data,
			JSON::Value const & value,
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
			process_unit(data, value.get_object(), native_string_index, enable_rtid);
			data.write_constant(k_done_identifier);
			version_data.write_constant(cbw<VersionNumber>(version.number));
			return;
		}

	public:

		static auto do_process_whole (
			OByteStreamView &   data_,
			JSON::Value const & value,
			Boolean const &     enable_string_index,
			Boolean const &     enable_rtid
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, value, enable_string_index, enable_rtid);
		}

	};

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

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

		static auto process_unit (
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
					value.set_number(cbw<Integer>(data.read_of<IntegerS8>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_8_zero : {
					value.set_number(cbw<Integer>(0_is8));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_8 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerU8>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_8_zero : {
					value.set_number(cbw<Integer>(0_iu8));
					break;
				}
				case TypeIdentifier::Value::integer_signed_16 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerS16>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_16_zero : {
					value.set_number(cbw<Integer>(0_is16));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_16 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerU16>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_16_zero : {
					value.set_number(cbw<Integer>(0_iu16));
					break;
				}
				case TypeIdentifier::Value::integer_signed_32 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerS32>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_32_zero : {
					value.set_number(cbw<Integer>(0_is32));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_32 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerU32>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_32_zero : {
					value.set_number(cbw<Integer>(0_iu32));
					break;
				}
				case TypeIdentifier::Value::integer_signed_64 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerS64>()));
					break;
				}
				case TypeIdentifier::Value::integer_signed_64_zero : {
					value.set_number(cbw<Integer>(0_is64));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_64 : {
					value.set_number(cbw<Integer>(data.read_of<IntegerU64>()));
					break;
				}
				case TypeIdentifier::Value::integer_unsigned_64_zero : {
					value.set_number(cbw<Integer>(0_iu64));
					break;
				}
				case TypeIdentifier::Value::floating_signed_32 : {
					value.set_number(cbw<Floating>(data.read_of<FloatingS32>()));
					break;
				}
				case TypeIdentifier::Value::floating_signed_32_zero : {
					value.set_number(cbw<Floating>(0.0_fs32));
					break;
				}
				case TypeIdentifier::Value::floating_signed_64 : {
					value.set_number(cbw<Floating>(data.read_of<FloatingS64>()));
					break;
				}
				case TypeIdentifier::Value::floating_signed_64_zero : {
					value.set_number(cbw<Floating>(0.0_fs64));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_unsigned_32 :
				case TypeIdentifier::Value::integer_variable_length_unsigned_32_equivalent : {
					value.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_u32(data)));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_signed_32 : {
					value.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_s32(data)));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_unsigned_64 :
				case TypeIdentifier::Value::integer_variable_length_unsigned_64_equivalent : {
					value.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_u64(data)));
					break;
				}
				case TypeIdentifier::Value::integer_variable_length_signed_64 : {
					value.set_number(cbw<Integer>(ProtocolBufferVariableLengthInteger::decode_s64(data)));
					break;
				}
				case TypeIdentifier::Value::string_native : {
					auto   size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
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
					auto   size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
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
					auto index = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					value.set_string(native_string_index[index]);
					break;
				}
				case TypeIdentifier::Value::string_unicode : {
					auto length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto content = CStringView{};
					StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), content, length);
					assert_test(content.size() == size);
					value.set_string(content);
					break;
				}
				case TypeIdentifier::Value::string_unicode_indexing : {
					auto length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					auto content = CStringView{};
					StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), content, length);
					assert_test(content.size() == size);
					value.set_string(content);
					unicode_string_index.append(content);
					break;
				}
				case TypeIdentifier::Value::string_unicode_indexed : {
					auto index = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
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
							auto sheet_length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
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
							auto sheet_length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto sheet_content = CStringView{};
							StringParser::read_utf8_string(self_cast<ICharacterStreamView>(data), sheet_content, sheet_length);
							assert_test(sheet_content.size() == sheet_size);
							auto alias_length = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
							auto alias_size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
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
					auto size = cbw<Size>(ProtocolBufferVariableLengthInteger::decode_u32(data));
					array.allocate(size);
					while (k_true) {
						auto value_type_identifier = data.read_of<TypeIdentifier>();
						if (value_type_identifier.value == TypeIdentifier::Value::array_end) {
							break;
						}
						array.append();
						process_unit(data, array.last(), native_string_index, unicode_string_index, value_type_identifier);
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
						process_unit(data, member_key, native_string_index, unicode_string_index, key_type_identifier);
						member_list.back().key = as_moveable(member_key.get_string());
						auto value_type_identifier = data.read_of<TypeIdentifier>();
						process_unit(data, member_list.back().value, native_string_index, unicode_string_index, value_type_identifier);
					}
					object.assign(
						member_list,
						[] (auto & element) -> auto&& {
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

		static auto process_whole (
			IByteStreamView & data,
			JSON::Value &     value
		) -> Void {
			data.read_constant(k_magic_identifier);
			data.read_constant(cbw<VersionNumber>(version.number));
			value.set_null();
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
			process_unit(data, value, native_string_index_list, unicode_string_index_list, TypeIdentifier{TypeIdentifier::Value::object_begin});
			data.read_constant(k_done_identifier);
			return;
		}

	public:

		static auto do_process_whole (
			IByteStreamView & data_,
			JSON::Value &     value
		) -> Void {
			M_use_zps_of(data);
			restruct(value);
			return process_whole(data, value);
		}

	};

}
