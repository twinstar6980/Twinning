#pragma once

#include "core/utility/data/json/value.hpp"
#include "core/utility/data/json/value_adapter_utility.hpp"
#include "core/utility/container/wrapper/wrapper.hpp"
#include "core/utility/string/parser.hpp"
#include "core/utility/misc/2d_type.hpp"

namespace TwinKleS::Core::JSON {

	#pragma region basic

	template <>
	struct ValueAdapter<Value> {

		using This = Value;

		using That = Value;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix = that;
			return;
		}

		static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix = as_moveable(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix;
			return;
		}

	};

	// ----------------

	template <typename TType> requires
		AutoConstraint
		&& (IsSame<TType, Null, Boolean, Number, String, Array, Object>)
	struct ValueAdapter<TType> {

		using This = Value;

		using That = TType;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set<That>(that);
			return;
		}

		static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix.set<That>(as_moveable(that));
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix.get<That>();
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Integer> {

		using This = Value;

		using That = Integer;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_number(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix.get_number().get_integer();
			return;
		}

	};

	template <>
	struct ValueAdapter<Floating> {

		using This = Value;

		using That = Floating;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_number(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix.get_number().get_floating();
			return;
		}

	};

	#pragma endregion

	#pragma region container

	template <typename TValue> requires
		AutoConstraint
	struct ValueAdapter<Wrapper<TValue>> {

		using This = Value;

		using That = Wrapper<TValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(that.get());
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.to(that.get());
			return;
		}

	};

	// ----------------

	template <typename TValue> requires
		AutoConstraint
	struct ValueAdapter<Optional<TValue>> {

		using This = Value;

		using That = Optional<TValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			if (that.has()) {
				thix.from(that.get());
			} else {
				thix.set_null();
			}
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			if (!thix.is_null()) {
				thix.to(that.set());
			} else {
				that.reset();
			}
			return;
		}

	};

	// ----------------

	template <typename ...TValue> requires
		AutoConstraint
	struct ValueAdapter<Variant<TValue...>> {

		using This = Value;

		using That = Variant<TValue...>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that,
			Size const & variant_index
		) -> Void {
			Generalization::execute<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index, auto variant_index> (ValuePackage<index, variant_index>) {
					thix.from(that.template get_of_index<variant_index>());
				}
			);
			return;
		}

		static auto to (
			This const & thix,
			That &       that,
			Size const & variant_index
		) -> Void {
			Generalization::execute<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index, auto variant_index> (ValuePackage<index, variant_index>) {
					thix.to(that.template set_of_index<variant_index>());
				}
			);
			return;
		}

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object(2_sz);
			auto   variant_index = that.index();
			thix_object("type"_sv).from(variant_index);
			thix_object("value"_sv).from(that, variant_index);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			auto   variant_index = Size{};
			thix_object["type"_sv].to(variant_index);
			thix_object["value"_sv].to(that, variant_index);
			return;
		}

	};

	template <typename TEnum, typename ...TValue> requires
		AutoConstraint
	struct ValueAdapter<EnumerableVariant<TEnum, TValue...>> {

		using This = Value;

		using That = EnumerableVariant<TEnum, TValue...>;

		// ----------------

		static auto from (
			This &        thix,
			That const &  that,
			TEnum const & variant_type
		) -> Void {
			auto variant_index = mbw<Size>(variant_type);
			Generalization::execute<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index, auto variant_index> (ValuePackage<index, variant_index>) {
					thix.from(that.template get_of_index<variant_index>());
				}
			);
			return;
		}

		static auto to (
			This const &  thix,
			That &        that,
			TEnum const & variant_type
		) -> Void {
			auto variant_index = mbw<Size>(variant_type);
			Generalization::execute<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index, auto variant_index> (ValuePackage<index, variant_index>) {
					thix.to(that.template set_of_index<variant_index>());
				}
			);
			return;
		}

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object(2_sz);
			auto   variant_type = that.type();
			thix_object("type"_sv).from(variant_type);
			thix_object("value"_sv).from(that, variant_type);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			auto   variant_type = TEnum{};
			thix_object["type"_sv].to(variant_type);
			thix_object["value"_sv].to(that, variant_type);
			return;
		}

	};

	// ----------------

	template <typename ...TValue> requires
		AutoConstraint
	struct ValueAdapter<Tuple<TValue...>> {

		using This = Value;

		using That = Tuple<TValue...>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array(mbw<Size>(sizeof...(TValue)));
			Generalization::iterate<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index, auto element_index> (ValuePackage<index, element_index>) {
					thix_array(mbw<Size>(element_index)).from(that.template get<element_index>());
				}
			);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			Generalization::iterate<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index, auto element_index> (ValuePackage<index, element_index>) {
					thix_array[mbw<Size>(element_index)].to(that.template set<element_index>());
				}
			);
			return;
		}

	};

	// ----------------

	template <typename TElement> requires
		AutoConstraint
	struct ValueAdapter<List<TElement>> {

		using This = Value;

		using That = List<TElement>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.convert(
				that,
				[] (auto & thix_element, auto & that_element) {
					thix_element.from(that_element);
				}
			);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			that.convert(
				thix_array,
				[] (auto & that_element, auto & thix_element) {
					thix_element.to(that_element);
				}
			);
			return;
		}

	};

	// ----------------

	template <typename TKey, typename TValue> requires
		AutoConstraint
	struct ValueAdapter<Map<TKey, TValue>> {

		using This = Value;

		using That = Map<TKey, TValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object();
			thix_object.convert(
				that,
				[] (auto & thix_member, auto & that_member) {
					thix_member.key.from(that_member.key);
					thix_member.value.from(that_member.value);
				}
			);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			that.convert(
				thix_object,
				[] (auto & that_member, auto & thix_member) {
					thix_member.key.to(that_member.key);
					thix_member.value.to(that_member.value);
				}
			);
			return;
		}

	};

	#pragma endregion

	#pragma region byte

	template <>
	struct ValueAdapter<Byte> {

		using This = Value;

		using That = Byte;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	#pragma endregion

	#pragma region byte list

	template <>
	struct ValueAdapter<ByteList> {

		using This = Value;

		using That = ByteList;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	#pragma endregion

	#pragma region misc

	template <typename TType> requires
		AutoConstraint
	struct ValueAdapter<AsEnum<TType>> {

		using This = Value;

		using That = AsEnum<TType>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	// ----------------

	template <typename TNumber> requires
		AutoConstraint
	struct ValueAdapter<Position2D<TNumber>> {

		using This = Value;

		using That = Position2D<TNumber>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array(2_sz);
			thix_array(1_ix).from(that.x);
			thix_array(2_ix).from(that.y);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			thix_array[1_ix].to(that.x);
			thix_array[2_ix].to(that.y);
			return;
		}

	};

	template <typename TNumber> requires
		AutoConstraint
	struct ValueAdapter<Size2D<TNumber>> {

		using This = Value;

		using That = Size2D<TNumber>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array(2_sz);
			thix_array(1_ix).from(that.width);
			thix_array(2_ix).from(that.height);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			thix_array[1_ix].to(that.width);
			thix_array[2_ix].to(that.height);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Path> {

		using This = Value;

		using That = Path;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	#pragma endregion

}
