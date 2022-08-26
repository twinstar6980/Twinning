#pragma once

#include "core/utility/misc/character_series/container.hpp"
#include "core/utility/misc/character_series/type.hpp"

#include <string_view>

namespace TwinKleS::Core {

	#pragma region type

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameV<t_constant, ZBoolean>)
		&& (IsCharacterWrapper<TElement>)
	class BasicStringView :
		public BasicCharacterListView<TElement, t_constant> {

	private: //

		using BasicCharacterListView = BasicCharacterListView<TElement, t_constant>;

		using CView = BasicStringView<TElement, true>;

	public: //

		using typename BasicCharacterListView::Element;

		using BasicCharacterListView::constant;

		using typename BasicCharacterListView::QElement;

		using typename BasicCharacterListView::QIterator;

	public: //

		#pragma region structor

		constexpr ~BasicStringView (
		) = default;

		// ----------------

		constexpr BasicStringView (
		) = default;

		constexpr BasicStringView (
			BasicStringView const & that
		) = default;

		constexpr BasicStringView (
			BasicStringView && that
		) = default;

		// ----------------

		using BasicCharacterListView::BasicCharacterListView;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BasicStringView const & that
		) -> BasicStringView& = default;

		constexpr auto operator = (
			BasicStringView && that
		) -> BasicStringView& = default;

		// ----------------

		implicit operator CView & () requires
			(!constant) {
			return self_cast<CView>(thiz);
		}

		implicit operator CView const & () const requires
			(!constant) {
			return self_cast<CView>(thiz);
		}

		#pragma endregion

		#pragma region case convert

		constexpr auto as_lower_case (
		) const -> Void requires
			(!constant) &&
			(IsSame<Element, Character>) {
			Range::each(thiz, CharacterType::as_alpha_lower);
			return;
		}

		constexpr auto as_upper_case (
		) const -> Void requires
			(!constant) &&
			(IsSame<Element, Character>) {
			Range::each(thiz, CharacterType::as_alpha_upper);
			return;
		}

		#pragma endregion

		#pragma region comparison

		constexpr auto equal_icase (
			CView const & that
		) const -> Boolean requires
			(IsSame<Element, Character>) {
			return Range::equal(thiz, that, CharacterType::equal_icase);
		}

		constexpr auto compare_3way (
			CView const & that
		) const -> StorageOrdering requires
			(IsSame<Element, Character>) {
			auto common_size = min(thiz.size(), that.size());
			for (auto & index : SizeRange{common_size}) {
				auto & thiz_element = thiz[index];
				auto & that_element = that[index];
				if (thiz_element != that_element) {
					return thiz_element > that_element ? (StorageOrdering::greater) : (StorageOrdering::less);
				}
			}
			if (thiz.size() == that.size()) {
				return StorageOrdering::equal;
			} else {
				return thiz.size() > that.size() ? (StorageOrdering::greater) : (StorageOrdering::less);
			}
		}

		#pragma endregion

		#pragma region hash

		// TODO : size type ?
		constexpr auto hash (
		) const -> IntegerU64 {
			auto offset = 14695981039346656037_iu64;
			auto prime = 1099511628211_iu64;
			auto result = offset;
			for (auto & element : thiz) {
				result *= prime;
				result ^= cbw<IntegerU64>(element);
			}
			return result;
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub (
			Size const & begin,
			Size const & size
		) const -> BasicStringView {
			assert_condition(begin + size <= thiz.size());
			return BasicStringView{thiz.iterator(begin), size};
		}

		constexpr auto head (
			Size const & size
		) const -> BasicStringView {
			assert_condition(size <= thiz.size());
			return thiz.sub(thiz.begin_index(), size);
		}

		constexpr auto tail (
			Size const & size
		) const -> BasicStringView {
			assert_condition(size <= thiz.size());
			return thiz.sub(thiz.end_index() - size, size);
		}

		#pragma endregion

	public: //

		#pragma region operator

		friend constexpr auto operator <=> (
			BasicStringView const & thix,
			BasicStringView const & that
		) -> StorageOrdering requires
			(IsSame<Element, Character>) {
			return thix.compare_3way(that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Element> requires
		AutoConstraint
	using VBasicStringView = BasicStringView<Element, false>;

	template <typename Element> requires
		AutoConstraint
	using CBasicStringView = BasicStringView<Element, true>;

	#pragma endregion

	#pragma region utility

	template <typename Element> requires
		CategoryConstraint<IsInstance<Element>>
		&& (IsCharacterWrapper<AsPure<Element>>)
	inline auto null_terminated_string_size_of (
		Pointer<Element> const & string
	) -> Size {
		return mbw<Size>(std::char_traits<typename AsPure<Element>::Value>::length(cast_pointer<typename AsPure<Element>::Value>(string).value));
	}

	#pragma endregion

}
