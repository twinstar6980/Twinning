module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.common.protocol_buffer_variable_length_integer;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::ProtocolBufferVariableLengthInteger {

	#pragma region zig zag

	namespace ZigZag {

		#pragma region encode & decode

		inline auto encode_32(
			IntegerS32 const & value
		) -> IntegerU32 {
			return cast_box_unsafe<IntegerU32>(value << 1_sz ^ value >> 31_sz);
		}

		inline auto encode_64(
			IntegerS64 const & value
		) -> IntegerU64 {
			return cast_box_unsafe<IntegerU64>(value << 1_sz ^ value >> 63_sz);
		}

		// ----------------

		inline auto decode_32(
			IntegerU32 const & value
		) -> IntegerS32 {
			return cast_box_unsafe<IntegerS32>(value >> 1_sz) ^ -cast_box_unsafe<IntegerS32>(value & 1_iu32);
		}

		inline auto decode_64(
			IntegerU64 const & value
		) -> IntegerS64 {
			return cast_box_unsafe<IntegerS64>(value >> 1_sz) ^ -cast_box_unsafe<IntegerS64>(value & 1_iu64);
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region parameter

	inline constexpr auto k_shift = Size{7_sz};

	inline constexpr auto k_top = Byte{0b1_b << k_shift};

	#pragma endregion

	#pragma region encode & decode for unsigned

	inline auto encode_u32(
		OutputBinaryStreamView & stream,
		IntegerU32               value
	) -> Void {
		while (value >= cast_box<IntegerU32>(k_top)) {
			stream.write(cast_box_unsafe<Byte>(value) & ~k_top | k_top);
			value >>= k_shift;
		}
		stream.write(cast_box_unsafe<Byte>(value));
		return;
	}

	inline auto encode_u64(
		OutputBinaryStreamView & stream,
		IntegerU64               value
	) -> Void {
		while (value >= cast_box<IntegerU64>(k_top)) {
			stream.write(cast_box_unsafe<Byte>(value) & ~k_top | k_top);
			value >>= k_shift;
		}
		stream.write(cast_box_unsafe<Byte>(value));
		return;
	}

	// ----------------

	inline auto decode_u32(
		InputBinaryStreamView & stream
	) -> IntegerU32 {
		auto value = 0_iu32;
		for (auto & shift_index : SizeRange{Math::compute_padded_unit_count(type_bit_size_of<IntegerU32>(), k_shift)}) {
			auto shift_count = k_shift * shift_index;
			auto byte = stream.read_of();
			if ((byte & k_top) == 0_b) {
				assert_test(shift_count != 28_sz || byte <= 0b0000'1111_b);
				value |= cast_box<IntegerU32>(byte) << shift_count;
				break;
			}
			else {
				assert_test(shift_count != 28_sz);
				value |= cast_box<IntegerU32>(byte & ~k_top) << shift_count;
			}
		}
		return value;
	}

	inline auto decode_u64(
		InputBinaryStreamView & stream
	) -> IntegerU64 {
		auto value = 0_iu64;
		for (auto & shift_index : SizeRange{Math::compute_padded_unit_count(type_bit_size_of<IntegerU64>(), k_shift)}) {
			auto shift_count = k_shift * shift_index;
			auto byte = stream.read_of();
			if ((byte & k_top) == 0_b) {
				assert_test(shift_count != 63_sz || byte <= 0b0000000'1_b);
				value |= cast_box<IntegerU64>(byte) << shift_count;
				break;
			}
			else {
				assert_test(shift_count != 63_sz);
				value |= cast_box<IntegerU64>(byte & ~k_top) << shift_count;
			}
		}
		return value;
	}

	#pragma endregion

	#pragma region encode & decode for signed

	inline auto encode_s32(
		OutputBinaryStreamView & stream,
		IntegerS32 const &       value
	) -> Void {
		return encode_u32(stream, ZigZag::encode_32(value));
	}

	inline auto encode_s64(
		OutputBinaryStreamView & stream,
		IntegerS64 const &       value
	) -> Void {
		return encode_u64(stream, ZigZag::encode_64(value));
	}

	// ----------------

	inline auto decode_s32(
		InputBinaryStreamView & stream
	) -> IntegerS32 {
		return ZigZag::decode_32(decode_u32(stream));
	}

	inline auto decode_s64(
		InputBinaryStreamView & stream
	) -> IntegerS64 {
		return ZigZag::decode_64(decode_u64(stream));
	}

	#pragma endregion

}
