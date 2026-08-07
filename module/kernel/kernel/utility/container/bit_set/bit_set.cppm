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
		&& (IsSameOf<t_size, Size>)
	class BitSet {

	public:

		using BoundedInteger = IntegerU64;

	protected:

		std::bitset<t_size.value> m_data;

	public:

		#pragma region constructor

		constexpr ~BitSet(
		) = default;

		// ----------------

		constexpr BitSet(
		) :
			m_data{} {
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

		auto size(
		) const -> Size {
			return make_box<Size>(thiz.m_data.size());
		}

		#pragma endregion

		#pragma region value

		auto get(
			Size const & index
		) -> Boolean {
			return thiz.m_data.test(unmake_box<ZSize>(index));
		}

		auto set(
			Size const &    index,
			Boolean const & value
		) -> Void {
			thiz.m_data.set(unmake_box<ZSize>(index), unmake_box<ZBoolean>(value));
			return;
		}

		// ----------------

		auto reset(
		) -> Void {
			thiz.m_data.reset();
			return;
		}

		auto reset(
			Size const & index
		) -> Void {
			thiz.m_data.reset(unmake_box<ZSize>(index));
			return;
		}

		// ----------------

		auto set(
		) -> Void {
			thiz.m_data.set();
			return;
		}

		auto set(
			Size const & index
		) -> Void {
			thiz.m_data.set(unmake_box<ZSize>(index));
			return;
		}

		#pragma endregion

		#pragma region utility

		auto all(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_data.all());
		}

		auto any(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_data.any());
		}

		auto none(
		) const -> Boolean {
			return make_box<Boolean>(thiz.m_data.none());
		}

		// ----------------

		auto count(
		) const -> Size {
			return make_box<Size>(thiz.m_data.count());
		}

		// ----------------

		auto flip(
		) -> Void {
			thiz.m_data.flip();
			return;
		}

		// ----------------

		template <auto t_sub_begin, auto t_sub_size> requires
			CategoryConstraint<>
			&& (IsSameOf<t_sub_begin, Size> && IsSameOf<t_sub_size, Size>)
			&& (t_sub_begin + t_sub_size <= t_size)
		auto sub(
		) -> BitSet<t_sub_size> {
			auto result = BitSet<t_sub_size>{};
			for (auto & index : SizeRange{t_sub_size}) {
				result.set(index, thiz.get(t_sub_begin + index));
			}
			return result;
		}

		#pragma endregion

		#pragma region from & to with integer

		auto from_integer(
			BoundedInteger const & value
		) -> Void {
			assert_test((value >> t_size) == make_box<BoundedInteger>(0));
			thiz.m_data = std::bitset<t_size.value>{unmake_box<BoundedInteger::Value>(value)};
			return;
		}

		auto to_integer(
		) const -> BoundedInteger {
			return make_box<BoundedInteger>(thiz.m_data.to_ullong());
		}

		#pragma endregion

	};

	#pragma endregion

}
