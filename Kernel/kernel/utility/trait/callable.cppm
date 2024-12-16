module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.callable;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;
import twinning.kernel.utility.trait.template_instance;
import twinning.kernel.utility.trait.object;
import twinning.kernel.utility.trait.package.type_package;
import twinning.kernel.utility.trait.package.value_package;

export namespace Twinning::Kernel::Trait {

	#pragma region type

	template <typename Result, typename ... Argument> requires
		CategoryConstraint<IsAnything<Result> && IsValid<Argument ...>>
	using AsGlobalFunction = ZPointer<Result  (Argument ...)>;

	template <typename Class, typename Result, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Class> && IsAnything<Result> && IsValid<Argument ...>>
	using AsVMemberFunction = ZMemberPointer<Class, Result  (Argument ...)>;

	template <typename Class, typename Result, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Class> && IsAnything<Result> && IsValid<Argument ...>>
	using AsCMemberFunction = ZMemberPointer<Class, Result  (Argument ...) const>;

	template <typename Class, auto constant, typename Result, typename ... Argument> requires
		CategoryConstraint<IsPureInstance<Class> && IsAnything<Result> && IsValid<Argument ...>>
		&& (IsSameV<constant, ZBoolean>)
	using AsMemberFunction = AsSwitch<!constant, AsVMemberFunction<Class, Result, Argument ...>, AsCMemberFunction<Class, Result, Argument ...>>;

	template <typename Class> requires
		CategoryConstraint<IsPureInstance<Class>>
		&& (requires { { &Class::operator () }; })
	using AsCallableClass = Class;

	#pragma endregion

	#pragma region trait

	template <typename TCallable>
	struct CallableTrait;

	// ----------------

	template <typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsAnything<TResult> && IsValid<TArgument ...>>
	struct GlobalFunctionTrait {

		using Argument = TypePackage<TArgument ...>;

		using Result = TResult;

	};

	template <typename TClass, auto t_constant, typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TClass> && IsAnything<TResult> && IsValid<TArgument ...>>
		&& (IsSameV<t_constant, ZBoolean>)
	struct MemberFunctionTrait {

		using Class = TClass;

		inline static constexpr auto constant = ZBoolean{t_constant};

		// ----------------

		using Argument = TypePackage<TArgument ...>;

		using Result = TResult;

	};

	template <typename TClass> requires
		CategoryConstraint<IsPureInstance<TClass>>
		&& (requires { { &TClass::operator () }; })
	struct CallableClassTrait {

		using Class = TClass;

		inline static constexpr auto constant = ZBoolean{CallableTrait<decltype(&TClass::operator ())>::constant};

		// ----------------

		using Argument = typename CallableTrait<decltype(&TClass::operator ())>::Argument;

		using Result = typename CallableTrait<decltype(&TClass::operator ())>::Result;

	};

	// ----------------

	template <typename TResult, typename ... TArgument> requires
		AutoConstraint
	struct CallableTrait<AsGlobalFunction<TResult, TArgument ...>> :
		GlobalFunctionTrait<TResult, TArgument ...> {

	};

	template <typename TClass, typename TResult, typename ... TArgument> requires
		AutoConstraint
	struct CallableTrait<AsVMemberFunction<TClass, TResult, TArgument ...>> :
		MemberFunctionTrait<TClass, false, TResult, TArgument ...> {

	};

	template <typename TClass, typename TResult, typename ... TArgument> requires
		AutoConstraint
	struct CallableTrait<AsCMemberFunction<TClass, TResult, TArgument ...>> :
		MemberFunctionTrait<TClass, true, TResult, TArgument ...> {

	};

	template <typename TClass> requires
		AutoConstraint
	struct CallableTrait<AsCallableClass<TClass>> :
		CallableClassTrait<TClass> {

	};

	#pragma endregion

	#pragma region concept

	template <typename Callable, typename ... Argument>
	concept IsInvocable =
		CategoryConstraint<IsPureInstance<Callable> && IsValid<Argument ...>>
		&& (std::is_invocable_v<Callable, Argument ...>)
		;

	template <typename It>
	concept IsCallable =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires { typename CallableTrait<It>::Result; })
		;

	// NOTE : EXPLAIN - just a name, generic callable object can not test
	template <typename It>
	concept IsGenericCallable =
		CategoryConstraint<IsPureInstance<It>>
		;

	// ----------------

	template <typename It>
	concept IsGlobalFunction =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsTemplateInstanceOfTNT<CallableTrait<It>, GlobalFunctionTrait>)
		;

	template <typename It>
	concept IsMemberFunction =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsTemplateInstanceOfTVTNT<CallableTrait<It>, MemberFunctionTrait>)
		;

	template <typename It>
	concept IsCallableClass =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsTemplateInstanceOfT<CallableTrait<It>, CallableClassTrait>)
		;

	template <typename It>
	concept IsFunction =
		CategoryConstraint<IsPureInstance<It>>
		&& (IsGlobalFunction<It> || IsMemberFunction<It>)
		;

	#pragma endregion

	#pragma region miscellaneous

	template <auto it> requires
		CategoryConstraint<>
		&& (IsCallable<decltype(it)>)
	using CallableTraitOf = CallableTrait<decltype(it)>;

	#pragma endregion

	#pragma region proxy function

	namespace Detail {

		template <auto function, typename ... ProxyArgument> requires
			NoneConstraint
		inline auto proxy_global_function (
			ProxyArgument ... argument
		) -> typename CallableTraitOf<function>::Result {
			return function(as_forward<ProxyArgument>(argument) ...);
		}

		template <auto function, typename ... ProxyArgument, auto ... argument_index> requires
			NoneConstraint
		inline constexpr auto make_proxy_global_function (
			ValuePackage<argument_index ...>
		) -> auto {
			return &proxy_global_function<function, AsSwitch<!IsVoid<typename TypePackage<ProxyArgument ...>::template Element<argument_index>>, typename TypePackage<ProxyArgument ...>::template Element<argument_index>, typename CallableTraitOf<function>::Argument::template Element<argument_index>> ...>;
		}

	}

	template <auto function, typename ... ProxyArgument> requires
		CategoryConstraint<IsValid<ProxyArgument ...>>
		&& (IsGlobalFunction<decltype(function)>)
		&& (sizeof...(ProxyArgument) == CallableTraitOf<function>::Argument::size)
	inline constexpr auto & proxy_global_function = *Detail::make_proxy_global_function<function, ProxyArgument ...>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

	// ----------------

	namespace Detail {

		template <auto function, typename Object, auto constant, typename Result, typename ... Argument> requires
			NoneConstraint
		inline auto normalized_member_function (
			AsConstantIf<Object, constant> & object,
			Argument ...                     argument
		) -> Result {
			return (object.*function)(as_forward<Argument>(argument) ...);
		}

		template <auto function, typename Object, auto ... argument_index> requires
			NoneConstraint
		inline constexpr auto make_normalized_member_function (
			ValuePackage<argument_index ...>
		) -> auto {
			return &normalized_member_function<function, Object, CallableTraitOf<function>::constant, typename CallableTraitOf<function>::Result, typename CallableTraitOf<function>::Argument::template Element<argument_index> ...>;
		}

	}

	template <auto function, typename Class = typename CallableTraitOf<function>::Class> requires
		CategoryConstraint<IsPureInstance<Class>>
		&& (IsMemberFunction<decltype(function)>)
		&& (IsDerivedFrom<Class, typename CallableTraitOf<function>::Class>)
	inline constexpr auto & normalized_member_function = *Detail::make_normalized_member_function<function, Class>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

	// ----------------

	namespace Detail {

		template <auto lambda, typename Result, typename ... Argument> requires
			NoneConstraint
		inline auto normalized_lambda (
			Argument ... argument
		) -> Result {
			return lambda(as_forward<Argument>(argument) ...);
		}

		template <auto lambda, auto ... argument_index> requires
			NoneConstraint
		inline constexpr auto make_normalized_lambda (
			ValuePackage<argument_index ...>
		) -> auto {
			return &normalized_lambda<lambda, typename CallableTraitOf<lambda>::Result, typename CallableTraitOf<lambda>::Argument::template Element<argument_index> ...>;
		}

	}

	template <auto lambda> requires
		CategoryConstraint<>
		&& (IsCallableClass<decltype(lambda)>)
	inline constexpr auto & normalized_lambda = *Detail::make_normalized_lambda<lambda>(AsValuePackageOfIndex<CallableTraitOf<lambda>::Argument::size>{});

	#pragma endregion

}
