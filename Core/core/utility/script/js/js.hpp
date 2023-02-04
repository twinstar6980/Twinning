#pragma once

#include "core/utility/container/wrapper/wrapper.hpp"
#include "core/utility/null.hpp"
#include "core/utility/miscellaneous/byte_series/container.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/data/json/value.hpp"
#include "core/utility/script/js/value_adapter.hpp"
#include "core/utility/script/js/handler.hpp"
#include "core/third/quickjs.hpp"

namespace TwinStar::Core::JS {

	#pragma region namespace alias

	namespace quickjs = Third::quickjs;

	#pragma endregion

	#pragma region declaration

	class Runtime;

	class Context;

	class Value;

	#pragma endregion

	#pragma region mutex of js thread

	inline auto g_mutex = std::mutex{};

	#pragma endregion

	#pragma region type

	class Runtime {

	protected:

		Pointer<quickjs::JSRuntime> m_runtime;
		Boolean                     m_is_holder;

	protected:

		#pragma region structor

		explicit Runtime (
			ZPointer<quickjs::JSRuntime> const & raw_value,
			Boolean const &                      is_holder
		) :
			m_runtime{raw_value},
			m_is_holder{is_holder} {
		}

		#pragma endregion

	public:

		#pragma region create

		static auto new_reference (
			ZPointer<quickjs::JSRuntime> const & runtime
		) -> Runtime {
			return Runtime{runtime, k_false};
		}

		static auto new_instance (
		) -> Runtime {
			return Runtime{quickjs::JS_NewRuntime(), k_true};
		}

		#pragma endregion

	public:

		#pragma region structor

		~Runtime (
		) {
			if (thiz.m_is_holder) {
				quickjs::JS_FreeRuntime(thiz._runtime());
			}
		}

		// ----------------

		Runtime (
		) = delete;

		Runtime (
			Runtime const & that
		) = delete;

		Runtime (
			Runtime && that
		) :
			m_runtime{},
			m_is_holder{} {
			thiz = as_moveable(that);
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Runtime const & that
		) -> Runtime& = delete;

		auto operator = (
			Runtime && that
		) -> Runtime& {
			thiz.m_runtime = that.m_runtime;
			thiz.m_is_holder = that.m_is_holder;
			that.m_runtime = k_null_pointer;
			that.m_is_holder = k_false;
			return thiz;
		}

		#pragma endregion

		#pragma region raw

		auto _runtime (
		) -> ZPointer<quickjs::JSRuntime> {
			return thiz.m_runtime.value;
		}

		#pragma endregion

		#pragma region context

		auto new_context (
		) -> Context;

		#pragma endregion

		#pragma region class

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		auto register_class (
			String const & name
		) -> Void;

		#pragma endregion

	};

	class Context {

	protected:

		Pointer<quickjs::JSContext> m_context;
		Boolean                     m_is_holder;

	protected:

		#pragma region structor

		explicit Context (
			ZPointer<quickjs::JSContext> const & raw_value,
			Boolean const &                      is_holder
		) :
			m_context{raw_value},
			m_is_holder{is_holder} {
		}

		#pragma endregion

	public:

		#pragma region create

		static auto new_reference (
			ZPointer<quickjs::JSContext> const & context
		) -> Context {
			return Context{context, k_false};
		}

		static auto new_instance (
			ZPointer<quickjs::JSRuntime> const & runtime
		) -> Context {
			return Context{quickjs::JS_NewContext(runtime), k_true};
		}

		#pragma endregion

	public:

		#pragma region structor

		~Context (
		) {
			if (thiz.m_is_holder) {
				quickjs::JS_FreeContext(thiz._context());
			}
		}

		// ----------------

		Context (
		) = delete;

		Context (
			Context const & that
		) = delete;

		Context (
			Context && that
		) :
			m_context{},
			m_is_holder{} {
			thiz = as_moveable(that);
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Context const & that
		) -> Context& = delete;

		auto operator = (
			Context && that
		) -> Context& {
			thiz.m_context = that.m_context;
			thiz.m_is_holder = that.m_is_holder;
			that.m_context = k_null_pointer;
			that.m_is_holder = k_false;
			return thiz;
		}

		#pragma endregion

		#pragma region raw

		auto _context (
		) -> ZPointer<quickjs::JSContext> {
			return thiz.m_context.value;
		}

		#pragma endregion

		#pragma region runtime

		auto runtime (
		) -> Runtime {
			return Runtime::new_reference(quickjs::JS_GetRuntime(thiz._context()));
		}

		#pragma endregion

		#pragma region value

		auto new_value (
		) -> Value;

		template <typename ValueObject> requires
			CategoryConstraint<IsValid<ValueObject>>
		auto new_value (
			ValueObject && value
		) -> Value;

		#pragma endregion

		#pragma region class

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		auto get_class_prototype (
		) -> Value;

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		auto set_class_prototype (
			Value && value
		) -> Void;

		#pragma endregion

		#pragma region special

		auto global_object (
		) -> Value;

		auto current_exception (
		) -> Value;

		auto throw_exception (
			Value && value
		) -> Void;

		#pragma endregion

		#pragma region evaluate

		auto evaluate (
			CStringView const & script,
			String const &      name = "<!evaluate>"_s
		) -> Value;

		#pragma endregion

	};

	class Value {

	protected:

		Optional<ZPointer<quickjs::JSContext>> m_context;
		quickjs::JSValue                       m_value;

	protected:

		#pragma region structor

		explicit Value (
			quickjs::JSValue const & value
		) :
			m_context{},
			m_value{value} {
		}

		explicit Value (
			ZPointer<quickjs::JSContext> const & context,
			quickjs::JSValue const &             value
		) :
			m_context{},
			m_value{value} {
			thiz.m_context.set(context);
		}

		#pragma endregion

	public:

		#pragma region create

		static auto new_reference (
			ZPointer<quickjs::JSContext> const & context,
			quickjs::JSValue const &             value
		) -> Value {
			return Value{context, quickjs::JS_DupValue(context, value)};
		}

		static auto new_instance (
			ZPointer<quickjs::JSContext> const & context,
			quickjs::JSValue const &             value = quickjs::JS_UNINITIALIZED_
		) -> Value {
			return Value{context, value};
		}

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		static auto new_instance_of (
			ZPointer<quickjs::JSContext> const & context,
			Argument && ...                      argument
		) -> Value {
			auto result = new_instance(context);
			result.from(as_forward<Argument>(argument) ...);
			return result;
		}

		#pragma endregion

	public:

		#pragma region structor

		~Value (
		) {
			thiz._reset_value();
		}

		// ----------------

		Value (
		) :
			Value{quickjs::JS_UNINITIALIZED_} {
		}

		Value (
			Value const & that
		) :
			Value{} {
			thiz.m_context = that.m_context;
			thiz.m_value = quickjs::JS_DupValue(as_variable(that)._context(), as_variable(that)._value());
		}

		Value (
			Value && that
		) :
			Value{} {
			thiz.m_context = that.m_context;
			thiz.m_value = that._release_value();
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Value const & that
		) -> Value& {
			thiz._reset_value();
			thiz.m_context = that.m_context;
			thiz.m_value = quickjs::JS_DupValue(as_variable(that)._context(), as_variable(that)._value());
			return thiz;
		}

		auto operator = (
			Value && that
		) -> Value& {
			thiz._reset_value();
			thiz.m_context = that.m_context;
			thiz.m_value = that._release_value();
			return thiz;
		}

		#pragma endregion

		#pragma region raw

		auto _context (
		) -> ZPointer<quickjs::JSContext> {
			return thiz.m_context.get();
		}

		auto _value (
		) -> quickjs::JSValue {
			return thiz.m_value;
		}

		#pragma endregion

		#pragma region raw bind

		auto _unbind_context (
		) -> Void {
			thiz.m_context.reset();
			return;
		}

		auto _unbind_value (
		) -> Void {
			thiz.m_value = quickjs::JS_UNINITIALIZED_;
			return;
		}

		auto _unbind (
		) -> Void {
			thiz._unbind_value();
			thiz._unbind_context();
			return;
		}

		auto _rebind_value (
			quickjs::JSValue const & new_value
		) -> Void {
			if (thiz.m_context) {
				quickjs::JS_FreeValue(thiz._context(), thiz._value());
			}
			thiz.m_value = new_value;
			return;
		}

		// ----------------

		auto _reset_value (
		) -> Void {
			return thiz._rebind_value(quickjs::JS_UNINITIALIZED_);
		}

		auto _release_value (
		) -> quickjs::JSValue {
			auto result = thiz.m_value;
			thiz._unbind_value();
			return result;
		}

		#pragma endregion

		#pragma region context

		auto context (
		) -> Context {
			return Context::new_reference(thiz._context());
		}

		#pragma endregion

		#pragma region value

		auto new_value (
		) -> Value {
			return thiz.new_instance(thiz._context());
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto new_value (
			Argument && ... argument
		) -> Value {
			return thiz.new_instance_of(thiz._context(), as_forward<Argument>(argument) ...);
		}

		#pragma endregion

		#pragma region check uninitialized

		auto is_uninitialized (
		) -> Boolean {
			return mbw<Boolean>(quickjs::JS_IsUninitialized(thiz._value()));
		}

		auto set_uninitialized (
		) -> Void {
			return thiz._rebind_value(quickjs::JS_UNINITIALIZED_);
		}

		#pragma endregion

		#pragma region is

		auto is_undefined (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsUndefined(thiz._value()));
		}

		auto is_null (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsNull(thiz._value()));
		}

		auto is_boolean (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsBool(thiz._value()));
		}

		auto is_bigint (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsBigInt(thiz._context(), thiz._value()));
		}

		auto is_number (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsNumber(thiz._value()));
		}

		auto is_string (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsString(thiz._value()));
		}

		auto is_object (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsObject(thiz._value()));
		}

		// ----------------

		auto is_exception (
		) -> Boolean {
			assert_condition(thiz.m_context);
			return mbw<Boolean>(quickjs::JS_IsException(thiz._value()));
		}

		#pragma endregion

		#pragma region get

		auto get_undefined (
		) -> Void {
			assert_condition(thiz.is_undefined());
			return;
		}

		auto get_null (
		) -> Null {
			assert_condition(thiz.is_null());
			return k_null;
		}

		auto get_boolean (
		) -> Boolean {
			assert_condition(thiz.is_boolean());
			auto raw_value = quickjs::JS_ToBool(thiz._context(), thiz._value());
			return mbw<Boolean>(raw_value);
		}

		auto get_bigint (
		) -> Integer {
			assert_condition(thiz.is_bigint());
			auto raw_value = int64_t{};
			quickjs::JS_ToBigInt64(thiz._context(), &raw_value, thiz._value());
			return mbw<Integer>(raw_value);
		}

		auto get_number (
		) -> Floating {
			assert_condition(thiz.is_number());
			auto raw_value = double{};
			quickjs::JS_ToFloat64(thiz._context(), &raw_value, thiz._value());
			return mbw<Floating>(raw_value);
		}

		auto get_string (
		) -> String {
			assert_condition(thiz.is_string());
			auto raw_length = size_t{};
			auto raw_value = quickjs::JS_ToCStringLen(thiz._context(), &raw_length, thiz._value());
			auto value = make_string(raw_value, raw_length);
			quickjs::JS_FreeCString(thiz._context(), raw_value);
			return value;
		}

		// ----------------

		auto get_object_of_array_buffer (
		) -> VByteListView {
			auto size = size_t{};
			auto data = quickjs::JS_GetArrayBuffer(thiz._context(), &size, thiz._value());
			assert_condition(data);
			return VByteListView{cast_pointer<Byte>(make_pointer(data)), mbw<Size>(size)};
		}

		#pragma endregion

		#pragma region set

		auto set_undefined (
		) -> Void {
			return thiz._rebind_value(quickjs::JS_UNDEFINED_);
		}

		auto set_null (
			Null const & value
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NULL_);
		}

		auto set_boolean (
			Boolean const & value
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewBool(thiz._context(), static_cast<int>(value.value)));
		}

		auto set_bigint (
			Integer const & value
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewBigInt64(thiz._context(), value.value));
		}

		auto set_number (
			Floating const & value
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewFloat64(thiz._context(), value.value));
		}

		auto set_string (
			String const & value
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewStringLen(thiz._context(), cast_pointer<char>(value.begin()).value, value.size().value));
		}

		// TODO : sv remove
		auto set_string (
			CStringView const & value
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewStringLen(thiz._context(), cast_pointer<char>(value.begin()).value, value.size().value));
		}

		// ----------------

		auto set_object_of_object (
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewObject(thiz._context()));
		}

		auto set_object_of_array (
		) -> Void {
			return thiz._rebind_value(quickjs::JS_NewArray(thiz._context()));
		}

		auto set_object_of_array_buffer (
			CByteListView const & data
		) -> Void {
			return thiz._rebind_value(
				quickjs::JS_NewArrayBufferCopy(
					thiz._context(),
					cast_pointer<uint8_t>(data.begin()).value,
					data.size().value
				)
			);
		}

		auto set_object_of_array_buffer (
			VByteListView const & data,
			Boolean const &       is_holder
		) -> Void {
			return thiz._rebind_value(
				quickjs::JS_NewArrayBuffer(
					thiz._context(),
					cast_pointer<uint8_t>(data.begin()).value,
					data.size().value,
					!is_holder
					? nullptr
					: [] (quickjs::JSRuntime * rt, void * opaque, void * ptr) -> auto {
						delete[] static_cast<uint8_t *>(ptr);
						return;
					},
					nullptr,
					false
				)
			);
		}

		template <auto function, auto forward_object> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSameV<forward_object, ZBoolean>)
			&& (!forward_object || CallableTraitOf<function>::Argument::size > 0_szz)
		auto set_object_of_native_function (
			String const & name
		) -> Void;

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsHandler<typename CallableTraitOf<function>::Result>)
		auto set_object_of_native_constructor (
			String const &  name,
			Boolean const & is_generic
		) -> Void;

		#pragma endregion

		#pragma region object prototype

		auto get_object_prototype (
		) -> Value {
			assert_condition(thiz.is_object());
			return thiz.new_instance(thiz._context(), quickjs::JS_GetPrototype(thiz._context(), thiz._value()));
		}

		auto set_object_prototype (
			Value && value
		) -> Void {
			assert_condition(thiz.is_object());
			quickjs::JS_SetPrototype(thiz._context(), thiz._value(), value._release_value());
			return;
		}

		#pragma endregion

		#pragma region object class name

		auto get_object_class_name (
		) -> String {
			assert_condition(thiz.is_object());
			return thiz.get_object_prototype().get_object_property("constructor"_sv).get_object_property("name"_sv).get_string();
		}

		// ----------------

		auto is_object_of_class (
			String const & name
		) -> Boolean {
			return thiz.is_object() && thiz.get_object_class_name() == name;
		}

		// TODO : sv remove
		auto is_object_of_class (
			CStringView const & name
		) -> Boolean {
			return thiz.is_object() && thiz.get_object_class_name() == name;
		}

		// ----------------

		auto is_object_of_object (
		) -> Boolean {
			return thiz.is_object_of_class("Object"_sv);
		}

		auto is_object_of_array (
		) -> Boolean {
			return thiz.is_object_of_class("Array"_sv);
		}

		#pragma endregion

		#pragma region object property

		auto define_object_property (
			String const & name,
			Value &&       value
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_DefinePropertyValue(thiz._context(), thiz._value(), atom, value._release_value(), quickjs::JS_PROP_C_W_E_);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		auto define_object_property (
			Size const & index,
			Value &&     value
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomUInt32(thiz._context(), static_cast<uint32_t>(index.value));
			auto result = quickjs::JS_DefinePropertyValue(thiz._context(), thiz._value(), atom, value._release_value(), quickjs::JS_PROP_C_W_E_);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		// ----------------

		auto define_object_property (
			String const & name,
			Value &&       getter,
			Value &&       setter
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_DefinePropertyGetSet(thiz._context(), thiz._value(), atom, getter._release_value(), setter._release_value(), quickjs::JS_PROP_C_W_E_);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		auto define_object_property (
			Size const & index,
			Value &&     getter,
			Value &&     setter
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomUInt32(thiz._context(), static_cast<uint32_t>(index.value));
			auto result = quickjs::JS_DefinePropertyGetSet(thiz._context(), thiz._value(), atom, getter._release_value(), setter._release_value(), quickjs::JS_PROP_C_W_E_);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		// ----------------

		auto delete_object_property (
			String const & name
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_DeleteProperty(thiz._context(), thiz._value(), atom, quickjs::JS_PROP_THROW_);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		auto delete_object_property (
			Size const & index
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomUInt32(thiz._context(), static_cast<uint32_t>(index.value));
			auto result = quickjs::JS_DeleteProperty(thiz._context(), thiz._value(), atom, quickjs::JS_PROP_THROW_);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		// ----------------

		auto has_object_property (
			String const & name
		) -> Boolean {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_HasProperty(thiz._context(), thiz._value(), atom);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			return mbw<Boolean>(result == 1);
		}

		auto has_object_property (
			Size const & index
		) -> Boolean {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomUInt32(thiz._context(), static_cast<uint32_t>(index.value));
			auto result = quickjs::JS_HasProperty(thiz._context(), thiz._value(), atom);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			return mbw<Boolean>(result == 1);
		}

		// ----------------

		// TODO : sv remove
		auto get_object_property (
			CStringView const & name
		) -> Value {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_GetProperty(thiz._context(), thiz._value(), atom);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			return thiz.new_instance(thiz._context(), result);
		}

		auto get_object_property (
			String const & name
		) -> Value {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_GetProperty(thiz._context(), thiz._value(), atom);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			return thiz.new_instance(thiz._context(), result);
		}

		auto get_object_property (
			Size const & index
		) -> Value {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomUInt32(thiz._context(), static_cast<uint32_t>(index.value));
			auto result = quickjs::JS_GetProperty(thiz._context(), thiz._value(), atom);
			quickjs::JS_FreeAtom(thiz._context(), atom);
			return thiz.new_instance(thiz._context(), result);
		}

		// ----------------

		// TODO : sv remove
		auto set_object_property (
			CStringView const & name,
			Value &&            value
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_SetProperty(thiz._context(), thiz._value(), atom, value._release_value());
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		auto set_object_property (
			String const & name,
			Value &&       value
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomLen(thiz._context(), cast_pointer<char>(name.begin()).value, name.size().value);
			auto result = quickjs::JS_SetProperty(thiz._context(), thiz._value(), atom, value._release_value());
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		auto set_object_property (
			Size const & index,
			Value &&     value
		) -> Void {
			assert_condition(thiz.is_object());
			auto atom = quickjs::JS_NewAtomUInt32(thiz._context(), static_cast<uint32_t>(index.value));
			auto result = quickjs::JS_SetProperty(thiz._context(), thiz._value(), atom, value._release_value());
			quickjs::JS_FreeAtom(thiz._context(), atom);
			assert_condition(result != -1);
			assert_condition(result == 1);
			return;
		}

		#pragma endregion

		#pragma region object own property

		auto collect_object_own_property (
		) -> Map<String, Value> {
			assert_condition(thiz.is_object());
			auto property_enum = ZPointer<quickjs::JSPropertyEnum>{};
			auto property_count = uint32_t{};
			quickjs::JS_GetOwnPropertyNames(thiz._context(), &property_enum, &property_count, thiz._value(), quickjs::JS_GPN_STRING_MASK_);
			auto result = Map<String, Value>{mbw<Size>(property_count)};
			for (auto & element : make_range_n(make_pointer(property_enum), mbw<Size>(property_count))) {
				auto name = quickjs::JS_AtomToCString(thiz._context(), element.atom);
				result.append(make_string(name), thiz.new_instance(thiz._context(), quickjs::JS_GetProperty(thiz._context(), thiz._value(), element.atom)));
				quickjs::JS_FreeCString(thiz._context(), name);
			}
			quickjs::js_free_prop_enum(thiz._context(), property_enum, property_count);
			return result;
		}

		// ----------------

		auto collect_object_own_property_of_object (
		) -> Map<String, Value> {
			assert_condition(thiz.is_object_of_object());
			auto map = thiz.collect_object_own_property();
			return map;
		}

		auto collect_object_own_property_of_array (
		) -> List<Value> {
			assert_condition(thiz.is_object_of_array());
			auto map = thiz.collect_object_own_property();
			auto list = List<Value>{};
			auto length = cbw<Size>(map["length"_sv].to_of<Floating>());
			list.allocate_full(length);
			for (auto & index : SizeRange{length}) {
				assert_condition(cbw<Size>(map.at(index).key.to_of<Integer>()) == index);
				list[index] = as_moveable(map.at(index).value);
			}
			return list;
		}

		#pragma endregion

		#pragma region function call

		auto call (
			List<Value> const & argument
		) -> Value {
			assert_condition(thiz.is_object());
			auto argument_value = Array<quickjs::JSValue>{};
			argument_value.assign(
				argument,
				[] (auto & element) -> auto {
					return as_variable(element)._value();
				}
			);
			return thiz.new_instance(thiz._context(), quickjs::JS_Call(thiz._context(), thiz._value(), quickjs::JS_UNDEFINED_, static_cast<int>(argument.size().value), argument_value.begin().value));
		}

		auto call_method (
			String const &      name,
			List<Value> const & argument
		) -> Value {
			assert_condition(thiz.is_object());
			auto function = thiz.get_object_property(name);
			auto argument_value = Array<quickjs::JSValue>{};
			argument_value.assign(
				argument,
				[] (auto & element) -> auto {
					return as_variable(element)._value();
				}
			);
			return thiz.new_instance(thiz._context(), quickjs::JS_Call(thiz._context(), function._value(), thiz._value(), static_cast<int>(argument.size().value), argument_value.begin().value));
		}

		#pragma endregion

		#pragma region from & to by adapter

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto from (
			That &&       that,
			Option && ... option
		) -> Void {
			assert_condition(thiz.m_context);
			thiz.set_uninitialized();
			ValueAdapter<AsPure<That>>::from(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto to (
			That &&       that,
			Option && ... option
		) -> That&& {
			assert_condition(thiz.m_context);
			ValueAdapter<AsPure<That>>::to(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return as_forward<That>(that);
		}

		// ----------------

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
		auto to_of (
			Option && ... option
		) -> That {
			auto that = That{};
			thiz.to(that, as_forward<Option>(option) ...);
			return that;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region detail

	namespace Detail {

		inline constexpr auto k_invalid_class_id = quickjs::JSClassID{0};

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		inline auto g_class_id = quickjs::JSClassID{k_invalid_class_id};

		// ----------------

		template <auto function, typename ... Argument> requires
			NoneConstraint
		inline auto call_native_function_inner (
			Argument && ... argument
		) -> typename CallableTraitOf<function>::Result {
			auto finalizer = make_finalizer(
				[] {
					g_mutex.unlock();
				},
				[] {
					g_mutex.lock();
				}
			);
			return function(as_forward<Argument>(argument) ...);
		}

		template <auto function, auto forward_object> requires
			NoneConstraint
		inline auto call_native_function (
			quickjs::JSContext * const & context,
			quickjs::JSValue const &     object,
			quickjs::JSValue * const &   argument
		) -> typename CallableTraitOf<function>::Result {
			return [&] <auto ... index> (ValuePackage<index ...>) -> typename CallableTraitOf<function>::Result {
				if constexpr (forward_object) {
					using Class = AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>;
					using Argument = AsTypePackageRemoveHead<typename CallableTraitOf<function>::Argument, 1_szz>;
					return call_native_function_inner<function>(
						as_lvalue(Value::new_reference(context, object).to_of<Class>()),
						as_forward<typename Argument::template Element<index>>(Value::new_reference(context, argument[index]).template to_of<AsPure<typename Argument::template Element<index>>>()) ...
					);
				} else {
					using Argument = typename CallableTraitOf<function>::Argument;
					return call_native_function_inner<function>(
						as_forward<typename Argument::template Element<index>>(Value::new_reference(context, argument[index]).template to_of<AsPure<typename Argument::template Element<index>>>()) ...
					);
				}
			}(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size - (forward_object ? (1) : (0))>{});
		}

		// ----------------

		template <auto function, auto forward_object> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSameV<forward_object, ZBoolean>)
		inline auto proxy_native_function (
			quickjs::JSContext * context,
			quickjs::JSValue     this_value,
			int                  argument_count,
			quickjs::JSValue *   argument
		) -> quickjs::JSValue {
			#if defined M_build_release
			constexpr auto throw_js_exception =
				[] (
				quickjs::JSContext * const & context,
				CStringView const &          message
			) -> auto {
				auto context_wrapper = Context::new_reference(context);
				context_wrapper.throw_exception(
					context_wrapper.evaluate(
						R"((message) => {
							let error = new Error();
							error.name = 'NativeError';
							error.message = message;
							error.stack = error.stack.substring(error.stack.indexOf('\n') + 1);
							return error;
						})"_sv
					).call(
						make_list<Value>(
							Value::new_instance_of(context, message)
						)
					)
				);
				return quickjs::JS_EXCEPTION_;
			};
			try {
			#endif
				if constexpr (IsVoid<typename CallableTraitOf<function>::Result>) {
					call_native_function<function, forward_object>(context, this_value, argument);
					return quickjs::JS_UNDEFINED_;
				} else {
					auto && native_result = call_native_function<function, forward_object>(context, this_value, argument);
					auto    result = Value::new_instance(context);
					result.from(as_forward<decltype(native_result)>(native_result));
					return result._release_value();
				}
			#if defined M_build_release
			} catch (Exception & exception) {
				return throw_js_exception(context, make_string_view(exception.what()));
			} catch (std::exception & exception) {
				return throw_js_exception(context, make_string_view(exception.what()));
			} catch (...) {
				return throw_js_exception(context, "unknown exception"_sv);
			}
			#endif
		}

	}

	#pragma endregion

	#pragma region type method implement

	inline auto Runtime::new_context (
	) -> Context {
		return Context::new_instance(thiz._runtime());
	}

	// ----------------

	template <typename Class> requires
		CategoryConstraint<IsPureInstance<Class>>
	inline auto Runtime::register_class (
		String const & name
	) -> Void {
		auto definition = quickjs::JSClassDef{
			cast_pointer<char>(make_null_terminated_string(name).begin()).value,
			[] (quickjs::JSRuntime * rt, quickjs::JSValue obj) -> void {
				delete static_cast<Handler<Class> *>(quickjs::JS_GetOpaque(obj, Detail::g_class_id<Class>));
				return;
			},
			nullptr,
			nullptr,
			nullptr
		};
		quickjs::JS_NewClassID(&Detail::g_class_id<Class>);
		quickjs::JS_NewClass(thiz._runtime(), Detail::g_class_id<Class>, &definition);
		return;
	}

	// ----------------

	inline auto Context::new_value (
	) -> Value {
		return Value::new_instance(thiz._context());
	}

	template <typename ValueObject> requires
		CategoryConstraint<IsValid<ValueObject>>
	inline auto Context::new_value (
		ValueObject && value
	) -> Value {
		return Value::new_instance_of(thiz._context(), value);
	}

	// ----------------

	template <typename Class> requires
		CategoryConstraint<IsPureInstance<Class>>
	inline auto Context::get_class_prototype (
	) -> Value {
		return Value::new_instance(thiz._context(), quickjs::JS_GetClassProto(thiz._context(), Detail::g_class_id<Class>));
	}

	template <typename Class> requires
		CategoryConstraint<IsPureInstance<Class>>
	inline auto Context::set_class_prototype (
		Value && value
	) -> Void {
		quickjs::JS_SetClassProto(thiz._context(), Detail::g_class_id<Class>, value._release_value());
		return;
	}

	// ----------------

	inline auto Context::global_object (
	) -> Value {
		return Value::new_instance(thiz._context(), quickjs::JS_GetGlobalObject(thiz._context()));
	}

	inline auto Context::current_exception (
	) -> Value {
		return Value::new_instance(thiz._context(), quickjs::JS_GetException(thiz._context()));
	}

	inline auto Context::throw_exception (
		Value && value
	) -> Void {
		quickjs::JS_Throw(thiz._context(), value._release_value());
		return;
	}

	// ----------------

	inline auto Context::evaluate (
		CStringView const & script,
		String const &      name
	) -> Value {
		return Value::new_instance(
			thiz._context(),
			quickjs::JS_Eval(
				thiz._context(),
				cast_pointer<char>(make_null_terminated_string(script).begin()).value,
				script.size().value,
				cast_pointer<char>(make_null_terminated_string(name).begin()).value,
				quickjs::JS_EVAL_FLAG_STRICT_
			)
		);
	}

	// ----------------

	template <auto function, auto forward_object> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
		&& (IsSameV<forward_object, ZBoolean>)
		&& (!forward_object || CallableTraitOf<function>::Argument::size > 0_szz)
	inline auto Value::set_object_of_native_function (
		String const & name
	) -> Void {
		return thiz._rebind_value(
			quickjs::JS_NewCFunction2(
				thiz._context(),
				&Detail::proxy_native_function<function, forward_object>,
				cast_pointer<char>(make_null_terminated_string(name).begin()).value,
				CallableTraitOf<function>::Argument::size,
				quickjs::JS_CFUNC_generic_,
				0
			)
		);
	}

	template <auto function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
		&& (IsHandler<typename CallableTraitOf<function>::Result>)
	inline auto Value::set_object_of_native_constructor (
		String const &  name,
		Boolean const & is_generic
	) -> Void {
		return thiz._rebind_value(
			quickjs::JS_NewCFunction2(
				thiz._context(),
				&Detail::proxy_native_function<function, false>,
				cast_pointer<char>(make_null_terminated_string(name).begin()).value,
				CallableTraitOf<function>::Argument::size,
				is_generic ? (quickjs::JS_CFUNC_generic_) : (quickjs::JS_CFUNC_constructor_),
				0
			)
		);
	}

	#pragma endregion

}
