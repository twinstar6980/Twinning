module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.integer_range;
import twinning.kernel.utility.math.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_constant, Boolean>)
		&& (IsCharacterBox<TElement>)
	class BasicStringView :
		public ArrayView<TElement, t_constant> {

	private:

		using ArrayView = ArrayView<TElement, t_constant>;

		using ConstantView = BasicStringView<TElement, k_true>;

	public:

		using typename ArrayView::Element;

		using ArrayView::constant;

		using typename ArrayView::QualifyElement;

		using typename ArrayView::QualifyIterator;

	public:

		#pragma region constructor

		constexpr ~BasicStringView(
		) = default;

		// ----------------

		constexpr BasicStringView(
		) :
			ArrayView{} {
			return;
		}

		constexpr BasicStringView(
			BasicStringView const & that
		) = default;

		constexpr BasicStringView(
			BasicStringView && that
		) = default;

		// ----------------

		using ArrayView::ArrayView;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BasicStringView const & that
		) -> BasicStringView & = default;

		constexpr auto operator =(
			BasicStringView && that
		) -> BasicStringView & = default;

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
		) const -> BasicStringView {
			assert_test(begin + count <= thiz.size());
			return BasicStringView{thiz.iterator(begin), count};
		}

		constexpr auto head(
			Size const & count
		) const -> BasicStringView {
			assert_test(count <= thiz.size());
			return thiz.sub(thiz.begin_index(), count);
		}

		constexpr auto tail(
			Size const & count
		) const -> BasicStringView {
			assert_test(count <= thiz.size());
			return thiz.sub(thiz.end_index() - count, count);
		}

		#pragma endregion

		#pragma region comparison

		constexpr auto compare_three_way(
			ConstantView const & that
		) const -> StrongOrdering {
			auto common_size = Math::minimum(thiz.size(), that.size());
			for (auto & index : SizeRange{common_size}) {
				auto & thiz_element = thiz[index];
				auto & that_element = that[index];
				if (thiz_element != that_element) {
					return thiz_element > that_element ? (StrongOrdering::greater) : (StrongOrdering::less);
				}
			}
			if (thiz.size() == that.size()) {
				return StrongOrdering::equal;
			}
			else {
				return thiz.size() > that.size() ? (StrongOrdering::greater) : (StrongOrdering::less);
			}
		}

		#pragma endregion

		#pragma region hash

		constexpr auto hash(
		) const -> IntegerU64 {
			auto offset = 14695981039346656037_iu64;
			auto prime = 1099511628211_iu64;
			auto result = offset;
			for (auto & element : thiz) {
				result ^= cast_box<IntegerU64>(element);
				result = unchecked_number_multiply(result, prime);
			}
			return result;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			BasicStringView const & thix,
			BasicStringView const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		// ----------------

		inline friend constexpr auto operator <=>(
			BasicStringView const & thix,
			BasicStringView const & that
		) -> StrongOrdering {
			return thix.compare_three_way(that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename TElement> requires
		AutomaticConstraint
	using VariableBasicStringView = BasicStringView<TElement, k_false>;

	template <typename TElement> requires
		AutomaticConstraint
	using ConstantBasicStringView = BasicStringView<TElement, k_true>;

	#pragma endregion

}
