module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.memory.utility;
import twinning.kernel.utility.math.utility;
import twinning.kernel.utility.container.array.array_view;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
	class Array {

	public:

		using Element = TElement;

		using VariableElement = Element;

		using VariableIterator = Pointer<VariableElement>;

		using VariableView = ArrayView<Element, k_false>;

		using ConstantElement = Element const;

		using ConstantIterator = Pointer<ConstantElement>;

		using ConstantView = ArrayView<Element, k_true>;

	protected:

		VariableIterator m_data;

		Size m_size;

		Size m_capacity;

	public:

		#pragma region constructor

		~Array(
		) {
			thiz.reset();
			return;
		}

		// ----------------

		Array(
		) :
			m_data{},
			m_size{},
			m_capacity{} {
			return;
		}

		Array(
			Array const & that
		) :
			Array{} {
			thiz = that;
			return;
		}

		Array(
			Array && that
		) :
			Array{} {
			thiz = as_moveable(that);
			return;
		}

		// ----------------

		explicit Array(
			Size const & size
		) :
			Array{} {
			thiz.allocate_full(size);
			return;
		}

		explicit Array(
			ConstantView const & view
		) :
			Array{} {
			thiz = view;
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			Array const & that
		) -> Array & {
			M_avoid_self_assignment();
			thiz.assign(that);
			return thiz;
		}

		auto operator =(
			Array && that
		) -> Array & {
			M_avoid_self_assignment();
			thiz.bind(that.m_data, that.m_size, that.m_capacity);
			that.unbind();
			return thiz;
		}

		// ----------------

		auto operator =(
			ConstantView const & that
		) -> Array & {
			thiz.assign(auto{that});
			return thiz;
		}

		// ----------------

		auto operator [](
			Size const & index
		) -> VariableElement & {
			return thiz.at(index);
		}

		auto operator [](
			Size const & index
		) const -> ConstantElement & {
			return thiz.at(index);
		}

		// ----------------

		implicit operator VariableView const &() {
			return thiz.as_view();
		}

		implicit operator ConstantView const &() const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		auto as_view(
		) -> VariableView const & {
			return unsafe_cast<VariableView>(thiz);
		}

		auto as_view(
		) const -> ConstantView const & {
			return unsafe_cast<ConstantView>(thiz);
		}

		#pragma endregion

		#pragma region data

		auto data(
		) -> VariableIterator {
			return thiz.as_view().data();
		}

		auto data(
		) const -> ConstantIterator {
			return thiz.as_view().data();
		}

		#pragma endregion

		#pragma region size

		auto size(
		) const -> Size {
			return thiz.as_view().size();
		}

		// ----------------

		auto empty(
		) const -> Boolean {
			return thiz.as_view().empty();
		}

		#pragma endregion

		#pragma region index

		auto begin_index(
		) const -> Size {
			return thiz.as_view().begin_index();
		}

		auto end_index(
		) const -> Size {
			return thiz.as_view().end_index();
		}

		// ----------------

		auto first_index(
		) const -> Size {
			return thiz.as_view().first_index();
		}

		auto last_index(
		) const -> Size {
			return thiz.as_view().last_index();
		}

		#pragma endregion

		#pragma region iterator

		auto iterator(
			Size const & index
		) -> VariableIterator {
			return thiz.as_view().iterator(index);
		}

		auto begin(
		) -> VariableIterator {
			return thiz.as_view().begin();
		}

		auto end(
		) -> VariableIterator {
			return thiz.as_view().end();
		}

		// ----------------

		auto iterator(
			Size const & index
		) const -> ConstantIterator {
			return thiz.as_view().iterator(index);
		}

		auto begin(
		) const -> ConstantIterator {
			return thiz.as_view().begin();
		}

		auto end(
		) const -> ConstantIterator {
			return thiz.as_view().end();
		}

		#pragma endregion

		#pragma region element

		auto at(
			Size const & index
		) -> VariableElement & {
			return thiz.as_view().at(index);
		}

		auto first(
		) -> VariableElement & {
			return thiz.as_view().first();
		}

		auto last(
		) -> VariableElement & {
			return thiz.as_view().last();
		}

		// ----------------

		auto at(
			Size const & index
		) const -> ConstantElement & {
			return thiz.as_view().at(index);
		}

		auto first(
		) const -> ConstantElement & {
			return thiz.as_view().first();
		}

		auto last(
		) const -> ConstantElement & {
			return thiz.as_view().last();
		}

		#pragma endregion

		#pragma region sub

		auto sub(
			Size const & begin,
			Size const & count
		) -> VariableView {
			return thiz.as_view().sub(begin, count);
		}

		auto head(
			Size const & count
		) -> VariableView {
			return thiz.as_view().head(count);
		}

		auto tail(
			Size const & count
		) -> VariableView {
			return thiz.as_view().tail(count);
		}

		// ----------------

		auto sub(
			Size const & begin,
			Size const & count
		) const -> ConstantView {
			return thiz.as_view().sub(begin, count);
		}

		auto head(
			Size const & count
		) const -> ConstantView {
			return thiz.as_view().head(count);
		}

		auto tail(
			Size const & count
		) const -> ConstantView {
			return thiz.as_view().tail(count);
		}

		#pragma endregion

		#pragma region capacity

		auto capacity(
		) const -> Size {
			return thiz.m_capacity;
		}

		auto reserve(
		) const -> Size {
			return thiz.capacity() - thiz.size();
		}

		auto full(
		) const -> Boolean {
			return thiz.size() == thiz.capacity();
		}

		#pragma endregion

		#pragma region bind

		auto unbind(
		) -> Void {
			thiz.m_data = k_null_pointer;
			thiz.m_size = 0_sz;
			thiz.m_capacity = 0_sz;
			return;
		}

		auto bind(
			VariableIterator const & data,
			Size const &             size,
			Size const &             capacity
		) -> Void {
			assert_test(size <= capacity);
			thiz.reset();
			thiz.m_data = data;
			thiz.m_size = size;
			thiz.m_capacity = capacity;
			return;
		}

		#pragma endregion

		#pragma region allocation

		auto reset(
		) -> Void {
			if (thiz.m_data != k_null_pointer) {
				Memory::free_array(thiz.m_data);
			}
			thiz.m_size = 0_sz;
			thiz.m_capacity = 0_sz;
			return;
		}

		auto allocate(
			Size const & capacity
		) -> Void {
			thiz.reset();
			thiz.m_data = Memory::allocate_array<Element>(capacity);
			thiz.m_size = 0_sz;
			thiz.m_capacity = capacity;
			return;
		}

		auto allocate_full(
			Size const & capacity
		) -> Void {
			thiz.allocate(capacity);
			thiz.resize(capacity);
			return;
		}

		auto reallocate(
			Size const & capacity
		) -> Void {
			auto old = as_moveable(thiz);
			auto old_size = Math::minimum(capacity, old.size());
			thiz.allocate(capacity);
			thiz.resize(old_size);
			Range::assign_from(thiz, Range::make_moveable_range_of(old.head(old_size)));
			old.reset();
			return;
		}

		#pragma endregion

		#pragma region resize

		auto resize(
			Size const & size
		) -> Void {
			assert_test(size <= thiz.capacity());
			thiz.m_size = size;
			return;
		}

		#pragma endregion

		#pragma region assign

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto assign(
			TRange && range
		) -> Void {
			auto _ = as_moveable(thiz);
			thiz.allocate_full(Range::size(as_forward<TRange>(range)));
			Range::assign_from(thiz, as_forward<TRange>(range));
			return;
		}

		template <typename TRange, typename TTransformer> requires
			CategoryConstraint<IsValid<TRange> && IsPureInstance<TTransformer>>
			&& (IsRange<AsPure<TRange>>)
			&& (IsGenericCallable<TTransformer>)
		auto assign(
			TRange &&            range,
			TTransformer const & transformer
		) -> Void {
			auto _ = as_moveable(thiz);
			thiz.allocate_full(Range::size(as_forward<TRange>(range)));
			Range::assign_from(thiz, as_forward<TRange>(range), transformer);
			return;
		}

		template <typename TRange, typename TConverter> requires
			CategoryConstraint<IsValid<TRange> && IsPureInstance<TConverter>>
			&& (IsRange<AsPure<TRange>>)
			&& (IsGenericCallable<TConverter>)
		auto convert(
			TRange &&          range,
			TConverter const & converter
		) -> Void {
			auto _ = as_moveable(thiz);
			thiz.allocate_full(Range::size(as_forward<TRange>(range)));
			Range::convert_from(thiz, as_forward<TRange>(range), converter);
			return;
		}

		#pragma endregion

		#pragma region insert

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Element, TArgument && ...>)
		auto insert(
			Size const &     position,
			TArgument && ... argument
		) -> VariableElement & {
			auto element = Element{as_forward<TArgument>(argument) ...};
			thiz.insert_space(position, 1_sz);
			restruct(thiz.at(position), as_moveable(element));
			return thiz.at(position);
		}

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto insert_range(
			Size const & position,
			TRange &&    range
		) -> Void {
			// NOTE: BUG: ub if memory overlap
			auto size = Range::size(as_forward<TRange>(range));
			thiz.insert_space(position, size);
			Range::restruct_from(thiz.sub(position, size), as_forward<TRange>(range));
			return;
		}

		// ----------------

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Element, TArgument && ...>)
		auto prepend(
			TArgument && ... argument
		) -> VariableElement & {
			return thiz.insert(thiz.begin_index(), as_forward<TArgument>(argument) ...);
		}

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto prepend_range(
			TRange && range
		) -> Void {
			return thiz.insert_range(thiz.begin_index(), as_forward<TRange>(range));
		}

		// ----------------

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Element, TArgument && ...>)
		auto append(
			TArgument && ... argument
		) -> VariableElement & {
			return thiz.insert(thiz.end_index(), as_forward<TArgument>(argument) ...);
		}

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto append_range(
			TRange && range
		) -> Void {
			return thiz.insert_range(thiz.end_index(), as_forward<TRange>(range));
		}

		#pragma endregion

		#pragma region remove

		auto remove(
			Size const & begin,
			Size const & count
		) -> Void {
			assert_test(begin + count <= thiz.end_index());
			auto move_count = thiz.size() - (begin + count);
			Range::assign_from(thiz.sub(begin, move_count), Range::make_moveable_range_of(thiz.sub(begin + count, move_count)));
			Range::restruct(thiz.tail(count));
			thiz.m_size -= count;
			return;
		}

		auto remove_at(
			Size const & position
		) -> Void {
			return thiz.remove(position, 1_sz);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator ==(
			Array const & thix,
			Array const & that
		) -> bool {
			return thix.as_view() == that.as_view();
		}

		#pragma endregion

	protected:

		#pragma region insert space

		auto insert_space(
			Size const & position,
			Size const & count
		) -> Void {
			assert_test(position <= thiz.end_index());
			if (count > thiz.reserve()) {
				thiz.reallocate((thiz.size() + count) * 2_sz);
			}
			auto shift_begin = thiz.end_index();
			auto shift_count = thiz.end_index() - position;
			thiz.m_size += count;
			if (shift_count != 0_sz) {
				Range::assign_from(Range::make_reverse_range_n(thiz.end(), shift_count), Range::make_moveable_range_of(Range::make_reverse_range_n(thiz.iterator(shift_begin), shift_count)));
			}
			return;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TElement, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TElement> && IsValid<TArgument ...>>
		&& (IsConstructible<TElement, TArgument &&> && ...)
	inline auto make_array(
		TArgument && ... argument
	) -> Array<TElement> {
		auto result = Array<TElement>{make_box<Size>(sizeof...(TArgument))};
		Generalization::each_with<>(
			[&]<auto t_index, typename TCurrentArgument>(ValuePackage<t_index>, TCurrentArgument && current_argument) {
				restruct(result.at(make_box<Size>(t_index)), as_forward<TCurrentArgument>(current_argument));
			},
			as_forward<TArgument>(argument) ...
		);
		return result;
	}

	#pragma endregion

}
