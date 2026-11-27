module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.package.value_package;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;
import twinning.kernel.utility.trait.template_instance;

export namespace Twinning::Kernel::Trait {

	#pragma region type

	template <auto ... t_element> requires
		CategoryConstraint<>
	struct ValuePackage {

		inline static constexpr auto size = ZSize{sizeof...(t_element)};

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, ZSize>)
			&& (t_index < size)
		using Element = decltype(t_element...[t_index]);

		template <auto t_index> requires
			CategoryConstraint<>
			&& (IsSameOf<t_index, ZSize>)
			&& (t_index < size)
		inline static constexpr auto element = t_element...[t_index];

		// ----------------

		template <typename TValue> requires
			CategoryConstraint<>
		inline static constexpr auto has(
			TValue const & value
		) -> ZBoolean {
			auto result = false;
			auto iterate =
				[&]<typename TCurrentValue>(
				TCurrentValue current_value
			) {
				if constexpr (IsSame<TCurrentValue, TValue>) {
					if (current_value == value) {
						result = true;
					}
				}
			};
			(iterate(t_element), ...);
			return result;
		}

	};

	// ----------------

	template <typename TIt>
	concept IsValuePackage = IsTemplateInstanceOfTnv<TIt, ValuePackage>;

	#pragma endregion

	#pragma region utility

	template <typename TPackage, auto t_begin, auto t_count> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsValuePackage<TPackage> && IsSameOf<t_begin, ZSize> && IsSameOf<t_count, ZSize>)
		&& (t_begin + t_count <= TPackage::size)
	using AsValuePackageSub = decltype(
		[]<auto ... t_index>(
		std::index_sequence<t_index ...>
	) -> ValuePackage<TPackage::template element<t_begin + t_index> ...> {
			return {};
		}(std::make_index_sequence<t_count>{})
	);

	template <typename TPackage, auto t_count> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsValuePackage<TPackage> && IsSameOf<t_count, ZSize>)
		&& (t_count <= TPackage::size)
	using AsValuePackageRemoveHead = AsValuePackageSub<TPackage, t_count, TPackage::size - t_count>;

	template <typename TPackage, auto t_count> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsValuePackage<TPackage> && IsSameOf<t_count, ZSize>)
		&& (t_count <= TPackage::size)
	using AsValuePackageRemoveTail = AsValuePackageSub<TPackage, 1_ixz, TPackage::size - t_count>;

	// ----------------

	template <typename TPackage1, typename TPackage2> requires
		CategoryConstraint<IsPureInstance<TPackage1> && IsPureInstance<TPackage2>>
		&& (IsValuePackage<TPackage1> && IsValuePackage<TPackage2>)
	using AsValuePackageConcat = decltype(
		[]<auto ... t_index_1, auto ... t_index_2>(
		std::index_sequence<t_index_1 ...>,
		std::index_sequence<t_index_2 ...>
	) -> ValuePackage<TPackage1::template element<t_index_1> ..., TPackage2::template element<t_index_2> ...> {
			return {};
		}(std::make_index_sequence<TPackage1::size>{}, std::make_index_sequence<TPackage2::size>{})
	);

	template <typename TPackage, auto ... t_element> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsValuePackage<TPackage>)
	using AsValuePackagePrepend = AsValuePackageConcat<ValuePackage<t_element ...>, TPackage>;

	template <typename TPackage, auto ... t_element> requires
		CategoryConstraint<IsPureInstance<TPackage>>
		&& (IsValuePackage<TPackage>)
	using AsValuePackageAppend = AsValuePackageConcat<TPackage, ValuePackage<t_element ...>>;

	// ----------------

	template <auto t_size> requires
		CategoryConstraint<>
		&& (IsSameOf<t_size, ZSize>)
	using AsValuePackageOfIndex = decltype(
		[]<auto ... t_index>(
		std::index_sequence<t_index ...>
	) -> ValuePackage<t_index ...> {
			return {};
		}(std::make_index_sequence<t_size>{})
	);

	#pragma endregion

}
