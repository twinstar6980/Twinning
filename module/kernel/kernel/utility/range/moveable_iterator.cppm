module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.moveable_iterator;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	class MoveableIterator {

	public:

		using Iterator = TIterator;

		using Value = AsUnmakeReference<decltype(*declare<Iterator const &>())>;

	protected:

		Iterator m_current;

	public:

		#pragma region constructor

		constexpr ~MoveableIterator(
		) = default;

		// ----------------

		constexpr MoveableIterator(
		) :
			m_current{} {
			return;
		}

		constexpr MoveableIterator(
			MoveableIterator const & that
		) = default;

		constexpr MoveableIterator(
			MoveableIterator && that
		) = default;

		// ----------------

		explicit constexpr MoveableIterator(
			Iterator const & current
		) :
			m_current{current} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			MoveableIterator const & that
		) -> MoveableIterator & = default;

		constexpr auto operator =(
			MoveableIterator && that
		) -> MoveableIterator & = default;

		// ----------------

		constexpr auto operator *(
		) const -> Value && {
			return as_moveable(*thiz.m_current);
		}

		// ----------------

		constexpr auto operator ++(
		) -> MoveableIterator & {
			++thiz.m_current;
			return thiz;
		}

		constexpr auto operator --(
		) -> MoveableIterator & {
			--thiz.m_current;
			return thiz;
		}

		// ----------------

		constexpr auto operator -(
			MoveableIterator const & that
		) const -> SSize {
			return thiz.m_current - that.m_current;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			MoveableIterator const & thix,
			MoveableIterator const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}
