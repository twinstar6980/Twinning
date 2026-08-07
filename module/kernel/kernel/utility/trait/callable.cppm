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

	template <typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsAnything<TResult> && IsValid<TArgument ...>>
	using AsGlobalFunction = ZPointer<TResult (TArgument ...)>;

	template <typename TClass, typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TClass> && IsAnything<TResult> && IsValid<TArgument ...>>
	using AsVariableMemberFunction = ZMemberPointer<TClass, TResult (TArgument ...)>;

	template <typename TClass, typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TClass> && IsAnything<TResult> && IsValid<TArgument ...>>
	using AsConstantMemberFunction = ZMemberPointer<TClass, TResult (TArgument ...) const>;

	template <typename TClass, auto t_constant, typename TResult, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TClass> && IsAnything<TResult> && IsValid<TArgument ...>>
		&& (IsSameOf<t_constant, ZBoolean>)
	using AsMemberFunction = AsSwitch<!t_constant, AsVariableMemberFunction<TClass, TResult, TArgument ...>, AsConstantMemberFunction<TClass, TResult, TArgument ...>>;

	template <typename TClass> requires
		CategoryConstraint<IsPureInstance<TClass>>
		&& (requires { { &TClass::operator () }; })
	using AsCallableClass = TClass;

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
		&& (IsSameOf<t_constant, ZBoolean>)
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

		using Argument = CallableTrait<decltype(&TClass::operator ())>::Argument;

		using Result = CallableTrait<decltype(&TClass::operator ())>::Result;

	};

	// ----------------

	template <typename TResult, typename ... TArgument> requires
		AutomaticConstraint
	struct CallableTrait<AsGlobalFunction<TResult, TArgument ...>> :
		GlobalFunctionTrait<TResult, TArgument ...> {

	};

	template <typename TClass, typename TResult, typename ... TArgument> requires
		AutomaticConstraint
	struct CallableTrait<AsVariableMemberFunction<TClass, TResult, TArgument ...>> :
		MemberFunctionTrait<TClass, false, TResult, TArgument ...> {

	};

	template <typename TClass, typename TResult, typename ... TArgument> requires
		AutomaticConstraint
	struct CallableTrait<AsConstantMemberFunction<TClass, TResult, TArgument ...>> :
		MemberFunctionTrait<TClass, true, TResult, TArgument ...> {

	};

	template <typename TClass> requires
		AutomaticConstraint
	struct CallableTrait<AsCallableClass<TClass>> :
		CallableClassTrait<TClass> {

	};

	#pragma endregion

	#pragma region concept

	template <typename TCallable, typename ... TArgument>
	concept IsInvocable =
		CategoryConstraint<IsPureInstance<TCallable> && IsValid<TArgument ...>>
		&& (std::is_invocable_v<TCallable, TArgument ...>)
		;

	template <typename TIt>
	concept IsCallable =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires { typename CallableTrait<TIt>::Result; })
		;

	// NOTE: EXPLAIN: just a name, generic callable object could not test
	template <typename TIt>
	concept IsGenericCallable =
		CategoryConstraint<IsPureInstance<TIt>>
		;

	// ----------------

	template <typename TIt>
	concept IsGlobalFunction =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsTemplateInstanceOfTtnt<CallableTrait<TIt>, GlobalFunctionTrait>)
		;

	template <typename TIt>
	concept IsMemberFunction =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsTemplateInstanceOfTtvtnt<CallableTrait<TIt>, MemberFunctionTrait>)
		;

	template <typename TIt>
	concept IsCallableClass =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsTemplateInstanceOfTt<CallableTrait<TIt>, CallableClassTrait>)
		;

	template <typename TIt>
	concept IsFunction =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (IsGlobalFunction<TIt> || IsMemberFunction<TIt>)
		;

	#pragma endregion

	#pragma region miscellaneous

	template <auto t_it> requires
		CategoryConstraint<>
		&& (IsCallable<decltype(t_it)>)
	using CallableTraitOf = CallableTrait<decltype(t_it)>;

	#pragma endregion

	#pragma region proxy function

	namespace Detail {

		template <auto t_function, typename ... TProxyArgument> requires
			NoneConstraint
		inline auto proxy_global_function(
			TProxyArgument ... argument
		) -> CallableTraitOf<t_function>::Result {
			return t_function(as_forward<TProxyArgument>(argument) ...);
		}

		template <auto t_function, typename ... TProxyArgument, auto ... t_argument_index> requires
			NoneConstraint
		inline constexpr auto make_proxy_global_function(
			ValuePackage<t_argument_index ...>
		) -> auto {
			return &proxy_global_function<t_function, AsSwitch<!IsVoid<typename TypePackage<TProxyArgument ...>::template Element<t_argument_index>>, typename TypePackage<TProxyArgument ...>::template Element<t_argument_index>, typename CallableTraitOf<t_function>::Argument::template Element<t_argument_index>> ...>;
		}

	}

	template <auto t_function, typename ... TProxyArgument> requires
		CategoryConstraint<IsValid<TProxyArgument ...>>
		&& (IsGlobalFunction<decltype(t_function)>)
		&& (sizeof...(TProxyArgument) == CallableTraitOf<t_function>::Argument::size)
	inline constexpr auto & proxy_global_function = *Detail::make_proxy_global_function<t_function, TProxyArgument ...>(AsValuePackageOfIndex<CallableTraitOf<t_function>::Argument::size>{});

	// ----------------

	namespace Detail {

		template <auto t_function, typename TObject, auto t_constant, typename TResult, typename ... TArgument> requires
			NoneConstraint
		inline auto normalized_member_function(
			AsConstantIf<TObject, t_constant> & object,
			TArgument ...                       argument
		) -> TResult {
			return (object.*t_function)(as_forward<TArgument>(argument) ...);
		}

		template <auto t_function, typename TObject, auto ... t_argument_index> requires
			NoneConstraint
		inline constexpr auto make_normalized_member_function(
			ValuePackage<t_argument_index ...>
		) -> auto {
			return &normalized_member_function<t_function, TObject, CallableTraitOf<t_function>::constant, typename CallableTraitOf<t_function>::Result, typename CallableTraitOf<t_function>::Argument::template Element<t_argument_index> ...>;
		}

	}

	template <auto t_function, typename TClass = CallableTraitOf<t_function>::Class> requires
		CategoryConstraint<IsPureInstance<TClass>>
		&& (IsMemberFunction<decltype(t_function)>)
		&& (IsDerivedFrom<TClass, typename CallableTraitOf<t_function>::Class>)
	inline constexpr auto & normalized_member_function = *Detail::make_normalized_member_function<t_function, TClass>(AsValuePackageOfIndex<CallableTraitOf<t_function>::Argument::size>{});

	// ----------------

	namespace Detail {

		template <auto t_lambda, typename TResult, typename ... TArgument> requires
			NoneConstraint
		inline auto normalized_lambda(
			TArgument ... argument
		) -> TResult {
			return t_lambda(as_forward<TArgument>(argument) ...);
		}

		template <auto t_lambda, auto ... t_argument_index> requires
			NoneConstraint
		inline constexpr auto make_normalized_lambda(
			ValuePackage<t_argument_index ...>
		) -> auto {
			return &normalized_lambda<t_lambda, typename CallableTraitOf<t_lambda>::Result, typename CallableTraitOf<t_lambda>::Argument::template Element<t_argument_index> ...>;
		}

	}

	template <auto t_lambda> requires
		CategoryConstraint<>
		&& (IsCallableClass<decltype(t_lambda)>)
	inline constexpr auto & normalized_lambda = *Detail::make_normalized_lambda<t_lambda>(AsValuePackageOfIndex<CallableTraitOf<t_lambda>::Argument::size>{});

	#pragma endregion

}
