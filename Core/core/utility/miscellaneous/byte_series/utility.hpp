#pragma once

#include "core/utility/container/stream/stream_view.hpp"
#include "core/utility/miscellaneous/byte_series/container.hpp"

namespace TwinStar::Core {

	#pragma region padding

	inline constexpr auto is_padded_size (
		Size const & size,
		Size const & unit
	) -> Boolean {
		return size % unit == k_none_size;
	}

	inline constexpr auto compute_padded_size (
		Size const & size,
		Size const & unit
	) -> Size {
		return is_padded_size(size, unit) ? (size) : ((size / unit + 1_sz) * unit);
	}

	inline constexpr auto compute_padding_size (
		Size const & size,
		Size const & unit
	) -> Size {
		return compute_padded_size(size, unit) - size;
	}

	#pragma endregion bit

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
	inline auto byte_nth_of (
		It &         it,
		Size const & index
	) -> AsConstantIf<Byte, IsCInstance<It>>& {
		return cast_pointer<Byte>(make_pointer(&it))[index];
	}

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsIntegerWrapper<It>)
	inline auto clip_bit (
		It const &   it,
		Size const & begin,
		Size const & size
	) -> It {
		return (it >> begin) & ~(~mbw<It>(0) << size);
	}

	template <typename It> requires
		CategoryConstraint<IsBaseWrapper<It>>
	inline auto reverse_endian (
		It const & it
	) -> It {
		if constexpr (k_type_size<It> == 1_sz) {
			return it;
		} else {
			union {
				It                        value;
				ZArray<ZByte, sizeof(It)> byte;
			}
				result = {};
			auto pointer = reinterpret_cast<ZPointer<ZByte const>>(&it);
			for (auto index = k_begin_index; index < k_type_size<It>; ++index) {
				result.byte[index.value] = pointer[(k_type_size<It> - 1_sz - index).value];
			}
			return result.value;
		}
	}

	#pragma region view cast

	template <typename SourceElement, auto constant> requires
		CategoryConstraint<IsPureInstance<SourceElement>>
		&& (IsSameV<constant, ZBoolean>)
	inline auto to_byte_view (
		ListView<SourceElement, constant> const & source
	) -> ByteListView<constant> {
		return ByteListView<constant>{cast_pointer<Byte>(source.begin()), source.size() * k_type_size<AsPure<SourceElement>>};
	}

	template <typename DestinationElement, template <typename, auto> typename DestinationView = ListView, auto constant> requires
		CategoryConstraint<IsPureInstance<DestinationElement>>
		&& (IsSameV<constant, ZBoolean>)
	inline auto from_byte_view (
		ByteListView<constant> const & source
	) -> DestinationView<DestinationElement, constant> {
		assert_test(is_padded_size(source.size(), k_type_size<DestinationElement>));
		return DestinationView<DestinationElement, constant>{cast_pointer<DestinationElement>(source.begin()), source.size() / k_type_size<DestinationElement>};
	}

	#pragma endregion

}
