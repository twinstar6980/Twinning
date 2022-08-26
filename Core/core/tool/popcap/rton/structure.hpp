#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/rton/version.hpp"

namespace TwinKleS::Core::Tool::PopCap::RTON::Structure {

	#pragma region magic identifier

	using MagicIdentifier = StaticByteArray<4_szz>;

	inline constexpr auto k_magic_identifier = MagicIdentifier{{'R'_b, 'T'_b, 'O'_b, 'N'_b}};

	#pragma endregion

	#pragma region version number

	using VersionNumber = IntegerU32;

	#pragma endregion

	#pragma region done identifier

	using DoneIdentifier = StaticByteArray<4_szz>;

	inline constexpr auto k_done_identifier = DoneIdentifier{{'D'_b, 'O'_b, 'N'_b, 'E'_b}};

	#pragma endregion

	#pragma region type identifier

	enum class TypeIdentifier : ZByte {
		// boolean
		boolean_false = 0x00,
		boolean_true = 0x01,
		// integer
		integer_signed_8 = 0x08,
		integer_signed_8_zero = 0x09,
		integer_unsigned_8 = 0x0A,
		integer_unsigned_8_zero = 0x0B,
		integer_signed_16 = 0x10,
		integer_signed_16_zero = 0x11,
		integer_unsigned_16 = 0x12,
		integer_unsigned_16_zero = 0x13,
		integer_signed_32 = 0x20,
		integer_signed_32_zero = 0x21,
		integer_unsigned_32 = 0x26,
		integer_unsigned_32_zero = 0x27,
		integer_signed_64 = 0x40,
		integer_signed_64_zero = 0x41,
		integer_unsigned_64 = 0x46,
		integer_unsigned_64_zero = 0x47,
		// floating
		floating_32 = 0x22,
		floating_32_zero = 0x23,
		floating_64 = 0x42,
		floating_64_zero = 0x43,
		// variable length integer
		integer_variable_length_unsigned_32 = 0x24,
		integer_variable_length_signed_32 = 0x25,
		integer_variable_length_unsigned_32_equivalent = 0x28,
		integer_variable_length_unsigned_64 = 0x44,
		integer_variable_length_signed_64 = 0x45,
		integer_variable_length_unsigned_64_equivalent = 0x48,
		// native string
		string_native = 0x81,
		string_native_indexing = 0x90,
		string_native_indexed = 0x91,
		// unicode string
		string_unicode = 0x82,
		string_unicode_indexing = 0x92,
		string_unicode_indexed = 0x93,
		// rtid string
		string_rtid = 0x83,
		string_rtid_null = 0x84,
		// array
		array_begin = 0x86,
		array_size = 0xFD,
		array_end = 0xFE,
		// object
		object_begin = 0x85,
		object_end = 0xFF,
		// todo : never appeared in known rton file
		_binary_cast = 0x87,
		_star = 0x02,
		_string_x1 = 0xB0,
		_string_x2 = 0xB1,
		_wide_string_x1 = 0xB2,
		_wide_string_x2 = 0xB3,
		_string_or_wide_string_x1 = 0xB4,
		_string_or_wide_string_x2 = 0xB5,
		_string_or_wide_string_x3 = 0xB6,
		_string_or_wide_string_x4 = 0xB7,
		_object_begin_x1 = 0xB8,
		_array_begin_x1 = 0xB9,
		_string_x3 = 0xBA,
		_binary_cast_x1 = 0xBB,
		_boolean_x1 = 0xBC,
	};

	#pragma endregion

	#pragma region rtid

	enum class RTIDTypeIdentifier : ZByte {
		null = 0x00,
		uid = 0x02,
		alias = 0x03,
	};

	namespace RTIDFormat {
		inline constexpr auto null = StringFormatter{"RTID(0)"_sf};
		inline constexpr auto uid = StringFormatter{"RTID({:d}.{:d}.{:08x}@{:s})"_sf};
		inline constexpr auto alias = StringFormatter{"RTID({:s}@{:s})"_sf};
	}

	// ----------------

	inline constexpr auto analysis_rtid (
		CStringView const & string
	) -> Optional<RTIDTypeIdentifier> {
		if (!(string.size() > "RTID()"_sl && string.head("RTID("_sl) == "RTID("_sv && string.tail(")"_sl) == ")"_sv)) {
			return k_null_optional;
		}
		auto content = string.sub("RTID("_sl, string.size() - "RTID()"_sl);
		if (content == "0"_sv) {
			return make_optional_of(RTIDTypeIdentifier::null);
		}
		auto at_position = Range::find_index(content, '@'_c);
		if (!at_position) {
			return k_null_optional;
		}
		// TODO : should test the content is number or not ?
		if (Range::count(content.head(at_position.get()), '.'_c) == 2_sz) {
			return make_optional_of(RTIDTypeIdentifier::uid);
		} else {
			return make_optional_of(RTIDTypeIdentifier::alias);
		}
	}

	#pragma endregion

	#pragma region binary cast

	inline constexpr auto k_binary_cast_format = StringFormatter{R"($BINARY("{:s}", {:d}))"_sf};

	#pragma endregion

}
