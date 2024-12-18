module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.range.range_wrapper;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.miscellaneous.allocation;
import twinning.kernel.utility.container.list.list_view;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
	class List {

	public:

		using Element = TElement;

		using Iterator = Pointer<Element>;

		using View = ListView<Element, k_false>;

		using VElement = Element;

		using VIterator = Pointer<Element>;

		using VView = ListView<Element, k_false>;

		using CElement = Element const;

		using CIterator = Pointer<Element const>;

		using CView = ListView<Element, k_true>;

	protected:

		Iterator m_data;

		Size m_size;

		Size m_capacity;

	public:

		#pragma region structor

		~List (
		) {
			thiz.reset();
		}

		// ----------------

		List (
		) :
			m_data{},
			m_size{},
			m_capacity{} {
		}

		List (
			List const & that
		) :
			List{} {
			thiz = that;
		}

		List (
			List && that
		) :
			List{} {
			thiz = as_moveable(that);
		}

		// ----------------

		explicit List (
			Size const & capacity
		) :
			List{} {
			thiz.allocate(capacity);
		}

		explicit List (
			CView const & view
		) :
			List{} {
			thiz = view;
		}

		explicit List (
			CIterator const & begin,
			Size const &      size
		) :
			List{CView{begin, size}} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			List const & that
		) -> List & {
			thiz.assign(that);
			return thiz;
		}

		auto operator = (
			List && that
		) -> List & {
			thiz.bind(that);
			that.unbind();
			return thiz;
		}

		// ----------------

		auto operator = (
			CView const & that
		) -> List & {
			thiz.assign(that);
			return thiz;
		}

		// ----------------

		auto operator [] (
			Size const & index
		) -> VElement & {
			return thiz.at(index);
		}

		auto operator [] (
			Size const & index
		) const -> CElement & {
			return thiz.at(index);
		}

		// ----------------

		auto operator () (
			Size const & index
		) -> VElement & {
			return thiz.at_force(index);
		}

		// ----------------

		implicit operator VView const & () {
			return thiz.as_view();
		}

		implicit operator CView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		auto as_view (
		) -> VView const & {
			return self_cast<VView>(thiz);
		}

		auto as_view (
		) const -> CView const & {
			return self_cast<CView>(thiz);
		}

		// ----------------

		auto view (
		) -> VView {
			return thiz.as_view();
		}

		auto view (
		) const -> CView {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region size & index

		auto size (
		) const -> Size {
			return thiz.as_view().size();
		}

		auto empty (
		) const -> Boolean {
			return thiz.as_view().empty();
		}

		// ----------------

		auto begin_index (
		) const -> Size {
			return thiz.as_view().begin_index();
		}

		auto end_index (
		) const -> Size {
			return thiz.as_view().end_index();
		}

		// ----------------

		auto first_index (
		) const -> Size {
			return thiz.as_view().first_index();
		}

		auto last_index (
		) const -> Size {
			return thiz.as_view().last_index();
		}

		#pragma endregion

		#pragma region iterator

		auto iterator (
			Size const & index
		) -> VIterator {
			return thiz.as_view().iterator(index);
		}

		auto begin (
		) -> VIterator {
			return thiz.as_view().begin();
		}

		auto end (
		) -> VIterator {
			return thiz.as_view().end();
		}

		// ----------------

		auto iterator (
			Size const & index
		) const -> CIterator {
			return thiz.as_view().iterator(index);
		}

		auto begin (
		) const -> CIterator {
			return thiz.as_view().begin();
		}

		auto end (
		) const -> CIterator {
			return thiz.as_view().end();
		}

		#pragma endregion

		#pragma region element

		auto at (
			Size const & index
		) -> VElement & {
			return thiz.as_view().at(index);
		}

		auto first (
		) -> VElement & {
			return thiz.as_view().first();
		}

		auto last (
		) -> VElement & {
			return thiz.as_view().last();
		}

		// ----------------

		auto at (
			Size const & index
		) const -> CElement & {
			return thiz.as_view().at(index);
		}

		auto first (
		) const -> CElement & {
			return thiz.as_view().first();
		}

		auto last (
		) const -> CElement & {
			return thiz.as_view().last();
		}

		#pragma endregion

		#pragma region sub

		auto sub (
			Size const & begin,
			Size const & size
		) -> VView {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) -> VView {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) -> VView {
			return thiz.as_view().tail(size);
		}

		// ----------------

		auto sub (
			Size const & begin,
			Size const & size
		) const -> CView {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) const -> CView {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) const -> CView {
			return thiz.as_view().tail(size);
		}

		#pragma endregion

		#pragma region capacity

		auto capacity (
		) const -> Size {
			return thiz.m_capacity;
		}

		auto reserve (
		) const -> Size {
			return thiz.capacity() - thiz.size();
		}

		auto full (
		) const -> Boolean {
			return thiz.size() == thiz.capacity();
		}

		#pragma endregion

		#pragma region set size

		auto set_size (
			Size const & size
		) -> Void {
			assert_test(size <= thiz.capacity());
			thiz.m_size = size;
			return;
		}

		auto shrink_size (
			Size const & size
		) -> Void {
			assert_test(size <= thiz.size());
			return thiz.set_size(thiz.size() - size);
		}

		auto expand_size (
			Size const & size
		) -> Void {
			assert_test(size <= thiz.reserve());
			return thiz.set_size(thiz.size() + size);
		}

		auto expand_size_to_full (
		) -> Void {
			return thiz.set_size(thiz.capacity());
		}

		#pragma endregion

		#pragma region bind

		auto unbind (
		) -> Void {
			thiz.m_data = k_null_pointer;
			thiz.m_size = k_none_size;
			thiz.m_capacity = k_none_size;
			return;
		}

		auto bind (
			Iterator const & begin,
			Size const &     size,
			Size const &     capacity
		) -> Void {
			assert_test(size <= capacity);
			thiz.reset();
			thiz.m_data = begin;
			thiz.m_size = size;
			thiz.m_capacity = capacity;
			return;
		}

		auto bind (
			Iterator const & begin,
			Size const &     size
		) -> Void {
			return thiz.bind(begin, size, size);
		}

		auto bind (
			View const & view
		) -> Void {
			return thiz.bind(view.begin(), view.size());
		}

		#pragma endregion

		#pragma region allocate

		auto reset (
		) -> Void {
			if (thiz.m_data != k_null_pointer) {
				free_instance_array(thiz.m_data);
			}
			thiz.m_size = k_none_size;
			thiz.m_capacity = k_none_size;
			return;
		}

		auto allocate (
			Size const & capacity
		) -> Void {
			thiz.reset();
			thiz.m_data = allocate_instance_array<Element>(capacity);
			thiz.m_size = k_none_size;
			thiz.m_capacity = capacity;
			return;
		}

		auto allocate_full (
			Size const & capacity
		) -> Void {
			thiz.allocate(capacity);
			thiz.expand_size_to_full();
			return;
		}

		// ----------------

		auto allocate_retain (
			Size const & capacity
		) -> Void {
			auto old = as_moveable(thiz);
			auto old_size = minimum(capacity, old.size());
			thiz.allocate(capacity);
			thiz.set_size(old_size);
			Range::assign_from(thiz, Range::make_moveable_range_of(old.head(old_size)));
			old.reset();
			return;
		}

		auto shrink (
			Size const & capacity
		) -> Void {
			assert_test(capacity <= thiz.capacity());
			return thiz.allocate_retain(thiz.capacity() - capacity);
		}

		auto expand (
			Size const & capacity
		) -> Void {
			return thiz.allocate_retain(thiz.capacity() + capacity);
		}

		auto shrink_to_fit (
		) -> Void {
			return thiz.allocate_retain(thiz.size());
		}

		#pragma endregion

		#pragma region assign

		template <typename RangeObject> requires
			CategoryConstraint<IsValid<RangeObject>>
			&& (IsRange<AsPure<RangeObject>>)
		auto assign (
			RangeObject && range
		) -> Void {
			thiz.allocate_full(Range::size(as_forward<RangeObject>(range)));
			Range::assign_from(thiz, as_forward<RangeObject>(range));
			return;
		}

		template <typename RangeObject, typename Transformer> requires
			CategoryConstraint<IsValid<RangeObject> && IsPureInstance<Transformer>>
			&& (IsRange<AsPure<RangeObject>>)
			&& (IsGenericCallable<Transformer>)
		auto assign (
			RangeObject &&      range,
			Transformer const & transformer
		) -> Void {
			thiz.allocate_full(Range::size(as_forward<RangeObject>(range)));
			Range::assign_from(thiz, as_forward<RangeObject>(range), transformer);
			return;
		}

		template <typename RangeObject, typename Converter> requires
			CategoryConstraint<IsValid<RangeObject> && IsPureInstance<Converter>>
			&& (IsRange<AsPure<RangeObject>>)
			&& (IsGenericCallable<Converter>)
		auto convert (
			RangeObject &&    range,
			Converter const & converter
		) -> Void {
			thiz.allocate_full(Range::size(as_forward<RangeObject>(range)));
			Range::convert_from(thiz, as_forward<RangeObject>(range), converter);
			return;
		}

		#pragma endregion

		#pragma region insert

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Element, Argument && ...>)
		auto insert (
			Size const &    position,
			Argument && ... argument
		) -> VElement & {
			thiz.insert_space(position, 1_sz);
			restruct(thiz.at(position), as_forward<Argument>(argument) ...);
			return thiz.at(position);
		}

		template <typename RangeObject> requires
			CategoryConstraint<IsValid<RangeObject>>
			&& (IsRange<AsPure<RangeObject>>)
		auto insert_list (
			Size const &   position,
			RangeObject && range
		) -> Void {
			auto size = Range::size(as_forward<RangeObject>(range));
			thiz.insert_space(position, size);
			Range::restruct_from(thiz.sub(position, size), as_forward<RangeObject>(range));
			return;
		}

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Element, Argument && ...>)
		auto prepend (
			Argument && ... argument
		) -> VElement & {
			return thiz.insert(thiz.begin_index(), as_forward<Argument>(argument) ...);
		}

		template <typename RangeObject> requires
			CategoryConstraint<IsValid<RangeObject>>
			&& (IsRange<AsPure<RangeObject>>)
		auto prepend_list (
			RangeObject && range
		) -> Void {
			return thiz.insert_list(thiz.begin_index(), as_forward<RangeObject>(range));
		}

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Element, Argument && ...>)
		auto append (
			Argument && ... argument
		) -> VElement & {
			return thiz.insert(thiz.end_index(), as_forward<Argument>(argument) ...);
		}

		template <typename RangeObject> requires
			CategoryConstraint<IsValid<RangeObject>>
			&& (IsRange<AsPure<RangeObject>>)
		auto append_list (
			RangeObject && range
		) -> Void {
			return thiz.insert_list(thiz.end_index(), as_forward<RangeObject>(range));
		}

		#pragma endregion

		#pragma region remove

		auto remove (
			Size const & begin,
			Size const & size
		) -> Void {
			assert_test(begin + size <= thiz.end_index());
			auto move_size = thiz.size() - (begin + size);
			Range::assign_from(thiz.sub(begin, move_size), Range::make_moveable_range_of(thiz.sub(begin + size, move_size)));
			Range::restruct(thiz.tail(size));
			thiz.m_size -= size;
			return;
		}

		auto remove_head (
			Size const & size = 1_sz
		) -> Void {
			return thiz.remove(thiz.begin_index(), size);
		}

		auto remove_tail (
			Size const & size = 1_sz
		) -> Void {
			return thiz.remove(thiz.end_index() - size, size);
		}

		auto remove_at (
			Size const & position
		) -> Void {
			return thiz.remove(position, 1_sz);
		}

		#pragma endregion

		#pragma region element force

		auto at_force (
			Size const & index
		) -> VElement & {
			if (index >= thiz.end_index()) {
				auto new_size = index + k_next_index;
				if (new_size > thiz.capacity()) {
					thiz.allocate_retain(new_size);
				}
				thiz.set_size(new_size);
			}
			return thiz.at(index);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator == (
			List const & thix,
			List const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	protected:

		#pragma region insert space

		auto insert_space (
			Size const & position,
			Size const & size
		) -> Void {
			assert_test(position <= thiz.end_index());
			if (size > thiz.reserve()) {
				thiz.allocate_retain(size > thiz.reserve() ? ((thiz.size() + size) * 2_sz) : (thiz.capacity()));
			}
			auto shift_begin = thiz.end_index();
			auto shift_count = thiz.end_index() - position;
			thiz.m_size += size;
			if (shift_count != k_none_size) {
				Range::assign_from(Range::make_reverse_range_n(thiz.end(), shift_count), Range::make_moveable_range_of(Range::make_reverse_range_n(thiz.iterator(shift_begin), shift_count)));
			}
			return;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Element, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Element> && IsValid<Argument ...>>
		&& (IsConstructible<Element, Argument &&> && ...)
	inline auto make_list (
		Argument && ... argument
	) -> List<Element> {
		auto result = List<Element>{mbox<Size>(sizeof...(Argument))};
		result.expand_size_to_full();
		Generalization::each_with<>(
			[&] <auto index, typename CurrentArgument> (ValuePackage<index>, CurrentArgument && current_argument) {
				restruct(result.at(mbox<Size>(index)), as_forward<CurrentArgument>(current_argument));
			},
			as_forward<Argument>(argument) ...
		);
		return result;
	}

	#pragma endregion

}
