module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.string.basic_string_stream_adapter;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
	class BasicString {

	public:

		using Element = TElement;

		using VariableElement = Element;

		using VariableIterator = Pointer<VariableElement>;

		using VariableView = BasicStringView<Element, k_false>;

		using ConstantElement = Element const;

		using ConstantIterator = Pointer<ConstantElement>;

		using ConstantView = BasicStringView<Element, k_true>;

	protected:

		Array<Element> m_container;

	public:

		#pragma region constructor

		~BasicString(
		) = default;

		// ----------------

		BasicString(
		) :
			m_container{} {
			return;
		}

		BasicString(
			BasicString const & that
		) = default;

		BasicString(
			BasicString && that
		) = default;

		// ----------------

		explicit BasicString(
			Size const & size
		) :
			BasicString{} {
			thiz.allocate_full(size);
			return;
		}

		explicit BasicString(
			ConstantIterator const & data,
			Size const &             size
		) :
			BasicString{} {
			thiz = ConstantView{data, size};
			return;
		}

		explicit BasicString(
			ConstantView const & view
		) :
			BasicString{} {
			thiz = view;
			return;
		}

		explicit BasicString(
			VariableView const & view
		) :
			BasicString{} {
			thiz = view;
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			BasicString const & that
		) -> BasicString & = default;

		auto operator =(
			BasicString && that
		) -> BasicString & = default;

		// ----------------

		auto operator =(
			ConstantView const & that
		) -> BasicString & {
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

		#pragma region bind

		auto unbind(
		) -> Void {
			return thiz.m_container.unbind();
		}

		auto bind(
			VariableIterator const & data,
			Size const &             size,
			Size const &             capacity
		) -> Void {
			return thiz.m_container.bind(data, size, capacity);
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

		#pragma region comparison

		auto compare_three_way(
			ConstantView const & that
		) const -> StrongOrdering {
			return thiz.as_view().compare_three_way(that);
		}

		#pragma endregion

		#pragma region hash

		auto hash(
		) const -> IntegerU64 {
			return thiz.as_view().hash();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator +(
			ConstantView const & thix,
			ConstantView const & that
		) -> BasicString {
			auto result = BasicString{};
			result.allocate(thix.size() + that.size());
			result.append_range(thix);
			result.append_range(that);
			return result;
		}

		inline friend auto operator +=(
			BasicString &        thix,
			ConstantView const & that
		) -> BasicString & {
			thix.append_range(that);
			return thix;
		}

		// ----------------

		inline friend auto operator ==(
			BasicString const &  thix,
			ConstantView const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		inline friend auto operator ==(
			BasicString const & thix,
			BasicString const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		// ----------------

		inline friend auto operator <=>(
			BasicString const &  thix,
			ConstantView const & that
		) -> StrongOrdering {
			return thix.compare_three_way(that);
		}

		inline friend auto operator <=>(
			BasicString const & thix,
			BasicString const & that
		) -> StrongOrdering {
			return thix.compare_three_way(that);
		}

		#pragma endregion

	};

	#pragma endregion

}
