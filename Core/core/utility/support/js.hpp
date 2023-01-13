#pragma once

#include "core/utility/script/js/js.hpp"
#include "core/utility/script/js/utility.hpp"
#include "core/utility/container/list/list.hpp"
#include "core/utility/container/map/map.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/data/json/value.hpp"
#include "core/utility/data/xml/node.hpp"
#include "core/utility/image/pixel.hpp"
#include "core/utility/file_system/path.hpp"

namespace TwinStar::Core::JS {

	#pragma region basic

	template <>
	struct ValueAdapter<Value> {

		using This = Value;

		using That = Value;

		// ----------------

		static auto from (
			This & thix,
			That & that
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
			This & thix,
			That & that
		) -> Void {
			that = thix;
			return;
		}

	};

	// ----------------

	template <typename TValue> requires
		AutoConstraint
	struct ValueAdapter<Handler<TValue>> {

		using This = Value;

		using That = Handler<TValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			assert_condition(Detail::g_class_id<TValue> != Detail::k_invalid_class_id);
			auto js_value = quickjs::JS_NewObjectClass(thix._context(), static_cast<int>(Detail::g_class_id<TValue>));
			quickjs::JS_SetOpaque(js_value, new That{that});
			thix._rebind_value(js_value);
			return;
		}

		static auto from (
			This &  thix,
			That && that
		) -> Void {
			assert_condition(Detail::g_class_id<TValue> != Detail::k_invalid_class_id);
			auto js_value = quickjs::JS_NewObjectClass(thix._context(), static_cast<int>(Detail::g_class_id<TValue>));
			quickjs::JS_SetOpaque(js_value, new That{as_moveable(that)});
			thix._rebind_value(js_value);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(Detail::g_class_id<TValue> != Detail::k_invalid_class_id);
			that = *static_cast<That *>(quickjs::JS_GetOpaque(thix._value(), Detail::g_class_id<TValue>));
			return;
		}

	};

	// ----------------

	template <auto t_function, auto t_forward_object> requires
		AutoConstraint
	struct ValueAdapter<FunctionWrapper<t_function, t_forward_object>> {

		using This = Value;

		using That = FunctionWrapper<t_function, t_forward_object>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_native_function<t_function, t_forward_object>(that.name);
			return;
		}

	};

	template <auto t_function> requires
		AutoConstraint
	struct ValueAdapter<ConstructorWrapper<t_function>> {

		using This = Value;

		using That = ConstructorWrapper<t_function>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_native_constructor<t_function>(that.name, that.is_generic);
			return;
		}

	};

	#pragma endregion

	#pragma region base

	template <>
	struct ValueAdapter<Null> {

		using This = Value;

		using That = Null;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_null(that);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_null());
			that = thix.get_null();
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Boolean> {

		using This = Value;

		using That = Boolean;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_boolean(that);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_boolean());
			that = thix.get_boolean();
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
			thix.set_bigint(that);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_bigint());
			that = thix.get_bigint();
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
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_number());
			that = thix.get_number();
			return;
		}

	};

	template <>
	struct ValueAdapter<NumberVariant> {

		using This = Value;

		using That = NumberVariant;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			if (that.is_integer()) {
				thix.set_bigint(that.get_integer());
			} else {
				thix.set_number(that.get_floating());
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			if (thix.is_number()) {
				that.set_floating(thix.get_number());
			} else if (thix.is_bigint()) {
				that.set_integer(thix.get_bigint());
			} else {
				assert_failed(R"(/* thix type is number or bigint */)");
			}
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Size> {

		using This = Value;

		using That = Size;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbw<Integer>(that));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_bigint());
			that = cbw<That>(thix.get_bigint());
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<String> {

		using This = Value;

		using That = String;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string(that);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_string());
			that = thix.get_string();
			return;
		}

	};

	template <auto t_constant> requires
		AutoConstraint
	struct ValueAdapter<StringView<t_constant>> {

		using This = Value;

		using That = StringView<t_constant>;

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string(that);
			return;
		}

	};

	// ----------------

	template <typename TType> requires
		AutoConstraint
		&& (IsEnumerationWrapper<TType>)
		&& (IsDerivedFrom<TType, Enumeration<typename TType::Value>>)
		&& (!IsSame<TType, Enumeration<typename TType::Value>>)
	struct ValueAdapter<TType> {

		using This = Value;

		using That = TType;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto string = String{};
			string.from(that);
			thix.set_string(string);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_string());
			auto string = thix.get_string();
			string.to(that);
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
			This & thix,
			That & that
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
			if (!that.has()) {
				thix.set_null(k_null);
			} else {
				thix.from(that.get());
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			if (thix.is_null()) {
				that.reset();
			} else {
				that.set();
				thix.to(that.get());
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

		static auto from (
			This &       thix,
			That const & that,
			Size const & variant_index
		) -> Void {
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.from(that.template get_of_index<mbw<Size>(index)>());
				}
			);
			return;
		}

		static auto to (
			This &       thix,
			That &       that,
			Size const & variant_index
		) -> Void {
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.to(that.template set_of_index<mbw<Size>(index)>());
				}
			);
			return;
		}

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_object();
			auto variant_index = that.index();
			thix.set_object_property("type"_sv, thix.new_value(variant_index));
			thix.set_object_property("value"_sv, thix.new_value(that, variant_index));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			assert_condition(thix_object.size() == 2_sz);
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

		static auto from (
			This &               thix,
			That const &         that,
			TEnumeration const & variant_type
		) -> Void {
			auto variant_index = cbw<Size>(variant_type);
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.from(that.template get_of_index<mbw<Size>(index)>());
				}
			);
			return;
		}

		static auto to (
			This &               thix,
			That &               that,
			TEnumeration const & variant_type
		) -> Void {
			auto variant_index = cbw<Size>(variant_type);
			Generalization::match<AsValuePackageOfIndex<sizeof...(TValue)>>(
				variant_index.value,
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.to(that.template set_of_index<mbw<Size>(index)>());
				}
			);
			return;
		}

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_object();
			auto variant_type = that.type();
			thix.set_object_property("type"_sv, thix.new_value(variant_type));
			thix.set_object_property("value"_sv, thix.new_value(that, variant_type));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			assert_condition(thix_object.size() == 2_sz);
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

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			Generalization::each<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.set_object_property(mbw<Size>(index), thix.new_value(that.template get<mbw<Size>(index)>()));
				}
			);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
			assert_condition(thix_array.size() == mbw<Size>(sizeof...(TValue)));
			Generalization::each<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index> (ValuePackage<index>, auto) {
					thix_array.at(mbw<Size>(index)).to(that.template get<mbw<Size>(index)>());
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
			thix.set_object_of_array();
			for (auto & index : SizeRange{that.size()}) {
				thix.set_object_property(index, thix.new_value(that[index]));
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
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
			thix.set_object_of_object();
			for (auto & index : SizeRange{that.size()}) {
				auto thix_property_key = String{};
				thix_property_key.from(that.at(index).key);
				thix.set_object_property(thix_property_key, thix.new_value(that.at(index).value));
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			that.convert(
				thix_object,
				[] (auto & that_element, auto & thix_element) {
					thix_element.key.to(that_element.key);
					thix_element.value.to(that_element.value);
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

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix.set_object_property(mbw<Size>(index), thix.new_value(Field::value_of(that)));
				}
			);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
			assert_condition(thix_array.size() == mbw<Size>(FieldPackage::size));
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix_array.at(mbw<Size>(index)).to(Field::value_of(that));
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

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_object();
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix.set_object_property(make_string_view(Field::name.view()), thix.new_value(Field::value_of(that)));
				}
			);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			assert_condition(thix_object.size() == mbw<Size>(FieldPackage::size));
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					assert_condition(thix_object.at(mbw<Size>(index)).key == make_string_view(Field::name.view()));
					thix_object.at(mbw<Size>(index)).value.to(Field::value_of(that));
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

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(up_cast<Variant<TValue ...>>(that), that.index());
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
			Generalization::match<ValuePackage<TValue::Reflection::MemberVariable::size ...>>(
				thix_array.size().value,
				[&] <auto index> (ValuePackage<index>, auto) {
					auto & that_value = that.template set_of_index<mbw<Size>(index)>();
					Generalization::each<typename TypePackage<TValue ...>::template Element<index>::Reflection::MemberVariable>(
						[&] <auto field_index, typename Field> (ValuePackage<field_index>, TypePackage<Field>) {
							thix_array.at(mbw<Size>(field_index)).to(Field::value_of(that_value));
						}
					);
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

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(up_cast<Variant<TValue ...>>(that), that.index());
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			Generalization::match<ValuePackage<TValue::Reflection::MemberVariable::size ...>>(
				thix_object.size().value,
				[&] <auto index> (ValuePackage<index>, auto) {
					auto & that_value = that.template set_of_index<mbw<Size>(index)>();
					Generalization::each<typename TypePackage<TValue ...>::template Element<index>::Reflection::MemberVariable>(
						[&] <auto field_index, typename Field> (ValuePackage<field_index>, TypePackage<Field>) {
							assert_condition(thix_object.at(mbw<Size>(field_index)).key == make_string_view(Field::name.view()));
							thix_object.at(mbw<Size>(field_index)).value.to(Field::value_of(that_value));
						}
					);
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
			thix.set_bigint(cbw<Integer>(that));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_bigint());
			that = cbw<That>(thix.get_bigint());
			return;
		}

	};

	template <>
	struct ValueAdapter<ByteArray> {

		using This = Value;

		using That = ByteArray;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array_buffer(that.view());
			return;
		}

		static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix.set_object_of_array_buffer(that.view(), k_true);
			that.unbind();
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			that.assign(thix.get_object_of_array_buffer());
			return;
		}

	};

	template <>
	struct ValueAdapter<VByteListView> {

		using This = Value;

		using That = VByteListView;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array_buffer(that, k_false);
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			restruct(that, thix.get_object_of_array_buffer());
			return;
		}

	};

	#pragma endregion

	#pragma region miscellaneous

	// ----------------

	template <typename XValue> requires
		AutoConstraint
	struct ValueAdapter<Position1D<XValue>> {

		using This = Value;

		using That = Position1D<XValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.x));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.x);
			return;
		}

	};

	template <typename XValue, typename YValue> requires
		AutoConstraint
	struct ValueAdapter<Position2D<XValue, YValue>> {

		using This = Value;

		using That = Position2D<XValue, YValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.x));
			thix.set_object_property(2_ix, thix.new_value(that.y));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.x);
			thix.get_object_property(2_ix).to(that.y);
			return;
		}

	};

	template <typename XValue, typename YValue, typename ZValue> requires
		AutoConstraint
	struct ValueAdapter<Position3D<XValue, YValue, ZValue>> {

		using This = Value;

		using That = Position3D<XValue, YValue, ZValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.x));
			thix.set_object_property(2_ix, thix.new_value(that.y));
			thix.set_object_property(3_ix, thix.new_value(that.z));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.x);
			thix.get_object_property(2_ix).to(that.y);
			thix.get_object_property(3_ix).to(that.z);
			return;
		}

	};

	template <typename XValue> requires
		AutoConstraint
	struct ValueAdapter<Size1D<XValue>> {

		using This = Value;

		using That = Size1D<XValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.width));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.width);
			return;
		}

	};

	template <typename XValue, typename YValue> requires
		AutoConstraint
	struct ValueAdapter<Size2D<XValue, YValue>> {

		using This = Value;

		using That = Size2D<XValue, YValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.width));
			thix.set_object_property(2_ix, thix.new_value(that.height));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.width);
			thix.get_object_property(2_ix).to(that.height);
			return;
		}

	};

	template <typename XValue, typename YValue, typename ZValue> requires
		AutoConstraint
	struct ValueAdapter<Size3D<XValue, YValue, ZValue>> {

		using This = Value;

		using That = Size3D<XValue, YValue, ZValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.width));
			thix.set_object_property(2_ix, thix.new_value(that.height));
			thix.set_object_property(3_ix, thix.new_value(that.depth));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.width);
			thix.get_object_property(2_ix).to(that.height);
			thix.get_object_property(3_ix).to(that.depth);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Image::Pixel> {

		using This = Value;

		using That = Image::Pixel;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.red));
			thix.set_object_property(2_ix, thix.new_value(that.green));
			thix.set_object_property(3_ix, thix.new_value(that.blue));
			thix.set_object_property(4_ix, thix.new_value(that.alpha));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.red);
			thix.get_object_property(2_ix).to(that.green);
			thix.get_object_property(3_ix).to(that.blue);
			thix.get_object_property(4_ix).to(that.alpha);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<JSON::Value> {

		using This = Value;

		using That = JSON::Value;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			using namespace JSON;
			switch (that.type().value) {
				case ValueType::Constant::null().value : {
					thix.from(that.get_null());
					break;
				}
				case ValueType::Constant::boolean().value : {
					thix.from(that.get_boolean());
					break;
				}
				case ValueType::Constant::number().value : {
					thix.from(that.get_number());
					break;
				}
				case ValueType::Constant::string().value : {
					thix.from(that.get_string());
					break;
				}
				case ValueType::Constant::array().value : {
					thix.from(that.get_array());
					break;
				}
				case ValueType::Constant::object().value : {
					thix.from(that.get_object());
					break;
				}
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			using namespace JSON;
			if (thix.is_null()) {
				thix.to(that.set_null());
			} else if (thix.is_boolean()) {
				thix.to(that.set_boolean());
			} else if (thix.is_number() || thix.is_bigint()) {
				thix.to(that.set_number());
			} else if (thix.is_string()) {
				thix.to(that.set_string());
			} else if (thix.is_object_of_array()) {
				thix.to(that.set_array());
			} else if (thix.is_object_of_object()) {
				thix.to(that.set_object());
			} else {
				assert_failed(R"(/* thix type is valid */)");
			}
			return;
		}

	};

	template <>
	struct ValueAdapter<XML::Node> {

		using This = Value;

		using That = XML::Node;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			using namespace XML;
			thix.set_object_of_object();
			auto type = that.type();
			thix.set_object_property("type"_sv, thix.new_value(type));
			auto node_value_js = thix.new_value();
			switch (type.value) {
				case NodeType::Constant::element().value : {
					auto & node_value = that.get_element();
					node_value_js.set_object_of_object();
					node_value_js.set_object_property("name"_sv, thix.new_value(node_value.name));
					node_value_js.set_object_property("attribute"_sv, thix.new_value(node_value.attribute));
					node_value_js.set_object_property("child"_sv, thix.new_value(node_value.child));
					break;
				}
				case NodeType::Constant::text().value : {
					auto & node_value = that.get_text();
					node_value_js.set_object_of_object();
					node_value_js.set_object_property("value"_sv, thix.new_value(node_value.value));
					node_value_js.set_object_property("cdata"_sv, thix.new_value(node_value.cdata));
					break;
				}
				case NodeType::Constant::comment().value : {
					auto & node_value = that.get_comment();
					node_value_js.set_object_of_object();
					node_value_js.set_object_property("value"_sv, thix.new_value(node_value.value));
					break;
				}
			}
			thix.set_object_property("value"_sv, as_moveable(node_value_js));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			using namespace XML;
			assert_condition(thix.is_object_of_object());
			auto type = NodeType{};
			thix.get_object_property("type"_sv).to(type);
			auto node_value_js = thix.get_object_property("value"_sv);
			switch (type.value) {
				case NodeType::Constant::element().value : {
					auto & node_value = that.set_element();
					assert_condition(node_value_js.is_object_of_object());
					node_value_js.get_object_property("name"_sv).to(node_value.name);
					node_value_js.get_object_property("attribute"_sv).to(node_value.attribute);
					node_value_js.get_object_property("child"_sv).to(node_value.child);
					break;
				}
				case NodeType::Constant::text().value : {
					auto & node_value = that.set_text();
					assert_condition(node_value_js.is_object_of_object());
					node_value_js.get_object_property("value"_sv).to(node_value.value);
					node_value_js.get_object_property("cdata"_sv).to(node_value.cdata);
					break;
				}
				case NodeType::Constant::comment().value : {
					auto & node_value = that.set_comment();
					assert_condition(node_value_js.is_object_of_object());
					node_value_js.get_object_property("value"_sv).to(node_value.value);
					break;
				}
			}
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
			thix.set_string(that.to_string());
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_string());
			that.from_string(thix.get_string());
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<IntegerU8> {

		using This = Value;

		using That = IntegerU8;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbw<Integer>(that));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_bigint());
			that = cbw<That>(thix.get_bigint());
			return;
		}

	};

	template <>
	struct ValueAdapter<IntegerU32> {

		using This = Value;

		using That = IntegerU32;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbw<Integer>(that));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_bigint());
			that = cbw<That>(thix.get_bigint());
			return;
		}

	};

	#pragma endregion

}
