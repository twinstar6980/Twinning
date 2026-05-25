module;

#include "kernel/common.hpp"

// TODO: remove make(_...)_range?

export module twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.reverse_iterator;
import twinning.kernel.utility.range.moveable_iterator;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	class GenericRange {

	public:

		using Iterator = TIterator;

	protected:

		Iterator m_begin;

		Iterator m_end;

	public:

		#pragma region constructor

		constexpr ~GenericRange(
		) = default;

		// ----------------

		constexpr GenericRange(
		) :
			m_begin{},
			m_end{} {
			return;
		}

		constexpr GenericRange(
			GenericRange const & that
		) = default;

		constexpr GenericRange(
			GenericRange && that
		) = default;

		// ----------------

		explicit constexpr GenericRange(
			Iterator const & begin,
			Iterator const & end
		) :
			m_begin{begin},
			m_end{end} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			GenericRange const & that
		) -> GenericRange & = default;

		constexpr auto operator =(
			GenericRange && that
		) -> GenericRange & = default;

		#pragma endregion

		#pragma region iterator

		constexpr auto begin(
		) const -> Iterator {
			return thiz.m_begin;
		}

		constexpr auto end(
		) const -> Iterator {
			return thiz.m_end;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	inline constexpr auto make_range(
		TIterator const & begin,
		TIterator const & end
	) -> GenericRange<TIterator> {
		return GenericRange{begin, end};
	}

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	inline constexpr auto make_range_n(
		TIterator const & begin,
		Size const &      size
	) -> GenericRange<TIterator> {
		return make_range(begin, begin + size);
	}

	template <typename TRange> requires
		CategoryConstraint<IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto make_range_of(
		TRange && range
	) -> auto {
		return make_range(range.begin(), range.end());
	}

	// ----------------

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	inline constexpr auto make_reverse_range(
		TIterator const & end,
		TIterator const & begin
	) -> auto {
		return make_range(ReverseIterator{end}, ReverseIterator{begin});
	}

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	inline constexpr auto make_reverse_range_n(
		TIterator const & end,
		Size const &      size
	) -> auto {
		return make_reverse_range(end, end - size);
	}

	template <typename TRange> requires
		CategoryConstraint<IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto make_reverse_range_of(
		TRange && range
	) -> auto {
		return make_reverse_range(range.end(), range.begin());
	}

	// ----------------

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	inline constexpr auto make_moveable_range(
		TIterator const & begin,
		TIterator const & end
	) -> auto {
		return make_range(MoveableIterator{begin}, MoveableIterator{end});
	}

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	inline constexpr auto make_moveable_range_n(
		TIterator const & begin,
		Size const &      size
	) -> auto {
		return make_moveable_range(begin, begin + size);
	}

	template <typename TRange> requires
		CategoryConstraint<IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline constexpr auto make_moveable_range_of(
		TRange && range
	) -> auto {
		return make_moveable_range(range.begin(), range.end());
	}

	#pragma endregion

}
