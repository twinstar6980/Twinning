module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.integer_range;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.range.integer_iterator;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	class IntegerRange {

	public:

		using Value = TValue;

		using Iterator = IntegerIterator<Value>;

	protected:

		Value m_begin;

		Value m_end;

		Value m_step;

	public:

		#pragma region constructor

		constexpr ~IntegerRange(
		) = default;

		// ----------------

		constexpr IntegerRange(
		) :
			m_begin{},
			m_end{},
			m_step{} {
			return;
		}

		constexpr IntegerRange(
			IntegerRange const & that
		) = default;

		constexpr IntegerRange(
			IntegerRange && that
		) = default;

		// ----------------

		explicit constexpr IntegerRange(
			Value const & begin,
			Value const & end,
			Value const & step = make_box<Value>(1)
		) :
			m_begin{begin},
			m_end{end},
			m_step{step} {
			assert_test(step != make_box<Value>(0));
			assert_test((end - begin) % step == make_box<Value>(0));
			return;
		}

		explicit constexpr IntegerRange(
			Value const & size
		) :
			m_begin{make_box<Value>(0)},
			m_end{size},
			m_step{make_box<Value>(1)} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			IntegerRange const & that
		) -> IntegerRange & = default;

		constexpr auto operator =(
			IntegerRange && that
		) -> IntegerRange & = default;

		#pragma endregion

		#pragma region iterator

		constexpr auto begin(
		) const -> Iterator {
			return Iterator{thiz.m_begin, thiz.m_step};
		}

		constexpr auto end(
		) const -> Iterator {
			return Iterator{thiz.m_end, thiz.m_step};
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	using SizeRange = IntegerRange<Size>;

	#pragma endregion

}

// NOTE: USING

export namespace Twinning::Kernel {
	using Range::SizeRange;
}
