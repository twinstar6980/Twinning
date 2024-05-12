#pragma once

// TODO : remove make(_...)_range ?

#include "kernel/utility/base_wrapper/wrapper.hpp"

namespace TwinStar::Kernel {

	#pragma region type

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	class RangeWrapper {

	public:

		using Iterator = TIterator;

	protected:

		Iterator m_begin;

		Iterator m_end;

	public:

		#pragma region structor

		constexpr ~RangeWrapper (
		) = default;

		// ----------------

		constexpr RangeWrapper (
		) :
			m_begin{},
			m_end{} {
		}

		constexpr RangeWrapper (
			RangeWrapper const & that
		) = default;

		constexpr RangeWrapper (
			RangeWrapper && that
		) = default;

		// ----------------

		explicit constexpr RangeWrapper (
			Iterator const & begin,
			Iterator const & end
		) :
			m_begin{begin},
			m_end{end} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			RangeWrapper const & that
		) -> RangeWrapper & = default;

		constexpr auto operator = (
			RangeWrapper && that
		) -> RangeWrapper & = default;

		#pragma endregion

		#pragma region iterator

		constexpr auto iterator (
			Size const & index
		) const -> Iterator {
			assert_test(index <= thiz.size());
			return thiz.m_begin + index;
		}

		constexpr auto begin (
		) const -> Iterator {
			return thiz.m_begin;
		}

		constexpr auto end (
		) const -> Iterator {
			return thiz.m_end;
		}

		#pragma endregion

		#pragma region size

		constexpr auto size (
		) const -> Size {
			if constexpr (IsSame<decltype(thiz.m_end - thiz.m_begin), Size>) {
				return thiz.m_end - thiz.m_begin;
			}
			else if constexpr (IsIntegerWrapper<decltype(thiz.m_end - thiz.m_begin)>) {
				return cbw<Size>(thiz.m_end - thiz.m_begin);
			}
			else if constexpr (IsBuiltinInteger<decltype(thiz.m_end - thiz.m_begin)>) {
				return mbw<Size>(thiz.m_end - thiz.m_begin);
			}
			else {
				static_assert(k_static_assert_fail<TIterator>);
				assert_fail("");
			}
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Iterator> requires
		CategoryConstraint<IsPureInstance<Iterator>>
		&& (IsIterator<Iterator>)
	inline constexpr auto make_range (
		Iterator const & begin,
		Iterator const & end
	) -> RangeWrapper<Iterator> {
		return RangeWrapper{begin, end};
	}

	template <typename Iterator> requires
		CategoryConstraint<IsPureInstance<Iterator>>
		&& (IsIterator<Iterator>)
	inline constexpr auto make_range_n (
		Iterator const & begin,
		Size const &     size
	) -> RangeWrapper<Iterator> {
		return make_range(begin, begin + size);
	}

	template <typename Range> requires
		CategoryConstraint<IsValid<Range>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto make_range_of (
		Range && range
	) -> auto {
		return make_range(range.begin(), range.end());
	}

	// ----------------

	template <typename Iterator> requires
		CategoryConstraint<IsPureInstance<Iterator>>
		&& (IsIterator<Iterator>)
	inline constexpr auto make_reverse_range (
		Iterator const & end,
		Iterator const & begin
	) -> auto {
		if constexpr (IsPointerWrapper<Iterator>) {
			return make_range(std::make_reverse_iterator(end.value), std::make_reverse_iterator(begin.value));
		}
		else {
			return make_range(std::make_reverse_iterator(end), std::make_reverse_iterator(begin));
		}
	}

	template <typename Iterator> requires
		CategoryConstraint<IsPureInstance<Iterator>>
		&& (IsIterator<Iterator>)
	inline constexpr auto make_reverse_range_n (
		Iterator const & end,
		Size const &     size
	) -> auto {
		return make_reverse_range(end, end - size);
	}

	template <typename Range> requires
		CategoryConstraint<IsValid<Range>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto make_reverse_range_of (
		Range && range
	) -> auto {
		return make_reverse_range(range.end(), range.begin());
	}

	// ----------------

	template <typename Iterator> requires
		CategoryConstraint<IsPureInstance<Iterator>>
		&& (IsIterator<Iterator>)
	inline constexpr auto make_moveable_range (
		Iterator const & begin,
		Iterator const & end
	) -> auto {
		if constexpr (IsPointerWrapper<Iterator>) {
			return make_range(std::make_move_iterator(begin.value), std::make_move_iterator(end.value));
		}
		else {
			return make_range(std::make_move_iterator(begin), std::make_move_iterator(end));
		}
	}

	template <typename Iterator> requires
		CategoryConstraint<IsPureInstance<Iterator>>
		&& (IsIterator<Iterator>)
	inline constexpr auto make_moveable_range_n (
		Iterator const & begin,
		Size const &     size
	) -> auto {
		return make_moveable_range(begin, begin + size);
	}

	template <typename Range> requires
		CategoryConstraint<IsValid<Range>>
		&& (IsRange<AsPure<Range>>)
	inline constexpr auto make_moveable_range_of (
		Range && range
	) -> auto {
		return make_moveable_range(range.begin(), range.end());
	}

	#pragma endregion

}
