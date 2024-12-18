module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.byte_series.container;
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
		&& (IsSameV<t_constant, Boolean>)
		&& (IsByteBox<TElement>)
	class BasicByteListView :
		public ListView<TElement, t_constant> {

	private:

		using CView = BasicByteListView<TElement, k_true>;

		using ListView = ListView<TElement, t_constant>;

	public:

		using typename ListView::Element;

		using ListView::constant;

		using typename ListView::QElement;

		using typename ListView::QIterator;

	public:

		#pragma region structor

		constexpr ~BasicByteListView (
		) = default;

		// ----------------

		constexpr BasicByteListView (
		) = default;

		constexpr BasicByteListView (
			BasicByteListView const & that
		) = default;

		constexpr BasicByteListView (
			BasicByteListView && that
		) = default;

		// ----------------

		using ListView::ListView;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BasicByteListView const & that
		) -> BasicByteListView & = default;

		constexpr auto operator = (
			BasicByteListView && that
		) -> BasicByteListView & = default;

		// ----------------

		implicit operator CView & () requires
			(!constant.value) {
			return self_cast<CView>(thiz);
		}

		implicit operator CView const & () const requires
			(!constant.value) {
			return self_cast<CView>(thiz);
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub (
			Size const & begin,
			Size const & size
		) const -> BasicByteListView {
			assert_test(begin + size <= thiz.size());
			return BasicByteListView{thiz.iterator(begin), size};
		}

		constexpr auto head (
			Size const & size
		) const -> BasicByteListView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.begin_index(), size);
		}

		constexpr auto tail (
			Size const & size
		) const -> BasicByteListView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.end_index() - size, size);
		}

		#pragma endregion

	};

	template <typename Element> requires
		AutoConstraint
	using VBasicByteListView = BasicByteListView<Element, k_false>;

	template <typename Element> requires
		AutoConstraint
	using CBasicByteListView = BasicByteListView<Element, k_true>;

	// ----------------

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsByteBox<TElement>)
	class BasicByteList :
		public List<TElement> {

	private:

		using List = List<TElement>;

	public:

		using typename List::Element;

		using typename List::Iterator;

		using View = BasicByteListView<Element, k_false>;

		using typename List::VElement;

		using typename List::VIterator;

		using VView = BasicByteListView<Element, k_false>;

		using typename List::CElement;

		using typename List::CIterator;

		using CView = BasicByteListView<Element, k_true>;

	public:

		#pragma region structor

		~BasicByteList (
		) = default;

		// ----------------

		BasicByteList (
		) = default;

		BasicByteList (
			BasicByteList const & that
		) = default;

		BasicByteList (
			BasicByteList && that
		) = default;

		// ----------------

		using List::List;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicByteList const & that
		) -> BasicByteList & = default;

		auto operator = (
			BasicByteList && that
		) -> BasicByteList & = default;

		// ----------------

		auto operator = (
			CView const & that
		) -> BasicByteList & {
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

	};

	template <typename TElement> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsByteBox<TElement>)
	class BasicByteArray :
		public Array<TElement> {

	private:

		using Array = Array<TElement>;

	public:

		using typename Array::Element;

		using typename Array::Iterator;

		using View = BasicByteListView<Element, k_false>;

		using typename Array::VElement;

		using typename Array::VIterator;

		using VView = BasicByteListView<Element, k_false>;

		using typename Array::CElement;

		using typename Array::CIterator;

		using CView = BasicByteListView<Element, k_true>;

	public:

		#pragma region structor

		~BasicByteArray (
		) = default;

		// ----------------

		BasicByteArray (
		) = default;

		BasicByteArray (
			BasicByteArray const & that
		) = default;

		BasicByteArray (
			BasicByteArray && that
		) = default;

		// ----------------

		using Array::Array;

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicByteArray const & that
		) -> BasicByteArray & = default;

		auto operator = (
			BasicByteArray && that
		) -> BasicByteArray & = default;

		// ----------------

		auto operator = (
			CView const & that
		) -> BasicByteArray & {
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

	};

	template <typename TElement, auto t_size> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameV<t_size, Size>)
		&& (IsByteBox<TElement>)
	class BasicStaticByteArray :
		public StaticArray<TElement, t_size> {

	private:

		using StaticArray = StaticArray<TElement, t_size>;

	public:

		using typename StaticArray::Element;

		using typename StaticArray::Iterator;

		using View = BasicByteListView<Element, k_false>;

		using typename StaticArray::VElement;

		using typename StaticArray::VIterator;

		using VView = BasicByteListView<Element, k_false>;

		using typename StaticArray::CElement;

		using typename StaticArray::CIterator;

		using CView = BasicByteListView<Element, k_true>;

	public:

		#pragma region structor

		constexpr ~BasicStaticByteArray (
		) = default;

		// ----------------

		constexpr BasicStaticByteArray (
		) = default;

		constexpr BasicStaticByteArray (
			BasicStaticByteArray const & that
		) = default;

		constexpr BasicStaticByteArray (
			BasicStaticByteArray && that
		) = default;

		// ----------------

		using StaticArray::StaticArray;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			BasicStaticByteArray const & that
		) -> BasicStaticByteArray & = default;

		constexpr auto operator = (
			BasicStaticByteArray && that
		) -> BasicStaticByteArray & = default;

		// ----------------

		implicit operator VView const & () {
			return thiz.as_view();
		}

		implicit operator CView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		constexpr auto view (
		) -> VView {
			return VView{thiz.begin(), thiz.size()};
		}

		constexpr auto view (
		) const -> CView {
			return CView{thiz.begin(), thiz.size()};
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <auto constant> requires
		AutoConstraint
	using ByteListView = BasicByteListView<Byte, constant>;

	using VByteListView = VBasicByteListView<Byte>;

	using CByteListView = CBasicByteListView<Byte>;

	using ByteList = BasicByteList<Byte>;

	using ByteArray = BasicByteArray<Byte>;

	template <auto size> requires
		AutoConstraint
	using StaticByteArray = BasicStaticByteArray<Byte, size>;

	#pragma endregion

}
