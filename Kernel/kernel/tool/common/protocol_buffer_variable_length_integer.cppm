module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.common.protocol_buffer_variable_length_integer;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::ProtocolBufferVariableLengthInteger {

	#pragma region zig zag

	namespace ZigZag {

		#pragma region encode & decode

		inline auto encode_32 (
			IntegerS32 const & value
		) -> IntegerU32 {
			return cbox<IntegerU32>(value << 1_sz ^ value >> 31_sz);
		}

		inline auto encode_64 (
			IntegerS64 const & value
		) -> IntegerU64 {
			return cbox<IntegerU64>(value << 1_sz ^ value >> 63_sz);
		}

		// ----------------

		inline auto decode_32 (
			IntegerU32 const & value
		) -> IntegerS32 {
			return cbox<IntegerS32>(value >> 1_sz) ^ -cbox<IntegerS32>(value & 1_iu32);
		}

		inline auto decode_64 (
			IntegerU64 const & value
		) -> IntegerS64 {
			return cbox<IntegerS64>(value >> 1_sz) ^ -cbox<IntegerS64>(value & 1_iu64);
		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region parameter

	inline constexpr auto k_z = Byte{0b00000000_b};

	inline constexpr auto k_b = Byte{0b10000000_b};

	inline constexpr auto k_x = Byte{0b01111111_b};

	#pragma endregion

	#pragma region encode & decode for unsigned

	inline auto encode_u32 (
		OByteStreamView & stream,
		IntegerU32        value
	) -> Void {
		while (value >= cbox<IntegerU32>(k_b)) {
			stream.write(cbox<Byte>(value) & k_x | k_b);
			value >>= 7_sz;
		}
		stream.write(cbox<Byte>(value));
		return;
	}

	inline auto encode_u64 (
		OByteStreamView & stream,
		IntegerU64        value
	) -> Void {
		while (value >= cbox<IntegerU64>(k_b)) {
			stream.write(cbox<Byte>(value) & k_x | k_b);
			value >>= 7_sz;
		}
		stream.write(cbox<Byte>(value));
		return;
	}

	// ----------------

	inline auto decode_u32 (
		IByteStreamView & stream
	) -> IntegerU32 {
		auto value = 0_iu32;
		for (auto & shift_count : SizeRange{k_begin_index, k_type_bit_count<IntegerU32>, 7_sz}) {
			auto byte = stream.read_of();
			if ((byte & k_b) != k_z) {
				value |= cbox<IntegerU32>(byte & k_x) << shift_count;
			}
			else {
				value |= cbox<IntegerU32>(byte) << shift_count;
				break;
			}
		}
		return value;
	}

	inline auto decode_u64 (
		IByteStreamView & stream
	) -> IntegerU64 {
		auto value = 0_iu64;
		for (auto & shift_count : SizeRange{k_begin_index, k_type_bit_count<IntegerU64>, 7_sz}) {
			auto byte = stream.read_of();
			if ((byte & k_b) != k_z) {
				value |= cbox<IntegerU64>(byte & k_x) << shift_count;
			}
			else {
				value |= cbox<IntegerU64>(byte) << shift_count;
				break;
			}
		}
		return value;
	}

	#pragma endregion

	#pragma region encode & decode for signed

	inline auto encode_s32 (
		OByteStreamView &  stream,
		IntegerS32 const & value
	) -> Void {
		return encode_u32(stream, ZigZag::encode_32(value));
	}

	inline auto encode_s64 (
		OByteStreamView &  stream,
		IntegerS64 const & value
	) -> Void {
		return encode_u64(stream, ZigZag::encode_64(value));
	}

	// ----------------

	inline auto decode_s32 (
		IByteStreamView & stream
	) -> IntegerS32 {
		return ZigZag::decode_32(decode_u32(stream));
	}

	inline auto decode_s64 (
		IByteStreamView & stream
	) -> IntegerS64 {
		return ZigZag::decode_64(decode_u64(stream));
	}

	#pragma endregion

}
