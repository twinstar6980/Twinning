module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.reflection_object_notation.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.reflection_object_notation.version;

export namespace Twinning::Kernel::Tool::PopCap::ReflectionObjectNotation {

	template <auto version> requires (check_version(version, {}, {}))
	struct Common {

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
				// floater
				floater_signed_32      = 0x22,
				floater_signed_32_zero = 0x23,
				floater_signed_64      = 0x42,
				floater_signed_64_zero = 0x43,
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

		inline static auto analysis_rtid (
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
			if (!at_position.has()) {
				return k_null_optional;
			}
			// TODO : should test the content is number or not ?
			if (Range::count(content.head(at_position.get()), '.'_c) == 2_sz) {
				return make_optional_of(RTIDTypeIdentifier{RTIDTypeIdentifier::Value::uid});
			}
			else {
				return make_optional_of(RTIDTypeIdentifier{RTIDTypeIdentifier::Value::alias});
			}
		}

		// ----------------

		inline static constexpr auto k_binary_blob_format = StringFormatter{R"($BINARY("{:s}", {:d}))"_sf};

	};

}
