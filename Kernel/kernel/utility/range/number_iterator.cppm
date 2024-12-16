module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.number_iterator;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TNumber> requires
		CategoryConstraint<IsPureInstance<TNumber>>
		&& (IsNumberBox<TNumber>)
	class NumberIterator {

	public:

		using Number = TNumber;

	protected:

		Number m_value;

		Number m_step;

	public:

		#pragma region structor

		constexpr ~NumberIterator (
		) = default;

		// ----------------

		constexpr NumberIterator (
		) :
			m_value{},
			m_step{} {
		}

		constexpr NumberIterator (
			NumberIterator const & that
		) = default;

		constexpr NumberIterator (
			NumberIterator && that
		) = default;

		// ----------------

		explicit constexpr NumberIterator (
			Number const & value,
			Number const & step
		) :
			m_value{value},
			m_step{step} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			NumberIterator const & that
		) -> NumberIterator & = default;

		constexpr auto operator = (
			NumberIterator && that
		) -> NumberIterator & = default;

		// ----------------

		constexpr auto operator * (
		) const -> Number const & {
			return thiz.m_value;
		}

		// ----------------

		constexpr auto operator ++ (
		) -> NumberIterator & {
			thiz.m_value += thiz.m_step;
			return thiz;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			NumberIterator const & thix,
			NumberIterator const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}
