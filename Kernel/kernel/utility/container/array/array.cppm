module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.miscellaneous.allocation;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.range_wrapper;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
	class Array {

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

	public:

		#pragma region structor

		~Array (
		) {
			thiz.reset();
		}

		// ----------------

		Array (
		) :
			m_data{},
			m_size{} {
		}

		Array (
			Array const & that
		) :
			Array{} {
			thiz = that;
		}

		Array (
			Array && that
		) :
			Array{} {
			thiz = as_moveable(that);
		}

		// ----------------

		explicit Array (
			Size const & size
		) :
			Array{} {
			thiz.allocate(size);
		}

		explicit Array (
			CView const & view
		) :
			Array{} {
			thiz = view;
		}

		explicit Array (
			CIterator const & begin,
			Size const &      size
		) :
			Array{CView{begin, size}} {
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Array const & that
		) -> Array & {
			thiz.assign(that);
			return thiz;
		}

		auto operator = (
			Array && that
		) -> Array & {
			thiz.bind(that);
			that.unbind();
			return thiz;
		}

		// ----------------

		auto operator = (
			CView const & that
		) -> Array & {
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

		#pragma region bind

		auto unbind (
		) -> Void {
			thiz.m_data = k_null_pointer;
			thiz.m_size = k_none_size;
			return;
		}

		auto bind (
			Iterator const & begin,
			Size const &     size
		) -> Void {
			thiz.reset();
			thiz.m_data = begin;
			thiz.m_size = size;
			return;
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
			return;
		}

		auto allocate (
			Size const & size
		) -> Void {
			thiz.reset();
			thiz.m_data = allocate_instance_array<Element>(size);
			thiz.m_size = size;
			return;
		}

		// ----------------

		auto allocate_retain (
			Size const & size
		) -> Void {
			auto old = as_moveable(thiz);
			auto old_size = minimum(size, old.size());
			thiz.allocate(size);
			Range::assign_from(thiz.head(old_size), Range::make_moveable_range_of(old.head(old_size)));
			old.reset();
			return;
		}

		auto shrink (
			Size const & size
		) -> Void {
			assert_test(size <= thiz.size());
			return thiz.allocate_retain(thiz.size() - size);
		}

		auto expand (
			Size const & size
		) -> Void {
			return thiz.allocate_retain(thiz.size() + size);
		}

		#pragma endregion

		#pragma region assign

		template <typename RangeObject> requires
			CategoryConstraint<IsValid<RangeObject>>
			&& (IsRange<AsPure<RangeObject>>)
		auto assign (
			RangeObject && range
		) -> Void {
			thiz.allocate(Range::size(as_forward<RangeObject>(range)));
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
			thiz.allocate(Range::size(as_forward<RangeObject>(range)));
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
			thiz.allocate(Range::size(as_forward<RangeObject>(range)));
			Range::convert_from(thiz, as_forward<RangeObject>(range), converter);
			return;
		}

		#pragma endregion

		#pragma region element force

		auto at_force (
			Size const & index
		) -> VElement & {
			if (index >= thiz.end_index()) {
				thiz.allocate_retain(index + k_next_index);
			}
			return thiz.at(index);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator == (
			Array const & thix,
			Array const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Element, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Element> && IsValid<Argument ...>>
		&& (IsConstructible<Element, Argument &&> && ...)
	inline auto make_array (
		Argument && ... argument
	) -> Array<Element> {
		auto result = Array<Element>{mbox<Size>(sizeof...(Argument))};
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
