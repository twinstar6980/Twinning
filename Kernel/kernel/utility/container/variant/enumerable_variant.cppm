module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.variant.enumerable_variant;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.variant.variant;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TEnumeration, typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TEnumeration> && IsPureInstance<TValue ...>>
		&& (IsEnumerationBox<TEnumeration>)
		&& (sizeof...(TValue) > 0_szz)
	class EnumerableVariant :
		public Variant<TValue ...> {

	private:

		using Variant = Variant<TValue ...>;

	public:

		using Enumeration = TEnumeration;

	public:

		#pragma region structor

		constexpr ~EnumerableVariant (
		) = default;

		// ----------------

		constexpr EnumerableVariant (
		) = default;

		constexpr EnumerableVariant (
			EnumerableVariant const & that
		) = default;

		constexpr EnumerableVariant (
			EnumerableVariant && that
		) = default;

		// ----------------

		using Variant::Variant;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			EnumerableVariant const & that
		) -> EnumerableVariant & = default;

		constexpr auto operator = (
			EnumerableVariant && that
		) -> EnumerableVariant & = default;

		#pragma endregion

		#pragma region type

		constexpr auto type (
		) const -> Enumeration {
			return cbox<Enumeration>(thiz.index());
		}

		#pragma endregion

		#pragma region value of type

		template <auto type> requires
			CategoryConstraint<>
			&& (IsSameV<type, Enumeration>)
			&& (static_cast<ZSize>(type.value) < sizeof...(TValue))
		constexpr auto is_of_type (
		) const -> Boolean {
			return thiz.template is<AsSelect<static_cast<ZSize>(type.value), TValue ...>>();
		}

		// ----------------

		template <auto type, typename ... Argument> requires
			CategoryConstraint<>
			&& (IsSameV<type, Enumeration>)
			&& (static_cast<ZSize>(type.value) < sizeof...(TValue))
			&& (IsConstructible<AsSelect<static_cast<ZSize>(type.value), TValue ...>, Argument && ...>)
		constexpr auto set_of_type (
			Argument && ... argument
		) -> AsSelect<static_cast<ZSize>(type.value), TValue ...> & {
			return thiz.template set<AsSelect<static_cast<ZSize>(type.value), TValue ...>>(as_forward<Argument>(argument) ...);
		}

		// ----------------

		template <auto type> requires
			CategoryConstraint<>
			&& (IsSameV<type, Enumeration>)
			&& (static_cast<ZSize>(type.value) < sizeof...(TValue))
		constexpr auto get_of_type (
		) -> AsSelect<static_cast<ZSize>(type.value), TValue ...> & {
			return thiz.template get<AsSelect<static_cast<ZSize>(type.value), TValue ...>>();
		}

		template <auto type> requires
			CategoryConstraint<>
			&& (IsSameV<type, Enumeration>)
			&& (static_cast<ZSize>(type.value) < sizeof...(TValue))
		constexpr auto get_of_type (
		) const -> AsSelect<static_cast<ZSize>(type.value), TValue ...> const & {
			return thiz.template get<AsSelect<static_cast<ZSize>(type.value), TValue ...>>();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator == (
			EnumerableVariant const & thix,
			EnumerableVariant const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}
