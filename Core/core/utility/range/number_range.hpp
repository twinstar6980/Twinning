#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"
#include "core/utility/range/number_iterator.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TNumber> requires
		CategoryConstraint<IsPureInstance<TNumber>>
		&& (IsNumberWrapper<TNumber>)
	class NumberRange {

	public:

		using Number = TNumber;

		using Iterator = NumberIterator<Number>;

	protected:

		Number m_begin;
		Number m_end;
		Number m_step;

	public:

		#pragma region structor

		constexpr ~NumberRange (
		) = default;

		// ----------------

		constexpr NumberRange (
		) = default;

		constexpr NumberRange (
			NumberRange const & that
		) = default;

		constexpr NumberRange (
			NumberRange && that
		) = default;

		// ----------------

		explicit constexpr NumberRange (
			Number const & begin,
			Number const & end,
			Number const & step = mbw<Number>(1)
		) :
			m_begin{begin},
			m_end{(end - begin) % step == mbw<Number>(0) ? (end) : (end + step - (end - begin) % step)},
			m_step{step} {
		}

		explicit constexpr NumberRange (
			Number const & size
		) :
			m_begin{mbw<Number>(0)},
			m_end{size},
			m_step{mbw<Number>(1)} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			NumberRange const & that
		) -> NumberRange& = default;

		constexpr auto operator = (
			NumberRange && that
		) -> NumberRange& = default;

		#pragma endregion

		#pragma region iterator

		constexpr auto begin (
		) const -> Iterator {
			return Iterator{thiz.m_begin, thiz.m_step};
		}

		constexpr auto end (
		) const -> Iterator {
			return Iterator{thiz.m_end, thiz.m_step};
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	using SizeRange = NumberRange<Size>;

	#pragma endregion

}
