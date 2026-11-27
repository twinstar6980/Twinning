module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.binary.basic_binary_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_constant, Boolean>)
		&& (IsByteBox<TElement>)
	class BasicBinaryView :
		public ArrayView<TElement, t_constant> {

	private:

		using ConstantView = BasicBinaryView<TElement, k_true>;

		using ArrayView = ArrayView<TElement, t_constant>;

	public:

		using typename ArrayView::Element;

		using ArrayView::constant;

		using typename ArrayView::QualifyElement;

		using typename ArrayView::QualifyIterator;

	public:

		#pragma region constructor

		constexpr ~BasicBinaryView(
		) = default;

		// ----------------

		constexpr BasicBinaryView(
		) :
			ArrayView{} {
			return;
		}

		constexpr BasicBinaryView(
			BasicBinaryView const & that
		) = default;

		constexpr BasicBinaryView(
			BasicBinaryView && that
		) = default;

		// ----------------

		using ArrayView::ArrayView;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BasicBinaryView const & that
		) -> BasicBinaryView & = default;

		constexpr auto operator =(
			BasicBinaryView && that
		) -> BasicBinaryView & = default;

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

		#pragma region sub

		constexpr auto sub(
			Size const & begin,
			Size const & count
		) const -> BasicBinaryView {
			assert_test(begin + count <= thiz.size());
			return BasicBinaryView{thiz.iterator(begin), count};
		}

		constexpr auto head(
			Size const & count
		) const -> BasicBinaryView {
			assert_test(count <= thiz.size());
			return thiz.sub(thiz.begin_index(), count);
		}

		constexpr auto tail(
			Size const & count
		) const -> BasicBinaryView {
			assert_test(count <= thiz.size());
			return thiz.sub(thiz.end_index() - count, count);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename TElement> requires
		AutomaticConstraint
	using VariableBasicBinaryView = BasicBinaryView<TElement, k_false>;

	template <typename TElement> requires
		AutomaticConstraint
	using ConstantBasicBinaryView = BasicBinaryView<TElement, k_true>;

	#pragma endregion

}
