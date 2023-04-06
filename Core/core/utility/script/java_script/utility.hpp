#pragma once

#include "core/utility/script/java_script/value.hpp"

namespace TwinStar::Core::JavaScript {

	#pragma region native function

	namespace Detail {

		template <auto function, typename ... Argument> requires
			NoneConstraint
		inline auto proxy_native_function_by_handler (
			NativeValueHandler<AsPure<Argument>> & ... argument
		) -> auto {
			using Result = typename CallableTraitOf<function>::Result;
			if constexpr (IsVoid<Result>) {
				return function(as_forward<Argument>(argument.value()) ...);
			} else if constexpr (IsInstance<Result>) {
				return NativeValueHandler<Result>::new_instance_allocate(function(as_forward<Argument>(argument.value()) ...));
			} else {
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

	template <auto t_function, auto t_forward_object> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsSameV<t_forward_object, ZBoolean>)
		&& (!t_forward_object || CallableTraitOf<t_function>::Argument::size > 0_szz)
	struct NativeFunctionWrapper {

		String name;

	};

	template <auto t_function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (IsNativeValueHandler<typename CallableTraitOf<t_function>::Result>)
	struct NativeConstructorWrapper {

		String name;

	};

	#pragma endregion

	#pragma region native class

	template <typename TClass> requires
		CategoryConstraint<IsPureInstance<TClass>>
	class NativeClassBuilder {

	public:

		using Class = TClass;

	protected:

		Optional<String> m_parent_name;
		String           m_name;
		Value            m_proto;
		Value            m_constructor;
		Value            m_parent;

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
			thiz.m_parent.context().runtime().template register_class<Class>(name);
			thiz.m_proto = thiz.m_parent.new_value();
			thiz.m_proto.set_object_of_object();
			thiz.m_parent.context().template set_class_proto<Class>(Value{thiz.m_proto});
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
			return !thiz.m_parent_name ? (thiz.m_name) : ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name));
		}

		#pragma endregion

		#pragma region constructor

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<typename CallableTraitOf<function>::Result, NativeValueHandler<Class>>)
		auto set_constructor (
		) -> NativeClassBuilder & {
			thiz.m_constructor = thiz.m_proto.new_value(NativeConstructorWrapper<function>{thiz.whole_name()});
			quickjs::JS_SetConstructor(thiz.m_constructor._context(), thiz.m_constructor._value(), thiz.m_proto._value());
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
				thiz.m_constructor.new_value(NativeFunctionWrapper<function, false>{name})
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
				thiz.m_constructor.new_value(NativeFunctionWrapper<function, false>{name})
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
				thiz.m_proto.new_value(NativeFunctionWrapper<function, true>{name})
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
				thiz.m_proto.new_value(NativeFunctionWrapper<getter, true>{name}),
				thiz.m_proto.new_value(NativeFunctionWrapper<setter, true>{name})
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
				thiz.m_proto.new_value(NativeFunctionWrapper<getter, true>{name}),
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
			&& (IsMemberFunction<decltype(function)>)
			&& (IsDerivedFrom<Class, typename CallableTraitOf<function>::Class>)
		auto add_member_function_proxy (
			String const & name
		) -> NativeClassBuilder & {
			return thiz.template add_member_function<&proxy_native_function_by_handler<&normalized_member_function<function, Class>>>(name);
		}

		template <auto function> requires
			CategoryConstraint<>
			&& (IsGlobalFunction<decltype(function)>)
			&& (IsSame<AsPure<typename CallableTraitOf<function>::Argument::template Element<1_ixz>>, Class>)
		auto add_member_function_imitate_proxy (
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
		String           m_name;
		Value            m_object;

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
			return !thiz.m_parent_name ? (thiz.m_name) : ("{}::{}"_sf(thiz.m_parent_name.get(), thiz.m_name));
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
			return thiz.add_variable(name, thiz.m_object.new_value(NativeFunctionWrapper<function, false>{name}));
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

	#pragma region exception

	class ExecutionException :
		public Exception {

	public:

		explicit ExecutionException (
			Value & exception
		) :
			Exception{"JavaScript.Execution", {}, std::source_location{}} {
			auto exception_message = exception.context().evaluate(
				R"((error) => {
					function split_error_stack(
						string,
					) {
						let list;
						if (string === undefined) {
							list = [`@ ?`];
						} else {
							list = string.split('\n').slice(0, -1).map((e) => {
								let result;
								let regexp_result = /    at (.*) \((.*)\)/.exec(e);
								if (regexp_result !== null) {
									result = `@ ${regexp_result[2] === 'native' ? ('<native>:?') : (regexp_result[2])} ${regexp_result[1]}`;
								} else {
									result = '@ ?';
								}
								return result;
							});
						}
						return list;
					}
					function parse_error_message(
						error,
					) {
						let title = '';
						let description = [];
						if (error instanceof Error) {
							if (error.name === 'NativeError') {
								title = `${error.name}`;
								description.push(...error.message.split('\n'));
							} else {
								title = `${error.name}: ${error.message}`;
							}
							description.push(...split_error_stack(error.stack));
						} else {
							title = `${error}`;
						}
						return [title, description];
					}
					let message = parse_error_message(error);
					return [message[0], ...message[1]];
				})"_sv
			).call(
				make_list<Value>(
					exception
				)
			);
			thiz.m_description.emplace_back(mss(catenate_string<String>(exception_message.to_of<List<String>>(), '\n'_c)));
		}

	};

	#pragma endregion

}
