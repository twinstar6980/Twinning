module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.object;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename Derived, typename Base>
	concept IsDerivedFrom =
		CategoryConstraint<IsPureInstance<Derived> && IsPureInstance<Base>>
		&& (std::is_base_of_v<Base, Derived> && std::is_convertible_v<Derived &, Base &>)
		;

	// ----------------

	template <typename It, typename ... Argument>
	concept IsConstructible =
		CategoryConstraint<IsPureInstance<It> && IsValid<Argument ...>>
		&& (std::is_constructible_v<It, Argument ...>)
		;

	template <typename It>
	concept IsDefaultConstructible =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_default_constructible_v<It>)
		;

	template <typename It>
	concept IsCopyConstructible =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_copy_constructible_v<It>)
		;

	template <typename It>
	concept IsMoveConstructible =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_move_constructible_v<It>)
		;

	// ----------------

	template <typename It>
	concept IsDestructible =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_destructible_v<It>)
		;

	// ----------------

	template <typename To, typename From>
	concept IsAssignable =
		CategoryConstraint<IsValid<To> && IsValid<From>>
		&& (std::is_assignable_v<To, From>)
		;

	template <typename It>
	concept IsCopyAssignable =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_copy_assignable_v<It>)
		;

	template <typename It>
	concept IsMoveAssignable =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_move_assignable_v<It>)
		;

	// ----------------

	template <typename From, typename To>
	concept IsConvertible =
		CategoryConstraint<IsValid<From> && IsValid<To>>
		&& (std::is_convertible_v<From, To>)
		;

	#pragma endregion

	#pragma region utility

	template <typename Base, typename Derived> requires
		CategoryConstraint<IsPureInstance<Base> && IsPureInstance<Derived>>
		&& (IsDerivedFrom<Derived, Base>)
	inline constexpr auto up_cast (
		Derived & it
	) -> Base & {
		return static_cast<Base &>(it);
	}

	template <typename Base, typename Derived> requires
		CategoryConstraint<IsPureInstance<Base> && IsPureInstance<Derived>>
		&& (IsDerivedFrom<Derived, Base>)
	inline constexpr auto up_cast (
		Derived const & it
	) -> Base const & {
		return static_cast<Base const &>(it);
	}

	template <typename Base, typename Derived> requires
		CategoryConstraint<IsPureInstance<Base> && IsPureInstance<Derived>>
		&& (IsDerivedFrom<Derived, Base>)
	inline constexpr auto up_cast (
		Derived && it
	) -> Base && {
		return static_cast<Base &&>(it);
	}

	// ----------------

	template <typename Derived, typename Base> requires
		CategoryConstraint<IsPureInstance<Derived> && IsPureInstance<Base>>
		&& (IsDerivedFrom<Derived, Base>)
	inline constexpr auto down_cast (
		Base & it
	) -> Derived & {
		return static_cast<Derived &>(it);
	}

	template <typename Derived, typename Base> requires
		CategoryConstraint<IsPureInstance<Derived> && IsPureInstance<Base>>
		&& (IsDerivedFrom<Derived, Base>)
	inline constexpr auto down_cast (
		Base const & it
	) -> Derived const & {
		return static_cast<Derived const &>(it);
	}

	template <typename Derived, typename Base> requires
		CategoryConstraint<IsPureInstance<Derived> && IsPureInstance<Base>>
		&& (IsDerivedFrom<Derived, Base>)
	inline constexpr auto down_cast (
		Base && it
	) -> Derived && {
		return static_cast<Derived &&>(it);
	}

	// ----------------

	// TODO : maybe bug ? use assignment instead of constructor if in consteval context
	template <typename It, typename ... Argument> requires
		CategoryConstraint<IsInstance<It> && IsValid<Argument ...>>
		&& (IsConstructible<AsPure<It>, Argument && ...>)
	inline constexpr auto construct (
		It &            it,
		Argument && ... argument
	) -> Void {
		if (std::is_constant_evaluated()) {
			it = AsUnmakeConstant<It>{as_forward<Argument>(argument) ...};
		}
		else {
			new(&as_variable(it)) AsUnmakeConstant<It>{as_forward<Argument>(argument) ...};
		}
		return;
	}

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
		&& (IsDestructible<AsPure<It>>)
	inline constexpr auto destruct (
		It & it
	) -> Void {
		if (std::is_constant_evaluated()) {
		}
		else {
			it.~It();
		}
		return;
	}

	template <typename It, typename ... Argument> requires
		CategoryConstraint<IsInstance<It> && IsValid<Argument ...>>
		&& (IsConstructible<AsPure<It>, Argument && ...>)
		&& (IsDestructible<AsPure<It>>)
	inline constexpr auto restruct (
		It &            it,
		Argument && ... argument
	) -> Void {
		destruct(it);
		construct(it, as_forward<Argument>(argument) ...);
		return;
	}

	#pragma endregion

}
