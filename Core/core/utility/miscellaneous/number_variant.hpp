#pragma once

#include "core/utility/container/variant/variant.hpp"
#include "core/utility/container/variant/enumerable_variant.hpp"

namespace TwinStar::Core {

	#pragma region sub type

	M_enumeration(
		M_wrap(NumberVariantType),
		M_wrap(
			floating,
			integer,
		),
	);

	#pragma endregion

	#pragma region type

	class NumberVariant :
		public EnumerableVariant<NumberVariantType, Floating, Integer> {

	public:

		#pragma region structor

		constexpr ~NumberVariant (
		) = default;

		// ----------------

		constexpr NumberVariant (
		) = default;

		constexpr NumberVariant (
			NumberVariant const & that
		) = default;

		constexpr NumberVariant (
			NumberVariant && that
		) = default;

		// ----------------

		using EnumerableVariant::EnumerableVariant;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			NumberVariant const & that
		) -> NumberVariant& = default;

		constexpr auto operator = (
			NumberVariant && that
		) -> NumberVariant& = default;

		#pragma endregion

		#pragma region value

		constexpr auto is_floating (
		) const -> Boolean {
			return thiz.is<Floating>();
		}

		constexpr auto is_integer (
		) const -> Boolean {
			return thiz.is<Integer>();
		}

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		constexpr auto set_floating (
			Argument && ... argument
		) -> Floating& {
			return thiz.set<Floating>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		constexpr auto set_integer (
			Argument && ... argument
		) -> Integer& {
			return thiz.set<Integer>(as_forward<Argument>(argument) ...);
		}

		// ----------------

		constexpr auto get_floating (
		) -> Floating& {
			return thiz.get<Floating>();
		}

		constexpr auto get_integer (
		) -> Integer& {
			return thiz.get<Integer>();
		}

		// ----------------

		constexpr auto get_floating (
		) const -> Floating const& {
			return thiz.get<Floating>();
		}

		constexpr auto get_integer (
		) const -> Integer const& {
			return thiz.get<Integer>();
		}

		#pragma endregion

	};

	#pragma endregion

}
