module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.tuple.tuple;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename ... TValue> requires
		CategoryConstraint<IsPureInstance<TValue ...>>
	class Tuple {

	public:

		using Value = TypePackage<TValue ...>;

	protected:

		std::tuple<TValue ...> m_value;

	public:

		#pragma region constructor

		constexpr ~Tuple(
		) = default;

		// ----------------

		constexpr Tuple(
		) :
			m_value{} {
			return;
		}

		constexpr Tuple(
			Tuple const & that
		) = default;

		constexpr Tuple(
			Tuple && that
		) = default;

		// ----------------

		template <typename ... TValueObject> requires
			CategoryConstraint<IsValid<TValueObject ...>>
			&& (IsSame<AsPure<TValueObject>, TValue> && ...)
		explicit constexpr Tuple(
			TValueObject && ... value
		) :
			m_value{as_forward<TValueObject>(value) ...} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			Tuple const & that
		) -> Tuple & = default;

		constexpr auto operator =(
			Tuple && that
		) -> Tuple & = default;

		#pragma endregion

		#pragma region value

		template <typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (sizeof...(TArgument) == sizeof...(TValue))
			&& (IsConstructible<TValue, TArgument &&> && ...)
		constexpr auto set(
			TArgument && ... argument
		) -> Void {
			Generalization::each_with<>(
				[&] <auto t_index, typename TCurrentArgument>(ValuePackage<t_index>, TCurrentArgument && current_argument) {
					restruct(thiz.template get<mbox<Size>(t_index)>(), as_forward<TCurrentArgument>(current_argument));
				},
				as_forward<TArgument>(argument) ...
			);
			return;
		}

		template <auto t_index, typename ... TArgument> requires
			CategoryConstraint<IsValid<TArgument ...>>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
			&& (IsConstructible<AsSelect<t_index.value, TValue ...>, TArgument && ...>)
		constexpr auto set(
			TArgument && ... argument
		) -> AsSelect<t_index.value, TValue ...> & {
			restruct(thiz.template get<t_index>(), as_forward<TArgument>(argument) ...);
			return std::get<t_index.value>(thiz.m_value);
		}

		// ----------------

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
		constexpr auto get(
		) -> AsSelect<t_index.value, TValue ...> & {
			return std::get<t_index.value>(thiz.m_value);
		}

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, Size>)
			&& (t_index.value < sizeof...(TValue))
		constexpr auto get(
		) const -> AsSelect<t_index.value, TValue ...> const & {
			return std::get<t_index.value>(thiz.m_value);
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend constexpr auto operator ==(
			Tuple const & thix,
			Tuple const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename ... TValue, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TValue ...> && IsValid<TArgument ...>>
	inline constexpr auto make_tuple(
		TArgument && ... argument
	) -> Tuple<TValue ...> {
		auto result = Tuple<TValue ...>{};
		result.set(as_forward<TArgument>(argument) ...);
		return result;
	}

	template <typename ... TValue> requires
		CategoryConstraint<IsValid<TValue ...>>
	inline constexpr auto make_tuple_of(
		TValue && ... value
	) -> Tuple<AsPure<TValue> ...> {
		return Tuple<AsPure<TValue> ...>{as_forward<TValue>(value) ...};
	}

	#pragma endregion

}
