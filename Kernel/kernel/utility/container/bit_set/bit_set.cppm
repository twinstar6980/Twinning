module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.bit_set.bit_set;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.number_range;

export namespace Twinning::Kernel {

	#pragma region type

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameV<t_size, Size>)
	class BitSet {

	public:

		using BoundedInteger = decltype([] {
			if constexpr (t_size <= 0_sz) {
				return declare<Void>();
			}
			else if constexpr (t_size <= 8_sz) {
				return declare<IntegerU8>();
			}
			else if constexpr (t_size <= 16_sz) {
				return declare<IntegerU16>();
			}
			else if constexpr (t_size <= 32_sz) {
				return declare<IntegerU32>();
			}
			else if constexpr (t_size <= 64_sz) {
				return declare<IntegerU64>();
			}
			else {
				return declare<Void>();
			}
		}());

	protected:

		std::bitset<t_size.value> m_data;

	public:

		#pragma region structor

		constexpr ~BitSet (
		) = default;

		// ----------------

		constexpr BitSet (
		) :
			m_data{} {
		}

		constexpr BitSet (
			BitSet const & that
		) = default;

		constexpr BitSet (
			BitSet && that
		) = default;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BitSet const & that
		) -> BitSet & = default;

		constexpr auto operator = (
			BitSet && that
		) -> BitSet & = default;

		#pragma endregion

		#pragma region size

		auto size (
		) const -> Size {
			return mbox<Size>(thiz.m_data.size());
		}

		#pragma endregion

		#pragma region value

		auto get (
			Size const & index
		) -> Boolean {
			return thiz.m_data.test(index.value);
		}

		auto set (
			Size const &    index,
			Boolean const & value
		) -> Void {
			thiz.m_data.set(index.value, value.value);
			return;
		}

		// ----------------

		auto reset (
		) -> Void {
			thiz.m_data.reset();
			return;
		}

		auto reset (
			Size const & index
		) -> Void {
			thiz.m_data.reset(index.value);
			return;
		}

		// ----------------

		auto set (
		) -> Void {
			thiz.m_data.set();
			return;
		}

		auto set (
			Size const & index
		) -> Void {
			thiz.m_data.set(index.value);
			return;
		}

		#pragma endregion

		#pragma region utility

		auto all (
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_data.all());
		}

		auto any (
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_data.any());
		}

		auto none (
		) const -> Boolean {
			return mbox<Boolean>(thiz.m_data.none());
		}

		// ----------------

		auto count (
		) const -> Size {
			return mbox<Size>(thiz.m_data.count());
		}

		// ----------------

		auto flip (
		) -> Void {
			thiz.m_data.flip();
			return;
		}

		// ----------------

		template <auto begin, auto size> requires
			CategoryConstraint<>
			&& (IsSameV<begin, Size> && IsSameV<size, Size>)
			&& (begin + size <= t_size)
		auto sub (
		) -> BitSet<size> {
			auto result = BitSet<size>{};
			for (auto & index : SizeRange{size}) {
				result.set(index, thiz.get(begin + index));
			}
			return result;
		}

		#pragma endregion

		#pragma region from & to with integer

		template <typename TargetInteger = BoundedInteger> requires
			AutoConstraint
			&& (IsSame<TargetInteger, BoundedInteger>)
			&& (!IsVoid<TargetInteger>)
		auto from_integer (
			TargetInteger const & value
		) -> Void {
			thiz.m_data = std::bitset<t_size.value>{value.value};
			return;
		}

		template <typename TargetInteger = BoundedInteger> requires
			AutoConstraint
			&& (IsSame<TargetInteger, BoundedInteger>)
			&& (!IsVoid<TargetInteger>)
		auto to_integer (
		) const -> TargetInteger {
			return mbox<TargetInteger>(thiz.m_data.to_ullong());
		}

		#pragma endregion

	};

	#pragma endregion

}
