module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.category;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename ... It>
	concept IsAnything =
		CustomConstraint
		;

	// ----------------

	template <typename ... It>
	concept IsVoid =
		CustomConstraint
		&& (IsAnything<It ...>)
		&& (std::is_void_v<It> && ...)
		;

	template <typename ... It>
	concept IsValid =
		CustomConstraint
		&& (IsAnything<It ...>)
		&& (!std::is_void_v<It> && ...)
		;

	// ----------------

	template <typename ... It>
	concept IsInstance =
		CustomConstraint
		&& (IsValid<It ...>)
		&& (!std::is_reference_v<It> && ...)
		;

	template <typename ... It>
	concept IsVInstance =
		CustomConstraint
		&& (IsInstance<It ...>)
		&& (!std::is_const_v<It> && ...)
		;

	template <typename ... It>
	concept IsCInstance =
		CustomConstraint
		&& (IsInstance<It ...>)
		&& (std::is_const_v<It> && ...)
		;

	// ----------------

	template <typename ... It>
	concept IsReference =
		CustomConstraint
		&& (IsValid<It ...>)
		&& (std::is_reference_v<It> && ...)
		;

	template <typename ... It>
	concept IsLReference =
		CustomConstraint
		&& (IsReference<It ...>)
		&& (std::is_lvalue_reference_v<It> && ...)
		;

	template <typename ... It>
	concept IsRReference =
		CustomConstraint
		&& (IsReference<It ...>)
		&& (std::is_rvalue_reference_v<It> && ...)
		;

	template <typename ... It>
	concept IsVReference =
		CustomConstraint
		&& (IsLReference<It ...>)
		&& (!std::is_const_v<std::remove_reference_t<It>> && ...)
		;

	template <typename ... It>
	concept IsCReference =
		CustomConstraint
		&& (IsLReference<It ...>)
		&& (!std::is_const_v<std::remove_reference_t<It>> && ...)
		;

	template <typename ... It>
	concept IsMReference =
		CustomConstraint
		&& (IsRReference<It ...>)
		;

	// ----------------

	template <typename ... It>
	concept IsPure =
		CustomConstraint
		&& (IsAnything<It ...>)
		&& (std::is_same_v<It, std::remove_cvref_t<It>> && ...)
		;

	template <typename ... It>
	concept IsPureInstance =
		CustomConstraint
		&& (IsPure<It ...>)
		&& (IsInstance<It> && ...)
		;

	// ----------------

	template <typename ... It>
	concept IsPointable =
		CustomConstraint
		&& (IsAnything<It ...>)
		&& (!IsReference<It> && ...)
		;

	#pragma endregion

	#pragma region utility

	template <typename It, typename ... Alternative>
	concept IsSame =
		CategoryConstraint<IsAnything<It> && IsAnything<Alternative ...>>
		&& (std::is_same_v<It, Alternative> || ...)
		;

	template <auto it, typename ... Alternative>
	concept IsSameV =
		CategoryConstraint<IsAnything<Alternative ...>>
		&& (std::is_same_v<std::remove_cv_t<decltype(it)>, Alternative> || ...)
		;

	// ----------------

	template <auto index, typename ... Alternative> requires
		CategoryConstraint<IsAnything<Alternative ...>>
		&& (IsSameV<index, ZSize>)
		&& (index < sizeof...(Alternative))
	using AsSelect = std::tuple_element_t<index, std::tuple<Alternative ...>>;

	template <auto condition, typename TrulyAlternative, typename FalsyAlternative> requires
		CategoryConstraint<IsAnything<TrulyAlternative> && IsAnything<FalsyAlternative>>
		&& (IsSameV<condition, ZBoolean>)
	using AsSwitch = std::conditional_t<condition, TrulyAlternative, FalsyAlternative>;

	#pragma endregion

	#pragma region qualification

	template <typename It> requires
		CategoryConstraint<IsAnything<It>>
	using AsMakeConstant = std::add_const_t<It>;

	template <typename It> requires
		CategoryConstraint<IsAnything<It>>
	using AsUnmakeConstant = std::remove_const_t<It>;

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsAnything<It>>
	using AsMakeLReference = std::add_lvalue_reference_t<It>;

	template <typename It> requires
		CategoryConstraint<IsAnything<It>>
	using AsMakeRReference = std::add_rvalue_reference_t<It>;

	template <typename It> requires
		CategoryConstraint<IsAnything<It>>
	using AsUnmakeReference = std::remove_reference_t<It>;

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsAnything<It>>
	using AsPure = std::remove_cvref_t<It>;

	#pragma endregion

	#pragma region utility

	template <typename It> requires
		CategoryConstraint<IsValid<It>>
	inline constexpr auto as_lvalue (
		It && it
	) -> AsUnmakeReference<It> & {
		return static_cast<AsUnmakeReference<It> &>(it);
	}

	template <typename It> requires
		CategoryConstraint<IsValid<It>>
	inline constexpr auto as_rvalue (
		It && it
	) -> AsUnmakeReference<It> && {
		return static_cast<AsUnmakeReference<It> &&>(it);
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
	inline constexpr auto as_variable (
		It & it
	) -> AsPure<It> & {
		return const_cast<AsPure<It> &>(it);
	}

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
	inline constexpr auto as_constant (
		It & it
	) -> AsPure<It> const & {
		return static_cast<AsPure<It> const &>(it);
	}

	template <typename It> requires
		CategoryConstraint<IsInstance<It>>
	inline constexpr auto as_moveable (
		It & it
	) -> AsPure<It> && {
		return static_cast<AsPure<It> &&>(it);
	}

	// ----------------

	template <typename It> requires
		CategoryConstraint<IsValid<It>>
	inline constexpr auto as_forward (
		AsUnmakeReference<It> & it
	) -> It && {
		return static_cast<It &&>(it);
	}

	template <typename It> requires
		CategoryConstraint<IsValid<It>>
	inline constexpr auto as_forward (
		AsUnmakeReference<It> && it
	) -> It && {
		return static_cast<It &&>(it);
	}

	// ----------------

	template <typename That, typename It> requires
		CategoryConstraint<IsPureInstance<That> && IsPureInstance<It>>
	inline auto self_cast (
		It & it
	) -> That & {
		return reinterpret_cast<That &>(it);
	}

	template <typename That, typename It> requires
		CategoryConstraint<IsPureInstance<That> && IsPureInstance<It>>
	inline auto self_cast (
		It const & it
	) -> That const & {
		return reinterpret_cast<That const &>(it);
	}

	template <typename That, typename It> requires
		CategoryConstraint<IsPureInstance<That> && IsPureInstance<It>>
	inline auto self_cast (
		It && it
	) -> That && {
		return reinterpret_cast<That &&>(it);
	}

	#pragma endregion

	#pragma region miscellaneous

	template <typename It, auto condition> requires
		CategoryConstraint<IsAnything<It>>
		&& (IsSameV<condition, ZBoolean>)
	using AsConstantIf = AsSwitch<condition, AsMakeConstant<It>, It>;

	#pragma endregion

}
