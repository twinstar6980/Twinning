module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.integer_iterator;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsIntegerBox<TValue>)
	class IntegerIterator {

	public:

		using Value = TValue;

	protected:

		Value m_value;

		Value m_step;

	public:

		#pragma region constructor

		constexpr ~IntegerIterator(
		) = default;

		// ----------------

		constexpr IntegerIterator(
		) :
			m_value{},
			m_step{} {
			return;
		}

		constexpr IntegerIterator(
			IntegerIterator const & that
		) = default;

		constexpr IntegerIterator(
			IntegerIterator && that
		) = default;

		// ----------------

		explicit constexpr IntegerIterator(
			Value const & value,
			Value const & step
		) :
			m_value{value},
			m_step{step} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			IntegerIterator const & that
		) -> IntegerIterator & = default;

		constexpr auto operator =(
			IntegerIterator && that
		) -> IntegerIterator & = default;

		// ----------------

		constexpr auto operator *(
		) const -> Value const & {
			return thiz.m_value;
		}

		// ----------------

		constexpr auto operator ++(
		) -> IntegerIterator & {
			thiz.m_value += thiz.m_step;
			return thiz;
		}

		constexpr auto operator --(
		) -> IntegerIterator & {
			thiz.m_value -= thiz.m_step;
			return thiz;
		}

		// ----------------

		constexpr auto operator -(
			IntegerIterator const & that
		) const -> SSize {
			assert_test(thiz.m_step == that.m_step);
			return cast_box<SSize>((thiz.m_value - that.m_value) / thiz.m_step);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			IntegerIterator const & thix,
			IntegerIterator const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}
