module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.memory.bitwise;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Bitwise {

	#pragma region basic

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline constexpr auto extract(
		TValue const & value,
		Size const &   begin,
		Size const &   count
	) -> TValue {
		auto mask = ~(~make_box<TValue>(0) << count);
		return (value >> begin) & mask;
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline constexpr auto infuse(
		TValue const & value,
		Size const &   begin,
		Size const &   count
	) -> TValue {
		auto mask = ~(~make_box<TValue>(0) << count);
		return (value & mask) << begin;
	}

	#pragma endregion

	#pragma region interleave

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline constexpr auto expand(
		TValue const & value
	) -> TValue {
		auto result = value;
		if constexpr (type_byte_size_of<TValue>() >= 8_sz) {
			result = (result | (result << 16_sz)) & cast_box_unsafe<TValue>(0b00000000'00000000'11111111'11111111'00000000'00000000'11111111'11111111_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 4_sz) {
			result = (result | (result << 8_sz)) & cast_box_unsafe<TValue>(0b00000000'11111111'00000000'11111111'00000000'11111111'00000000'11111111_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 2_sz) {
			result = (result | (result << 4_sz)) & cast_box_unsafe<TValue>(0b00001111'00001111'00001111'00001111'00001111'00001111'00001111'00001111_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 1_sz) {
			result = (result | (result << 2_sz)) & cast_box_unsafe<TValue>(0b00110011'00110011'00110011'00110011'00110011'00110011'00110011'00110011_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 0_sz) {
			result = (result | (result << 1_sz)) & cast_box_unsafe<TValue>(0b01010101'01010101'01010101'01010101'01010101'01010101'01010101'01010101_iu64);
		}
		return result;
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline constexpr auto shrink(
		TValue const & value
	) -> TValue {
		auto result = value & cast_box_unsafe<TValue>(0b01010101'01010101'01010101'01010101'01010101'01010101'01010101'01010101_iu64);
		if constexpr (type_byte_size_of<TValue>() >= 0_sz) {
			result = (result | (result >> 1_sz)) & cast_box_unsafe<TValue>(0b00110011'00110011'00110011'00110011'00110011'00110011'00110011'00110011_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 1_sz) {
			result = (result | (result >> 2_sz)) & cast_box_unsafe<TValue>(0b00001111'00001111'00001111'00001111'00001111'00001111'00001111'00001111_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 2_sz) {
			result = (result | (result >> 4_sz)) & cast_box_unsafe<TValue>(0b00000000'11111111'00000000'11111111'00000000'11111111'00000000'11111111_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 4_sz) {
			result = (result | (result >> 8_sz)) & cast_box_unsafe<TValue>(0b00000000'00000000'11111111'11111111'00000000'00000000'11111111'11111111_iu64);
		}
		if constexpr (type_byte_size_of<TValue>() >= 8_sz) {
			result = (result | (result >> 16_sz)) & cast_box_unsafe<TValue>(0b00000000'00000000'00000000'00000000'11111111'11111111'11111111'11111111_iu64);
		}
		return result;
	}

	#pragma endregion

}
