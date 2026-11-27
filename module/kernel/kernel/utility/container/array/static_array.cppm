module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.array.static_array;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.generic_range;
import twinning.kernel.utility.math.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_size> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_size, Size>)
	class StaticArray {

	public:

		using Element = TElement;

		using VariableElement = Element;

		using VariableIterator = Pointer<VariableElement>;

		using VariableView = ArrayView<Element, k_false>;

		using ConstantElement = Element const;

		using ConstantIterator = Pointer<ConstantElement>;

		using ConstantView = ArrayView<Element, k_true>;

	protected:

		ZArray<Element, t_size == 0_sz ? (1_szz) : (t_size.value)> m_data;

	public:

		#pragma region constructor

		constexpr ~StaticArray(
		) = default;

		// ----------------

		constexpr StaticArray(
		) :
			m_data{Element{}} {
			return;
		}

		constexpr StaticArray(
			StaticArray const & that
		) = default;

		constexpr StaticArray(
			StaticArray && that
		) = default;

		// ----------------

		explicit constexpr StaticArray(
			ConstantView const & view
		) :
			StaticArray{} {
			Range::assign_from(Range::make_range_n(thiz.begin(), Math::minimum(view.size(), thiz.size())), view);
			return;
		}

		explicit constexpr StaticArray(
			ZArray<Element, t_size.value> const & raw
		) :
			StaticArray{ConstantView{make_pointer(static_cast<decltype(&*raw)>(raw)), t_size}} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			StaticArray const & that
		) -> StaticArray & = default;

		constexpr auto operator =(
			StaticArray && that
		) -> StaticArray & = default;

		// ----------------

		constexpr auto operator [](
			Size const & index
		) -> VariableElement & {
			return thiz.at(index);
		}

		constexpr auto operator [](
			Size const & index
		) const -> ConstantElement & {
			return thiz.at(index);
		}

		// ----------------

		implicit operator VariableView() {
			return thiz.view();
		}

		implicit operator ConstantView() const {
			return thiz.view();
		}

		#pragma endregion

		#pragma region view

		constexpr auto view(
		) -> VariableView {
			return VariableView{thiz.data(), thiz.size()};
		}

		constexpr auto view(
		) const -> ConstantView {
			return ConstantView{thiz.data(), thiz.size()};
		}

		#pragma endregion

		#pragma region data

		constexpr auto data(
		) -> VariableIterator {
			return VariableIterator{thiz.m_data};
		}

		constexpr auto data(
		) const -> ConstantIterator {
			return ConstantIterator{thiz.m_data};
		}

		#pragma endregion

		#pragma region size

		constexpr auto size(
		) const -> Size {
			return t_size;
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
		) -> VariableIterator {
			assert_test(index <= thiz.end_index());
			return VariableIterator{thiz.m_data} + index;
		}

		constexpr auto begin(
		) -> VariableIterator {
			return VariableIterator{thiz.m_data} + thiz.begin_index();
		}

		constexpr auto end(
		) -> VariableIterator {
			return VariableIterator{thiz.m_data} + thiz.end_index();
		}

		// ----------------

		constexpr auto iterator(
			Size const & index
		) const -> ConstantIterator {
			assert_test(index <= thiz.end_index());
			return ConstantIterator{thiz.m_data} + index;
		}

		constexpr auto begin(
		) const -> ConstantIterator {
			return thiz.iterator(thiz.begin_index());
		}

		constexpr auto end(
		) const -> ConstantIterator {
			return thiz.iterator(thiz.end_index());
		}

		#pragma endregion

		#pragma region element

		constexpr auto at(
			Size const & index
		) -> VariableElement & {
			assert_test(index < thiz.end_index());
			return thiz.m_data[unmake_box<ZSize>(index)];
		}

		constexpr auto first(
		) -> VariableElement & {
			return thiz.at(thiz.first_index());
		}

		constexpr auto last(
		) -> VariableElement & {
			return thiz.at(thiz.last_index());
		}

		// ----------------

		constexpr auto at(
			Size const & index
		) const -> ConstantElement & {
			assert_test(index < thiz.end_index());
			return thiz.m_data[unmake_box<ZSize>(index)];
		}

		constexpr auto first(
		) const -> ConstantElement & {
			return thiz.at(thiz.first_index());
		}

		constexpr auto last(
		) const -> ConstantElement & {
			return thiz.at(thiz.last_index());
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			StaticArray const & thix,
			StaticArray const & that
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
	inline constexpr auto make_static_array(
		TArgument && ... argument
	) -> StaticArray<TElement, make_box<Size>(sizeof...(TArgument))> {
		auto result = StaticArray<TElement, make_box<Size>(sizeof...(TArgument))>{};
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
