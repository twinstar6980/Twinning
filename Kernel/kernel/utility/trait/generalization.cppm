module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.generalization;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;
import twinning.kernel.utility.trait.callable;
import twinning.kernel.utility.trait.package.type_package;
import twinning.kernel.utility.trait.package.value_package;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel::Trait::Generalization {

	#pragma region utility

	template <typename ... TPackage, typename TExecutor> requires
		CategoryConstraint<IsPureInstance<TPackage ...> && IsPureInstance<TExecutor>>
		&& ((IsTypePackage<TPackage> || IsValuePackage<TPackage>) && ...)
		&& ((TPackage::size == AsSelect<1_ixz, TPackage ..., TypePackage<>>::size) && ...)
		&& (IsGenericCallable<TExecutor>)
	inline constexpr auto each(
		TExecutor const & executor
	) -> Void {
		auto iterate = [&]<auto t_element_index>(
			ValuePackage<t_element_index>
		) {
			executor(
				ValuePackage<t_element_index>{},
				[] <typename TCurrentPackage>(TCurrentPackage) {
					if constexpr (IsTypePackage<TCurrentPackage>) {
						return TypePackage<typename TCurrentPackage::template Element<t_element_index>>{};
					}
					else {
						return ValuePackage<TCurrentPackage::template element<t_element_index>>{};
					}
				}(TPackage{}) ...
			);
		};
		[&]<auto ... t_element_index>(
			ValuePackage<t_element_index ...>
		) {
				(iterate(ValuePackage<t_element_index>{}), ...);
			}(AsValuePackageOfIndex<AsSelect<1_ixz, TPackage ..., TypePackage<>>::size>{});
		return;
	}

	template <typename ... TPackage, typename TExecutor, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TPackage ...> && IsPureInstance<TExecutor> && IsValid<TArgument ...>>
		&& ((IsTypePackage<TPackage> || IsValuePackage<TPackage>) && ...)
		&& ((TPackage::size == sizeof...(TArgument)) && ...)
		&& (IsGenericCallable<TExecutor>)
	inline constexpr auto each_with(
		TExecutor const & executor,
		TArgument && ...  argument
	) -> Void {
		auto iterate = [&]<auto t_element_index, typename TCurrentArgument>(
			ValuePackage<t_element_index>,
			TCurrentArgument && current_argument
		) {
			executor(
				ValuePackage<t_element_index>{},
				[] <typename TCurrentPackage>(TCurrentPackage) {
					if constexpr (IsTypePackage<TCurrentPackage>) {
						return TypePackage<typename TCurrentPackage::template Element<t_element_index>>{};
					}
					else {
						return ValuePackage<TCurrentPackage::template element<t_element_index>>{};
					}
				}(TPackage{}) ...,
				as_forward<TCurrentArgument>(current_argument)
			);
		};
		[&]<auto ... t_element_index>(
			ValuePackage<t_element_index ...>
		) {
				(iterate(ValuePackage<t_element_index>{}, as_forward<TArgument>(argument)), ...);
			}(AsValuePackageOfIndex<sizeof...(TArgument)>{});
		return;
	}

	// ----------------

	template <typename TPackage, typename TCondition, typename TExecutor> requires
		CategoryConstraint<IsPureInstance<TPackage> && IsPureInstance<TCondition> && IsPureInstance<TExecutor>>
		&& (IsValuePackage<TPackage>)
		&& (IsGenericCallable<TExecutor>)
	inline constexpr auto match(
		TCondition const & condition,
		TExecutor const &  executor
	) -> Void {
		auto has_case = false;
		each<TPackage>(
			[&]<auto t_index, auto t_element>(ValuePackage<t_index>, ValuePackage<t_element>) {
				if (t_element == condition) {
					executor(ValuePackage<t_index>{}, ValuePackage<t_element>{});
					has_case = true;
				}
			}
		);
		assert_test(has_case);
		return;
	}

	#pragma endregion

}
