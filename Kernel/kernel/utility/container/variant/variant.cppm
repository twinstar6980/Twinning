module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.variant.variant;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TValue ...>>
		&& (sizeof...(TValue) > 0_szz)
	class Variant {

	public:

		using Value = TypePackage<TValue ...>;

	protected:

		std::variant<TValue ...> m_value;

	public:

		#pragma region constructor

		constexpr ~Variant(
		) = default;

		// ----------------

		constexpr Variant(
		) :
			m_value{} {
			return;
		}

		constexpr Variant(
			Variant const & that
		) = default;

		constexpr Variant(
			Variant && that
		) = default;

		// ----------------

		template <typename TValueObject> requires
			CategoryConstraint<IsValid<TValueObject>>
			&& (IsSame<AsPure<TValueObject>, TValue ...>)
		explicit constexpr Variant(
			TValueObject && value
		) :
			m_value{as_forward<TValueObject>(value)} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			Variant const & that
		) -> Variant & = default;

		constexpr auto operator =(
			Variant && that
		) -> Variant & = default;

		#pragma endregion

		#pragma region value

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
			&& (IsSame<TValueObject, TValue ...>)
		constexpr auto is(
		) const -> Boolean {
			return std::holds_alternative<TValueObject>(thiz.m_value);
		}

		// ----------------

		template <typename TValueObject, typename ... TArgument> requires
			CategoryConstraint<IsPureInstance<TValueObject> && IsValid<TArgument ...>>
			&& (IsSame<TValueObject, TValue ...>)
			&& (IsConstructible<TValueObject, TArgument && ...>)
		constexpr auto set(
			TArgument && ... argument
		) -> TValueObject & {
			thiz.m_value.template emplace<TValueObject>(as_forward<TArgument>(argument) ...);
			return std::get<TValueObject>(thiz.m_value);
		}

		// ----------------

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
			&& (IsSame<TValueObject, TValue ...>)
		constexpr auto get(
		) -> TValueObject & {
			assert_test(thiz.template is<TValueObject>());
			return std::get<TValueObject>(thiz.m_value);
		}

		template <typename TValueObject> requires
			CategoryConstraint<IsPureInstance<TValueObject>>
			&& (IsSame<TValueObject, TValue ...>)
		constexpr auto get(
		) const -> TValueObject const & {
			assert_test(thiz.template is<TValueObject>());
			return std::get<TValueObject>(thiz.m_value);
		}

		#pragma endregion

		#pragma region index

		constexpr auto index(
		) const -> Size {
			return mbox<Size>(thiz.m_value.index());
		}

		#pragma endregion

		#pragma region value of index

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
		constexpr auto is_of_index(
		) const -> Boolean {
			return thiz.template is<AsSelect<t_index.value, TValue ...>>();
		}

		// ----------------

		template <auto t_index, typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
			&& (IsConstructible<AsSelect<t_index.value, TValue ...>, TArgument && ...>)
		constexpr auto set_of_index(
			TArgument && ... argument
		) -> AsSelect<t_index.value, TValue ...> & {
			return thiz.template set<AsSelect<t_index.value, TValue ...>>(as_forward<TArgument>(argument) ...);
		}

		// ----------------

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
		constexpr auto get_of_index(
		) -> AsSelect<t_index.value, TValue ...> & {
			return thiz.template get<AsSelect<t_index.value, TValue ...>>();
		}

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
		constexpr auto get_of_index(
		) const -> AsSelect<t_index.value, TValue ...> const & {
			return thiz.template get<AsSelect<t_index.value, TValue ...>>();
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			Variant const & thix,
			Variant const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TActiveValue, typename ... TValue, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TActiveValue> && IsPureInstance<TValue ...> && IsValid<TArgument ...>>
	inline constexpr auto make_variant(
		TArgument && ... argument
	) -> Variant<TValue ...> {
		auto result = Variant<TValue ...>{};
		result.template set<TActiveValue>(as_forward<TArgument>(argument) ...);
		return result;
	}

	template <typename TActiveValue, typename ... TValue> requires
		CategoryConstraint<IsValid<TActiveValue> && IsPureInstance<TValue ...>>
	inline constexpr auto make_variant_of(
		TActiveValue && value
	) -> Variant<AsPure<TValue> ...> {
		return Variant<AsPure<TValue> ...>{as_forward<TActiveValue>(value)};
	}

	#pragma endregion

}
