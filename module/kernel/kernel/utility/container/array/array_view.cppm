module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_constant, Boolean>)
	class ArrayView {

	private:

		using ConstantView = ArrayView<TElement, k_true>;

	public:

		using Element = TElement;

		inline static constexpr auto constant = Boolean{t_constant};

		using QualifyElement = AsConstantIf<Element, constant.value>;

		using QualifyIterator = Pointer<QualifyElement>;

	protected:

		QualifyIterator m_data;

		Size m_size;

	public:

		#pragma region constructor

		constexpr ~ArrayView(
		) = default;

		// ----------------

		constexpr ArrayView(
		) :
			m_data{},
			m_size{} {
			return;
		}

		constexpr ArrayView(
			ArrayView const & that
		) = default;

		constexpr ArrayView(
			ArrayView && that
		) = default;

		// ----------------

		explicit constexpr ArrayView(
			QualifyIterator const & data,
			Size const &            size
		) :
			ArrayView{} {
			thiz.set(data, size);
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			ArrayView const & that
		) -> ArrayView & = default;

		constexpr auto operator =(
			ArrayView && that
		) -> ArrayView & = default;

		// ----------------

		constexpr auto operator [](
			Size const & index
		) const -> QualifyElement & {
			return thiz.at(index);
		}

		// ----------------

		implicit operator ConstantView &() requires
			(!constant.value) {
			return unsafe_cast<ConstantView>(thiz);
		}

		implicit operator ConstantView const &() const requires
			(!constant.value) {
			return unsafe_cast<ConstantView>(thiz);
		}

		#pragma endregion

		#pragma region set

		constexpr auto reset(
		) -> Void {
			thiz.m_data = k_null_pointer;
			thiz.m_size = 0_sz;
			return;
		}

		constexpr auto set(
			QualifyIterator const & data,
			Size const &            size
		) -> Void {
			thiz.m_data = data;
			thiz.m_size = size;
			return;
		}

		#pragma endregion

		#pragma region data

		constexpr auto data(
		) const -> QualifyIterator {
			return thiz.m_data;
		}

		#pragma endregion

		#pragma region size

		constexpr auto size(
		) const -> Size {
			return thiz.m_size;
		}

		// ----------------

		constexpr auto empty(
		) const -> Boolean {
			return thiz.size() == 0_sz;
		}

		#pragma endregion

		#pragma region index

		constexpr auto begin_index(
		) const -> Size {
			return 0_sz;
		}

		constexpr auto end_index(
		) const -> Size {
			return thiz.size();
		}

		// ----------------

		constexpr auto first_index(
		) const -> Size {
			assert_test(!thiz.empty());
			return thiz.begin_index();
		}

		constexpr auto last_index(
		) const -> Size {
			assert_test(!thiz.empty());
			return thiz.end_index() - 1_sz;
		}

		#pragma endregion

		#pragma region iterator

		constexpr auto iterator(
			Size const & index
		) const -> QualifyIterator {
			assert_test(index <= thiz.end_index());
			return thiz.m_data + index;
		}

		constexpr auto begin(
		) const -> QualifyIterator {
			return thiz.iterator(thiz.begin_index());
		}

		constexpr auto end(
		) const -> QualifyIterator {
			return thiz.iterator(thiz.end_index());
		}

		#pragma endregion

		#pragma region element

		constexpr auto at(
			Size const & index
		) const -> QualifyElement & {
			assert_test(index < thiz.end_index());
			return thiz.m_data.dereference(index);
		}

		constexpr auto first(
		) const -> QualifyElement & {
			return thiz.at(thiz.first_index());
		}

		constexpr auto last(
		) const -> QualifyElement & {
			return thiz.at(thiz.last_index());
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub(
			Size const & begin,
			Size const & count
		) const -> ArrayView {
			assert_test(begin + count <= thiz.size());
			return ArrayView{thiz.iterator(begin), count};
		}

		constexpr auto head(
			Size const & count
		) const -> ArrayView {
			assert_test(count <= thiz.size());
			return thiz.sub(thiz.begin_index(), count);
		}

		constexpr auto tail(
			Size const & count
		) const -> ArrayView {
			assert_test(count <= thiz.size());
			return thiz.sub(thiz.end_index() - count, count);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			ArrayView const & thix,
			ArrayView const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename TElement> requires
		AutomaticConstraint
	using VariableArrayView = ArrayView<TElement, k_false>;

	template <typename TElement> requires
		AutomaticConstraint
	using ConstantArrayView = ArrayView<TElement, k_true>;

	#pragma endregion

}
