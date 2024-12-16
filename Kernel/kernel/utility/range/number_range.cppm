module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.number_iterator;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TNumber> requires
		CategoryConstraint<IsPureInstance<TNumber>>
		&& (IsNumberBox<TNumber>)
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
		) :
			m_begin{},
			m_end{},
			m_step{} {
		}

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
			Number const & step = mbox<Number>(1)
		) :
			m_begin{begin},
			m_end{(end - begin) % step == mbox<Number>(0) ? (end) : (end + step - (end - begin) % step)},
			m_step{step} {
		}

		explicit constexpr NumberRange (
			Number const & size
		) :
			m_begin{mbox<Number>(0)},
			m_end{size},
			m_step{mbox<Number>(1)} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			NumberRange const & that
		) -> NumberRange & = default;

		constexpr auto operator = (
			NumberRange && that
		) -> NumberRange & = default;

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

// NOTE : USING

export namespace Twinning::Kernel {
	using Range::SizeRange;
}
