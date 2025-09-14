module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.range_wrapper;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_constant, Boolean>)
	class ListView {

	private:

		using ConstantView = ListView<TElement, k_true>;

	public:

		using Element = TElement;

		inline static constexpr auto constant = Boolean{t_constant};

		using QualifyElement = AsConstantIf<Element, constant.value>;

		using QualifyIterator = Pointer<AsConstantIf<Element, constant.value>>;

	protected:

		QualifyIterator m_data;

		Size m_size;

	public:

		#pragma region constructor

		constexpr ~ListView (
		) = default;

		// ----------------

		constexpr ListView (
		) :
			m_data{},
			m_size{} {
			return;
		}

		constexpr ListView (
			ListView const & that
		) = default;

		constexpr ListView (
			ListView && that
		) = default;

		// ----------------

		explicit constexpr ListView (
			QualifyIterator const & begin,
			Size const &            size
		) :
			m_data{begin},
			m_size{size} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			ListView const & that
		) -> ListView & = default;

		constexpr auto operator = (
			ListView && that
		) -> ListView & = default;

		// ----------------

		constexpr auto operator [] (
			Size const & index
		) const -> QualifyElement & {
			return thiz.at(index);
		}

		// ----------------

		implicit operator ConstantView & () requires
			(!constant.value) {
			return self_cast<ConstantView>(thiz);
		}

		implicit operator ConstantView const & () const requires
			(!constant.value) {
			return self_cast<ConstantView>(thiz);
		}

		#pragma endregion

		#pragma region set

		constexpr auto reset (
		) -> Void {
			thiz.m_data = k_null_pointer;
			thiz.m_size = k_none_size;
			return;
		}

		constexpr auto set (
			QualifyIterator const & data,
			Size const &            size
		) -> Void {
			thiz.m_data = data;
			thiz.m_size = size;
			return;
		}

		#pragma endregion

		#pragma region size & index

		constexpr auto size (
		) const -> Size {
			return thiz.m_size;
		}

		constexpr auto empty (
		) const -> Boolean {
			return thiz.size() == k_none_size;
		}

		// ----------------

		constexpr auto begin_index (
		) const -> Size {
			return k_begin_index;
		}

		constexpr auto end_index (
		) const -> Size {
			return thiz.size();
		}

		// ----------------

		constexpr auto first_index (
		) const -> Size {
			assert_test(!thiz.empty());
			return thiz.begin_index();
		}

		constexpr auto last_index (
		) const -> Size {
			assert_test(!thiz.empty());
			return thiz.end_index() - k_next_index;
		}

		#pragma endregion

		#pragma region iterator

		constexpr auto iterator (
			Size const & index
		) const -> QualifyIterator {
			assert_test(index <= thiz.end_index());
			return QualifyIterator{thiz.m_data + index};
		}

		constexpr auto begin (
		) const -> QualifyIterator {
			return QualifyIterator{thiz.m_data + thiz.begin_index()};
		}

		constexpr auto end (
		) const -> QualifyIterator {
			return QualifyIterator{thiz.m_data + thiz.end_index()};
		}

		#pragma endregion

		#pragma region element

		constexpr auto at (
			Size const & index
		) const -> QualifyElement & {
			assert_test(index < thiz.end_index());
			return thiz.m_data.dereference(index);
		}

		constexpr auto first (
		) const -> QualifyElement & {
			return thiz.at(thiz.first_index());
		}

		constexpr auto last (
		) const -> QualifyElement & {
			return thiz.at(thiz.last_index());
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub (
			Size const & begin,
			Size const & size
		) const -> ListView {
			assert_test(begin + size <= thiz.size());
			return ListView{thiz.iterator(begin), size};
		}

		constexpr auto head (
			Size const & size
		) const -> ListView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.begin_index(), size);
		}

		constexpr auto tail (
			Size const & size
		) const -> ListView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.end_index() - size, size);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			ListView const & thix,
			ListView const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Element> requires
		AutomaticConstraint
	using VariableListView = ListView<Element, k_false>;

	template <typename Element> requires
		AutomaticConstraint
	using ConstantListView = ListView<Element, k_true>;

	#pragma endregion

}
