module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.json;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.wrapper.wrapper;
import twinning.kernel.utility.miscellaneous.dimension;
import twinning.kernel.utility.miscellaneous.record;
import twinning.kernel.utility.data.json.value;
import twinning.kernel.utility.data.json.value_adapter;
import twinning.kernel.utility.container.variant.variant;
import twinning.kernel.utility.container.variant.enumerable_variant;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.tuple.tuple;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.map.map;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.storage.path;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.byte_series.stream;
import twinning.kernel.utility.support.string;

export namespace Twinning::Kernel::JSON {

	#pragma region basic

	template <>
	struct ValueAdapter<Value> {

		using This = Value;

		using That = Value;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix = that;
			return;
		}

		inline static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix = as_moveable(that);
			return;
		}

		inline static auto to (
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set<That>(that);
			return;
		}

		inline static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix.set<That>(as_moveable(that));
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix.get<That>();
			return;
		}

	};

	#pragma endregion

	#pragma region base

	template <>
	struct ValueAdapter<Integer> {

		using This = Value;

		using That = Integer;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_number(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix.get_number().get_integer();
			return;
		}

	};

	template <>
	struct ValueAdapter<Floater> {

		using This = Value;

		using That = Floater;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_number(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix.get_number().get_floater();
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Size> {

		using This = Value;

		using That = Size;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_number(cbox<Integer>(that));
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = cbox<Size>(thix.get_number().get_integer());
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Character> {

		using This = Value;

		using That = Character;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Unicode> {

		using This = Value;

		using That = Unicode;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	// ----------------

	template <typename TType> requires
		AutoConstraint
		&& (IsEnumerationBox<TType>)
		&& (IsDerivedFrom<TType, Enumeration<typename TType::Value>>)
		&& (!IsSame<TType, Enumeration<typename TType::Value>>)
	struct ValueAdapter<TType> {

		using This = Value;

		using That = TType;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(that.get());
			return;
		}

		inline static auto to (
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			if (!that.has()) {
				thix.set_null();
			}
			else {
				thix.from(that.get());
			}
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			if (thix.is_null()) {
				that.reset();
			}
			else {
				thix.to(that.set());
			}
			return;
		}

	};

	// ----------------

	template <typename ... TValue> requires
		AutoConstraint
	struct ValueAdapter<Variant<TValue ...>> {

		using This = Value;

		using That = Variant<TValue ...>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that,
			Size const & variant_index
		) -> Void {
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.from(that.template get_of_index<mbox<Size>(index)>());
				}
			);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that,
			Size const & variant_index
		) -> Void {
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.to(that.template set_of_index<mbox<Size>(index)>());
				}
			);
			return;
		}

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object();
			thix_object.allocate(2_sz);
			auto variant_index = that.index();
			thix_object("type"_sv).from(variant_index);
			thix_object("value"_sv).from(that, variant_index);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			assert_test(thix_object.size() == 2_sz);
			auto variant_index = Size{};
			thix_object["type"_sv].to(variant_index);
			thix_object["value"_sv].to(that, variant_index);
			return;
		}

	};

	template <typename TEnumeration, typename ... TValue> requires
		AutoConstraint
	struct ValueAdapter<EnumerableVariant<TEnumeration, TValue ...>> {

		using This = Value;

		using That = EnumerableVariant<TEnumeration, TValue ...>;

		// ----------------

		inline static auto from (
			This &               thix,
			That const &         that,
			TEnumeration const & variant_type
		) -> Void {
			auto variant_index = cbox<Size>(variant_type);
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.from(that.template get_of_index<mbox<Size>(index)>());
				}
			);
			return;
		}

		inline static auto to (
			This const &         thix,
			That &               that,
			TEnumeration const & variant_type
		) -> Void {
			auto variant_index = cbox<Size>(variant_type);
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.to(that.template set_of_index<mbox<Size>(index)>());
				}
			);
			return;
		}

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object();
			thix_object.allocate(2_sz);
			auto variant_type = that.type();
			thix_object("type"_sv).from(variant_type);
			thix_object("value"_sv).from(that, variant_type);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			assert_test(thix_object.size() == 2_sz);
			auto variant_type = TEnumeration{};
			thix_object["type"_sv].to(variant_type);
			thix_object["value"_sv].to(that, variant_type);
			return;
		}

	};

	// ----------------

	template <typename ... TValue> requires
		AutoConstraint
	struct ValueAdapter<Tuple<TValue ...>> {

		using This = Value;

		using That = Tuple<TValue ...>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate_full(mbox<Size>(sizeof...(TValue)));
			Generalization::each<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index> (ValuePackage<index>, auto) {
					thix_array.at(mbox<Size>(index)).from(that.template get<mbox<Size>(index)>());
				}
			);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == mbox<Size>(sizeof...(TValue)));
			Generalization::each<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index> (ValuePackage<index>, auto) {
					thix_array.at(mbox<Size>(index)).to(that.template set<mbox<Size>(index)>());
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.convert(
				that,
				[] (auto & thix_element, auto & that_element) -> auto {
					thix_element.from(that_element);
					return;
				}
			);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			that.convert(
				thix_array,
				[] (auto & that_element, auto & thix_element) -> auto {
					thix_element.to(that_element);
					return;
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object();
			thix_object.convert(
				that,
				[] (auto & thix_member, auto & that_member) -> auto {
					thix_member.key.from(that_member.key);
					thix_member.value.from(that_member.value);
					return;
				}
			);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			that.convert(
				thix_object,
				[] (auto & that_member, auto & thix_member) -> auto {
					thix_member.key.to(that_member.key);
					thix_member.value.to(that_member.value);
					return;
				}
			);
			return;
		}

	};

	#pragma endregion

	#pragma region record

	template <typename TType> requires
		AutoConstraint
		&& (IsDerivedFrom<TType, ListRecord>)
	struct ValueAdapter<TType> {

		using This = Value;

		using That = TType;

		using FieldPackage = typename TType::Reflection::MemberVariable;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate_full(mbox<Size>(FieldPackage::size));
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix_array.at(mbox<Size>(index)).from(Field::value_of(that));
				}
			);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == mbox<Size>(FieldPackage::size));
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix_array.at(mbox<Size>(index)).to(Field::value_of(that));
				}
			);
			return;
		}

	};

	template <typename TType> requires
		AutoConstraint
		&& (IsDerivedFrom<TType, MapRecord>)
	struct ValueAdapter<TType> {

		using This = Value;

		using That = TType;

		using FieldPackage = typename TType::Reflection::MemberVariable;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_object = thix.set_object();
			thix_object.allocate_full(mbox<Size>(FieldPackage::size));
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix_object.at(mbox<Size>(index)).key = make_string_view(Field::name.view());
					thix_object.at(mbox<Size>(index)).value.from(Field::value_of(that));
				}
			);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_object = thix.get_object();
			assert_test(thix_object.size() == mbox<Size>(FieldPackage::size));
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					assert_test(thix_object.at(mbox<Size>(index)).key == make_string_view(Field::name.view()));
					thix_object.at(mbox<Size>(index)).value.to(Field::value_of(that));
				}
			);
			return;
		}

	};

	// ----------------

	template <typename ... TValue> requires
		AutoConstraint
	struct ValueAdapter<ListRecordVariant<TValue ...>> {

		using This = Value;

		using That = ListRecordVariant<TValue ...>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(up_cast<Variant<TValue ...>>(that), that.index());
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			Generalization::match<ValuePackage<TValue::Reflection::MemberVariable::size ...>>(
				thix.get_array().size().value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.to(that.template set_of_index<mbox<Size>(index)>());
				}
			);
			return;
		}

	};

	template <typename ... TValue> requires
		AutoConstraint
	struct ValueAdapter<MapRecordVariant<TValue ...>> {

		using This = Value;

		using That = MapRecordVariant<TValue ...>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(up_cast<Variant<TValue ...>>(that), that.index());
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			Generalization::match<ValuePackage<TValue::Reflection::MemberVariable::size ...>>(
				thix.get_object().size().value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.to(that.template set_of_index<mbox<Size>(index)>());
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<ByteList> {

		using This = Value;

		using That = ByteList;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	#pragma endregion

	#pragma region miscellaneous

	template <typename XValue> requires
		AutoConstraint
	struct ValueAdapter<Position1<XValue>> {

		using This = Value;

		using That = Position1<XValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate(1_sz);
			thix_array(1_ix).from(that.x);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == 1_sz);
			thix_array[1_ix].to(that.x);
			return;
		}

	};

	template <typename XValue, typename YValue> requires
		AutoConstraint
	struct ValueAdapter<Position2<XValue, YValue>> {

		using This = Value;

		using That = Position2<XValue, YValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate(2_sz);
			thix_array(1_ix).from(that.x);
			thix_array(2_ix).from(that.y);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == 2_sz);
			thix_array[1_ix].to(that.x);
			thix_array[2_ix].to(that.y);
			return;
		}

	};

	template <typename XValue, typename YValue, typename ZValue> requires
		AutoConstraint
	struct ValueAdapter<Position3<XValue, YValue, ZValue>> {

		using This = Value;

		using That = Position3<XValue, YValue, ZValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate(3_sz);
			thix_array(1_ix).from(that.x);
			thix_array(2_ix).from(that.y);
			thix_array(3_ix).from(that.z);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == 3_sz);
			thix_array[1_ix].to(that.x);
			thix_array[2_ix].to(that.y);
			thix_array[3_ix].to(that.z);
			return;
		}

	};

	template <typename XValue> requires
		AutoConstraint
	struct ValueAdapter<Size1<XValue>> {

		using This = Value;

		using That = Size1<XValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate(1_sz);
			thix_array(1_ix).from(that.width);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == 1_sz);
			thix_array[1_ix].to(that.width);
			return;
		}

	};

	template <typename XValue, typename YValue> requires
		AutoConstraint
	struct ValueAdapter<Size2<XValue, YValue>> {

		using This = Value;

		using That = Size2<XValue, YValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate(2_sz);
			thix_array(1_ix).from(that.width);
			thix_array(2_ix).from(that.height);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == 2_sz);
			thix_array[1_ix].to(that.width);
			thix_array[2_ix].to(that.height);
			return;
		}

	};

	template <typename XValue, typename YValue, typename ZValue> requires
		AutoConstraint
	struct ValueAdapter<Size3<XValue, YValue, ZValue>> {

		using This = Value;

		using That = Size3<XValue, YValue, ZValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto & thix_array = thix.set_array();
			thix_array.allocate(3_sz);
			thix_array(1_ix).from(that.width);
			thix_array(2_ix).from(that.height);
			thix_array(3_ix).from(that.depth);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto & thix_array = thix.get_array();
			assert_test(thix_array.size() == 3_sz);
			thix_array[1_ix].to(that.width);
			thix_array[2_ix].to(that.height);
			thix_array[3_ix].to(that.depth);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Path> {

		using This = Value;

		using That = Path;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string().from(that);
			return;
		}

		inline static auto to (
			This const & thix,
			That &       that
		) -> Void {
			thix.get_string().to(that);
			return;
		}

	};

	#pragma endregion

}
