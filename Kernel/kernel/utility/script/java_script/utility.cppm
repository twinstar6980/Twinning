module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.script.java_script.utility;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.script.java_script.value;
import twinning.kernel.utility.null;
import twinning.kernel.utility.miscellaneous.allocation;
import twinning.kernel.utility.miscellaneous.finalizer;
import twinning.kernel.utility.container.variant.variant;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.string.format;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.support.fmt;
import twinning.kernel.utility.thread.mutex;
import twinning.kernel.utility.thread.unlocker;
import twinning.kernel.third.quickjs_ng;

export namespace Twinning::Kernel::JavaScript {

	#pragma region global mutex

	inline auto g_mutex = Thread::Mutex{};

	#pragma endregion

	#pragma region native value

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	class NativeValueHandler {

	public:

		using Value = TValue;

	protected:

		Variant<Null, Pointer<Value>, std::shared_ptr<Value>> m_value;

	protected:

		#pragma region structor

		explicit NativeValueHandler (
			Pointer<Value> const & value,
			Boolean const &        is_holder
		) :
			NativeValueHandler{} {
			if (is_holder) {
				thiz.m_value.template set<std::shared_ptr<Value>>(value.value);
			}
			else {
				thiz.m_value.template set<Pointer<Value>>(value);
			}
		}

		#pragma endregion

	public:

		#pragma region structor

		~NativeValueHandler (
		) = default;

		// ----------------

		NativeValueHandler (
		) :
			m_value{} {
		}

		NativeValueHandler (
			NativeValueHandler const & that
		) = default;

		NativeValueHandler (
			NativeValueHandler && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			NativeValueHandler const & that
		) -> NativeValueHandler & = default;

		auto operator = (
			NativeValueHandler && that
		) -> NativeValueHandler & = default;

		#pragma endregion

		#pragma region value

		auto value (
		) -> Value & {
			assert_test(!thiz.m_value.template is<Null>());
			if (thiz.m_value.template is<Pointer<Value>>()) {
				return *thiz.m_value.template get<Pointer<Value>>();
			}
			else if (thiz.m_value.template is<std::shared_ptr<Value>>()) {
				return *thiz.m_value.template get<std::shared_ptr<Value>>();
			}
			else {
				throw ImpossibleException{};
			}
		}

		#pragma endregion

	public:

		#pragma region create

		inline static auto new_instance (
			Value & value
		) -> NativeValueHandler {
			return NativeValueHandler{make_pointer(&value), k_true};
		}

		inline static auto new_reference (
			Value & value
		) -> NativeValueHandler {
			return NativeValueHandler{make_pointer(&value), k_false};
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Value, Argument && ...>)
		inline static auto new_instance_allocate (
			Argument && ... argument
		) -> NativeValueHandler {
			return new_instance(*allocate_instance<Value>(as_forward<Argument>(argument) ...));
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsNativeValueHandler = IsTemplateInstanceOfT<It, NativeValueHandler>;

	#pragma endregion

	#pragma region native function

	namespace Detail {

		template <auto function, typename ... Argument> requires
			NoneConstraint
		inline auto call_native_function_wrapper_inner (
			Argument && ... argument
		) -> typename CallableTraitOf<function>::Result {
			auto unlocker = Thread::Unlocker{g_mutex};
			return function(as_forward<Argument>(argument) ...);
		}

		template <auto function, auto forward_object> requires
			NoneConstraint
		inline auto call_native_function_wrapper (
			Context &     context,
			Value &       object,
			List<Value> & argument
		) -> typename CallableTraitOf<function>::Result {
			return [&] <auto ... index> (ValuePackage<index ...>) -> typename CallableTraitOf<function>::Result {
				if constexpr (!forward_object) {
					using Argument = typename CallableTraitOf<function>::Argument;
					return call_native_function_wrapper_inner<function>(
						as_forward<typename Argument::template Element<index>>(argument[mbox<Size>(index)].template to_of<AsPure<typename Argument::template Element<index>>>()) ...
					);
				}
				else {
					using Class = AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>;
					using Argument = AsTypePackageRemoveHead<typename CallableTraitOf<function>::Argument, 1_szz>;
					return call_native_function_wrapper_inner<function>(
						as_lvalue(object.to_of<Class>()),
						as_forward<typename Argument::template Element<index>>(argument[mbox<Size>(index)].template to_of<AsPure<typename Argument::template Element<index>>>()) ...
					);
				}
			}(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size - (!forward_object ? (0) : (1))>{});
		}

		template <auto function, auto forward_object> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSameV<forward_object, Boolean>)
		inline auto proxy_native_function_wrapper (
			Context &     context,
			Value &       object,
			List<Value> & argument,
			Value &       result
		) -> Void {
			if constexpr (IsVoid<typename CallableTraitOf<function>::Result>) {
				call_native_function_wrapper<function, forward_object>(context, object, argument);
				result.set_undefined();
			}
			else {
				result.from(call_native_function_wrapper<function, forward_object>(context, object, argument));
			}
			return;
		}

		template <auto function, auto forward_object> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSameV<forward_object, Boolean>)
		inline constexpr auto make_proxy_native_function_wrapper (
		) -> auto {
			return &proxy_native_function_wrapper<function, forward_object>;
		}

	}

	template <auto function, auto forward_object> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
		&& (IsSameV<forward_object, Boolean>)
	inline constexpr auto & proxy_native_function_wrapper = *Detail::make_proxy_native_function_wrapper<function, forward_object>();

	// ----------------

	namespace Detail {

		template <auto function, typename ... Argument> requires
			NoneConstraint
		inline auto proxy_native_function_by_handler (
			NativeValueHandler<AsPure<Argument>> & ... argument
		) -> auto {
			using Result = typename CallableTraitOf<function>::Result;
			if constexpr (IsVoid<Result>) {
				return function(as_forward<Argument>(argument.value()) ...);
			}
			else if constexpr (IsInstance<Result>) {
				return NativeValueHandler<Result>::new_instance_allocate(function(as_forward<Argument>(argument.value()) ...));
			}
			else {
				return NativeValueHandler<AsPure<Result>>::new_reference(function(as_forward<Argument>(argument.value()) ...));
			}
		}

		template <auto function, auto ... argument_index> requires
			NoneConstraint
		inline constexpr auto make_proxy_native_function_by_handler (
			ValuePackage<argument_index ...>
		) -> auto {
			return &proxy_native_function_by_handler<function, typename CallableTraitOf<function>::Argument::template Element<argument_index> ...>;
		}

	}

	template <auto function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
		&& (IsVoid<typename CallableTraitOf<function>::Result> || IsInstance<typename CallableTraitOf<function>::Result> || IsVReference<typename CallableTraitOf<function>::Result>)
	inline constexpr auto & proxy_native_function_by_handler = *Detail::make_proxy_native_function_by_handler<function>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

	// ----------------

	namespace Detail {

		template <typename Type, typename ... Argument> requires
			NoneConstraint
		inline constexpr auto proxy_native_function_allocate_by_handler (
			NativeValueHandler<AsPure<Argument>> & ... argument
		) -> NativeValueHandler<Type> {
			return NativeValueHandler<Type>::new_instance_allocate(as_forward<Argument>(argument.value()) ...);
		}

		template <typename Type, typename ... Argument> requires
			NoneConstraint
		inline constexpr auto make_proxy_native_function_allocate_by_handler (
		) -> auto {
			return &proxy_native_function_allocate_by_handler<Type, Argument ...>;
		}

	}

	template <typename Type, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Type> && IsValid<Argument ...>>
		&& (IsConstructible<Type, Argument && ...>)
	inline constexpr auto & proxy_native_function_allocate_by_handler = *Detail::make_proxy_native_function_allocate_by_handler<Type, Argument ...>();

	// ----------------

	M_enumeration(
		M_wrap(NativeFunctionWrapperType),
		M_wrap(
			function,
			method,
			constructor,
		),
	);

	template <auto t_function, auto t_type> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsSameV<t_type, NativeFunctionWrapperType>)
		&& ((t_type == NativeFunctionWrapperType::Constant::function()) ||
			(t_type == NativeFunctionWrapperType::Constant::method() && CallableTraitOf<t_function>::Argument::size > 0_szz) ||
			(t_type == NativeFunctionWrapperType::Constant::constructor() && IsValid<typename CallableTraitOf<t_function>::Result>))
	struct NativeFunctionWrapper {

		String name{};

	};

	#pragma endregion

	#pragma region native class

	namespace Detail {

		inline constexpr auto k_invalid_native_class_id = Integer{0};

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		inline auto g_native_class_id = Integer{0};

		// ----------------

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		inline auto free_native_class (
			Runtime & rt,
			Value &   obj
		) -> Void {
			delete static_cast<NativeValueHandler<Class> *>(Third::quickjs_ng::$JS_GetOpaque(obj._value(), static_cast<Third::quickjs_ng::$JSClassID>(g_native_class_id<Class>.value)));
			return;
		}

	}

	// ----------------

	template <typename TClass> requires
		CategoryConstraint<IsPureInstance<TClass>>
	class NativeClassBuilder {

	public:

		using Class = TClass;

	protected:

		Optional<String> m_parent_name;

		String m_name;

		Value m_proto;

		Value m_constructor;

		Value m_parent;

	public:

		#pragma region structor

		~NativeClassBuilder (
		) = default;

		// ----------------

		NativeClassBuilder (
		) = delete;

		NativeClassBuilder (
			NativeClassBuilder const & that
		) = delete;

		NativeClassBuilder (
			NativeClassBuilder && that
		) = default;

		// ----------------

		explicit NativeClassBuilder (
			Optional<String> const & parent_name,
			String const &           name,
			Value &                  parent
		) :
			m_parent_name{parent_name},
			m_name{name},
			m_proto{},
			m_constructor{},
			m_parent{} {
			thiz.m_parent = Value{parent};
			thiz.m_proto = thiz.m_parent.new_value();
			thiz.m_proto.set_object_of_object();
			thiz.m_parent.context().runtime().template register_class<&Detail::free_native_class<Class>>(Detail::g_native_class_id<Class>, name);
			thiz.m_parent.context().set_class_proto(Detail::g_native_class_id<Class>, Value{thiz.m_proto});
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			NativeClassBuilder const & that
		) -> NativeClassBuilder & = delete;

		auto operator = (
			NativeClassBuilder && that
		) -> NativeClassBuilder & = default;

		#pragma endregion

		#pragma region name

		auto whole_name (
		) -> String {
			return !thiz.m_parent_name.has() ? (thiz.m_name) : ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name));
		}

		#pragma endregion

		#pragma region constructor

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<typename CallableTraitOf<function>::Result, NativeValueHandler<Class>>)
		auto set_constructor (
		) -> NativeClassBuilder & {
			thiz.m_constructor = thiz.m_proto.new_value(NativeFunctionWrapper<function, NativeFunctionWrapperType::Constant::constructor()>{thiz.whole_name()});
			Third::quickjs_ng::$JS_SetConstructor(thiz.m_constructor._context(), thiz.m_constructor._value(), thiz.m_proto._value());
			thiz.m_parent.define_object_property(
				thiz.m_name,
				Value{thiz.m_constructor}
			);
			return thiz;
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<typename CallableTraitOf<function>::Result, NativeValueHandler<Class>>)
		auto add_constructor (
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_constructor.define_object_property(
				name,
				thiz.m_constructor.new_value(NativeFunctionWrapper<function, NativeFunctionWrapperType::Constant::function()>{name})
			);
			return thiz;
		}

		#pragma endregion

		#pragma region function

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
		auto add_static_function (
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_constructor.define_object_property(
				name,
				thiz.m_constructor.new_value(NativeFunctionWrapper<function, NativeFunctionWrapperType::Constant::function()>{name})
			);
			return thiz;
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>>)
		auto add_member_function (
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_proto.define_object_property(
				name,
				thiz.m_proto.new_value(NativeFunctionWrapper<function, NativeFunctionWrapperType::Constant::method()>{name})
			);
			return thiz;
		}

		#pragma endregion

		#pragma region getter & setter

		template <auto getter, auto setter> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(getter)>)
			&& (IsGlobalFunction<decltype(setter)>)
			&& (CallableTraitOf<getter>::Argument::size == 1_szz && IsSame<AsPure<typename CallableTraitOf<getter>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>> && !IsVoid<typename CallableTraitOf<getter>::Result>)
			&& (CallableTraitOf<setter>::Argument::size == 2_szz && IsSame<AsPure<typename CallableTraitOf<setter>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>> && IsVoid<typename CallableTraitOf<setter>::Result>)
		auto add_getter_setter (
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_proto.define_object_property(
				name,
				thiz.m_proto.new_value(NativeFunctionWrapper<getter, NativeFunctionWrapperType::Constant::method()>{name}),
				thiz.m_proto.new_value(NativeFunctionWrapper<setter, NativeFunctionWrapperType::Constant::method()>{name})
			);
			return thiz;
		}

		template <auto getter> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(getter)>)
			&& (CallableTraitOf<getter>::Argument::size == 1_szz && IsSame<AsPure<typename CallableTraitOf<getter>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>> && !IsVoid<typename CallableTraitOf<getter>::Result>)
		auto add_getter (
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_proto.define_object_property(
				name,
				thiz.m_proto.new_value(NativeFunctionWrapper<getter, NativeFunctionWrapperType::Constant::method()>{name}),
				[&] {
					auto it = thiz.m_proto.new_value();
					it.set_undefined();
					return it;
				}()
			);
			return thiz;
		}

		#pragma endregion

		#pragma region by proxy

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Class, Argument ...>)
		auto add_constructor_allocate_proxy (
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_constructor<&proxy_native_function_allocate_by_handler<Class, Argument ...>>(name);
		}

		// ----------------

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
		auto add_static_function_proxy (
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_static_function<&proxy_native_function_by_handler<function>>(name);
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>, Class>)
		auto add_member_function_proxy (
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_member_function<&proxy_native_function_by_handler<function>>(name);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region native space

	class NativeSpaceBuilder {

	protected:

		Optional<String> m_parent_name;

		String m_name;

		Value m_object;

	public:

		#pragma region structor

		~NativeSpaceBuilder (
		) = default;

		// ----------------

		NativeSpaceBuilder (
		) = delete;

		NativeSpaceBuilder (
			NativeSpaceBuilder const & that
		) = delete;

		NativeSpaceBuilder (
			NativeSpaceBuilder && that
		) = default;

		// ----------------

		explicit NativeSpaceBuilder (
			Optional<String> const & parent_name,
			String const &           name,
			Value &                  parent
		) :
			m_parent_name{parent_name},
			m_name{name},
			m_object{} {
			thiz.m_object = parent.new_value();
			thiz.m_object.set_object_of_object();
			parent.define_object_property(thiz.m_name, Value{thiz.m_object});
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			NativeSpaceBuilder const & that
		) -> NativeSpaceBuilder & = delete;

		auto operator = (
			NativeSpaceBuilder && that
		) -> NativeSpaceBuilder & = default;

		#pragma endregion

		#pragma region name

		auto whole_name (
		) -> String {
			return !thiz.m_parent_name.has() ? (thiz.m_name) : ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name));
		}

		#pragma endregion

		#pragma region member

		auto add_space (
			String const & name
		) -> NativeSpaceBuilder {
			return NativeSpaceBuilder{make_optional_of(thiz.whole_name()), name, thiz.m_object};
		}

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		auto add_class (
			String const & name
		) -> NativeClassBuilder<Class> {
			return NativeClassBuilder<Class>{make_optional_of(thiz.whole_name()), name, thiz.m_object};
		}

		auto add_variable (
			String const & name,
			Value &&       value
		) -> NativeSpaceBuilder & {
			thiz.m_object.define_object_property(name, as_moveable(value));
			return thiz;
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
		auto add_function (
			String const & name
		) -> NativeSpaceBuilder & {
			return thiz.add_variable(name, thiz.m_object.new_value(NativeFunctionWrapper<function, NativeFunctionWrapperType::Constant::function()>{name}));
		}

		// ----------------

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
		auto add_function_proxy (
			String const & name
		) -> NativeSpaceBuilder & {
			return thiz.add_function<&proxy_native_function_by_handler<function>>(name);
		}

		#pragma endregion

	};

	#pragma endregion

}
