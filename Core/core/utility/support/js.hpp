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

namespace TwinKleS::Core::JS {

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

	template <typename TValue> requires
		AutoConstraint
	struct ValueAdapter<BooleanWrapper<TValue>> {

		using This = Value;

		using That = BooleanWrapper<TValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_boolean(cbw<Boolean>(that));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_boolean());
			that = cbw<That>(thix.get_boolean());
			return;
		}

	};

	// ----------------

	template <typename TValue> requires
		AutoConstraint
	struct ValueAdapter<IntegerWrapper<TValue>> {

		using This = Value;

		using That = IntegerWrapper<TValue>;

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

	template <typename TValue> requires
		AutoConstraint
	struct ValueAdapter<FloatingWrapper<TValue>> {

		using This = Value;

		using That = FloatingWrapper<TValue>;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.set_number(cbw<Floating>(that));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_number());
			that = cbw<That>(thix.get_number());
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

	#pragma endregion

	#pragma region container

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
				thix.define_object_property_value(index, thix.new_value(that[index]));
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object_of_array());
			auto size = cbw<Size>(thix.get_object_property("length"_sv).to_of<Floating>());
			that.allocate_full(size);
			for (auto & index : SizeRange{size}) {
				thix.get_object_property(index).to(that[index]);
			}
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
			thix.set_object();
			for (auto & index : SizeRange{that.size()}) {
				thix.define_object_property_value(that.at(index).key, thix.new_value(that.at(index).value));
			}
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object());
			auto property_map = thix.get_object_own_property();
			that.allocate_full(property_map.size());
			for (auto & index : SizeRange{property_map.size()}) {
				that.at(index).key = property_map.at(index).key;
				property_map.at(index).value.to(that.at(index).value);
			}
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

	// ----------------

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
			that.assign(thix.get_object_data_of_array_buffer());
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
			restruct(that, thix.get_object_data_of_array_buffer());
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
			thix.set_object_of_array();
			thix.define_object_property_value(1_ix, thix.new_value(that.x));
			thix.define_object_property_value(2_ix, thix.new_value(that.y));
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
			thix.set_object_of_array();
			thix.define_object_property_value(1_ix, thix.new_value(that.width));
			thix.define_object_property_value(2_ix, thix.new_value(that.height));
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
			thix.define_object_property_value(1_ix, thix.new_value(that.red));
			thix.define_object_property_value(2_ix, thix.new_value(that.green));
			thix.define_object_property_value(3_ix, thix.new_value(that.blue));
			thix.define_object_property_value(4_ix, thix.new_value(that.alpha));
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
			switch (that.type()) {
				case ValueType::null : {
					thix.from(that.get_null());
					break;
				}
				case ValueType::boolean : {
					thix.from(that.get_boolean());
					break;
				}
				case ValueType::number : {
					thix.from(that.get_number());
					break;
				}
				case ValueType::string : {
					thix.from(that.get_string());
					break;
				}
				case ValueType::array : {
					thix.from(that.get_array());
					break;
				}
				case ValueType::object : {
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
			} else if (thix.is_object()) {
				if (thix.is_object_of_array()) {
					thix.to(that.set_array());
				} else {
					thix.to(that.set_object());
				}
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
			thix.set_object();
			auto type = that.type();
			thix.define_object_property_value("type"_sv, thix.new_value(type));
			auto node_value_js = thix.new_value();
			switch (type) {
				case NodeType::element : {
					auto & node_value = that.get_element();
					node_value_js.set_object();
					node_value_js.define_object_property_value("name"_sv, thix.new_value(node_value.name));
					node_value_js.define_object_property_value("attribute"_sv, thix.new_value(node_value.attribute));
					node_value_js.define_object_property_value("child"_sv, thix.new_value(node_value.child));
					break;
				}
				case NodeType::text : {
					auto & node_value = that.get_text();
					node_value_js.set_object();
					node_value_js.define_object_property_value("value"_sv, thix.new_value(node_value.value));
					node_value_js.define_object_property_value("cdata"_sv, thix.new_value(node_value.cdata));
					break;
				}
				case NodeType::comment : {
					auto & node_value = that.get_comment();
					node_value_js.set_object();
					node_value_js.define_object_property_value("value"_sv, thix.new_value(node_value.value));
					break;
				}
			}
			thix.define_object_property_value("value"_sv, as_moveable(node_value_js));
			return;
		}

		static auto to (
			This & thix,
			That & that
		) -> Void {
			assert_condition(thix.is_object());
			using namespace XML;
			auto type = NodeType{};
			thix.get_object_property("type"_sv).to(type);
			auto node_value_js = thix.get_object_property("value"_sv);
			switch (type) {
				case NodeType::element : {
					that.set_element();
					auto & node_value = that.get_element();
					node_value_js.get_object_property("name"_sv).to(node_value.name);
					node_value_js.get_object_property("attribute"_sv).to(node_value.attribute);
					node_value_js.get_object_property("child"_sv).to(node_value.child);
					break;
				}
				case NodeType::text : {
					that.set_text();
					auto & node_value = that.get_text();
					node_value_js.get_object_property("value"_sv).to(node_value.value);
					node_value_js.get_object_property("cdata"_sv).to(node_value.cdata);
					break;
				}
				case NodeType::comment : {
					that.set_comment();
					auto & node_value = that.get_comment();
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
	struct ValueAdapter<Boolean> :
		ValueAdapter<BooleanWrapper<Boolean::Value>> {
	};

	template <>
	struct ValueAdapter<Integer> :
		ValueAdapter<IntegerWrapper<Integer::Value>> {
	};

	template <>
	struct ValueAdapter<Floating> :
		ValueAdapter<FloatingWrapper<Floating::Value>> {
	};

	template <>
	struct ValueAdapter<Byte> :
		ValueAdapter<IntegerWrapper<Byte::Value>> {
	};

	template <>
	struct ValueAdapter<Size> :
		ValueAdapter<IntegerWrapper<Size::Value>> {
	};

	#pragma endregion

}
