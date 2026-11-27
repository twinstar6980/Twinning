module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.memory.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.range.integer_range;
import twinning.kernel.utility.memory.endian;
import twinning.kernel.utility.container.array.static_array;

export namespace Twinning::Kernel::Memory {

	#pragma region basic

	template <typename TDestination, typename TSource> requires
		CategoryConstraint<IsPureInstance<TDestination> && IsPureInstance<TSource>>
	inline constexpr auto cast(
		TSource const & it
	) -> TDestination {
		return std::bit_cast<TDestination>(it);
	}

	template <typename TDestination, typename TSource> requires
		CategoryConstraint<IsPointable<TDestination> && IsPointable<TSource>>
	inline constexpr auto copy(
		Pointer<TDestination> const & destination,
		Pointer<TSource> const &      source,
		Size const &                  size
	) -> Void {
		std::memmove(unmake_pointer(destination), unmake_pointer(source), unmake_box<ZSize>(size));
		return;
	}

	#pragma endregion

	#pragma region allocation

	template <typename TIt, typename ... TArgument> requires
		CategoryConstraint<IsInstance<TIt> && IsValid<TArgument ...>>
		&& (IsConstructible<AsPure<TIt>, TArgument && ...>)
	inline constexpr auto allocate(
		TArgument && ... argument
	) -> Pointer<TIt> {
		auto pointer = make_pointer(new TIt{as_forward<TArgument>(argument) ...});
		return pointer;
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto free(
		Pointer<TIt> & pointer
	) -> Void {
		delete pointer.value;
		pointer = k_null_pointer;
		return;
	}

	// ----------------

	template <typename TIt, typename ... TArgument> requires
		CategoryConstraint<IsInstance<TIt> && IsPureInstance<TArgument ...>>
		&& (IsDefaultConstructible<AsPure<TIt>>)
		&& (IsConstructible<AsPure<TIt>, TArgument const & ...>)
	inline constexpr auto allocate_array(
		Size const &          size,
		TArgument const & ... argument
	) -> Pointer<TIt> {
		auto pointer = make_pointer(new TIt[unmake_box<ZSize>(size)]{});
		if constexpr (sizeof...(TArgument) != 0_szz) {
			Range::restruct(Range::make_range_n(pointer, size), argument ...);
		}
		return pointer;
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto free_array(
		Pointer<TIt> & pointer
	) -> Void {
		delete[] pointer.value;
		pointer = k_null_pointer;
		return;
	}

	#pragma endregion

	#pragma region endian

	inline constexpr auto query_endian(
	) -> Endian {
		if constexpr (std::endian::native == std::endian::little) {
			return Endian::Constant::little();
		}
		else {
			return Endian::Constant::big();
		}
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	inline constexpr auto reverse_endian(
		TValue const & value
	) -> TValue {
		if constexpr (type_byte_size_of<TValue>() == 1_sz) {
			return value;
		}
		else {
			auto forward = cast<StaticArray<Byte, type_byte_size_of<TValue>()>>(value);
			auto backward = StaticArray<Byte, type_byte_size_of<TValue>()>{};
			for (auto & index : SizeRange{type_byte_size_of<TValue>()}) {
				backward[index] = forward[type_byte_size_of<TValue>() - 1_sz - index];
			}
			return cast<TValue>(backward);
		}
	}

	#pragma endregion

	#pragma region convert

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	inline constexpr auto from_data(
		StaticArray<Byte, type_byte_size_of<TValue>()> const & data,
		Endian const &                                         endian
	) -> TValue {
		auto value = cast<TValue>(data);
		if (endian != query_endian()) {
			value = reverse_endian(value);
		}
		return value;
	}

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	inline constexpr auto to_data(
		TValue const & value,
		Endian const & endian
	) -> StaticArray<Byte, type_byte_size_of<TValue>()> {
		auto actual_value = value;
		if (endian != query_endian()) {
			actual_value = reverse_endian(actual_value);
		}
		return cast<StaticArray<Byte, type_byte_size_of<TValue>()>>(actual_value);
	}

	#pragma endregion

}
