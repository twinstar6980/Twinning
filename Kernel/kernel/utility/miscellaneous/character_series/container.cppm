module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.container.static_array.static_array;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_constant, Boolean>)
		&& (IsCharacterBox<TElement>)
	class BasicCharacterListView :
		public ListView<TElement, t_constant> {

	private:

		using ConstantView = BasicCharacterListView<TElement, k_true>;

		using ListView = ListView<TElement, t_constant>;

	public:

		using typename ListView::Element;

		using ListView::constant;

		using typename ListView::QualifyElement;

		using typename ListView::QualifyIterator;

	public:

		#pragma region constructor

		constexpr ~BasicCharacterListView (
		) = default;

		// ----------------

		constexpr BasicCharacterListView (
		) = default;

		constexpr BasicCharacterListView (
			BasicCharacterListView const & that
		) = default;

		constexpr BasicCharacterListView (
			BasicCharacterListView && that
		) = default;

		// ----------------

		using ListView::ListView;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BasicCharacterListView const & that
		) -> BasicCharacterListView & = default;

		constexpr auto operator = (
			BasicCharacterListView && that
		) -> BasicCharacterListView & = default;

		// ----------------

		implicit operator ConstantView & () requires
			(!constant.value) {
			return self_cast<ConstantView>(thiz);
		}

		implicit operator ConstantView const & () const requires
			(!constant.value) {
			return self_cast<ConstantView>(thiz);
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub (
			Size const & begin,
			Size const & size
		) const -> BasicCharacterListView {
			assert_test(begin + size <= thiz.size());
			return BasicCharacterListView{thiz.iterator(begin), size};
		}

		constexpr auto head (
			Size const & size
		) const -> BasicCharacterListView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.begin_index(), size);
		}

		constexpr auto tail (
			Size const & size
		) const -> BasicCharacterListView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.end_index() - size, size);
		}

		#pragma endregion

	};

	template <typename Element> requires
		AutomaticConstraint
	using VariableBasicCharacterListView = BasicCharacterListView<Element, k_false>;

	template <typename Element> requires
		AutomaticConstraint
	using ConstantBasicCharacterListView = BasicCharacterListView<Element, k_true>;

	// ----------------

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
	class BasicCharacterList :
		public List<TElement> {

	private:

		using List = List<TElement>;

	public:

		using typename List::Element;

		using typename List::Iterator;

		using View = BasicCharacterListView<Element, k_false>;

		using typename List::VariableElement;

		using typename List::VariableIterator;

		using VariableView = BasicCharacterListView<Element, k_false>;

		using typename List::ConstantElement;

		using typename List::ConstantIterator;

		using ConstantView = BasicCharacterListView<Element, k_true>;

	public:

		#pragma region constructor

		~BasicCharacterList (
		) = default;

		// ----------------

		BasicCharacterList (
		) = default;

		BasicCharacterList (
			BasicCharacterList const & that
		) = default;

		BasicCharacterList (
			BasicCharacterList && that
		) = default;

		// ----------------

		using List::List;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicCharacterList const & that
		) -> BasicCharacterList & = default;

		auto operator = (
			BasicCharacterList && that
		) -> BasicCharacterList & = default;

		// ----------------

		auto operator = (
			ConstantView const & that
		) -> BasicCharacterList & {
			thiz.assign(that);
			return thiz;
		}

		// ----------------

		implicit operator VariableView const & () {
			return thiz.as_view();
		}

		implicit operator ConstantView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		auto as_view (
		) -> VariableView const & {
			return self_cast<VariableView>(thiz);
		}

		auto as_view (
		) const -> ConstantView const & {
			return self_cast<ConstantView>(thiz);
		}

		// ----------------

		auto view (
		) -> VariableView {
			return thiz.as_view();
		}

		auto view (
		) const -> ConstantView {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region sub

		auto sub (
			Size const & begin,
			Size const & size
		) -> VariableView {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) -> VariableView {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) -> VariableView {
			return thiz.as_view().tail(size);
		}

		// ----------------

		auto sub (
			Size const & begin,
			Size const & size
		) const -> ConstantView {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) const -> ConstantView {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) const -> ConstantView {
			return thiz.as_view().tail(size);
		}

		#pragma endregion

	};

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsCharacterBox<TElement>)
	class BasicCharacterArray :
		public Array<TElement> {

	private:

		using Array = Array<TElement>;

	public:

		using typename Array::Element;

		using typename Array::Iterator;

		using View = BasicCharacterListView<Element, k_false>;

		using typename Array::VariableElement;

		using typename Array::VariableIterator;

		using VariableView = BasicCharacterListView<Element, k_false>;

		using typename Array::ConstantElement;

		using typename Array::ConstantIterator;

		using ConstantView = BasicCharacterListView<Element, k_true>;

	public:

		#pragma region constructor

		~BasicCharacterArray (
		) = default;

		// ----------------

		BasicCharacterArray (
		) = default;

		BasicCharacterArray (
			BasicCharacterArray const & that
		) = default;

		BasicCharacterArray (
			BasicCharacterArray && that
		) = default;

		// ----------------

		using Array::Array;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicCharacterArray const & that
		) -> BasicCharacterArray & = default;

		auto operator = (
			BasicCharacterArray && that
		) -> BasicCharacterArray & = default;

		// ----------------

		auto operator = (
			ConstantView const & that
		) -> BasicCharacterArray & {
			thiz.assign(that);
			return thiz;
		}

		// ----------------

		implicit operator VariableView const & () {
			return thiz.as_view();
		}

		implicit operator ConstantView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		auto as_view (
		) -> VariableView const & {
			return self_cast<VariableView>(thiz);
		}

		auto as_view (
		) const -> ConstantView const & {
			return self_cast<ConstantView>(thiz);
		}

		// ----------------

		auto view (
		) -> VariableView {
			return thiz.as_view();
		}

		auto view (
		) const -> ConstantView {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region sub

		auto sub (
			Size const & begin,
			Size const & size
		) -> VariableView {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) -> VariableView {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) -> VariableView {
			return thiz.as_view().tail(size);
		}

		// ----------------

		auto sub (
			Size const & begin,
			Size const & size
		) const -> ConstantView {
			return thiz.as_view().sub(begin, size);
		}

		auto head (
			Size const & size
		) const -> ConstantView {
			return thiz.as_view().head(size);
		}

		auto tail (
			Size const & size
		) const -> ConstantView {
			return thiz.as_view().tail(size);
		}

		#pragma endregion

	};

	template <typename TElement, auto t_size> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameOf<t_size, Size>)
		&& (IsCharacterBox<TElement>)
	class BasicStaticCharacterArray :
		public StaticArray<TElement, t_size> {

	private:

		using StaticArray = StaticArray<TElement, t_size>;

	public:

		using typename StaticArray::Element;

		using typename StaticArray::Iterator;

		using View = BasicCharacterListView<Element, k_false>;

		using typename StaticArray::VariableElement;

		using typename StaticArray::VariableIterator;

		using VariableView = BasicCharacterListView<Element, k_false>;

		using typename StaticArray::ConstantElement;

		using typename StaticArray::ConstantIterator;

		using ConstantView = BasicCharacterListView<Element, k_true>;

	public:

		#pragma region constructor

		constexpr ~BasicStaticCharacterArray (
		) = default;

		// ----------------

		constexpr BasicStaticCharacterArray (
		) = default;

		constexpr BasicStaticCharacterArray (
			BasicStaticCharacterArray const & that
		) = default;

		constexpr BasicStaticCharacterArray (
			BasicStaticCharacterArray && that
		) = default;

		// ----------------

		using StaticArray::StaticArray;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BasicStaticCharacterArray const & that
		) -> BasicStaticCharacterArray & = default;

		constexpr auto operator = (
			BasicStaticCharacterArray && that
		) -> BasicStaticCharacterArray & = default;

		// ----------------

		implicit operator VariableView const & () {
			return thiz.as_view();
		}

		implicit operator ConstantView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		constexpr auto view (
		) -> VariableView {
			return VariableView{thiz.begin(), thiz.size()};
		}

		constexpr auto view (
		) const -> ConstantView {
			return ConstantView{thiz.begin(), thiz.size()};
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <auto constant> requires
		AutomaticConstraint
	using CharacterListView = BasicCharacterListView<Character, constant>;

	using VariableCharacterListView = VariableBasicCharacterListView<Character>;

	using ConstantCharacterListView = ConstantBasicCharacterListView<Character>;

	using CharacterList = BasicCharacterList<Character>;

	using CharacterArray = BasicCharacterArray<Character>;

	template <auto size> requires
		AutomaticConstraint
	using StaticCharacterArray = BasicStaticCharacterArray<Character, size>;

	#pragma endregion

}
