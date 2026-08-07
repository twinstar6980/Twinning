module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.range.reverse_iterator;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Range {

	#pragma region type

	template <typename TIterator> requires
		CategoryConstraint<IsPureInstance<TIterator>>
		&& (IsIterator<TIterator>)
	class ReverseIterator {

	public:

		using Iterator = TIterator;

		using Value = AsUnmakeReference<decltype(*declare<Iterator const &>())>;

	protected:

		Iterator m_current;

	public:

		#pragma region constructor

		constexpr ~ReverseIterator(
		) = default;

		// ----------------

		constexpr ReverseIterator(
		) :
			m_current{} {
			return;
		}

		constexpr ReverseIterator(
			ReverseIterator const & that
		) = default;

		constexpr ReverseIterator(
			ReverseIterator && that
		) = default;

		// ----------------

		explicit constexpr ReverseIterator(
			Iterator const & current
		) :
			m_current{current} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			ReverseIterator const & that
		) -> ReverseIterator & = default;

		constexpr auto operator =(
			ReverseIterator && that
		) -> ReverseIterator & = default;

		// ----------------

		constexpr auto operator *(
		) const -> Value & {
			auto temporary = thiz.m_current;
			--temporary;
			return *temporary;
		}

		// ----------------

		constexpr auto operator ++(
		) -> ReverseIterator & {
			--thiz.m_current;
			return thiz;
		}

		constexpr auto operator --(
		) -> ReverseIterator & {
			++thiz.m_current;
			return thiz;
		}

		// ----------------

		constexpr auto operator -(
			ReverseIterator const & that
		) const -> SSize {
			return that.m_current - thiz.m_current;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			ReverseIterator const & thix,
			ReverseIterator const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}
