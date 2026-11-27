module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.list;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.container.array.array;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
	class List {

	public:

		using Element = TElement;

		using VariableElement = Element;

		using VariableIterator = Pointer<VariableElement>;

		using ConstantElement = Element const;

		using ConstantIterator = Pointer<ConstantElement>;

	protected:

		Array<Element> m_container;

	public:

		#pragma region constructor

		~List(
		) = default;

		// ----------------

		List(
		) :
			m_container{} {
			return;
		}

		List(
			List const & that
		) = default;

		List(
			List && that
		) = default;

		// ----------------

		explicit List(
			Size const & size
		) :
			List{} {
			thiz.allocate_full(size);
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			List const & that
		) -> List & = default;

		auto operator =(
			List && that
		) -> List & = default;

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

		auto operator ()(
			Size const & index
		) -> VariableElement & {
			return thiz.at_force(index);
		}

		#pragma endregion

		#pragma region size

		auto size(
		) const -> Size {
			return thiz.m_container.size();
		}

		// ----------------

		auto empty(
		) const -> Boolean {
			return thiz.m_container.empty();
		}

		#pragma endregion

		#pragma region index

		auto begin_index(
		) const -> Size {
			return thiz.m_container.begin_index();
		}

		auto end_index(
		) const -> Size {
			return thiz.m_container.end_index();
		}

		// ----------------

		auto first_index(
		) const -> Size {
			return thiz.m_container.first_index();
		}

		auto last_index(
		) const -> Size {
			return thiz.m_container.last_index();
		}

		#pragma endregion

		#pragma region iterator

		auto iterator(
			Size const & index
		) -> VariableIterator {
			return thiz.m_container.iterator(index);
		}

		auto begin(
		) -> VariableIterator {
			return thiz.m_container.begin();
		}

		auto end(
		) -> VariableIterator {
			return thiz.m_container.end();
		}

		// ----------------

		auto iterator(
			Size const & index
		) const -> ConstantIterator {
			return thiz.m_container.iterator(index);
		}

		auto begin(
		) const -> ConstantIterator {
			return thiz.m_container.begin();
		}

		auto end(
		) const -> ConstantIterator {
			return thiz.m_container.end();
		}

		#pragma endregion

		#pragma region element

		auto at(
			Size const & index
		) -> VariableElement & {
			return thiz.m_container.at(index);
		}

		auto first(
		) -> VariableElement & {
			return thiz.m_container.first();
		}

		auto last(
		) -> VariableElement & {
			return thiz.m_container.last();
		}

		// ----------------

		auto at(
			Size const & index
		) const -> ConstantElement & {
			return thiz.m_container.at(index);
		}

		auto first(
		) const -> ConstantElement & {
			return thiz.m_container.first();
		}

		auto last(
		) const -> ConstantElement & {
			return thiz.m_container.last();
		}

		#pragma endregion

		#pragma region capacity

		auto capacity(
		) const -> Size {
			return thiz.m_container.capacity();
		}

		auto reserve(
		) const -> Size {
			return thiz.m_container.reserve();
		}

		auto full(
		) const -> Boolean {
			return thiz.m_container.full();
		}

		#pragma endregion

		#pragma region allocation

		auto reset(
		) -> Void {
			return thiz.m_container.reset();
		}

		auto allocate(
			Size const & capacity
		) -> Void {
			return thiz.m_container.allocate(capacity);
		}

		auto allocate_full(
			Size const & capacity
		) -> Void {
			return thiz.m_container.allocate_full(capacity);
		}

		auto reallocate(
			Size const & capacity
		) -> Void {
			return thiz.m_container.reallocate(capacity);
		}

		#pragma endregion

		#pragma region resize

		auto resize(
			Size const & size
		) -> Void {
			return thiz.m_container.resize(size);
		}

		#pragma endregion

		#pragma region assign

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto assign(
			TRange && range
		) -> Void {
			return thiz.m_container.assign(as_forward<TRange>(range));
		}

		template <typename TRange, typename TTransformer> requires
			CategoryConstraint<IsValid<TRange> && IsPureInstance<TTransformer>>
			&& (IsRange<AsPure<TRange>>)
			&& (IsGenericCallable<TTransformer>)
		auto assign(
			TRange &&            range,
			TTransformer const & transformer
		) -> Void {
			return thiz.m_container.assign(as_forward<TRange>(range), transformer);
		}

		template <typename TRange, typename TConverter> requires
			CategoryConstraint<IsValid<TRange> && IsPureInstance<TConverter>>
			&& (IsRange<AsPure<TRange>>)
			&& (IsGenericCallable<TConverter>)
		auto convert(
			TRange &&          range,
			TConverter const & converter
		) -> Void {
			return thiz.m_container.convert(as_forward<TRange>(range), converter);
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
			return thiz.m_container.insert(position, as_forward<TArgument>(argument) ...);
		}

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto insert_range(
			Size const & position,
			TRange &&    range
		) -> Void {
			return thiz.m_container.insert_range(position, as_forward<TRange>(range));
		}

		// ----------------

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Element, TArgument && ...>)
		auto prepend(
			TArgument && ... argument
		) -> VariableElement & {
			return thiz.m_container.prepend(as_forward<TArgument>(argument) ...);
		}

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto prepend_range(
			TRange && range
		) -> Void {
			return thiz.m_container.prepend_range(as_forward<TRange>(range));
		}

		// ----------------

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Element, TArgument && ...>)
		auto append(
			TArgument && ... argument
		) -> VariableElement & {
			return thiz.m_container.append(as_forward<TArgument>(argument) ...);
		}

		template <typename TRange> requires
			CategoryConstraint<IsValid<TRange>>
			&& (IsRange<AsPure<TRange>>)
		auto append_range(
			TRange && range
		) -> Void {
			return thiz.m_container.append_range(as_forward<TRange>(range));
		}

		#pragma endregion

		#pragma region remove

		auto remove(
			Size const & begin,
			Size const & count
		) -> Void {
			return thiz.m_container.remove(begin, count);
		}

		auto remove_at(
			Size const & position
		) -> Void {
			return thiz.m_container.remove_at(position);
		}

		#pragma endregion

		#pragma region element force

		auto at_force(
			Size const & index
		) -> VariableElement & {
			if (index >= thiz.end_index()) {
				auto new_size = index + 1_sz;
				if (new_size > thiz.capacity()) {
					thiz.reallocate(new_size * 2_sz);
				}
				thiz.resize(new_size);
			}
			return thiz.at(index);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator ==(
			List const & thix,
			List const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TElement, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TElement> && IsValid<TArgument ...>>
		&& (IsConstructible<TElement, TArgument &&> && ...)
	inline auto make_list(
		TArgument && ... argument
	) -> List<TElement> {
		auto result = List<TElement>{make_box<Size>(sizeof...(TArgument))};
		Generalization::each_with<>(
			[&]<auto t_index, typename TCurrentArgument>(ValuePackage<t_index>, TCurrentArgument && current_argument) {
				restruct(result.at(make_box<Size>(t_index)), as_forward<TCurrentArgument>(current_argument));
			},
			as_forward<TArgument>(argument) ...
		);
		return result;
	}

	template <typename TElement, typename TRange> requires
		CategoryConstraint<IsPureInstance<TElement> && IsValid<TRange>>
		&& (IsRange<AsPure<TRange>>)
	inline auto make_list_of(
		TRange && range
	) -> List<TElement> {
		auto result = List<TElement>{};
		result.assign(as_forward<TRange>(range));
		return result;
	}

	#pragma endregion

}
