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
import twinning.kernel.utility.support.std;
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

		#pragma region constructor

		explicit NativeValueHandler(
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
			return;
		}

		#pragma endregion

	public:

		#pragma region constructor

		~NativeValueHandler(
		) = default;

		// ----------------

		NativeValueHandler(
		) :
			m_value{} {
			return;
		}

		NativeValueHandler(
			NativeValueHandler const & that
		) = default;

		NativeValueHandler(
			NativeValueHandler && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator =(
			NativeValueHandler const & that
		) -> NativeValueHandler & = default;

		auto operator =(
			NativeValueHandler && that
		) -> NativeValueHandler & = default;

		#pragma endregion

		#pragma region value

		auto value(
		) -> Value & {
			assert_test(!thiz.m_value.template is<Null>());
			if (thiz.m_value.template is<Pointer<Value>>()) {
				return *thiz.m_value.template get<Pointer<Value>>();
			}
			else if (thiz.m_value.template is<std::shared_ptr<Value>>()) {
				return *thiz.m_value.template get<std::shared_ptr<Value>>();
			}
			else {
				throw UnreachableException{};
			}
		}

		#pragma endregion

	public:

		#pragma region create

		inline static auto new_instance(
			Value & value
		) -> NativeValueHandler {
			return NativeValueHandler{make_pointer_of(value), k_true};
		}

		inline static auto new_reference(
			Value & value
		) -> NativeValueHandler {
			return NativeValueHandler{make_pointer_of(value), k_false};
		}

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Value, TArgument && ...>)
		inline static auto new_instance_allocate(
			TArgument && ... argument
		) -> NativeValueHandler {
			return new_instance(*allocate_instance<Value>(as_forward<TArgument>(argument) ...));
		}

		#pragma endregion

	};

	// ----------------

	template <typename TIt>
	concept IsNativeValueHandler = IsTemplateInstanceOfTt<TIt, NativeValueHandler>;

	#pragma endregion

	#pragma region native function

	namespace Detail {

		template <auto t_function, typename ... TArgument> requires
			NoneConstraint
		inline auto call_native_function_wrapper_inner(
			TArgument && ... argument
		) -> CallableTraitOf<t_function>::Result {
			auto unlocker = Thread::Unlocker{g_mutex};
			return t_function(as_forward<TArgument>(argument) ...);
		}

		template <auto t_function, auto t_forward_object> requires
			NoneConstraint
		inline auto call_native_function_wrapper(
			Context &     context,
			Value &       object,
			List<Value> & argument
		) -> CallableTraitOf<t_function>::Result {
			return [&] <auto ... t_index>(ValuePackage<t_index ...>) -> CallableTraitOf<t_function>::Result {
				if constexpr (!t_forward_object) {
					using Argument = CallableTraitOf<t_function>::Argument;
					return call_native_function_wrapper_inner<t_function>(
						as_forward<typename Argument::template Element<t_index>>(argument[mbox<Size>(t_index)].template to_of<AsPure<typename Argument::template Element<t_index>>>()) ...
					);
				}
				else {
					using Class = AsPure<typename CallableTraitOf<t_function>::Argument::template Element<1_ixz>>;
					using Argument = AsTypePackageRemoveHead<typename CallableTraitOf<t_function>::Argument, 1_szz>;
					return call_native_function_wrapper_inner<t_function>(
						as_left(object.to_of<Class>()),
						as_forward<typename Argument::template Element<t_index>>(argument[mbox<Size>(t_index)].template to_of<AsPure<typename Argument::template Element<t_index>>>()) ...
					);
				}
			}(AsValuePackageOfIndex<CallableTraitOf<t_function>::Argument::size - (!t_forward_object ? (0) : (1))>{});
		}

		template <auto t_function, auto t_forward_object> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
			&& (IsSameOf<t_forward_object, Boolean>)
		inline auto proxy_native_function_wrapper(
			Context &     context,
			Value &       object,
			List<Value> & argument,
			Value &       result
		) -> Void {
			if constexpr (IsVoid<typename CallableTraitOf<t_function>::Result>) {
				call_native_function_wrapper<t_function, t_forward_object>(context, object, argument);
				result.set_undefined();
			}
			else {
				result.from(call_native_function_wrapper<t_function, t_forward_object>(context, object, argument));
			}
			return;
		}

		template <auto t_function, auto t_forward_object> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
			&& (IsSameOf<t_forward_object, Boolean>)
		inline constexpr auto make_proxy_native_function_wrapper(
		) -> auto {
			return &proxy_native_function_wrapper<t_function, t_forward_object>;
		}

	}

	template <auto t_function, auto t_forward_object> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsSameOf<t_forward_object, Boolean>)
	inline constexpr auto & proxy_native_function_wrapper = *Detail::make_proxy_native_function_wrapper<t_function, t_forward_object>();

	// ----------------

	namespace Detail {

		template <auto t_function, typename ... TArgument> requires
			NoneConstraint
		inline auto proxy_native_function_by_handler(
			NativeValueHandler<AsPure<TArgument>> & ... argument
		) -> auto {
			using Result = CallableTraitOf<t_function>::Result;
			if constexpr (IsVoid<Result>) {
				return t_function(as_forward<TArgument>(argument.value()) ...);
			}
			else if constexpr (IsInstance<Result>) {
				return NativeValueHandler<Result>::new_instance_allocate(t_function(as_forward<TArgument>(argument.value()) ...));
			}
			else {
				return NativeValueHandler<AsPure<Result>>::new_reference(t_function(as_forward<TArgument>(argument.value()) ...));
			}
		}

		template <auto t_function, auto ... t_argument_index> requires
			NoneConstraint
		inline constexpr auto make_proxy_native_function_by_handler(
			ValuePackage<t_argument_index ...>
		) -> auto {
			return &proxy_native_function_by_handler<t_function, typename CallableTraitOf<t_function>::Argument::template Element<t_argument_index> ...>;
		}

	}

	template <auto t_function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsVoid<typename CallableTraitOf<t_function>::Result> || IsInstance<typename CallableTraitOf<t_function>::Result> || IsVariableReference<typename CallableTraitOf<t_function>::Result>)
	inline constexpr auto & proxy_native_function_by_handler = *Detail::make_proxy_native_function_by_handler<t_function>(AsValuePackageOfIndex<CallableTraitOf<t_function>::Argument::size>{});

	// ----------------

	namespace Detail {

		template <typename TType, typename ... TArgument> requires
			NoneConstraint
		inline constexpr auto proxy_native_function_allocate_by_handler(
			NativeValueHandler<AsPure<TArgument>> & ... argument
		) -> NativeValueHandler<TType> {
			return NativeValueHandler<TType>::new_instance_allocate(as_forward<TArgument>(argument.value()) ...);
		}

		template <typename TType, typename ... TArgument> requires
			NoneConstraint
		inline constexpr auto make_proxy_native_function_allocate_by_handler(
		) -> auto {
			return &proxy_native_function_allocate_by_handler<TType, TArgument ...>;
		}

	}

	template <typename TType, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TType> && IsValid<TArgument ...>>
		&& (IsConstructible<TType, TArgument && ...>)
	inline constexpr auto & proxy_native_function_allocate_by_handler = *Detail::make_proxy_native_function_allocate_by_handler<TType, TArgument ...>();

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
		&& (IsSameOf<t_type, NativeFunctionWrapperType>)
		&& ((t_type == NativeFunctionWrapperType::Constant::function()) ||
			(t_type == NativeFunctionWrapperType::Constant::method() && CallableTraitOf<t_function>::Argument::size > 0_szz) ||
			(t_type == NativeFunctionWrapperType::Constant::constructor() && IsValid<typename CallableTraitOf<t_function>::Result>))
	struct NativeFunctionWrapper {

		String name{};

	};

	#pragma endregion

	#pragma region native class

	namespace Detail {

		inline constexpr auto k_invalid_native_class_identifier = Integer{0};

		template <typename TClass> requires
			CategoryConstraint<IsPureInstance<TClass>>
		inline auto g_native_class_identifier = Integer{0};

		// ----------------

		template <typename TClass> requires
			CategoryConstraint<IsPureInstance<TClass>>
		inline auto free_native_class(
			Runtime & rt,
			Value &   obj
		) -> Void {
			delete static_cast<NativeValueHandler<TClass> *>(Third::quickjs_ng::$JS_GetOpaque(obj._value(), static_cast<Third::quickjs_ng::$JSClassID>(g_native_class_identifier<TClass>.value)));
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

		#pragma region constructor

		~NativeClassBuilder(
		) = default;

		// ----------------

		NativeClassBuilder(
		) = delete;

		NativeClassBuilder(
			NativeClassBuilder const & that
		) = delete;

		NativeClassBuilder(
			NativeClassBuilder && that
		) = default;

		// ----------------

		explicit NativeClassBuilder(
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
			thiz.m_parent.context().runtime().template register_class<&Detail::free_native_class<Class>>(Detail::g_native_class_identifier<Class>, name);
			thiz.m_parent.context().set_class_proto(Detail::g_native_class_identifier<Class>, Value{thiz.m_proto});
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			NativeClassBuilder const & that
		) -> NativeClassBuilder & = delete;

		auto operator =(
			NativeClassBuilder && that
		) -> NativeClassBuilder & = default;

		#pragma endregion

		#pragma region name

		auto whole_name(
		) -> String {
			return !thiz.m_parent_name.has() ? (thiz.m_name) : ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name));
		}

		#pragma endregion

		#pragma region constructor

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
			&& (IsSame<typename CallableTraitOf<t_function>::Result, NativeValueHandler<Class>>)
		auto set_constructor(
		) -> NativeClassBuilder & {
			thiz.m_constructor = thiz.m_proto.new_value(NativeFunctionWrapper<t_function, NativeFunctionWrapperType::Constant::constructor()>{thiz.whole_name()});
			Third::quickjs_ng::$JS_SetConstructor(thiz.m_constructor._context(), thiz.m_constructor._value(), thiz.m_proto._value());
			thiz.m_parent.define_object_property(
				thiz.m_name,
				Value{thiz.m_constructor}
			);
			return thiz;
		}

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
			&& (IsSame<typename CallableTraitOf<t_function>::Result, NativeValueHandler<Class>>)
		auto add_constructor(
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_constructor.define_object_property(
				name,
				thiz.m_constructor.new_value(NativeFunctionWrapper<t_function, NativeFunctionWrapperType::Constant::function()>{name})
			);
			return thiz;
		}

		#pragma endregion

		#pragma region function

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
		auto add_static_function(
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_constructor.define_object_property(
				name,
				thiz.m_constructor.new_value(NativeFunctionWrapper<t_function, NativeFunctionWrapperType::Constant::function()>{name})
			);
			return thiz;
		}

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<t_function>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>>)
		auto add_member_function(
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_proto.define_object_property(
				name,
				thiz.m_proto.new_value(NativeFunctionWrapper<t_function, NativeFunctionWrapperType::Constant::method()>{name})
			);
			return thiz;
		}

		#pragma endregion

		#pragma region getter & setter

		template <auto t_getter, auto t_setter> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_getter)>)
			&& (IsGlobalFunction<decltype(t_setter)>)
			&& (CallableTraitOf<t_getter>::Argument::size == 1_szz && IsSame<AsPure<typename CallableTraitOf<t_getter>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>> && !IsVoid<typename CallableTraitOf<t_getter>::Result>)
			&& (CallableTraitOf<t_setter>::Argument::size == 2_szz && IsSame<AsPure<typename CallableTraitOf<t_setter>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>> && IsVoid<typename CallableTraitOf<t_setter>::Result>)
		auto add_getter_setter(
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_proto.define_object_property(
				name,
				thiz.m_proto.new_value(NativeFunctionWrapper<t_getter, NativeFunctionWrapperType::Constant::method()>{name}),
				thiz.m_proto.new_value(NativeFunctionWrapper<t_setter, NativeFunctionWrapperType::Constant::method()>{name})
			);
			return thiz;
		}

		template <auto t_getter> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_getter)>)
			&& (CallableTraitOf<t_getter>::Argument::size == 1_szz && IsSame<AsPure<typename CallableTraitOf<t_getter>::Argument::template Element<1_ixz>>, NativeValueHandler<Class>> && !IsVoid<typename CallableTraitOf<t_getter>::Result>)
		auto add_getter(
			String const & name
		) -> NativeClassBuilder & {
			thiz.m_proto.define_object_property(
				name,
				thiz.m_proto.new_value(NativeFunctionWrapper<t_getter, NativeFunctionWrapperType::Constant::method()>{name}),
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

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsConstructible<Class, TArgument ...>)
		auto add_constructor_allocate_proxy(
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_constructor<&proxy_native_function_allocate_by_handler<Class, TArgument ...>>(name);
		}

		// ----------------

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
		auto add_static_function_proxy(
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_static_function<&proxy_native_function_by_handler<t_function>>(name);
		}

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<t_function>::Argument::template Element<1_ixz>>, Class>)
		auto add_member_function_proxy(
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_member_function<&proxy_native_function_by_handler<t_function>>(name);
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

		#pragma region constructor

		~NativeSpaceBuilder(
		) = default;

		// ----------------

		NativeSpaceBuilder(
		) = delete;

		NativeSpaceBuilder(
			NativeSpaceBuilder const & that
		) = delete;

		NativeSpaceBuilder(
			NativeSpaceBuilder && that
		) = default;

		// ----------------

		explicit NativeSpaceBuilder(
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
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			NativeSpaceBuilder const & that
		) -> NativeSpaceBuilder & = delete;

		auto operator =(
			NativeSpaceBuilder && that
		) -> NativeSpaceBuilder & = default;

		#pragma endregion

		#pragma region name

		auto whole_name(
		) -> String {
			return !thiz.m_parent_name.has() ? (thiz.m_name) : ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name));
		}

		#pragma endregion

		#pragma region member

		auto add_space(
			String const & name
		) -> NativeSpaceBuilder {
			return NativeSpaceBuilder{make_optional_of(thiz.whole_name()), name, thiz.m_object};
		}

		template <typename TClass> requires
			CategoryConstraint<IsPureInstance<TClass>>
		auto add_class(
			String const & name
		) -> NativeClassBuilder<TClass> {
			return NativeClassBuilder<TClass>{make_optional_of(thiz.whole_name()), name, thiz.m_object};
		}

		auto add_variable(
			String const & name,
			Value &&       value
		) -> NativeSpaceBuilder & {
			thiz.m_object.define_object_property(name, as_moveable(value));
			return thiz;
		}

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
		auto add_function(
			String const & name
		) -> NativeSpaceBuilder & {
			return thiz.add_variable(name, thiz.m_object.new_value(NativeFunctionWrapper<t_function, NativeFunctionWrapperType::Constant::function()>{name}));
		}

		// ----------------

		template <auto t_function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(t_function)>)
		auto add_function_proxy(
			String const & name
		) -> NativeSpaceBuilder & {
			return thiz.add_function<&proxy_native_function_by_handler<t_function>>(name);
		}

		#pragma endregion

	};

	#pragma endregion

}
