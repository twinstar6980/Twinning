module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.map.map_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.miscellaneous.key_value_pair;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.optional_view;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TKey, typename TValue, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue>>
		&& (IsSameV<t_constant, Boolean>)
	class MapView :
		protected ListView<KeyValuePair<TKey, TValue>, t_constant> {

	private:

		using CView = MapView<TKey, TValue, k_true>;

		using ListView = ListView<KeyValuePair<TKey, TValue>, t_constant>;

	public:

		using Key = TKey;

		using Value = TValue;

		inline static constexpr auto constant = Boolean{t_constant};

		using typename ListView::Element;

		using QKey = AsConstantIf<Key, constant.value>;

		using QValue = AsConstantIf<Value, constant.value>;

		using typename ListView::QElement;

		using typename ListView::QIterator;

	public:

		#pragma region structor

		constexpr ~MapView (
		) = default;

		// ----------------

		constexpr MapView (
		) = default;

		constexpr MapView (
			MapView const & that
		) = default;

		constexpr MapView (
			MapView && that
		) = default;

		// ----------------

		using ListView::ListView;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			MapView const & that
		) -> MapView & = default;

		constexpr auto operator = (
			MapView && that
		) -> MapView & = default;

		// ----------------

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		constexpr auto operator [] (
			KeyObject const & key
		) const -> QValue & {
			return thiz.query(key).value;
		}

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

		#pragma region as list

		auto as_list (
		) -> ListView & {
			return self_cast<ListView>(thiz);
		}

		auto as_list (
		) const -> ListView const & {
			return self_cast<ListView>(thiz);
		}

		#pragma endregion

		#pragma region set

		using ListView::reset;

		using ListView::set;

		#pragma endregion

		#pragma region size & index

		using ListView::size;

		using ListView::empty;

		// ----------------

		using ListView::begin_index;

		using ListView::end_index;

		// ----------------

		using ListView::first_index;

		using ListView::last_index;

		#pragma endregion

		#pragma region iterator

		using ListView::iterator;

		using ListView::begin;

		using ListView::end;

		#pragma endregion

		#pragma region element

		using ListView::at;

		using ListView::first;

		using ListView::last;

		#pragma endregion

		#pragma region element of key

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		constexpr auto find_key (
			KeyObject const & key
		) const -> Optional<Size> {
			return Range::find_index_if(
				thiz,
				[&] (auto & element) -> auto {
					return element.key == key;
				}
			);
		}

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		constexpr auto has_key (
			KeyObject const & key
		) const -> Boolean {
			return thiz.find_key(key).has();
		}

		// ----------------

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		constexpr auto query (
			KeyObject const & key
		) const -> QElement & {
			auto index = thiz.find_key(key);
			assert_test(index.has());
			return thiz.at(index.get());
		}

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		constexpr auto query_if (
			KeyObject const & key
		) const -> OptionalView<Element, constant> {
			auto index = thiz.find_key(key);
			return !index.has() ? (k_null_optional) : (OptionalView<Element, constant>{thiz.at(index.get())});
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub (
			Size const & begin,
			Size const & size
		) const -> MapView {
			assert_test(begin + size <= thiz.size());
			return MapView{thiz.iterator(begin), size};
		}

		constexpr auto head (
			Size const & size
		) const -> MapView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.begin_index(), size);
		}

		constexpr auto tail (
			Size const & size
		) const -> MapView {
			assert_test(size <= thiz.size());
			return thiz.sub(thiz.end_index() - size, size);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			MapView const & thix,
			MapView const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename Key, typename Value> requires
		AutoConstraint
	using VMapView = MapView<Key, Value, k_false>;

	template <typename Key, typename Value> requires
		AutoConstraint
	using CMapView = MapView<Key, Value, k_true>;

	#pragma endregion

}
