module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.category;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename ... TIt>
	concept IsAnything =
		CustomConstraint
		;

	// ----------------

	template <typename ... TIt>
	concept IsVoid =
		CustomConstraint
		&& (IsAnything<TIt ...>)
		&& (std::is_void_v<TIt> && ...)
		;

	template <typename ... TIt>
	concept IsValid =
		CustomConstraint
		&& (IsAnything<TIt ...>)
		&& (!std::is_void_v<TIt> && ...)
		;

	// ----------------

	template <typename ... TIt>
	concept IsInstance =
		CustomConstraint
		&& (IsValid<TIt ...>)
		&& (!std::is_reference_v<TIt> && ...)
		;

	template <typename ... TIt>
	concept IsVariableInstance =
		CustomConstraint
		&& (IsInstance<TIt ...>)
		&& (!std::is_const_v<TIt> && ...)
		;

	template <typename ... TIt>
	concept IsConstantInstance =
		CustomConstraint
		&& (IsInstance<TIt ...>)
		&& (std::is_const_v<TIt> && ...)
		;

	// ----------------

	template <typename ... TIt>
	concept IsReference =
		CustomConstraint
		&& (IsValid<TIt ...>)
		&& (std::is_reference_v<TIt> && ...)
		;

	template <typename ... TIt>
	concept IsLeftReference =
		CustomConstraint
		&& (IsReference<TIt ...>)
		&& (std::is_lvalue_reference_v<TIt> && ...)
		;

	template <typename ... TIt>
	concept IsRightReference =
		CustomConstraint
		&& (IsReference<TIt ...>)
		&& (std::is_rvalue_reference_v<TIt> && ...)
		;

	template <typename ... TIt>
	concept IsVariableReference =
		CustomConstraint
		&& (IsLeftReference<TIt ...>)
		&& (!std::is_const_v<std::remove_reference_t<TIt>> && ...)
		;

	template <typename ... TIt>
	concept IsConstantReference =
		CustomConstraint
		&& (IsLeftReference<TIt ...>)
		&& (!std::is_const_v<std::remove_reference_t<TIt>> && ...)
		;

	template <typename ... TIt>
	concept IsMoveableReference =
		CustomConstraint
		&& (IsRightReference<TIt ...>)
		;

	// ----------------

	template <typename ... TIt>
	concept IsPure =
		CustomConstraint
		&& (IsAnything<TIt ...>)
		&& (std::is_same_v<TIt, std::remove_cvref_t<TIt>> && ...)
		;

	template <typename ... TIt>
	concept IsPureInstance =
		CustomConstraint
		&& (IsPure<TIt ...>)
		&& (IsInstance<TIt> && ...)
		;

	// ----------------

	template <typename ... TIt>
	concept IsPointable =
		CustomConstraint
		&& (IsAnything<TIt ...>)
		&& (!IsReference<TIt> && ...)
		;

	#pragma endregion

	#pragma region utility

	template <typename TIt, typename ... TAlternative>
	concept IsSame =
		CategoryConstraint<IsAnything<TIt> && IsAnything<TAlternative ...>>
		&& (std::is_same_v<TIt, TAlternative> || ...)
		;

	template <auto t_it, typename ... TAlternative>
	concept IsSameOf =
		CategoryConstraint<IsAnything<TAlternative ...>>
		&& (std::is_same_v<std::remove_cv_t<decltype(t_it)>, TAlternative> || ...)
		;

	// ----------------

	template <auto t_index, typename ... TAlternative> requires
		CategoryConstraint<IsAnything<TAlternative ...>>
		&& (IsSameOf<t_index, ZSize>)
		&& (t_index < sizeof...(TAlternative))
	using AsSelect = std::tuple_element_t<t_index, std::tuple<TAlternative ...>>;

	template <auto t_condition, typename TTrulyAlternative, typename TFalsyAlternative> requires
		CategoryConstraint<IsAnything<TTrulyAlternative> && IsAnything<TFalsyAlternative>>
		&& (IsSameOf<t_condition, ZBoolean>)
	using AsSwitch = std::conditional_t<t_condition, TTrulyAlternative, TFalsyAlternative>;

	#pragma endregion

	#pragma region qualification

	template <typename TIt> requires
		CategoryConstraint<IsAnything<TIt>>
	using AsMakeConstant = std::add_const_t<TIt>;

	template <typename TIt> requires
		CategoryConstraint<IsAnything<TIt>>
	using AsUnmakeConstant = std::remove_const_t<TIt>;

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsAnything<TIt>>
	using AsMakeLeftReference = std::add_lvalue_reference_t<TIt>;

	template <typename TIt> requires
		CategoryConstraint<IsAnything<TIt>>
	using AsMakeRightReference = std::add_rvalue_reference_t<TIt>;

	template <typename TIt> requires
		CategoryConstraint<IsAnything<TIt>>
	using AsUnmakeReference = std::remove_reference_t<TIt>;

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsAnything<TIt>>
	using AsPure = std::remove_cvref_t<TIt>;

	#pragma endregion

	#pragma region utility

	template <typename TIt> requires
		CategoryConstraint<IsValid<TIt>>
	inline constexpr auto as_left(
		TIt && it
	) -> AsUnmakeReference<TIt> & {
		return static_cast<AsUnmakeReference<TIt> &>(it);
	}

	template <typename TIt> requires
		CategoryConstraint<IsValid<TIt>>
	inline constexpr auto as_right(
		TIt && it
	) -> AsUnmakeReference<TIt> && {
		return static_cast<AsUnmakeReference<TIt> &&>(it);
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto as_variable(
		TIt & it
	) -> AsPure<TIt> & {
		return const_cast<AsPure<TIt> &>(it);
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto as_constant(
		TIt & it
	) -> AsPure<TIt> const & {
		return static_cast<AsPure<TIt> const &>(it);
	}

	template <typename TIt> requires
		CategoryConstraint<IsInstance<TIt>>
	inline constexpr auto as_moveable(
		TIt & it
	) -> AsPure<TIt> && {
		return static_cast<AsPure<TIt> &&>(it);
	}

	// ----------------

	template <typename TIt> requires
		CategoryConstraint<IsValid<TIt>>
	inline constexpr auto as_forward(
		AsUnmakeReference<TIt> & it
	) -> TIt && {
		return static_cast<TIt &&>(it);
	}

	template <typename TIt> requires
		CategoryConstraint<IsValid<TIt>>
	inline constexpr auto as_forward(
		AsUnmakeReference<TIt> && it
	) -> TIt && {
		return static_cast<TIt &&>(it);
	}

	// ----------------

	template <typename TThat, typename TIt> requires
		CategoryConstraint<IsPureInstance<TThat> && IsPureInstance<TIt>>
	inline auto self_cast(
		TIt & it
	) -> TThat & {
		return reinterpret_cast<TThat &>(it);
	}

	template <typename TThat, typename TIt> requires
		CategoryConstraint<IsPureInstance<TThat> && IsPureInstance<TIt>>
	inline auto self_cast(
		TIt const & it
	) -> TThat const & {
		return reinterpret_cast<TThat const &>(it);
	}

	template <typename TThat, typename TIt> requires
		CategoryConstraint<IsPureInstance<TThat> && IsPureInstance<TIt>>
	inline auto self_cast(
		TIt && it
	) -> TThat && {
		return reinterpret_cast<TThat &&>(it);
	}

	#pragma endregion

	#pragma region miscellaneous

	template <typename TIt, auto t_condition> requires
		CategoryConstraint<IsAnything<TIt>>
		&& (IsSameOf<t_condition, ZBoolean>)
	using AsConstantIf = AsSwitch<t_condition, AsMakeConstant<TIt>, TIt>;

	#pragma endregion

}
