#pragma once

#include "core/utility/base_wrapper/wrapper.hpp"
#include <tuple>

namespace TwinKleS::Core {

	#pragma region type

	template <typename ...TValue> requires
		CategoryConstraint<IsPureInstance<TValue...>>
	class Tuple {

	public: //

		using Value = TypePackage<TValue...>;

	protected: //

		std::tuple<TValue...> m_value{};

	public: //

		#pragma region structor

		constexpr ~Tuple (
		) = default;

		// ----------------

		constexpr Tuple (
		) = default;

		constexpr Tuple (
			Tuple const & that
		) = default;

		constexpr Tuple (
			Tuple && that
		) = default;

		// ----------------

		template <typename ...ValueObject> requires
			CategoryConstraint<IsValid<ValueObject...>>
			&& (IsSame<AsPure<ValueObject>, TValue> && ...)
		explicit constexpr Tuple (
			ValueObject && ...value
		) :
			m_value{as_forward<ValueObject>(value)...} {
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			Tuple const & that
		) -> Tuple& = default;

		constexpr auto operator = (
			Tuple && that
		) -> Tuple& = default;

		#pragma endregion

		#pragma region value

		template <typename ...Argument> requires
			CategoryConstraint<IsValid<Argument...>>
			&& (sizeof...(Argument) == sizeof...(TValue))
			&& (IsConstructible<TValue, Argument &&> && ...)
		constexpr auto set (
			Argument && ...argument
		) -> Void {
			[&] <auto ...index> (
				ValuePackage<index...>
			) -> auto {
					(restruct(thiz.template get<index>(), as_forward<Argument>(argument)), ...);
					return;
				}(AsValuePackageOfIndex<sizeof...(Argument)>{});
			return;
		}

		template <auto index, typename ...Argument> requires
			CategoryConstraint<IsValid<Argument...>>
			&& (IsSameV<index, ZSize>)
			&& (index < sizeof...(TValue))
			&& (IsConstructible<AsSelect<index, TValue...>, Argument &&...>)
		constexpr auto set (
			Argument && ...argument
		) -> AsSelect<index, TValue...>& {
			restruct(thiz.template get<index>(), as_forward<Argument>(argument)...);
			return std::get<index>(thiz.m_value);
		}

		// ----------------

		template <auto index> requires
			CategoryConstraint<>
			&& (IsSameV<index, ZSize>)
			&& (index < sizeof...(TValue))
		constexpr auto get (
		) -> AsSelect<index, TValue...>& {
			return std::get<index>(thiz.m_value);
		}

		template <auto index> requires
			CategoryConstraint<>
			&& (IsSameV<index, ZSize>)
			&& (index < sizeof...(TValue))
		constexpr auto get (
		) const -> AsSelect<index, TValue...> const& {
			return std::get<index>(thiz.m_value);
		}

		#pragma endregion

	public: //

		#pragma region operator

		friend constexpr auto operator == (
			Tuple const & thix,
			Tuple const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename ...Value, typename ...Argument> requires
		CategoryConstraint<IsPureInstance<Value...> && IsValid<Argument...>>
	inline constexpr auto make_tuple (
		Argument && ...argument
	) -> Tuple<Value...> {
		auto result = Tuple<Value...>{};
		result.set(as_forward<Argument>(argument)...);
		return result;
	}

	template <typename ...Value> requires
		CategoryConstraint<IsValid<Value...>>
	inline constexpr auto make_tuple_of (
		Value && ...value
	) -> Tuple<AsPure<Value>...> {
		return Tuple<AsPure<Value>...>{as_forward<Value>(value)...};
	}

	#pragma endregion

}
