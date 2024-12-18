module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.map.map;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.miscellaneous.key_value_pair;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.map.map_view;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.optional.optional_view;
import twinning.kernel.utility.container.optional.null_optional;
import twinning.kernel.utility.range.algorithm;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TKey, typename TValue> requires
		CategoryConstraint<IsPureInstance<TKey> && IsPureInstance<TValue>>
	class Map :
		protected List<KeyValuePair<TKey, TValue>> {

	private:

		using List = List<KeyValuePair<TKey, TValue>>;

	public:

		using Key = TKey;

		using Value = TValue;

		using typename List::Element;

		using typename List::Iterator;

		using View = MapView<Key, Value, k_false>;

		using VKey = Key;

		using VValue = Value;

		using typename List::VElement;

		using typename List::VIterator;

		using VView = MapView<Key, Value, k_false>;

		using CKey = TKey const;

		using CValue = TValue const;

		using typename List::CElement;

		using typename List::CIterator;

		using CView = MapView<Key, Value, k_true>;

	public:

		#pragma region structor

		~Map (
		) = default;

		// ----------------

		Map (
		) = default;

		Map (
			Map const & that
		) = default;

		Map (
			Map && that
		) = default;

		// ----------------

		using List::List;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Map const & that
		) -> Map & = default;

		auto operator = (
			Map && that
		) -> Map & = default;

		// ----------------

		auto operator = (
			CView const & that
		) -> Map & {
			thiz.as_list() = that.as_list();
			return thiz;
		}

		// ----------------

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto operator [] (
			KeyObject const & key
		) -> VValue & {
			return thiz.query(key).value;
		}

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto operator [] (
			KeyObject const & key
		) const -> CValue & {
			return thiz.query(key).value;
		}

		// ----------------

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto operator () (
			KeyObject const & key
		) -> VValue & {
			return thiz.query_force(key).value;
		}

		// ----------------

		implicit operator VView const & () {
			return thiz.as_view();
		}

		implicit operator CView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region as list

		auto as_list (
		) -> List & {
			return self_cast<List>(thiz);
		}

		auto as_list (
		) const -> List const & {
			return self_cast<List>(thiz);
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

		#pragma region size & index

		using List::size;

		using List::empty;

		// ----------------

		using List::begin_index;

		using List::end_index;

		// ----------------

		using List::first_index;

		using List::last_index;

		#pragma endregion

		#pragma region iterator

		using List::iterator;

		using List::begin;

		using List::end;

		#pragma endregion

		#pragma region element

		using List::at;

		using List::first;

		using List::last;

		#pragma endregion

		#pragma region element of key

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto find_key (
			KeyObject const & key
		) const -> Optional<Size> {
			return thiz.as_view().find_key(key);
		}

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto has_key (
			KeyObject const & key
		) const -> Boolean {
			return thiz.as_view().has_key(key);
		}

		// ----------------

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto query (
			KeyObject const & key
		) -> VElement & {
			return thiz.as_view().query(key);
		}

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto query_if (
			KeyObject const & key
		) -> VOptionalView<Element> {
			return thiz.as_view().query_if(key);
		}

		// ----------------

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto query (
			KeyObject const & key
		) const -> CElement & {
			return thiz.as_view().query(key);
		}

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto query_if (
			KeyObject const & key
		) const -> COptionalView<Element> {
			return thiz.as_view().query_if(key);
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

		#pragma region capacity

		using List::capacity;

		using List::reserve;

		using List::full;

		#pragma endregion

		#pragma region set size

		using List::set_size;

		using List::shrink_size;

		using List::expand_size;

		using List::expand_size_to_full;

		#pragma endregion

		#pragma region bind

		using List::unbind;

		using List::bind;

		#pragma endregion

		#pragma region allocate

		using List::reset;

		using List::allocate;

		using List::allocate_full;

		// ----------------

		using List::allocate_retain;

		using List::shrink;

		using List::expand;

		using List::shrink_to_fit;

		#pragma endregion

		#pragma region assign

		using List::assign;

		using List::convert;

		#pragma endregion

		#pragma region insert

		using List::insert;

		using List::insert_list;

		// ----------------

		using List::prepend;

		using List::prepend_list;

		// ----------------

		using List::append;

		using List::append_list;

		#pragma endregion

		#pragma region remove

		using List::remove;

		using List::remove_head;

		using List::remove_tail;

		using List::remove_at;

		#pragma endregion

		#pragma region element force

		using List::at_force;

		#pragma endregion

		#pragma region element force of key

		template <typename KeyObject> requires
			CategoryConstraint<IsPureInstance<KeyObject>>
		auto query_force (
			KeyObject const & key
		) -> VElement & {
			auto index = thiz.find_key(key);
			return index.has() ? (thiz.at(index.get())) : (thiz.append(key, Value{}));
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator == (
			Map const & thix,
			Map const & that
		) -> bool {
			return Range::equal(thix, that);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename Key, typename Value, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Key> && IsPureInstance<Value> && IsValid<Argument ...>>
		&& (IsConstructible<KeyValuePair<Key, Value>, Argument &&> && ...)
	inline auto make_map (
		Argument && ... argument
	) -> Map<Key, Value> {
		auto result = Map<Key, Value>{mbox<Size>(sizeof...(Argument))};
		result.expand_size_to_full();
		Generalization::each_with<>(
			[&] <auto index, typename CurrentArgument> (ValuePackage<index>, CurrentArgument && current_argument) {
				restruct(result.at(mbox<Size>(index)), as_forward<CurrentArgument>(current_argument));
			},
			as_forward<Argument>(argument) ...
		);
		return result;
	}

	#pragma endregion

}
