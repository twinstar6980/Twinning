module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.byte_series.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.static_array.static_array;
import twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.miscellaneous.byte_series.container;

export namespace Twinning::Kernel {

	#pragma region size

	inline constexpr auto is_padded_size(
		Size const & size,
		Size const & unit
	) -> Boolean {
		return size % unit == 0_sz;
	}

	inline constexpr auto compute_padded_size(
		Size const & size,
		Size const & unit
	) -> Size {
		return is_padded_size(size, unit) ? (size) : ((size / unit + 1_sz) * unit);
	}

	inline constexpr auto compute_padding_size(
		Size const & size,
		Size const & unit
	) -> Size {
		return compute_padded_size(size, unit) - size;
	}

	// ----------------

	inline constexpr auto is_padded_size_of_exponent_of_2(
		Size const & size
	) -> Boolean {
		return (size != 0_sz) && ((size & (size - 1_sz)) == 0_sz);
	}

	#pragma endregion

	#pragma region bit

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline auto clip_bit(
		TValue const & value,
		Size const &   begin,
		Size const &   size
	) -> TValue {
		return (value >> begin) & ~(~make_box<TValue>(0) << size);
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	inline auto expand_bit(
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
	inline auto shrink_bit(
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

	template <typename TValue> requires
		CategoryConstraint<IsBaseBox<TValue>>
	inline auto reverse_bit(
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

	#pragma endregion

	#pragma region view

	template <typename TSourceElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TSourceElement>>
		&& (IsSameOf<t_constant, Boolean>)
	inline auto to_byte_view(
		ListView<TSourceElement, t_constant> const & source
	) -> ByteListView<t_constant> {
		return ByteListView<t_constant>{cast_pointer<Byte>(source.begin()), source.size() * k_type_size<AsPure<TSourceElement>>};
	}

	template <typename TDestinationElement, template <typename, auto> typename TDestinationView = ListView, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TDestinationElement>>
		&& (IsSameOf<t_constant, Boolean>)
	inline auto from_byte_view(
		ByteListView<t_constant> const & source
	) -> TDestinationView<TDestinationElement, t_constant> {
		assert_test(is_padded_size(source.size(), k_type_size<TDestinationElement>));
		return TDestinationView<TDestinationElement, t_constant>{cast_pointer<TDestinationElement>(source.begin()), source.size() / k_type_size<TDestinationElement>};
	}

	#pragma endregion

}
