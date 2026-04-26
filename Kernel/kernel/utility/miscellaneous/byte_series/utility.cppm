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

	#pragma region padding

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

	#pragma endregion bit

	template <typename TIt> requires
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsIntegerBox<TIt>)
	inline auto clip_bit(
		TIt const &  it,
		Size const & begin,
		Size const & size
	) -> TIt {
		return (it >> begin) & ~(~mbox<TIt>(0) << size);
	}

	template <typename TIt> requires
		CategoryConstraint<IsBaseBox<TIt>>
	inline auto reverse_endian(
		TIt const & it
	) -> TIt {
		if constexpr (k_type_size<TIt> == 1_sz) {
			return it;
		}
		else {
			auto forward = cast_pointer<ZByte>(make_pointer_of(it));
			auto backward = StaticArray<ZByte, k_type_size<TIt>>{};
			for (auto & index : SizeRange{k_type_size<TIt>}) {
				backward[index] = forward[k_type_size<TIt> - 1_sz - index];
			}
			return cast_pointer<TIt>(backward.begin()).dereference();
		}
	}

	#pragma region view cast

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
