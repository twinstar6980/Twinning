module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.memory.bitwise;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.container.static_array.static_array;

export namespace Twinning::Kernel::Bitwise {

	#pragma region basic

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseBox<TValue>)
	inline auto reverse(
		TValue const & value
	) -> TValue {
		if constexpr (k_type_size<TValue> == 1_sz) {
			return value;
		}
		else {
			auto forward = make_pointer_unsafe<ZByte>(&value);
			auto backward = StaticArray<ZByte, k_type_size<TValue>>{};
			for (auto & index : SizeRange{k_type_size<TValue>}) {
				backward[index] = forward[k_type_size<TValue> - 1_sz - index];
			}
			return cast_pointer<TValue>(backward.begin()).dereference();
		}
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline auto extract(
		TValue const & value,
		Size const &   begin,
		Size const &   size
	) -> TValue {
		return (value >> begin) & ~(~make_box<TValue>(0) << size);
	}

	#pragma endregion

	#pragma region interleave

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline auto expand(
		TValue const & value
	) -> TValue {
		auto result = value;
		if constexpr (k_type_size<TValue> >= 8_sz) {
			result = (result | (result << 16_sz)) & cast_box_unsafe<TValue>(0b00000000'00000000'11111111'11111111'00000000'00000000'11111111'11111111_iu64);
		}
		if constexpr (k_type_size<TValue> >= 4_sz) {
			result = (result | (result << 8_sz)) & cast_box_unsafe<TValue>(0b00000000'11111111'00000000'11111111'00000000'11111111'00000000'11111111_iu64);
		}
		if constexpr (k_type_size<TValue> >= 2_sz) {
			result = (result | (result << 4_sz)) & cast_box_unsafe<TValue>(0b00001111'00001111'00001111'00001111'00001111'00001111'00001111'00001111_iu64);
		}
		if constexpr (k_type_size<TValue> >= 1_sz) {
			result = (result | (result << 2_sz)) & cast_box_unsafe<TValue>(0b00110011'00110011'00110011'00110011'00110011'00110011'00110011'00110011_iu64);
		}
		if constexpr (k_type_size<TValue> >= 0_sz) {
			result = (result | (result << 1_sz)) & cast_box_unsafe<TValue>(0b01010101'01010101'01010101'01010101'01010101'01010101'01010101'01010101_iu64);
		}
		return result;
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline auto shrink(
		TValue const & value
	) -> TValue {
		auto result = value & cast_box_unsafe<TValue>(0b01010101'01010101'01010101'01010101'01010101'01010101'01010101'01010101_iu64);
		if constexpr (k_type_size<TValue> >= 0_sz) {
			result = (result | (result >> 1_sz)) & cast_box_unsafe<TValue>(0b00110011'00110011'00110011'00110011'00110011'00110011'00110011'00110011_iu64);
		}
		if constexpr (k_type_size<TValue> >= 1_sz) {
			result = (result | (result >> 2_sz)) & cast_box_unsafe<TValue>(0b00001111'00001111'00001111'00001111'00001111'00001111'00001111'00001111_iu64);
		}
		if constexpr (k_type_size<TValue> >= 2_sz) {
			result = (result | (result >> 4_sz)) & cast_box_unsafe<TValue>(0b00000000'11111111'00000000'11111111'00000000'11111111'00000000'11111111_iu64);
		}
		if constexpr (k_type_size<TValue> >= 4_sz) {
			result = (result | (result >> 8_sz)) & cast_box_unsafe<TValue>(0b00000000'00000000'11111111'11111111'00000000'00000000'11111111'11111111_iu64);
		}
		if constexpr (k_type_size<TValue> >= 8_sz) {
			result = (result | (result >> 16_sz)) & cast_box_unsafe<TValue>(0b00000000'00000000'00000000'00000000'11111111'11111111'11111111'11111111_iu64);
		}
		return result;
	}

	#pragma endregion

}
