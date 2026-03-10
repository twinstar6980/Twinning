module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.object;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename TDerived, typename TBase>
	concept IsDerivedFrom =
		CategoryConstraint<IsPureInstance<TDerived> && IsPureInstance<TBase>>
		&& (std::is_base_of_v<TBase, TDerived> && std::is_convertible_v<TDerived &, TBase &>)
		;

	// ----------------

	template <typename TIt, typename ... TArgument>
	concept IsConstructible =
		CategoryConstraint<IsPureInstance<TIt> && IsValid<TArgument ...>>
		&& (std::is_constructible_v<TIt, TArgument ...>)
		;

	template <typename TIt>
	concept IsDefaultConstructible =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_default_constructible_v<TIt>)
		;

	template <typename TIt>
	concept IsCopyConstructible =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_copy_constructible_v<TIt>)
		;

	template <typename TIt>
	concept IsMoveConstructible =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_move_constructible_v<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsDestructible =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_destructible_v<TIt>)
		;

	// ----------------

	template <typename TTo, typename TFrom>
	concept IsAssignable =
		CategoryConstraint<IsValid<TTo> && IsValid<TFrom>>
		&& (std::is_assignable_v<TTo, TFrom>)
		;

	template <typename TIt>
	concept IsCopyAssignable =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_copy_assignable_v<TIt>)
		;

	template <typename TIt>
	concept IsMoveAssignable =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_move_assignable_v<TIt>)
		;

	// ----------------

	template <typename TFrom, typename TTo>
	concept IsConvertible =
		CategoryConstraint<IsValid<TFrom> && IsValid<TTo>>
		&& (std::is_convertible_v<TFrom, TTo>)
		;

	#pragma endregion

	#pragma region utility

	template <typename TBase, typename TDerived> requires
		CategoryConstraint<IsPureInstance<TBase> && IsPureInstance<TDerived>>
		&& (IsDerivedFrom<TDerived, TBase>)
	inline constexpr auto up_cast(
		TDerived & it
	) -> TBase & {
		return static_cast<TBase &>(it);
	}

	template <typename TBase, typename TDerived> requires
		CategoryConstraint<IsPureInstance<TBase> && IsPureInstance<TDerived>>
		&& (IsDerivedFrom<TDerived, TBase>)
	inline constexpr auto up_cast(
		TDerived const & it
	) -> TBase const & {
		return static_cast<TBase const &>(it);
	}

	template <typename TBase, typename TDerived> requires
		CategoryConstraint<IsPureInstance<TBase> && IsPureInstance<TDerived>>
		&& (IsDerivedFrom<TDerived, TBase>)
	inline constexpr auto up_cast(
		TDerived && it
	) -> TBase && {
		return static_cast<TBase &&>(it);
	}

	// ----------------

	template <typename TDerived, typename TBase> requires
		CategoryConstraint<IsPureInstance<TDerived> && IsPureInstance<TBase>>
		&& (IsDerivedFrom<TDerived, TBase>)
	inline constexpr auto down_cast(
		TBase & it
	) -> TDerived & {
		return static_cast<TDerived &>(it);
	}

	template <typename TDerived, typename TBase> requires
		CategoryConstraint<IsPureInstance<TDerived> && IsPureInstance<TBase>>
		&& (IsDerivedFrom<TDerived, TBase>)
	inline constexpr auto down_cast(
		TBase const & it
	) -> TDerived const & {
		return static_cast<TDerived const &>(it);
	}

	template <typename TDerived, typename TBase> requires
		CategoryConstraint<IsPureInstance<TDerived> && IsPureInstance<TBase>>
		&& (IsDerivedFrom<TDerived, TBase>)
	inline constexpr auto down_cast(
		TBase && it
	) -> TDerived && {
		return static_cast<TDerived &&>(it);
	}

	// ----------------

	// TODO: maybe bug, use assignment instead of constructor if in consteval context
	template <typename TIt, typename ... TArgument> requires
		CategoryConstraint<IsInstance<TIt> && IsValid<TArgument ...>>
		&& (IsConstructible<AsPure<TIt>, TArgument && ...>)
	inline constexpr auto construct(
		TIt &            it,
		TArgument && ... argument
	) -> Void {
		if consteval {
			it = AsUnmakeConstant<TIt>{as_forward<TArgument>(argument) ...};
		}
		else {
			new(&as_variable(it)) AsUnmakeConstant<TIt>{as_forward<TArgument>(argument) ...};
		}
		return;
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
		&& (IsDestructible<AsPure<TIt>>)
	inline constexpr auto destruct(
		TIt & it
	) -> Void {
		if consteval {
		}
		else {
			it.~TIt();
		}
		return;
	}

	template <typename TIt, typename ... TArgument> requires
		CategoryConstraint<IsInstance<TIt> && IsValid<TArgument ...>>
		&& (IsConstructible<AsPure<TIt>, TArgument && ...>)
		&& (IsDestructible<AsPure<TIt>>)
	inline constexpr auto restruct(
		TIt &            it,
		TArgument && ... argument
	) -> Void {
		destruct(it);
		construct(it, as_forward<TArgument>(argument) ...);
		return;
	}

	#pragma endregion

}
