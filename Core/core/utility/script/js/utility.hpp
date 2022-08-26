#pragma once

#include "core/utility/script/js/js.hpp"

namespace TwinKleS::Core::JS {

	#pragma region proxy function by handler

	namespace Detail {

		template <auto function, typename ...Argument> requires
			NoneConstraint
		inline auto proxy_function_by_handler (
			Handler<AsPure<Argument>> & ...argument
		) -> auto {
			using Result = typename CallableTraitOf<function>::Result;
			if constexpr (IsVoid<Result>) {
				return function(as_forward<Argument>(argument.value())...);
			} else if constexpr (IsInstance<Result>) {
				return Handler<Result>::new_instance_allocate(function(as_forward<Argument>(argument.value())...));
			} else {
				return Handler<AsPure<Result>>::new_reference(function(as_forward<Argument>(argument.value())...));
			}
		}

		template <auto function, auto ...argument_index> requires
			NoneConstraint
		inline constexpr auto make_proxy_function_by_handler (
			ValuePackage<argument_index...>
		) -> auto {
			return &proxy_function_by_handler<function, typename CallableTraitOf<function>::Argument::template Element<argument_index>...>;
		}

	}

	// ----------------

	template <auto function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
		&& (IsVoid<typename CallableTraitOf<function>::Result> || IsInstance<typename CallableTraitOf<function>::Result> || IsVReference<typename CallableTraitOf<function>::Result>)
	inline constexpr auto & proxy_function_by_handler = *Detail::make_proxy_function_by_handler<function>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

	#pragma endregion

	#pragma region proxy allocate handler

	namespace Detail {

		template <typename Type, typename ...Argument> requires
			NoneConstraint
		inline constexpr auto proxy_allocate_handler (
			Handler<AsPure<Argument>> & ...argument
		) -> Handler<Type> {
			return Handler<Type>::new_instance_allocate(as_forward<Argument>(argument.value())...);
		}

		template <typename Type, typename ...Argument> requires
			NoneConstraint
		inline constexpr auto make_proxy_allocate_handler (
		) -> auto {
			return &proxy_allocate_handler<Type, Argument...>;
		}

	}

	// ----------------

	template <typename Type, typename ...Argument> requires
		CategoryConstraint<IsPureInstance<Type> && IsValid<Argument...>>
		&& (IsConstructible<Type, Argument &&...>)
	inline constexpr auto & proxy_allocate_handler = *Detail::make_proxy_allocate_handler<Type, Argument...>();

	#pragma endregion

	#pragma region function wrapper

	template <auto t_function, auto t_forward_object> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsSameV<t_forward_object, ZBoolean>)
		&& (!t_forward_object || CallableTraitOf<t_function>::Argument::size > 0_szz)
	struct FunctionWrapper {

		String name;

	};

	template <auto t_function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsHandler<typename CallableTraitOf<t_function>::Result>)
	struct ConstructorWrapper {

		String  name;
		Boolean is_generic;

	};

	#pragma endregion

	#pragma region class & namespace builder

	template <typename TClass> requires
		CategoryConstraint<IsPureInstance<TClass>>
	class ClassBuilder {

	public: //

		using Class = TClass;

	protected: //

		Context          m_context;
		Value            m_parent;
		Optional<String> m_parent_name;
		String           m_name;
		Value            m_prototype;
		Value            m_constructor;

	public: //

		#pragma region structor

		~ClassBuilder (
		) = default;

		// ----------------

		ClassBuilder (
		) = delete;

		ClassBuilder (
			ClassBuilder const & that
		) = delete;

		ClassBuilder (
			ClassBuilder && that
		) = default;

		// ----------------

		explicit ClassBuilder (
			Context &                context,
			Value &                  parent,
			Optional<String> const & parent_name,
			String const &           name
		) :
			m_context{Context::new_reference(context._context())},
			m_parent{Value::new_reference(parent._context(), parent._value())},
			m_parent_name{parent_name},
			m_name{name},
			m_prototype{},
			m_constructor{} {
			thiz.m_context.runtime().template register_class<Class>(name);
			thiz.m_prototype = thiz.m_context.new_value();
			thiz.m_prototype.set_object();
			thiz.m_context.template set_class_prototype<Class>(Value{thiz.m_prototype});
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			ClassBuilder const & that
		) -> ClassBuilder& = delete;

		auto operator = (
			ClassBuilder && that
		) -> ClassBuilder& = default;

		#pragma endregion

		#pragma region whole name

		auto whole_name (
		) -> String {
			return thiz.m_parent_name ? ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name)) : (thiz.m_name);
		}

		#pragma endregion

		#pragma region constructor

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<typename CallableTraitOf<function>::Result, Handler<Class>>)
		auto set_constructor (
		) -> ClassBuilder& {
			thiz.m_constructor = thiz.m_context.new_value(ConstructorWrapper<function>{thiz.whole_name(), k_false});
			quickjs::JS_SetConstructor(thiz.m_context._context(), thiz.m_constructor._value(), thiz.m_prototype._value());
			thiz.m_parent.define_object_property_value(
				thiz.m_name,
				Value::new_reference(thiz.m_context._context(), thiz.m_constructor._value())
			);
			return thiz;
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<typename CallableTraitOf<function>::Result, Handler<Class>>)
		auto add_second_constructor (
			String const & name
		) -> ClassBuilder& {
			thiz.m_constructor.define_object_property_value(
				name,
				thiz.m_context.new_value(ConstructorWrapper<function>{thiz.whole_name(), k_true})
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
		) -> ClassBuilder& {
			thiz.m_constructor.define_object_property_value(
				name,
				thiz.m_context.new_value(FunctionWrapper<function, false>{name})
			);
			return thiz;
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>, Handler<Class>>)
		auto add_member_function (
			String const & name
		) -> ClassBuilder& {
			thiz.m_prototype.define_object_property_value(
				name,
				thiz.m_context.new_value(FunctionWrapper<function, true>{name})
			);
			return thiz;
		}

		#pragma endregion

		#pragma region getter & setter

		template <auto getter, auto setter> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(getter)>)
			&& (IsGlobalFunction<decltype(setter)>)
			&& (CallableTraitOf<getter>::Argument::size == 1_szz && IsSame<AsPure<typename CallableTraitOf<getter>::Argument::template Element<1_ixz>>, Handler<Class>> && !IsVoid<typename CallableTraitOf<getter>::Result>)
			&& (CallableTraitOf<setter>::Argument::size == 2_szz && IsSame<AsPure<typename CallableTraitOf<setter>::Argument::template Element<1_ixz>>, Handler<Class>> && IsVoid<typename CallableTraitOf<setter>::Result>)
		auto add_getter_setter (
			String const & name
		) -> ClassBuilder& {
			thiz.m_prototype.define_object_property_getter_setter(
				name,
				thiz.m_context.new_value(FunctionWrapper<getter, true>{name}),
				thiz.m_context.new_value(FunctionWrapper<setter, true>{name})
			);
			return thiz;
		}

		template <auto getter> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(getter)>)
			&& (CallableTraitOf<getter>::Argument::size == 1_szz && IsSame<AsPure<typename CallableTraitOf<getter>::Argument::template Element<1_ixz>>, Handler<Class>> && !IsVoid<typename CallableTraitOf<getter>::Result>)
		auto add_getter (
			String const & name
		) -> ClassBuilder& {
			thiz.m_prototype.define_object_property_getter_setter(
				name,
				thiz.m_context.new_value(FunctionWrapper<getter, true>{name}),
				[&] {
					auto it = thiz.m_context.new_value();
					it.set_undefined();
					return it;
				}()
			);
			return thiz;
		}

		#pragma endregion

		#pragma region by proxy

		template <typename ...Argument> requires
			CategoryConstraint<IsValid<Argument...>>
			&& (IsConstructible<Class, Argument...>)
		auto set_constructor_allocate_proxy (
		) -> ClassBuilder& {
			return thiz.template set_constructor<&proxy_allocate_handler<Class, Argument...>>();
		}

		template <typename ...Argument> requires
			CategoryConstraint<IsValid<Argument...>>
			&& (IsConstructible<Class, Argument...>)
		auto add_second_constructor_allocate_proxy (
			String const & name
		) -> ClassBuilder& {
			return thiz.template add_second_constructor<&proxy_allocate_handler<Class, Argument...>>(name);
		}

		// ----------------

		template <auto function> requires
			CategoryConstraint<>
			&& (IsMemberFunction<decltype(function)>)
			&& (IsDerivedFrom<Class, typename CallableTraitOf<function>::Class>)
		auto add_member_function_proxy (
			String const & name
		) -> ClassBuilder& {
			return thiz.template add_member_function<&proxy_function_by_handler<&normalized_member_function<function, Class>>>(name);
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>, Class>)
		auto add_member_function_imitate_proxy (
			String const & name
		) -> ClassBuilder& {
			return thiz.template add_member_function<&proxy_function_by_handler<function>>(name);
		}

		#pragma endregion

	};

	class NamespaceBuilder {

	protected: //

		Context          m_context;
		Optional<String> m_parent_name;
		String           m_name;
		Value            m_object;

	public: //

		#pragma region structor

		~NamespaceBuilder (
		) = default;

		// ----------------

		NamespaceBuilder (
		) = delete;

		NamespaceBuilder (
			NamespaceBuilder const & that
		) = delete;

		NamespaceBuilder (
			NamespaceBuilder && that
		) = default;

		// ----------------

		explicit NamespaceBuilder (
			Context &                context,
			Value &                  parent,
			Optional<String> const & parent_name,
			String const &           name
		) :
			m_context{Context::new_reference(context._context())},
			m_parent_name{parent_name},
			m_name{name},
			m_object{} {
			thiz.m_object = thiz.m_context.new_value();
			thiz.m_object.set_object();
			parent.define_object_property_value(thiz.m_name, Value::new_reference(thiz.m_object._context(), thiz.m_object._value()));
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			NamespaceBuilder const & that
		) -> NamespaceBuilder& = delete;

		auto operator = (
			NamespaceBuilder && that
		) -> NamespaceBuilder& = default;

		#pragma endregion

		#pragma region whole name

		auto whole_name (
		) -> String {
			return thiz.m_parent_name ? ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name)) : (thiz.m_name);
		}

		#pragma endregion

		#pragma region member

		auto add_namespace (
			String const & name
		) -> NamespaceBuilder {
			return NamespaceBuilder{thiz.m_context, thiz.m_object, make_optional_of(thiz.whole_name()), name};
		}

		template <typename Class> requires
			CategoryConstraint<IsPureInstance<Class>>
		auto add_class (
			String const & name
		) -> ClassBuilder<Class> {
			return ClassBuilder<Class>{thiz.m_context, thiz.m_object, make_optional_of(thiz.whole_name()), name};
		}

		auto add_variable (
			String const & name,
			Value &&       value
		) -> NamespaceBuilder& {
			thiz.m_object.define_object_property_value(name, as_moveable(value));
			return thiz;
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
		auto add_function (
			String const & name
		) -> NamespaceBuilder& {
			return thiz.add_variable(name, thiz.m_context.new_value(FunctionWrapper<function, false>{name}));
		}

		// ----------------

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
		auto add_function_proxy (
			String const & name
		) -> NamespaceBuilder& {
			return thiz.add_function<&proxy_function_by_handler<function>>(name);
		}

		#pragma endregion

	};

	#pragma endregion

}
