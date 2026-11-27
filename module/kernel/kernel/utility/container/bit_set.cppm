module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.bit_set;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.box;
import twinning.kernel.utility.range.integer_range;

export namespace Twinning::Kernel {

	#pragma region type

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameOf<t_size, Size>)
		&& (t_size <= 64_sz)
	class BitSet {

	public:

		using BoundedInteger = IntegerU64;

	protected:

		std::bitset<t_size.value> m_container;

	public:

		#pragma region constructor

		constexpr ~BitSet(
		) = default;

		// ----------------

		constexpr BitSet(
		) :
			m_container{} {
			return;
		}

		constexpr BitSet(
			BitSet const & that
		) = default;

		constexpr BitSet(
			BitSet && that
		) = default;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BitSet const & that
		) -> BitSet & = default;

		constexpr auto operator =(
			BitSet && that
		) -> BitSet & = default;

		#pragma endregion

		#pragma region size

		constexpr auto size(
		) const -> Size {
			return make_box<Size>(thiz.m_container.size());
		}

		#pragma endregion

		#pragma region value

		constexpr auto get(
			Size const & index
		) const -> Boolean {
			assert_test(index < thiz.size());
			return thiz.m_container.test(unmake_box<ZSize>(index));
		}

		constexpr auto set(
			Size const &    index,
			Boolean const & value
		) -> Void {
			assert_test(index < thiz.size());
			thiz.m_container.set(unmake_box<ZSize>(index), unmake_box<ZBoolean>(value));
			return;
		}

		// ----------------

		constexpr auto reset(
		) -> Void {
			thiz.m_container.reset();
			return;
		}

		constexpr auto reset(
			Size const & index
		) -> Void {
			assert_test(index < thiz.size());
			thiz.m_container.reset(unmake_box<ZSize>(index));
			return;
		}

		// ----------------

		constexpr auto set(
		) -> Void {
			thiz.m_container.set();
			return;
		}

		constexpr auto set(
			Size const & index
		) -> Void {
			assert_test(index < thiz.size());
			thiz.m_container.set(unmake_box<ZSize>(index));
			return;
		}

		#pragma endregion

		#pragma region utility

		constexpr auto all(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_container.all());
		}

		constexpr auto any(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_container.any());
		}

		constexpr auto none(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_container.none());
		}

		// ----------------

		constexpr auto count(
		) const -> Size {
			return make_box<Size>(thiz.m_container.count());
		}

		// ----------------

		constexpr auto flip(
		) -> Void {
			thiz.m_container.flip();
			return;
		}

		// ----------------

		template <auto t_begin, auto t_count> requires
			CategoryConstraint<>
			&& (IsSameOf<t_begin, Size> && IsSameOf<t_count, Size>)
			&& (t_begin + t_count <= t_size)
		constexpr auto sub(
		) const -> BitSet<t_count> {
			auto result = BitSet<t_count>{};
			for (auto & index : SizeRange{t_count}) {
				result.set(index, thiz.get(t_begin + index));
			}
			return result;
		}

		#pragma endregion

		#pragma region convert

		constexpr auto from_integer(
			BoundedInteger const & value
		) -> Void {
			assert_test((value >> t_size) == make_box<BoundedInteger>(0));
			thiz.m_container = std::bitset<t_size.value>{unmake_box<BoundedInteger::Value>(value)};
			return;
		}

		constexpr auto to_integer(
		) const -> BoundedInteger {
			return make_box<BoundedInteger>(thiz.m_container.to_ullong());
		}

		#pragma endregion

	};

	#pragma endregion

}
