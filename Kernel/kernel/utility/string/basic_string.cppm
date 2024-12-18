module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.basic_string_adapter;
import twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
	class BasicString :
		public BasicCharacterList<TElement> {

	private:

		using BasicCharacterList = BasicCharacterList<TElement>;

	public:

		using typename BasicCharacterList::Element;

		using typename BasicCharacterList::Iterator;

		using View = BasicStringView<TElement, k_false>;

		using typename BasicCharacterList::VElement;

		using typename BasicCharacterList::VIterator;

		using VView = BasicStringView<TElement, k_false>;

		using typename BasicCharacterList::CElement;

		using typename BasicCharacterList::CIterator;

		using CView = BasicStringView<TElement, k_true>;

	public:

		#pragma region structor

		~BasicString (
		) = default;

		// ----------------

		BasicString (
		) = default;

		BasicString (
			BasicString const & that
		) = default;

		BasicString (
			BasicString && that
		) = default;

		// ----------------

		using BasicCharacterList::BasicCharacterList;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicString const & that
		) -> BasicString & = default;

		auto operator = (
			BasicString && that
		) -> BasicString & = default;

		// ----------------

		auto operator = (
			CView const & that
		) -> BasicString & {
			thiz.assign(that);
			return thiz;
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
		) -> View const & {
			return self_cast<View>(thiz);
		}

		auto as_view (
		) const -> CView const & {
			return self_cast<CView>(thiz);
		}

		// ----------------

		auto view (
		) -> View {
			return thiz.as_view();
		}

		auto view (
		) const -> CView {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region case convert

		auto as_lower_case (
		) -> Void requires
			(IsSame<Element, Character>) {
			return thiz.as_view().as_lower_case();
		}

		auto as_upper_case (
		) -> Void requires
			(IsSame<Element, Character>) {
			return thiz.as_view().as_upper_case();
		}

		#pragma endregion

		#pragma region comparison

		auto equal_icase (
			CView const & that
		) const -> Boolean requires
			(IsSame<Element, Character>) {
			return thiz.as_view().equal_icase(that);
		}

		auto compare_3way (
			CView const & that
		) const -> StrongOrdering requires
			(IsSame<Element, Character>) {
			return thiz.as_view().compare_3way(that);
		}

		#pragma endregion

		#pragma region hash

		auto hash (
		) const -> IntegerU64 {
			return thiz.as_view().hash();
		}

		#pragma endregion

		#pragma region sub

		auto sub (
			Size const & begin,
			Size const & size
		) -> View {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) -> View {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) -> View {
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

		#pragma region from & to by adapter

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto from (
			That &&       that,
			Option && ... option
		) -> Void {
			BasicStringAdapter<Element, AsPure<That>>::from(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto to (
			That &&       that,
			Option && ... option
		) const -> Void {
			BasicStringAdapter<Element, AsPure<That>>::to(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		// ----------------

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
		auto to_of (
			Option && ... option
		) const -> That {
			auto that = That{};
			thiz.to(that, as_forward<Option>(option) ...);
			return that;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator + (
			CView const & thix,
			CView const & that
		) -> BasicString {
			auto result = BasicString{thix.size() + that.size()};
			result.append_list(thix);
			result.append_list(that);
			return result;
		}

		inline friend auto operator += (
			BasicString & thix,
			CView const & that
		) -> BasicString & {
			thix.append_list(that);
			return thix;
		}

		// ----------------

		inline friend auto operator <=> (
			BasicString const & thix,
			BasicString const & that
		) -> StrongOrdering requires
			(IsSame<Element, Character>) {
			return thix.compare_3way(that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Source> requires
		CategoryConstraint<IsPureInstance<Source>>
		&& (IsConvertible<Source, BasicStringView<typename Source::Element, k_true> const &>)
	inline auto make_null_terminated_string (
		Source const & source
	) -> BasicString<typename Source::Element> {
		auto result = BasicString<typename Source::Element>{source.size() + 1_sz};
		result.expand_size_to_full();
		Range::assign_from(result.head(source.size()), source);
		result.last() = mbox<typename Source::Element>('\0');
		result.shrink_size(1_sz);
		return result;
	}

	// ----------------

	template <typename Result, typename Source, typename Separator> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Source> && IsPureInstance<Separator>>
		&& (IsTemplateInstanceOfT<Result, List>)
		&& (IsConvertible<Source, CListView<typename Source::Element> const &>)
		&& (IsRange<Separator>)
	inline auto split_string (
		Source const &    source,
		Separator const & separator
	) -> Result {
		auto result = Result{};
		if (!source.empty()) {
			result.allocate_full(Range::count_many(source, separator) + 1_sz);
			auto sub_count = k_none_size;
			auto sub_begin = k_begin_index;
			auto sub_end = k_begin_index;
			auto get_next_sub_range = [&] (
			) -> Void {
				result[sub_count] = source.sub(sub_begin, sub_end - sub_begin);
				sub_begin = sub_end + k_next_index;
				++sub_count;
				return;
			};
			for (auto & element : source) {
				if (Range::has(separator, element)) {
					get_next_sub_range();
				}
				++sub_end;
			}
			get_next_sub_range();
		}
		return result;
	}

	// ----------------

	template <typename Source> requires
		CategoryConstraint<IsPureInstance<Source>>
		&& (IsConvertible<Source, CListView<typename Source::Element> const &>)
	inline auto compute_catenate_string_size (
		Source const & source
	) -> Size {
		auto size = k_none_size;
		if (!source.empty()) {
			for (auto & element : source) {
				size += element.size();
			}
			size += source.size() - 1_sz;
		}
		return size;
	}

	template <typename Result, typename Source, typename Separator> requires
		CategoryConstraint<IsPureInstance<Result> && IsPureInstance<Source> && IsPureInstance<Separator>>
		&& (IsTemplateInstanceOfT<Result, List>)
		&& (IsConvertible<Source, CListView<typename Source::Element> const &>)
	inline auto catenate_string (
		Source const &    source,
		Separator const & separator
	) -> Result {
		auto result = Result{compute_catenate_string_size(source)};
		if (!source.empty()) {
			for (auto & element : source.head(source.size() - 1_sz)) {
				result.append_list(element);
				result.append(separator);
			}
			result.append_list(source.last());
		}
		return result;
	}

	#pragma endregion

}
