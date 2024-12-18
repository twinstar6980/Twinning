module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.java_script;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.container.wrapper.wrapper;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.variant.variant;
import twinning.kernel.utility.container.variant.enumerable_variant;
import twinning.kernel.utility.container.tuple.tuple;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.map.map;
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.data.json.value;
import twinning.kernel.utility.data.xml.node;
import twinning.kernel.utility.image.pixel;
import twinning.kernel.utility.script.java_script.value;
import twinning.kernel.utility.script.java_script.value_adapter;
import twinning.kernel.utility.script.java_script.utility;
import twinning.kernel.utility.null;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.number_variant;
import twinning.kernel.utility.miscellaneous.dimension;
import twinning.kernel.utility.miscellaneous.record;
import twinning.kernel.utility.storage.path;
import twinning.kernel.utility.image.regular;
import twinning.kernel.third.quickjs_ng;

export namespace Twinning::Kernel::JavaScript {

	#pragma region basic

	template <>
	struct ValueAdapter<Value> {

		using This = Value;

		using That = Value;

		// ----------------

		inline static auto from (
			This & thix,
			That & that
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
	struct ValueAdapter<NativeValueHandler<TValue>> {

		using This = Value;

		using That = NativeValueHandler<TValue>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			assert_test(Detail::g_native_class_id<TValue> != Detail::k_invalid_native_class_id);
			auto thix_value = Third::quickjs_ng::$JS_NewObjectClass(thix._context(), static_cast<int>(Detail::g_native_class_id<TValue>.value));
			auto thix_opaque = new That{that};
			Third::quickjs_ng::$JS_SetOpaque(thix_value, thix_opaque);
			thix._rebind_value(thix_value);
			return;
		}

		inline static auto from (
			This &  thix,
			That && that
		) -> Void {
			assert_test(Detail::g_native_class_id<TValue> != Detail::k_invalid_native_class_id);
			auto thix_value = Third::quickjs_ng::$JS_NewObjectClass(thix._context(), static_cast<int>(Detail::g_native_class_id<TValue>.value));
			auto thix_opaque = new That{as_moveable(that)};
			Third::quickjs_ng::$JS_SetOpaque(thix_value, thix_opaque);
			thix._rebind_value(thix_value);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(Detail::g_native_class_id<TValue> != Detail::k_invalid_native_class_id);
			auto thix_value = thix._value();
			auto thix_opaque = Third::quickjs_ng::$JS_GetOpaque(thix_value, static_cast<Third::quickjs_ng::$JSClassID>(Detail::g_native_class_id<TValue>.value));
			assert_test(thix_opaque != nullptr);
			that = *static_cast<That *>(thix_opaque);
			return;
		}

	};

	template <auto t_function, auto t_type> requires
		AutoConstraint
	struct ValueAdapter<NativeFunctionWrapper<t_function, t_type>> {

		using This = Value;

		using That = NativeFunctionWrapper<t_function, t_type>;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			if constexpr (t_type == NativeFunctionWrapperType::Constant::function()) {
				thix.set_object_of_native_function<&proxy_native_function_wrapper<t_function, k_false>>(that.name, k_false);
			}
			if constexpr (t_type == NativeFunctionWrapperType::Constant::method()) {
				thix.set_object_of_native_function<&proxy_native_function_wrapper<t_function, k_true>>(that.name, k_false);
			}
			if constexpr (t_type == NativeFunctionWrapperType::Constant::constructor()) {
				thix.set_object_of_native_function<&proxy_native_function_wrapper<t_function, k_false>>(that.name, k_true);
			}
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_null(that);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_null());
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_boolean(that);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_boolean());
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(that);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_bigint());
			that = thix.get_bigint();
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
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_number());
			that = thix.get_number();
			return;
		}

	};

	template <>
	struct ValueAdapter<NumberVariant> {

		using This = Value;

		using That = NumberVariant;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			if (that.is_integer()) {
				thix.set_bigint(that.get_integer());
			}
			else {
				thix.set_number(that.get_floater());
			}
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			if (thix.is_bigint()) {
				that.set_integer(thix.get_bigint());
			}
			else if (thix.is_number()) {
				that.set_floater(thix.get_number());
			}
			else {
				assert_fail(R"(/* thix type is bigint or number */)");
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbox<Integer>(that));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_bigint());
			that = cbox<That>(thix.get_bigint());
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<String> {

		using This = Value;

		using That = String;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_string(that);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_string());
			that = thix.get_string();
			return;
		}

	};

	template <auto t_constant> requires
		AutoConstraint
	struct ValueAdapter<StringView<t_constant>> {

		using This = Value;

		using That = StringView<t_constant>;

		// ----------------

		inline static auto from (
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
			auto string = String{};
			string.from(that);
			thix.set_string(string);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_string());
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(that.get());
			return;
		}

		inline static auto to (
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			if (!that.has()) {
				thix.set_null(k_null);
			}
			else {
				thix.from(that.get());
			}
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			if (thix.is_null()) {
				that.reset();
			}
			else {
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
			This &       thix,
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
			thix.set_object_of_object();
			auto variant_index = that.index();
			thix.set_object_property("type"_sv, thix.new_value(variant_index));
			thix.set_object_property("value"_sv, thix.new_value(that, variant_index));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
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
			This &               thix,
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
			thix.set_object_of_object();
			auto variant_type = that.type();
			thix.set_object_property("type"_sv, thix.new_value(variant_type));
			thix.set_object_property("value"_sv, thix.new_value(that, variant_type));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
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
			thix.set_object_of_array();
			Generalization::each<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index> (ValuePackage<index>, auto) {
					thix.set_object_property(mbox<Size>(index), thix.new_value(that.template get<mbox<Size>(index)>()));
				}
			);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
			assert_test(thix_array.size() == mbox<Size>(sizeof...(TValue)));
			Generalization::each<AsValuePackageOfIndex<sizeof...(TValue)>>(
				[&] <auto index> (ValuePackage<index>, auto) {
					thix_array.at(mbox<Size>(index)).to(that.template get<mbox<Size>(index)>());
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
			thix.set_object_of_array();
			for (auto & index : SizeRange{that.size()}) {
				thix.set_object_property(index, thix.new_value(that[index]));
			}
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
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
			thix.set_object_of_object();
			for (auto & index : SizeRange{that.size()}) {
				auto thix_property_key = String{};
				thix_property_key.from(that.at(index).key);
				thix.set_object_property(thix_property_key, thix.new_value(that.at(index).value));
			}
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			that.convert(
				thix_object,
				[] (auto & that_element, auto & thix_element) -> auto {
					thix_element.key.to(that_element.key);
					thix_element.value.to(that_element.value);
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
			thix.set_object_of_array();
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix.set_object_property(mbox<Size>(index), thix.new_value(Field::value_of(that)));
				}
			);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
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
			thix.set_object_of_object();
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix.set_object_property(make_string_view(Field::name.view()), thix.new_value(Field::value_of(that)));
				}
			);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
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
			This & thix,
			That & that
		) -> Void {
			auto thix_array = thix.collect_object_own_property_of_array();
			Generalization::match<ValuePackage<TValue::Reflection::MemberVariable::size ...>>(
				thix_array.size().value,
				[&] <auto index> (ValuePackage<index>, auto) {
					auto & that_value = that.template set_of_index<mbox<Size>(index)>();
					Generalization::each<typename TypePackage<TValue ...>::template Element<index>::Reflection::MemberVariable>(
						[&] <auto field_index, typename Field> (ValuePackage<field_index>, TypePackage<Field>) {
							thix_array.at(mbox<Size>(field_index)).to(Field::value_of(that_value));
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.from(up_cast<Variant<TValue ...>>(that), that.index());
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			auto thix_object = thix.collect_object_own_property_of_object();
			Generalization::match<ValuePackage<TValue::Reflection::MemberVariable::size ...>>(
				thix_object.size().value,
				[&] <auto index> (ValuePackage<index>, auto) {
					auto & that_value = that.template set_of_index<mbox<Size>(index)>();
					Generalization::each<typename TypePackage<TValue ...>::template Element<index>::Reflection::MemberVariable>(
						[&] <auto field_index, typename Field> (ValuePackage<field_index>, TypePackage<Field>) {
							assert_test(thix_object.at(mbox<Size>(field_index)).key == make_string_view(Field::name.view()));
							thix_object.at(mbox<Size>(field_index)).value.to(Field::value_of(that_value));
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbox<Integer>(that));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_bigint());
			that = cbox<That>(thix.get_bigint());
			return;
		}

	};

	template <>
	struct ValueAdapter<ByteArray> {

		using This = Value;

		using That = ByteArray;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array_buffer(that.view());
			return;
		}

		inline static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix.set_object_of_array_buffer(that.view(), k_true);
			that.unbind();
			return;
		}

		inline static auto to (
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_object_of_array_buffer(that, k_false);
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			restruct(that, thix.get_object_of_array_buffer());
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
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.x));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.x);
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
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.x));
			thix.set_object_property(2_ix, thix.new_value(that.y));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.x);
			thix.get_object_property(2_ix).to(that.y);
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
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.x));
			thix.set_object_property(2_ix, thix.new_value(that.y));
			thix.set_object_property(3_ix, thix.new_value(that.z));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.x);
			thix.get_object_property(2_ix).to(that.y);
			thix.get_object_property(3_ix).to(that.z);
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
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.width));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.width);
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
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.width));
			thix.set_object_property(2_ix, thix.new_value(that.height));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.width);
			thix.get_object_property(2_ix).to(that.height);
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
			thix.set_object_of_array();
			thix.set_object_property(1_ix, thix.new_value(that.width));
			thix.set_object_property(2_ix, thix.new_value(that.height));
			thix.set_object_property(3_ix, thix.new_value(that.depth));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.width);
			thix.get_object_property(2_ix).to(that.height);
			thix.get_object_property(3_ix).to(that.depth);
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<JSON::Value> {

		using This = Value;

		using That = JSON::Value;

		// ----------------

		inline static auto from (
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

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			using namespace JSON;
			if (thix.is_null()) {
				thix.to(that.set_null());
			}
			else if (thix.is_boolean()) {
				thix.to(that.set_boolean());
			}
			else if (thix.is_number() || thix.is_bigint()) {
				thix.to(that.set_number());
			}
			else if (thix.is_string()) {
				thix.to(that.set_string());
			}
			else if (thix.is_object_of_array()) {
				thix.to(that.set_array());
			}
			else if (thix.is_object_of_object()) {
				thix.to(that.set_object());
			}
			else {
				assert_fail(R"(/* thix type is valid */)");
			}
			return;
		}

	};

	template <>
	struct ValueAdapter<XML::Node> {

		using This = Value;

		using That = XML::Node;

		// ----------------

		inline static auto from (
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

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			using namespace XML;
			assert_test(thix.is_object_of_object());
			auto type = NodeType{};
			thix.get_object_property("type"_sv).to(type);
			auto node_value_js = thix.get_object_property("value"_sv);
			switch (type.value) {
				case NodeType::Constant::element().value : {
					auto & node_value = that.set_element();
					assert_test(node_value_js.is_object_of_object());
					node_value_js.get_object_property("name"_sv).to(node_value.name);
					node_value_js.get_object_property("attribute"_sv).to(node_value.attribute);
					node_value_js.get_object_property("child"_sv).to(node_value.child);
					break;
				}
				case NodeType::Constant::text().value : {
					auto & node_value = that.set_text();
					assert_test(node_value_js.is_object_of_object());
					node_value_js.get_object_property("value"_sv).to(node_value.value);
					node_value_js.get_object_property("cdata"_sv).to(node_value.cdata);
					break;
				}
				case NodeType::Constant::comment().value : {
					auto & node_value = that.set_comment();
					assert_test(node_value_js.is_object_of_object());
					node_value_js.get_object_property("value"_sv).to(node_value.value);
					break;
				}
			}
			return;
		}

	};

	// ----------------

	template <>
	struct ValueAdapter<Image::Color> {

		using This = Value;

		using That = Image::Color;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbox<Integer>(that));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_bigint());
			that = cbox<That>(thix.get_bigint());
			return;
		}

	};

	template <>
	struct ValueAdapter<Image::Pixel> {

		using This = Value;

		using That = Image::Pixel;

		// ----------------

		inline static auto from (
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

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_object_of_array());
			thix.get_object_property(1_ix).to(that.red);
			thix.get_object_property(2_ix).to(that.green);
			thix.get_object_property(3_ix).to(that.blue);
			thix.get_object_property(4_ix).to(that.alpha);
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
			thix.set_string(that.to_string());
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_string());
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

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbox<Integer>(that));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_bigint());
			that = cbox<That>(thix.get_bigint());
			return;
		}

	};

	template <>
	struct ValueAdapter<IntegerU32> {

		using This = Value;

		using That = IntegerU32;

		// ----------------

		inline static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_bigint(cbox<Integer>(that));
			return;
		}

		inline static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_test(thix.is_bigint());
			that = cbox<That>(thix.get_bigint());
			return;
		}

	};

	#pragma endregion

}
