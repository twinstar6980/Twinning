#pragma once

#include "core/utility/trait/package/value_package.hpp"
#include "core/utility/trait/callable.hpp"

namespace TwinKleS::Core::Trait::Generalization {

	#pragma region utility

	template <typename ConditionPackage, typename Executor> requires
		CategoryConstraint<IsPureInstance<ConditionPackage> && IsPureInstance<Executor>>
		&& (IsValuePackage<ConditionPackage>)
		&& (IsGenericCallable<Executor>)
	inline auto iterate (
		Executor const & executor
	) -> Void {
		auto iterate =
			[&] <auto current_index, auto current_condition> (
			ValuePackage<current_index, current_condition>
		) {
			executor(ValuePackage<current_index, current_condition>{});
		};
		[&] <auto ...index> (
			ValuePackage<index...>
		) {
				(iterate(ValuePackage<index, ConditionPackage::template element<index>>{}), ...);
			}(AsValuePackageOfIndex<ConditionPackage::size>{});
		return;
	}

	template <typename ConditionPackage, typename Condition, typename Executor> requires
		CategoryConstraint<IsPureInstance<ConditionPackage> && IsPureInstance<Condition> && IsPureInstance<Executor>>
		&& (IsValuePackage<ConditionPackage>)
		&& (IsGenericCallable<Executor>)
	inline auto execute (
		Condition const & condition,
		Executor const &  executor
	) -> Void {
		auto state = false;
		iterate<ConditionPackage>(
			[&] <auto current_index, auto current_condition> (ValuePackage<current_index, current_condition>) {
				if (current_condition == condition) {
					executor(ValuePackage<current_index, current_condition>{});
					state = true;
				}
			}
		);
		assert_condition(state);
		return;
	}

	#pragma endregion

}
